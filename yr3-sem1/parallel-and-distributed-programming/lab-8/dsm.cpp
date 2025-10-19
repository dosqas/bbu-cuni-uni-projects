#include "dsm.hpp"
#include <mpi.h>
#include <iostream>

DSM::DSM(int r, int s) : rank(r), size(s) {
    lamport_clock = 0;
}

// Set the list of subscribers for a variable
// Also set the callback function to be called on updates
// so that we can notify when the variable changes
void DSM::set_subscribers(int var_id, const std::vector<int>& subs, std::function<void(int)> callback) {
    subscribers[var_id] = subs;
    callbacks[var_id] = callback;
}

// Write a new value to a variable and notify subscribers
void DSM::write(int var_id, int value) {
    // Increment Lamport clock
    // The Lamport clock is incremented on each local event (write)
    // This ensures that each event has a unique timestamp and helps maintain a total order of events
    // so that the process can correctly order events from different processes
    lamport_clock++;
    variables[var_id] = value;

    // Send the update to other subscribers
    if (subscribers.count(var_id)) {
        for (int sub : subscribers[var_id]) {
            if (sub != rank) {
                int msg[3] = { var_id, value, lamport_clock };
                // Send the variable update message to the subscriber
                // Count = 3: var_id, value, lamport_clock
                // Variables are int, so we use MPI_INT
                // Send to the subscriber process
                MPI_Send(msg, 3, MPI_INT, sub, 0, MPI_COMM_WORLD);
            }
        }
    }

    // Trigger local callback
    if (callbacks.count(var_id)) {
        callbacks[var_id](value);
    }
}

// If the variable matches expected, update it to new_value
bool DSM::compare_and_exchange(int var_id, int expected, int new_value) {
    if (variables[var_id] == expected) {
        write(var_id, new_value);
        return true;
    }
    return false;
}

void DSM::run() {
    MPI_Status status;
    int flag;
    int msg[3];

    // Structure to hold messages with their Lamport clocks
    struct Message {
        int var_id;
        int value;
        int lamport_clock;
        int source_rank;

        // For priority queue (min-heap based on Lamport clock)
        bool operator>(const Message& other) const {
            if (lamport_clock != other.lamport_clock)
                return lamport_clock > other.lamport_clock;
            return source_rank > other.source_rank;  // Tie-breaker
        }
    };

    // Min-heap to order messages by Lamport clock
    std::priority_queue<Message, std::vector<Message>, std::greater<Message>> message_queue;

    // Collect all messages
    while (true) {
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        if (flag) {
            MPI_Recv(msg, 3, MPI_INT, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &status);

            Message m;
            m.var_id = msg[0];
            m.value = msg[1];
            m.lamport_clock = msg[2];
            m.source_rank = status.MPI_SOURCE;

            message_queue.push(m);
        }
        else {
            break;
        }
    }

    // Process messages in Lamport clock order
    while (!message_queue.empty()) {
        Message m = message_queue.top();
        message_queue.pop();

        // Update Lamport clock
        lamport_clock = std::max(lamport_clock, m.lamport_clock) + 1;

        // Update local variable
        variables[m.var_id] = m.value;

        // Trigger callback
        if (callbacks.count(m.var_id)) {
            callbacks[m.var_id](m.value);
        }
    }
}