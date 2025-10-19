#include <mpi.h>
#include <iostream>
#include <windows.h>
#include "dsm.hpp"

const int NUM_VARIABLES_PER_PROCESS = 3;

// Change to debug folder: cd x64/Debug
// Run: mpiexec -n 4 lab-8.exe

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    DSM dsm(rank, size);

    // Initialize variables and subscriptions

    // Var 1: All processes subscribe to all variables
    //int total_vars = size * NUM_VARIABLES_PER_PROCESS;
    //for (int var_id = 0; var_id < total_vars; var_id++) {
    //    // All processes subscribe to all variables
    //    dsm.set_subscribers(var_id, { 0, 1, 2, 3 }, [var_id, rank](int value) {
    //        std::cout << "Process " << rank << " sees variable " << var_id
    //            << " change to " << value << std::endl;
    //        std::cout.flush();
    //        });
    //}

	// Var 2: Some processes subscribe to specific variables
    for (int p = 0; p < size; p++) {
        for (int i = 0; i < NUM_VARIABLES_PER_PROCESS; i++) {
            int var_id = p * NUM_VARIABLES_PER_PROCESS + i;
            if (rank == p || rank == (p + 1) % size) { // Subscribe to own and next process's variables
                dsm.set_subscribers(var_id, { p, (p + 1) % size }, [var_id, rank](int value) {
                    std::cout << "Process " << rank << " sees variable " << var_id
                        << " change to " << value << std::endl;
                    std::cout.flush();
                    });
            }
        }
	}

    // Synchronize before starting
    MPI_Barrier(MPI_COMM_WORLD);

    // Example writes
    if (rank == 0) {
        dsm.write(0, 42);
        dsm.compare_and_exchange(1, 0, 99);
		dsm.compare_and_exchange(4, 0, 99);  // Process 0 does not subscribe to var 4, so no output expected
        dsm.write(4, 99);                    // Same here ^
        dsm.compare_and_exchange(1, 99, 67);
        dsm.compare_and_exchange(1, 69, 41); // Should not change
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Run DSM to process messages
    dsm.run();

    MPI_Finalize();
    return 0;
}