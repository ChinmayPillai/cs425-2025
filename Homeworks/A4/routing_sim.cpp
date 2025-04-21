#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

const int INF = 9999;

void printDVRTable(int node, const vector<vector<int>>& table, const vector<vector<int>>& nextHop) {
    cout << "Node " << node << " Routing Table:\n";
    cout << "Dest\tMetric\tNext Hop\n";
    for (int i = 0; i < table.size(); ++i) {
        cout << i << "\t" << table[node][i] << "\t";
        if (nextHop[node][i] == -1) cout << "-";
        else cout << nextHop[node][i];
        cout << endl;
    }
    cout << endl;
}

void simulateDVR(const vector<vector<int>>& graph) {
    int n = graph.size();
    vector<vector<int>> dist = graph;
    vector<vector<int>> nextHop(n, vector<int>(n));
    
    // Initialize the nextHop matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {

            // If j is same node as i
            if (i == j) {
                // No next hop to itself
                nextHop[i][j] = -1;  
            }
            // If there is a reachable link from i to j 
            else if (graph[i][j] != INF && graph[i][j] != 0) { 
                nextHop[i][j] = j;   
            } 
            // If there is no reachable link from i to j
            else {
                nextHop[i][j] = -1;  // No direct link
            }
        }
    }
    
    // Run the DVR algorithm until no updates are made
    bool updated;
    int iterations = 0;
    
    do {
        updated = false;
        iterations++;
        
        // For each node i
        for (int i = 0; i < n; ++i) {  
            // For each destination j
            for (int j = 0; j < n; ++j) {  
                // Skip if source and destination are same
                if (i == j) 
                    continue; 

                // For each potential intermediate node k
                for (int k = 0; k < n; ++k) { 

                    // Skip if intermediate is source or destination 
                    if (i == k || j == k) 
                        continue;  
                    
                    // Skip if no path to intermediate
                    if (dist[i][k] == INF || dist[i][k] == 0) 
                        continue;  
                    // Skip if no path from intermediate to destination
                    if (dist[k][j] == INF || dist[k][j] == 0) 
                        continue;  
                    
                    // Check if going through k is better than current route
                    // Note: dist[i][j] == 0 means no link exists since i != j
                    int newDist = dist[i][k] + dist[k][j];
                    if (newDist < dist[i][j] || dist[i][j] == 0) {
                        // Update distance and next hop
                        dist[i][j] = newDist;
                        nextHop[i][j] = nextHop[i][k];  
                        updated = true;
                    }
                }
            }
        }
        
        // Print DVR tables after each iteration
        if (updated) {
            cout << "--- DVR Iteration " << iterations << " ---\n";
            for (int i = 0; i < n; ++i) {
                printDVRTable(i, dist, nextHop);
            }
        }
    } while (updated);

    // cout << "--- DVR Final Tables ---\n";
    for (int i = 0; i < n; ++i) printDVRTable(i, dist, nextHop);
}

void printLSRTable(int src, const vector<int>& dist, const vector<int>& prev) {
    cout << "Node " << src << " Routing Table:\n";
    cout << "Dest\tMetric\tNext Hop\n";
    for (int i = 0; i < dist.size(); ++i) {
        if (i == src) continue;
        cout << i << "\t" << dist[i] << "\t";
        int hop = i;
        while (prev[hop] != src && prev[hop] != -1)
            hop = prev[hop];
        cout << (prev[hop] == -1 ? -1 : hop) << endl;
    }
    cout << endl;
}

void simulateLSR(const vector<vector<int>>& graph) {
    int n = graph.size();
    for (int src = 0; src < n; ++src) {
        vector<int> dist(n, INF);
        vector<int> prev(n, -1);
        vector<bool> visited(n, false);
        dist[src] = 0;
        
        // Optimized Dijkstra's algorithm using a priority queue

        // Create a min-heap via priority queue to store {distance, node}
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        // Push the {distance = 0, source node} into the priority queue
        pq.push({0, src});
        // Set distance of source node to 0
        dist[src] = 0;

        // While the priority queue is not empty, i.e. processable nodes exist
        while (!pq.empty()) {

            // Get the node with the smallest distance from top of min-heap
            int u = pq.top().second;
            // Remove the corresponding element from the priority queue
            pq.pop();

            // If the node has already been visited, skip it
            if (visited[u]) 
                continue;
            // Else mark it as visited
            visited[u] = true;

            // For each neighbor of the current node
            for (int v = 0; v < n; ++v) {
                // Skip if same node
                if (u == v) 
                    continue;  
                
                // If there is a link from u to v which is reachable(not infinite) 
                // and v is not visited yet
                if (graph[u][v] != 0 && graph[u][v] != INF && !visited[v]) {

                    // Calculate distance to v through u
                    int newDist = dist[u] + graph[u][v];
                    // If the new distance is less than the current known distance to v
                    if (newDist < dist[v]) {
                        // Update the distance and previous node
                        // And add the {newDistance, v} pair to the priority queue to process
                        dist[v] = newDist;
                        prev[v] = u;
                        pq.push({newDist, v});
                    }
                }
            }
        }
        
        printLSRTable(src, dist, prev);
    }
}

vector<vector<int>> readGraphFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }
    
    int n;
    file >> n;
    vector<vector<int>> graph(n, vector<int>(n));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> graph[i][j];

    file.close();
    return graph;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    string filename = argv[1];
    vector<vector<int>> graph = readGraphFromFile(filename);

    cout << "\n--- Distance Vector Routing Simulation ---\n";
    simulateDVR(graph);

    cout << "\n--- Link State Routing Simulation ---\n";
    simulateLSR(graph);

    return 0;
}

