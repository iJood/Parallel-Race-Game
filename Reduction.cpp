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
const int GRID_SIZE = 100;     // Steps for more movement
const double POWER_UP_CHANCE = 0.5;  // Increased power-up chance
const double EVENT_CHANCE = 0.1;     // Reduced slowdown chance

// Struct to store result information
struct Result {
    int winner_id = -1; // Player ID of the winner
    int position = -1;  // Winning position

    // Custom "max" logic for reduction
    void combine(const Result &other) {
        if (other.position > position) {
            winner_id = other.winner_id;
            position = other.position;
        }
    }
};

// Declare reduction for the Result struct
#pragma omp declare reduction(max_result : Result : omp_out.combine(omp_in)) initializer(omp_priv = Result())

// Function to calculate player movement
void calculate_movement(int player_id, vector<int> &positions, vector<double> &speed, Result &local_result) {
    int thread_id = omp_get_thread_num(); // Get the thread number
    cout << "Thread " << thread_id << " is processing Player #" << player_id + 1 << endl;

    for (int step = 0; step < GRID_SIZE; ++step) {
        // Simulate meaningful movement
        double move = speed[player_id] * (1 + static_cast<double>(rand()) / RAND_MAX);
        if (static_cast<double>(rand()) / RAND_MAX < POWER_UP_CHANCE) {
            move += 10; // Power-up boosts movement
        }
        positions[player_id] += static_cast<int>(move); // Update player position

        if (static_cast<double>(rand()) / RAND_MAX < EVENT_CHANCE) {
            positions[player_id] -= 3; // Mild slowdown
        }

        // If the player reaches the finish line, update local result
        if (positions[player_id] >= FINISH_LINE) {
            local_result.winner_id = player_id;
            local_result.position = positions[player_id];
            return;
        }
    }
}

void race_simulation() {
    cout << "Using OpenMP with " << omp_get_max_threads() << " threads." << endl;
    vector<int> positions(NUM_PLAYERS, 0);  // Player positions
    vector<double> speed(NUM_PLAYERS, 0);  // Player speeds

    #pragma omp parallel for
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        speed[i] = 1 + static_cast<double>(rand()) / RAND_MAX * 2; // Random speed between 1 and 3
    }

    // Global result to store the final winner
    Result global_result;

    #pragma omp parallel for reduction(max_result : global_result)
    for (int player_id = 0; player_id < NUM_PLAYERS; ++player_id) {
        Result local_result;
        calculate_movement(player_id, positions, speed, local_result);
        global_result.combine(local_result);
    }

    // Output the result
    if (global_result.winner_id != -1) {
        cout << "Winner: Player " << global_result.winner_id + 1 << endl;
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

    return 0;
}

