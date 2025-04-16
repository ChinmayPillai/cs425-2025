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
    cout << "Dest\tCost\tNext Hop\n";
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
            if (i == j) {
                nextHop[i][j] = -1;  // No next hop for self
            } else if (graph[i][j] != INF && graph[i][j] != 0) { 
                nextHop[i][j] = j;   // Direct link exists
            } else {
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
        
        for (int i = 0; i < n; ++i) {  // For each node i
            for (int j = 0; j < n; ++j) {  // For each destination j
                if (i == j) continue;  // Skip if source and destination are same
                
                for (int k = 0; k < n; ++k) {  // For each potential intermediate node k
                    if (i == k || j == k) continue;  // Skip if intermediate is source or destination
                    if (dist[i][k] == INF || dist[i][k] == 0) continue;  // Skip if no path to intermediate
                    if (dist[k][j] == INF || dist[k][j] == 0) continue;  // Skip if no path from intermediate to destination
                    
                    // Check if going through k is better than current route
                    int newDist = dist[i][k] + dist[k][j];
                    if (newDist < dist[i][j] || dist[i][j] == 0) {
                        dist[i][j] = newDist;
                        nextHop[i][j] = nextHop[i][k];  // Update next hop
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

    cout << "--- DVR Final Tables ---\n";
    for (int i = 0; i < n; ++i) printDVRTable(i, dist, nextHop);
}

void printLSRTable(int src, const vector<int>& dist, const vector<int>& prev) {
    cout << "Node " << src << " Routing Table:\n";
    cout << "Dest\tCost\tNext Hop\n";
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
        
        // Dijkstra's algorithm implementation
        for (int count = 0; count < n - 1; ++count) {
            // Find the vertex with minimum distance value
            int minDist = INF;
            int u = -1;
            
            for (int v = 0; v < n; ++v) {
                if (!visited[v] && dist[v] < minDist) {
                    minDist = dist[v];
                    u = v;
                }
            }
            
            // If no minimum was found, all remaining vertices are unreachable
            if (u == -1) break;
            
            // Mark the picked vertex as visited
            visited[u] = true;
            
            // Update dist value of adjacent vertices
            for (int v = 0; v < n; ++v) {
                // Update dist[v] only if:
                // 1. There is an edge from u to v
                // 2. The vertex v is not yet visited
                // 3. The path through u is shorter than current value of dist[v]
                if (!visited[v] && 
                    graph[u][v] != 0 && 
                    graph[u][v] != INF && 
                    dist[u] != INF && 
                    dist[u] + graph[u][v] < dist[v]) {
                    
                    dist[v] = dist[u] + graph[u][v];
                    prev[v] = u;  // Store the previous node
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

