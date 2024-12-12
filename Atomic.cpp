#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <omp.h>

using namespace std;

const int NUM_PLAYERS = 200;
const int FINISH_LINE = 100;   // Shortened finish line for simplicity
const int GRID_SIZE = 100;     // Increased steps for more movement
const double POWER_UP_CHANCE = 0.5;  // Increased power-up chance
const double EVENT_CHANCE = 0.1;     // Reduced slowdown chance

void calculate_movement(int player_id, vector<int> &positions, vector<double> &speed, bool &winner_found, int &winner) {
    int thread_num = omp_get_thread_num(); // Get the thread number
    cout << "Thread " << thread_num << " is processing Player #" << player_id + 1 << endl;

    for (int step = 0; step < GRID_SIZE; ++step) {
        if (winner_found) return; // Exit if a winner is already found

        // Simulate meaningful movement
        double move = speed[player_id] * (1 + static_cast<double>(rand()) / RAND_MAX);
        if (static_cast<double>(rand()) / RAND_MAX < POWER_UP_CHANCE) {
            move += 10; // Power-up boosts movement
        }
        positions[player_id] += static_cast<int>(move); // Update player position

        if (static_cast<double>(rand()) / RAND_MAX < EVENT_CHANCE) {
            positions[player_id] -= 3; // Mild slowdown
        }

        // Check if the player has reached the finish line
        if (positions[player_id] >= FINISH_LINE) {
            #pragma omp atomic
            winner_found += 1; // Atomically increment winner_found

            #pragma omp critical // Use a critical section to update shared resources
            {
                if (!winner_found) { // This is the first thread to set the winner
                    winner_found = true;
                    winner = player_id; // Set the winner’s ID using a critical section
                }
            }
            return; // Exit as this player already won
        }
    }
}

void race_simulation() {
    cout << "Using OpenMP with " << omp_get_max_threads() << " threads." << endl;
    vector<int> positions(NUM_PLAYERS, 0);       // Player positions
    vector<double> speed(NUM_PLAYERS, 0);       // Player speeds
    bool winner_found = false;                  // Flag to track if a winner is found (0 = false, 1 = true)
    int winner = -1;                            // Stores the winning player’s ID

    #pragma omp parallel for
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        speed[i] = 1 + static_cast<double>(rand()) / RAND_MAX * 2; // Random speed between 1 and 3
    }

    #pragma omp parallel for
    for (int player_id = 0; player_id < NUM_PLAYERS; ++player_id) {
        calculate_movement(player_id, positions, speed, winner_found, winner);
    }

    // Output the result
    if (winner_found) {
        cout << "Winner: Player " << winner + 1 << endl;
    } else {
        cout << "No winner detected." << endl;
    }
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    auto start_time = chrono::high_resolution_clock::now();
    race_simulation();
    auto end_time = chrono::high_resolution_clock::now();

    auto duration_us = chrono::duration_cast<chrono::microseconds>(end_time - start_time);
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Execution Time: " << duration_us.count() << " µs (" << duration_ms.count() << " ms)" << endl;

    return 0;
}
