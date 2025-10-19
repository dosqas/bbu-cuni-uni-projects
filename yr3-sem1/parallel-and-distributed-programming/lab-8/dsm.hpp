#pragma once
#include <vector>
#include <map>
#include <functional>
#include <queue>
#include <tuple>

struct VariableUpdate {
    int var_id;
    int value;
    int timestamp;  // Lamport timestamp
    int sender;
};

class DSM {
    int rank;
    int size;
    int lamport_clock = 0;

    // Local copy of variables
    std::map<int, int> variables;

    // Static subscription info
    std::map<int, std::vector<int>> subscribers;

    // Callbacks
    std::map<int, std::function<void(int)>> callbacks;

    // Pending updates to enforce total order
    std::priority_queue<
        std::tuple<int, int, int, int>,
        std::vector<std::tuple<int, int, int, int>>,
        std::greater<>
    > pending_updates;  // (timestamp, sender, var_id, value)

public:
    DSM(int r, int s);

    void set_subscribers(int var_id, const std::vector<int>& subs, std::function<void(int)> callback);

    void write(int var_id, int value);
    bool compare_and_exchange(int var_id, int expected, int new_value);
    void run();
};
