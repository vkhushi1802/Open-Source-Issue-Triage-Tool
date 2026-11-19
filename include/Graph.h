
#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

// Represents a connection between two vertices
struct Edge {
    int destination;
    int weight;
};

// Represents a weighted graph
struct Graph {
    int vertices;

    vector<vector<Edge>> adjacencyList;

    Graph(int v) {
        vertices = v;
        adjacencyList.resize(v);
    }

    // Add an edge between two vertices
    void addEdge(int source, int destination, int weight) {

        adjacencyList[source].push_back(
            {destination, weight}
        );
    }


    void addUndirectedEdge(int source, int destination, int weight) {
    // Add an edge in both directions
    adjacencyList[source].push_back({destination, weight});
    adjacencyList[destination].push_back({source, weight});
}
};




struct MSTEdge {
    int source;
    int destination;
    int weight;
};


struct WeightedEdge {
    int source;
    int destination;
    int weight;
};

vector<int> dijkstra(Graph& graph, int source) {

    // Step 1: Store the shortest distance to every vertex
    vector<int> distance(graph.vertices, INT_MAX);

    // Step 2: Create a Min Priority Queue
    // Each entry contains {distance, vertex}
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > minHeap;

    // Step 3: Distance from source to itself is 0
    distance[source] = 0;

    minHeap.push({0, source});

    // Step 4: Process vertices
    while (!minHeap.empty()) {

        int currentDistance = minHeap.top().first;
        int currentVertex = minHeap.top().second;

        minHeap.pop();

        // Ignore outdated entries
        if (currentDistance > distance[currentVertex]) {
            continue;
        }

        // Step 5: Visit all neighbours
        for (int i = 0;
             i < graph.adjacencyList[currentVertex].size();
             i++) {

            Edge edge = graph.adjacencyList[currentVertex][i];

            int neighbour = edge.destination;
            int weight = edge.weight;

            // Step 6: Relaxation
            if (distance[currentVertex] != INT_MAX &&
                distance[currentVertex] + weight < distance[neighbour]) {

                distance[neighbour] =
                    distance[currentVertex] + weight;

                minHeap.push({
                    distance[neighbour],
                    neighbour
                });
            }
        }
    }

    return distance;
}



vector<int> shortestPath(Graph& graph, int source, int destination) {

    // Step 1: Store shortest distances
    vector<int> distance(graph.vertices, INT_MAX);

    // Step 2: Store the parent of every vertex
    vector<int> parent(graph.vertices, -1);

    // Step 3: Create Min Priority Queue
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > minHeap;

    distance[source] = 0;

    minHeap.push({0, source});

    // Step 4: Run Dijkstra
    while (!minHeap.empty()) {

        int currentDistance = minHeap.top().first;
        int currentVertex = minHeap.top().second;

        minHeap.pop();

        if (currentDistance > distance[currentVertex]) {
            continue;
        }

        for (int i = 0;
             i < graph.adjacencyList[currentVertex].size();
             i++) {

            Edge edge = graph.adjacencyList[currentVertex][i];

            int neighbour = edge.destination;
            int weight = edge.weight;

            // Step 5: Check for a shorter path
            if (distance[currentVertex] != INT_MAX &&
                distance[currentVertex] + weight < distance[neighbour]) {

                distance[neighbour] =
                    distance[currentVertex] + weight;

                // Remember where we came from
                parent[neighbour] = currentVertex;

                minHeap.push({
                    distance[neighbour],
                    neighbour
                });
            }
        }
    }

    // Step 6: Check if destination is reachable
    if (distance[destination] == INT_MAX) {
        return {};
    }

    // Step 7: Reconstruct the path backwards
    vector<int> path;

    int current = destination;

    while (current != -1) {

        path.push_back(current);

        current = parent[current];
    }

    // Step 8: Reverse to get source-to-destination order
    reverse(path.begin(), path.end());

    return path;
}



vector<MSTEdge> primMST(Graph& graph) {

    int n = graph.vertices;

    // Stores the minimum cost needed to connect each vertex
    vector<int> key(n, INT_MAX);

    // Stores the parent of each vertex in the MST
    vector<int> parent(n, -1);

    // Tracks which vertices are already included in the MST
    vector<bool> inMST(n, false);

    // Min-heap: {edge weight, vertex}
    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > minHeap;

    // Start from vertex 0
    key[0] = 0;
    minHeap.push({0, 0});

    vector<MSTEdge> mstEdges;

    while (!minHeap.empty()) {

        int currentVertex = minHeap.top().second;
        minHeap.pop();

        // Ignore a vertex already included
        if (inMST[currentVertex]) {
            continue;
        }

        // Include this vertex in the MST
        inMST[currentVertex] = true;

        // If it has a parent, record the MST edge
        if (parent[currentVertex] != -1) {
            mstEdges.push_back({
                parent[currentVertex],
                currentVertex,
                key[currentVertex]
            });
        }

        // Explore all neighbouring vertices
        for (int i = 0;
             i < graph.adjacencyList[currentVertex].size();
             i++) {

            Edge edge = graph.adjacencyList[currentVertex][i];

            int neighbour = edge.destination;
            int weight = edge.weight;

            // Choose a cheaper connection if possible
            if (!inMST[neighbour] && weight < key[neighbour]) {

                key[neighbour] = weight;
                parent[neighbour] = currentVertex;

                minHeap.push({weight, neighbour});
            }
        }
    }

    // A connected graph with n vertices needs n-1 MST edges
    if (mstEdges.size() != n - 1) {
        return {};
    }

    return mstEdges;
}



// Finds which group a vertex belongs to
int findParent(vector<int>& parent, int vertex) {

    if (parent[vertex] == vertex) {
        return vertex;
    }

    // Path compression
    parent[vertex] = findParent(parent, parent[vertex]);

    return parent[vertex];
}


// Joins two different groups
void unionSets(vector<int>& parent, vector<int>& rank,
               int a, int b) {

    int parentA = findParent(parent, a);
    int parentB = findParent(parent, b);

    // Already in the same group
    if (parentA == parentB) {
        return;
    }

    // Attach the smaller-rank tree below the larger one
    if (rank[parentA] < rank[parentB]) {
        parent[parentA] = parentB;
    }
    else if (rank[parentA] > rank[parentB]) {
        parent[parentB] = parentA;
    }
    else {
        parent[parentB] = parentA;
        rank[parentA]++;
    }
}


vector<MSTEdge> kruskalMST(int vertices, vector<MSTEdge> edges) {

    // Step 1: Sort edges by increasing weight
    sort(edges.begin(), edges.end(),
         [](MSTEdge a, MSTEdge b) {
             return a.weight < b.weight;
         });

    // Step 2: Initially, every vertex is its own parent
    vector<int> parent(vertices);
    vector<int> rank(vertices, 0);

    for (int i = 0; i < vertices; i++) {
        parent[i] = i;
    }

    vector<MSTEdge> result;

    // Step 3: Process edges from cheapest to most expensive
    for (int i = 0; i < edges.size(); i++) {

        MSTEdge current = edges[i];

        int parentA = findParent(parent, current.source);
        int parentB = findParent(parent, current.destination);

        // Step 4: Only add the edge if it doesn't create a cycle
        if (parentA != parentB) {

            result.push_back(current);

            // Merge the two groups
            unionSets(parent, rank,
                      current.source, current.destination);
        }

        // A spanning tree has exactly V - 1 edges
        if (result.size() == vertices - 1) {
            break;
        }
    }

    

    return result;
}



vector<int> bellmanFord(int vertices,
                        vector<WeightedEdge>& edges,
                        int source,
                        bool& negativeCycle) {

    // Initially, no negative cycle has been detected
    negativeCycle = false;

    // Step 1: Set all distances to infinity
    vector<int> distance(vertices, INT_MAX);

    // Source distance is zero
    distance[source] = 0;

    // Step 2: Relax all edges V-1 times
    for (int i = 0; i < vertices - 1; i++) {

        bool updated = false;

        for (int j = 0; j < edges.size(); j++) {

            int u = edges[j].source;
            int v = edges[j].destination;
            int weight = edges[j].weight;

            // We can only calculate from a reachable vertex
            if (distance[u] != INT_MAX &&
                distance[u] + weight < distance[v]) {

                distance[v] = distance[u] + weight;
                updated = true;
            }
        }

        // Stop early if no distance changed
        if (!updated) {
            break;
        }
    }

    // Step 3: Check for a reachable negative cycle
    for (int j = 0; j < edges.size(); j++) {

        int u = edges[j].source;
        int v = edges[j].destination;
        int weight = edges[j].weight;

        if (distance[u] != INT_MAX &&
            distance[u] + weight < distance[v]) {

            negativeCycle = true;
            break;
        }
    }

    return distance;
}








vector<vector<int>> floydWarshall(int vertices,
                                  vector<vector<int>> distance) {

    // Try every vertex as an intermediate vertex
    for (int k = 0; k < vertices; k++) {

        // Starting vertex
        for (int i = 0; i < vertices; i++) {

            // Destination vertex
            for (int j = 0; j < vertices; j++) {

                // Check whether both parts of the path exist
                if (distance[i][k] != INT_MAX &&
                    distance[k][j] != INT_MAX) {

                    // Calculate the distance through k
                    int newDistance =
                        distance[i][k] + distance[k][j];

                    // Update if the new path is shorter
                    if (newDistance < distance[i][j]) {
                        distance[i][j] = newDistance;
                    }
                }
            }
        }
    }

    return distance;
}

















// Finds an available developer for an issue
bool findMatching(int issue,
                  vector<vector<int>>& adj,
                  vector<bool>& visited,
                  vector<int>& developerMatch) {

    // Check all developers compatible with this issue
    for (int i = 0; i < adj[issue].size(); i++) {

        int developer = adj[issue][i];

        // Don't visit the same developer twice in one search
        if (visited[developer]) {
            continue;
        }

        visited[developer] = true;

        // If developer is free, assign them
        // OR try to move their previous issue to another developer
        if (developerMatch[developer] == -1 ||
            findMatching(developerMatch[developer],
                         adj, visited, developerMatch)) {

            developerMatch[developer] = issue;
            return true;
        }
    }

    return false;
}





// Finds maximum bipartite matching
int maximumBipartiteMatching(int issues,
                             int developers,
                             vector<vector<int>>& adj) {

    // Stores which issue is assigned to each developer
    // -1 means the developer is currently unassigned
    vector<int> developerMatch(developers, -1);

    int matchingCount = 0;

    // Try to assign every issue
    for (int i = 0; i < issues; i++) {

        // Fresh visited array for each issue
        vector<bool> visited(developers, false);

        if (findMatching(i, adj, visited, developerMatch)) {
            matchingCount++;
        }
    }

    // Display final assignments
    cout << "\nMaximum Bipartite Matching:\n";

    for (int i = 0; i < developers; i++) {

        if (developerMatch[i] != -1) {
            cout << "Issue " << developerMatch[i]
                 << " assigned to Developer " << i << endl;
        }
    }

    return matchingCount;
}



















bool findAugmentingPath(vector<vector<int>>& residual,
                        int source,
                        int sink,
                        vector<int>& parent,
                        int vertices) {

    vector<bool> visited(vertices, false);

    queue<int> q;

    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {

        int current = q.front();
        q.pop();

        for (int next = 0; next < vertices; next++) {

            // Check if there is remaining capacity
            // and the vertex has not been visited
            if (!visited[next] && residual[current][next] > 0) {

                visited[next] = true;
                parent[next] = current;

                // We reached the destination
                if (next == sink) {
                    return true;
                }

                q.push(next);
            }
        }
    }

    return false;
}
int edmondsKarp(int vertices,
                vector<vector<int>> capacity,
                int source,
                int sink) {

    // Initially, residual capacity equals original capacity
    vector<vector<int>> residual = capacity;

    vector<int> parent(vertices);

    int maximumFlow = 0;

    // Continue while an augmenting path exists
    while (findAugmentingPath(
        residual, source, sink, parent, vertices)) {

        // Find the minimum residual capacity
        // along the selected path
        int pathFlow = INT_MAX;

        int current = sink;

        while (current != source) {

            int previous = parent[current];

            pathFlow = min(
                pathFlow,
                residual[previous][current]
            );

            current = previous;
        }

        // Update residual capacities
        current = sink;

        while (current != source) {

            int previous = parent[current];

            // Reduce forward capacity
            residual[previous][current] -= pathFlow;

            // Increase reverse capacity
            residual[current][previous] += pathFlow;

            current = previous;
        }

        maximumFlow += pathFlow;
    }

    return maximumFlow;
}