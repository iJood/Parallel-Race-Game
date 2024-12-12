# Parallel Race Game

## Project Overview
The **Parallel Race Game** is a dynamic racing simulation that leverages **parallel computing** concepts to enhance performance and introduce real-time player decision-making. Developed using **C++** with **OpenMP**, this project explores multithreading and synchronization techniques to tackle race conditions and improve execution efficiency.

## Key Features
- **Parallelized Player Movements**: Players move independently with random events, ensuring unpredictability and engagement.
- **Synchronization Techniques**: Implements critical sections, atomic operations, and reduction directives to address race conditions.
- **Performance Analysis**: Benchmarks execution times across varying thread counts (1, 2, 4, 8 cores).

## Technologies Used
- **Programming Language**: C++
- **Parallel Computing Framework**: OpenMP
- **Benchmarking Tools**: Execution time analysis

## Project Objectives
1. Develop a racing simulation leveraging parallel computing principles.
2. Identify and resolve race conditions arising from shared resources.
3. Analyze the trade-offs of various synchronization techniques.

## Performance Results
The project tested three synchronization techniques:
1. **Critical Sections**: Eliminates race conditions but incurs high overhead at increased thread counts.
2. **Atomic Operations**: Lightweight synchronization with moderate efficiency at lower thread counts.
3. **Reduction Directives**: Best performance across thread counts with minimal synchronization overhead.

### Benchmark Results
| Synchronization Method | 1 Thread | 2 Threads | 4 Threads | 8 Threads |
|-------------------------|----------|-----------|-----------|-----------|
| No Synchronization      | 4262 ms  | 3941 ms   | 3258 ms   | 3096 ms   |
| Critical Sections       | 1.688 ms | 1.477 ms  | 1.215 ms  | 15.810 ms |
| Atomic Operations       | 1.606 ms | 1.203 ms  | 1.528 ms  | 7.631 ms  |
| Reduction Clauses       | 3.101 ms | 2.181 ms  | 1.836 ms  | 13.225 ms |

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/iJood/parallel-race-game.git
   ```
2. Navigate to the project directory:
   ```bash
   cd parallel-race-game
   ```
3. Compile the code:
   ```bash
   g++ -fopenmp -o parallel_race parallel_race.cpp
   ```
4. Run the program:
   ```bash
   ./parallel_race
   ```

## Usage
The simulation allows users to:
- Configure the number of players and threads.
- Observe real-time race outcomes and performance metrics.

## Contributing
Contributions are welcome! Please follow these steps:
1. Fork the repository.
2. Create a feature branch:
   ```bash
   git checkout -b feature-name
   ```
3. Commit your changes:
   ```bash
   git commit -m 'Add a feature'
   ```
4. Push the branch:
   ```bash
   git push origin feature-name
   ```
5. Open a Pull Request.

## Authors
- Jood Alotaibi
- Amjad AlKhalifa
- Sara AlDossary
- Jood AlGhamdi
- Layan AlNahdi
- Maryam AlMutawah

## License
This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgments
Special thanks to Dr. Rabab AlKhalifa for guidance and mentorship.

