# Routing Protocols Assignment

## Features Implemented
- Distance Vector Routing (DVR) algorithm with iteration-by-iteration output
- Link State Routing (LSR) algorithm using Dijkstra's shortest path algorithm
- Input parsing from adjacency matrix files
- Proper handling of special values (0 for no self-cost, 9999 for unreachable links)
- Complete routing table generation showing destination, cost, and next hop
- DVR convergence detection (termination when no further updates occur)

## Design Decisions

### DVR Algorithm Implementation
- We chose to implement the Bellman-Ford approach for the Distance Vector Routing algorithm, which iteratively updates routing tables until convergence.
- We decided to print DVR tables after each iteration to visualize the convergence process, which helps in understanding how the algorithm works.
- For handling unreachable nodes, we maintain the special value (9999) rather than traditional infinity to be consistent with the input format.

### LSR Algorithm Implementation
- We implemented Dijkstra's algorithm for Link State Routing with a straightforward approach using visited array rather than a priority queue.
- This decision was made for clarity and readability, though a priority queue implementation would have been more efficient for larger networks.
- We trace back through the predecessor array to determine the next hop for each destination, ensuring the routing tables show the correct path information.

### Code Structure
- We maintained the existing function structure to keep the code organized and clear.
- We added detailed comments to explain the algorithm steps and complex operations.
- We included error handling for file operations to ensure robustness.

## Implementation

### High-Level Function Overview

#### `simulateDVR(const vector<vector<int>>& graph)`
- Initializes distance and next hop matrices based on the input graph
- Iteratively updates routing tables using the Bellman-Ford algorithm principle
- Continues updates until no changes occur (convergence)
- Outputs routing tables after each iteration and the final state

#### `simulateLSR(const vector<vector<int>>& graph)`
- For each node as source, computes shortest paths to all other nodes
- Uses Dijkstra's algorithm to find optimal routes
- Tracks previous nodes to determine the path
- Outputs the final routing table for each node

#### `printDVRTable` and `printLSRTable`
- Format and display routing tables according to assignment specifications
- Handle special cases like self-routes and unreachable destinations

#### `readGraphFromFile`
- Parses the input file to construct the adjacency matrix
- Handles potential file access errors

### Code Flow

1. **Program Initialization**:
   - Parse command-line arguments
   - Read and construct the network graph from the input file

2. **DVR Simulation**:
   - Initialize routing tables with direct connections
   - Iteratively update tables based on information from neighbors
   - Print tables after each iteration
   - Terminate when no further updates occur

3. **LSR Simulation**:
   - For each node as source:
     - Run Dijkstra's algorithm to find shortest paths
     - Compute next hop information
     - Print the resulting routing table

4. **Program Termination**:
   - Clean up resources
   - Exit

## Testing

### Correctness Testing
- We verified correctness using the example input provided in the assignment description.
- We manually traced through the algorithms to ensure the routing tables matched expected outputs.
- We tested edge cases including:
  - Networks with unreachable nodes (9999 cost links)
  - Networks with varying sizes (small and medium-sized)

### Test Cases
- We created simple test networks with known shortest paths to verify algorithm correctness.
- We tested with symmetric and asymmetric cost matrices to ensure the algorithms handle directional links properly.
- We performed manual verification of next hop calculations to ensure routing tables were accurate.

## Restrictions
- The implementation assumes that the input graph follows the format specified in the assignment.
- The DVR algorithm might take longer to converge for larger networks or networks with specific topologies.
- The LSR implementation has O(n²) time complexity, which could be improved with a priority queue for larger networks.

## Challenges

### Algorithm Understanding
- Initially, we faced challenges in understanding how to properly update the next hop information in the DVR algorithm.
- We solved this by carefully studying the relationship between the distance matrix and next hop matrix, ensuring that when a path is updated, the next hop is updated accordingly.

### Convergence Detection
- Detecting when the DVR algorithm has converged was challenging as it required tracking whether any updates occurred in an iteration.
- We implemented a boolean flag to track updates and break the loop when no updates occur.

### Next Hop Calculation in LSR
- Computing the correct next hop for the LSR algorithm was difficult as it required tracing back through the predecessor array.
- We implemented a loop that traverses the predecessor array until finding a node directly connected to the source.

## Contribution of Each Member

| Member | Contribution |
|--------|--------------|
| Chinmay Pillai (200298) | 1/3 |
| Ashutosh Dwivedi (200214) | 1/3 |
| Shubham Kumar (200967) | 1/3 |

The team members have given equal contribution throughout the project. All the task inclusive but not limited to coding, understanding of code, creation of readme file etc. involves each team member at some point of time.

## Declaration

We declare that we did not indulge in plagiarism and all code has been written by our team members. Any references used have been properly cited in the Sources Referred section.

## Feedback

- The assignment provided a valuable hands-on experience with low-level networking concepts
- Examining the server code helped understand the expected client behavior
- It would be helpful to have additional guidance on debugging raw socket implementations


## Sources Referred

- Lecture notes