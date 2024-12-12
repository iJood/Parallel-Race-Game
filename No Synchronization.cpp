#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib> // For random seed
#include <ctime>   // For random generation (if needed for threads)
#include <mutex>   // For thread-safety
#include <cmath>
#include <omp.h>

using namespace std;

// Constants
const int NUM_PLAYERS = 20;  // Number of players
const int FINISH_LINE = 100; // Distance to the finish line
const int GRID_SIZE = 20;    // Simulated grid for player positions
const double POWER_UP_CHANCE = 0.2; // 20% chance for a random event
const double EVENT_CHANCE = 0.2;    // 20% chance for a random event

// Mutex for rand() (not thread-safe by default)
mutex rand_mutex;

// Function to simulate complex player movement
bool calculate_movement(int player_id, vector<vector<int>> &positions, vector<double> &speed) {
    int thread_id = omp_get_thread_num(); // Get thread ID for debugging
    cout << "Thread " << thread_id << " is processing Player #" << player_id + 1 << endl;

    for (int step = 0; step < GRID_SIZE; ++step) {
        // Complex movement calculation with starting-point elements
        double move;

        {
            lock_guard<mutex> lock(rand_mutex); // Protecting "rand()"
            move = speed[player_id] * (static_cast<double>(rand()) / RAND_MAX * 2 * M_PI);
        }

        move = max(1.0, move); // Ensure at least 1 unit of movement

        // Power-up boosts movement
        {
            lock_guard<mutex> lock(rand_mutex); // Protecting "rand()" again
            if (static_cast<double>(rand()) / RAND_MAX < POWER_UP_CHANCE) {
                move += 10; // Power-up boosts movement
                cout << "Player " << player_id + 1 << " got a power-up! Moved " << static_cast<int>(move) << " units." << endl;
            }
        }

        // Apply the movement (race condition)
        positions[player_id][step] += static_cast<int>(move);

        // Random event check (slowdown)
        {
            lock_guard<mutex> lock(rand_mutex); // Protecting "rand()" again
            if (static_cast<double>(rand()) / RAND_MAX < EVENT_CHANCE) {
                move = max(1.0, move - 5); // Random event reduces movement
                cout << "Player " << player_id + 1 << " hit a slowdown! Moved " << static_cast<int>(move) << " units." << endl;
            }
        }

        // Print movement update
        cout << "Player " << player_id + 1 << " moved to position " << positions[player_id][step] << endl;

        // Check if the player reached the finish line
        if (positions[player_id][step] >= FINISH_LINE) {
            cout << "Player #" << player_id + 1 << " has reached the finish line!" << endl;
            return true; // This player won the race
        }
    }

    return false;
}

// Sequential main simulation with nested loops and longer input data
void race_simulation() {
    vector<vector<int>> positions(NUM_PLAYERS, vector<int>(GRID_SIZE, 0)); // RACE CONDITION 1: Shared positions

    vector<double> speed(NUM_PLAYERS); // RACE CONDITION 2: Shared speed values

    // Set random speed for each player
    #pragma omp parallel for
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        speed[i] = 1 + static_cast<double>(rand()) / RAND_MAX * 2; // Random speed between 1 and 3
    }

    int winner = -1;

    // Outer loop to simulate multiple rounds of the race
    for (int round_num = 0; round_num < GRID_SIZE; ++round_num) {
        cout << "Round " << round_num + 1 << " begins!" << endl;

        // Inner loop for player movements
        #pragma omp parallel for
        for (int player_id = 0; player_id < NUM_PLAYERS; ++player_id) {
            if (winner == -1) { // Only set the winner once
                if (calculate_movement(player_id, positions, speed)) {
                    winner = player_id;
                }
            }
        }

        if (winner != -1) {
            break;
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0))); // Seed random number generation RACE CONDITION 3: "rand()" is not thread-safe

    // Run the race simulation
    race_simulation();

    return 0;
}
