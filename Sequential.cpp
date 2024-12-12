#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>  // For random numbers
#include <ctime>    // For random seed

using namespace std;

// Constants
const int NUM_PLAYERS = 10;     // Number of players
const int FINISH_LINE = 200;   // Distance to the finish line
const int GRID_SIZE = 20;      // Simulated grid to represent positions
const double POWER_UP_CHANCE = 0.1;  // 10% chance for a power-up
const double EVENT_CHANCE = 0.2;     // 20% chance for a random event

// Function to simulate complex player movement
bool calculate_movement(int player_id, vector<vector<int> >& positions, vector<double>& speed) {
    for (int step = 0; step < GRID_SIZE; ++step) {
        // Complex movement calculation with floating-point operations
        double move = speed[player_id] * sin(static_cast<double>(rand()) / RAND_MAX * 2 * M_PI);
        move = max(1.0, move);  // Ensure at least 1 unit of movement

        // Power-up boost
        if (static_cast<double>(rand()) / RAND_MAX < POWER_UP_CHANCE) {
            move += 10;  // Power-up boosts movement
            cout << "Player " << player_id + 1 << " got a power-up! Moved " << static_cast<int>(move) << " units." << endl;
        }

        // Apply the movement
        positions[player_id][step] += static_cast<int>(move);

        // Random event check (slowdown)
        if (static_cast<double>(rand()) / RAND_MAX < EVENT_CHANCE) {
            positions[player_id][step] -= 5;  // Random event reduces movement
            cout << "Player " << player_id + 1 << " hit a slowdown!" << endl;
        }

        // Print movement update
        cout << "Player " << player_id + 1 << " moved to position " << positions[player_id][step] << endl;

        // Check if the player reached the finish line
        if (positions[player_id][step] >= FINISH_LINE) {
            cout << "Player " << player_id + 1 << " has reached the finish line!" << endl;
            return true;  // This player won the race
        }
    }
    return false;
}

// Sequential main simulation with nested loops and larger input data
void race_simulation() {
    // Initialize player positions and speeds
    vector<vector<int> > positions(NUM_PLAYERS, vector<int>(GRID_SIZE, 0));  // 2D array for each player's position
    vector<double> speed(NUM_PLAYERS);  // Random speed for each player

    // Set random speeds for each player
    for (int i = 0; i < NUM_PLAYERS; ++i) {
        speed[i] = 1 + static_cast<double>(rand()) / RAND_MAX * 2;  // Random speed between 1 and 3
    }

    int winner = -1;

    // Outer loop to simulate multiple rounds of the race
    for (int round_num = 0; round_num < GRID_SIZE; ++round_num) {
        cout << "--- Round " << round_num + 1 << " ---" << endl;

        // Inner loop for player movements
        for (int player_id = 0; player_id < NUM_PLAYERS; ++player_id) {
            if (calculate_movement(player_id, positions, speed)) {
                winner = player_id;
                break;
            }
        }

        if (winner != -1) {
            break;
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));  // Seed random number generator

    // Run the race simulation
    race_simulation();

return 0;
}
