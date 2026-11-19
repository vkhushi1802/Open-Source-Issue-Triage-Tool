
#include <iostream>
#include "CSVreader.h"
#include "Validator.h"
#include "Huffman.h"
#include "Graph.h"
#include <vector>
#include <climits>
#include "Assignment.h"

using namespace std;

int main() {

    vector<Issue> issues = loadIssues("data/issues.csv");

    if (hasDuplicateIssueIDs(issues)) {
    cout << "\nDuplicate Issue IDs found!" << endl;
}
else {
    cout << "\nNo duplicate Issue IDs found!" << endl;
}






    cout << "Total Issues Loaded: " << issues.size() << endl;

    for (int i = 0; i < issues.size(); i++) {
        cout << "\nIssue ID: " << issues[i].id << endl;
        cout << "Title: " << issues[i].title << endl;
        cout << "Priority: " << issues[i].priority << endl;
        cout << "Status: " << issues[i].status << endl;


         // Display all skills required for this issue
    cout << "Required Skills: ";

    for (const Skill& skill : issues[i].requiredskills) {
        cout << skill.name << " ";
    }

    cout << endl;
        
    }


    


    vector<Developer> developers = loadDevelopers("data/developers.csv");
    developers= loadDeveloperSkills("data/developer_skills.csv", developers);


cout << "\nTotal Developers Loaded: " << developers.size() << endl;

for (int i = 0; i < developers.size(); i++) {
    cout << "\nDeveloper ID: " << developers[i].id << endl;
    cout << "Name: " << developers[i].name << endl;
    cout << "Available Hours: " << developers[i].availHours << endl;
    cout << "Current Workload: " << developers[i].currentWorkload << endl;
    cout << "Experience: " << developers[i].experience << endl;
}

cout << "\n\n========== DEVELOPER SKILLS ==========" << endl;

for (int i = 0; i < developers.size(); i++) {

    cout << "\nDeveloper: " << developers[i].name << endl;

    for (int j = 0; j < developers[i].skills.size(); j++) {

        cout << "Skill: " << developers[i].skills[j].name << endl;
        cout << "Proficiency: " << developers[i].skills[j].proficiency << endl;
    }
}

vector<Dependency> dependencies = loadDependencies("data/dependencies.csv");

cout << "Total dependencies Loaded: " << dependencies.size() << endl;

    for (int i = 0; i < dependencies.size(); i++) {
        cout << "\nIssue ID: " << dependencies[i].issueId << endl;
        cout << "Depends on Issue ID: " << dependencies[i].dependsOnIssueId << endl;
    }



cout << "\n========== DEPENDENCY VALIDATION ==========" << endl;

// Validate dependencies before running any dependency-based algorithm.
bool dependenciesValid = validateDependencies(issues, dependencies);

if (dependenciesValid) {
    cout << "All dependencies are valid!" << endl;
}
else {
    cout << "Some dependencies are invalid!" << endl;
}

// Only check for cycles when all dependency IDs are valid.
// This prevents graph algorithms from accessing non-existent issue IDs.
bool circularDependency = false;

if (dependenciesValid) {
    circularDependency = hasCircularDependency(issues, dependencies);
}

cout << "\n========== DUPLICATE DETECTION ==========" << endl;

if (hasDuplicateIssueIDs(issues)) {
    cout << "Nested Loop: Duplicates found!" << endl;
}
else {
    cout << "Nested Loop: No duplicates found!" << endl;
}

if (hasDuplicateIssueIDsMergeSort(issues)) {
    cout << "Merge Sort: Duplicates found!" << endl;
}
else {
    cout << "Merge Sort: No duplicates found!" << endl;
}








// Generate dependency-aware priority order only when dependency data is valid
// and does not contain a cycle.
vector<int> issueOrder;
vector<Assignment> assignments;

if (dependenciesValid && !circularDependency) {

    issueOrder = greedyIssueOrder(issues, dependencies);

    // Assign issues to suitable developers
    assignments = assignDevelopers(issues, developers, issueOrder);
}
else {
    cout << "\nAssignment skipped because dependency data is invalid "
         << "or contains a circular dependency." << endl;
}


















cout << "\n========== CYCLE DETECTION ==========" << endl;

if (!dependenciesValid) {
    cout << "Cycle detection skipped because dependency data is invalid."
         << endl;
}
else if (circularDependency) {
    cout << "Circular dependency detected!" << endl;
}
else {
    cout << "No circular dependencies found!" << endl;
}



cout << "\n========== TOPOLOGICAL SORTING ==========" << endl;

vector<int> executionOrder;

if (dependenciesValid && !circularDependency) {
    executionOrder = topologicalSort(issues, dependencies);

    if (!executionOrder.empty()) {

        cout << "Recommended Issue Execution Order:" << endl;

        for (int i = 0; i < executionOrder.size(); i++) {

            cout << i + 1 << ". Issue "
                 << executionOrder[i] << endl;
        }
    }
    else {
        cout << "Unable to generate topological order." << endl;
    }
}
else {
    cout << "Topological sorting skipped because dependency data "
         << "is invalid or contains a cycle." << endl;
}



cout << "\n========== GREEDY ISSUE PRIORITIZATION ==========" << endl;

if (dependenciesValid && !circularDependency && !issueOrder.empty()) {

    cout << "Greedy Recommended Execution Order:" << endl;

    for (int i = 0; i < issueOrder.size(); i++) {

        int id = issueOrder[i];

        for (int j = 0; j < issues.size(); j++) {

            if (issues[j].id == id) {

                cout << i + 1 << ". Issue " << id
                     << " | " << issues[j].title
                     << " | Priority: " << issues[j].priority
                     << " | Hours: " << issues[j].estimatedHours
                     << endl;

                break;
            }
        }
    }
}
else {
    cout << "Unable to generate order. Check dependencies!" << endl;
}



cout << "\n========== ACTIVITY SELECTION ==========" << endl;

vector<Activity> activities = {
    {101, 9, 11},
    {102, 10, 12},
    {103, 11, 13},
    {104, 12, 14},
    {105, 13, 15},
    {106, 14, 16}
};

vector<Activity> selectedActivities =
    activitySelection(activities);

cout << "Selected non-overlapping activities:" << endl;

for (int i = 0; i < selectedActivities.size(); i++) {

    cout << "Issue " << selectedActivities[i].issueId
         << " | Start: " << selectedActivities[i].startTime
         << ":00 | End: " << selectedActivities[i].endTime
         << ":00" << endl;
}

cout << "Total selected activities: "
     << selectedActivities.size() << endl;





cout << "\n========== INTERVAL PARTITIONING ==========" << endl;

vector<Activity> schedulingActivities = {
    {101, 9, 12},
    {102, 10, 13},
    {103, 11, 14},
    {104, 12, 15},
    {105, 13, 16}
};

vector<vector<Activity>> developerSchedules =
    intervalPartitioning(schedulingActivities);

for (int i = 0; i < developerSchedules.size(); i++) {

    cout << "\nDeveloper " << i + 1 << ":" << endl;

    for (int j = 0; j < developerSchedules[i].size(); j++) {

        cout << "Issue " << developerSchedules[i][j].issueId
             << " | Start: " << developerSchedules[i][j].startTime
             << ":00 | End: " << developerSchedules[i][j].endTime
             << ":00" << endl;
    }
}

cout << "\nMinimum developers required: "
     << developerSchedules.size() << endl;





// 0/1 Knapsack - Resource Allocation

int availableHours = 10;

vector<int> selectedIssueIDs;

int maxPriority = knapsack(
    issues,
    availableHours,
    selectedIssueIDs
);

cout << "\n========== KNAPSACK ==========" << endl;

cout << "Available Hours: "
     << availableHours << endl;

cout << "\nSelected Issues:" << endl;

for (int i = 0; i < selectedIssueIDs.size(); i++) {

    for (int j = 0; j < issues.size(); j++) {

        if (selectedIssueIDs[i] == issues[j].id) {

            cout << "Issue ID: " << issues[j].id
                 << " | " << issues[j].title
                 << " | Hours: " << issues[j].estimatedHours
                 << " | Priority: " << issues[j].priority
                 << endl;
        }
    }
}

cout << "\nMaximum Total Priority: "
     << maxPriority << endl;



string sampleText = "AAAABBC";

HuffmanNode* root = buildHuffmanTree(sampleText);

cout << "\nHuffman Tree built successfully!" << endl;

if (root != nullptr) {
    cout << "Root frequency: " << root->frequency << endl;
}


string codes[256];

generateHuffmanCodes(root, "", codes);

cout << "\nHuffman Codes:" << endl;

for (int i = 0; i < 256; i++) {

    if (!codes[i].empty()) {

        cout << (char)i << " : " << codes[i] << endl;
    }
}


string encodedText = encodeText(sampleText, codes);

cout << "\nOriginal Text: " << sampleText << endl;

cout << "Encoded Text: " << encodedText << endl;

cout << "Original Size: "
     << sampleText.length() * 8 << " bits" << endl;

cout << "Compressed Size: "
     << encodedText.length() << " bits" << endl;





string decodedText = decodeText(encodedText, root);

cout << "\nDecoded Text: " << decodedText << endl;

if (decodedText == sampleText) {
    cout << "Decoding successful! Original text recovered."
         << endl;
}
else {
    cout << "Decoding failed!" << endl;
}




// Create a graph with 5 vertices
Graph graph(5);

// Add directed edges
graph.addEdge(0, 1, 4);
graph.addEdge(0, 2, 2);
graph.addEdge(2, 1, 1);
graph.addEdge(1, 3, 5);
graph.addEdge(2, 3, 8);
graph.addEdge(3, 4, 2);

// Run Dijkstra starting from vertex 0
vector<int> distances = dijkstra(graph, 0);

cout << "\nDijkstra Shortest Distances:\n";

for (int i = 0; i < distances.size(); i++) {

    cout << "Vertex " << i << " : ";

    if (distances[i] == INT_MAX) {
        cout << "Unreachable";
    }
    else {
        cout << distances[i];
    }

    cout << endl;
}


vector<int> path = shortestPath(graph, 0, 4);

cout << "\nShortest Path from 0 to 4: ";

if (path.empty()) {
    cout << "No path exists.";
}
else {
    for (int i = 0; i < path.size(); i++) {

        cout << path[i];

        if (i != path.size() - 1) {
            cout << " -> ";
        }
    }
}

cout << endl;



cout << "\n--- Prim's Minimum Spanning Tree ---\n";

Graph primGraph(5);

// Add undirected weighted edges
primGraph.addUndirectedEdge(0, 1, 2);
primGraph.addUndirectedEdge(0, 3, 6);
primGraph.addUndirectedEdge(1, 2, 3);
primGraph.addUndirectedEdge(1, 3, 8);
primGraph.addUndirectedEdge(1, 4, 5);
primGraph.addUndirectedEdge(2, 4, 7);
primGraph.addUndirectedEdge(3, 4, 9);

vector<MSTEdge> mst = primMST(primGraph);

int totalCost = 0;

if (mst.empty()) {
    cout << "MST could not be created. Graph may be disconnected.\n";
}
else {
    cout << "Edges in Minimum Spanning Tree:\n";

    for (int i = 0; i < mst.size(); i++) {

        cout << mst[i].source
             << " -- "
             << mst[i].destination
             << " : "
             << mst[i].weight << endl;

        totalCost += mst[i].weight;
    }

    cout << "Total MST Cost: " << totalCost << endl;
}


vector<MSTEdge> edges = {
    {0, 1, 4},
    {0, 2, 2},
    {1, 2, 1},
    {1, 3, 5},
    {2, 3, 8},
    {3, 4, 2},
    {2, 4, 10}
};

vector<MSTEdge> kruskalResult = kruskalMST(5, edges);
int kruskalTotalCost = 0;


cout << "\nKruskal's Minimum Spanning Tree:\n";

for (int i = 0; i < kruskalResult.size(); i++) {

    cout << kruskalResult[i].source << " - "
         << kruskalResult[i].destination << " : "
         << kruskalResult[i].weight << endl;

    kruskalTotalCost += kruskalResult[i].weight;
}

cout << "Kruskal Total MST Cost: "
     << kruskalTotalCost << endl;



     // Bellman-Ford Test

vector<WeightedEdge> bellmanEdges = {
    {0, 1, 4},
    {0, 2, 5},
    {1, 2, -2},
    {1, 3, 6},
    {2, 3, 3},
    {3, 4, 2}
};

bool negativeCycle = false;

vector<int> bellmanDistances =
    bellmanFord(5, bellmanEdges, 0, negativeCycle);

cout << "\nBellman-Ford Shortest Distances:\n";

if (negativeCycle) {

    cout << "Negative cycle detected!\n";

}
else {

    for (int i = 0; i < bellmanDistances.size(); i++) {

        cout << "Vertex " << i << " : ";

        if (bellmanDistances[i] == INT_MAX) {
            cout << "Unreachable";
        }
        else {
            cout << bellmanDistances[i];
        }

        cout << endl;
    }
}



// Floyd-Warshall Test

vector<vector<int>> distanceMatrix = {
    {0,   3,   INT_MAX, 7},
    {8,   0,   2,       INT_MAX},
    {5,   INT_MAX, 0,   1},
    {2,   INT_MAX, INT_MAX, 0}
};

vector<vector<int>> shortestDistances =
    floydWarshall(4, distanceMatrix);

cout << "\nFloyd-Warshall Shortest Distance Matrix:\n";

for (int i = 0; i < shortestDistances.size(); i++) {

    for (int j = 0; j < shortestDistances[i].size(); j++) {

        if (shortestDistances[i][j] == INT_MAX) {
            cout << "INF ";
        }
        else {
            cout << shortestDistances[i][j] << "   ";
        }
    }

    cout << endl;
}

// Check for negative cycles
bool hasNegativeCycle = false;

for (int i = 0; i < shortestDistances.size(); i++) {

    if (shortestDistances[i][i] < 0) {
        hasNegativeCycle = true;
        break;
    }
}

if (hasNegativeCycle) {
    cout << "Negative cycle detected!\n";
}
else {
    cout << "No negative cycle detected!\n";
}


// Bipartite Matching Test

int issueCount = 3;
int developerCount = 3;

// Each inner vector contains compatible developer indices
vector<vector<int>> matchingGraph = {
    {0, 2},  // Issue 0 can be assigned to Developer 0 or 2
    {1},     // Issue 1 can be assigned to Developer 1
    {0, 2}   // Issue 2 can be assigned to Developer 0 or 2
};

int maximumMatching = maximumBipartiteMatching(
    issueCount,
    developerCount,
    matchingGraph
);

cout << "Total assignments: " << maximumMatching << endl;









// Maximum Flow Test

int flowVertices = 6;

vector<vector<int>> capacity = {

    //  0   1   2   3   4   5
    {   0, 16, 13,  0,  0,  0 }, // 0
    {   0,  0, 10, 12,  0,  0 }, // 1
    {   0,  4,  0,  0, 14,  0 }, // 2
    {   0,  0,  9,  0,  0, 20 }, // 3
    {   0,  0,  0,  7,  0,  4 }, // 4
    {   0,  0,  0,  0,  0,  0 }  // 5
};

int source = 0;
int sink = 5;

int maximumFlow = edmondsKarp(
    flowVertices, capacity, source, sink
);

cout << "\nMaximum Flow: " << maximumFlow << endl;










cout << "\n========== DEVELOPER ASSIGNMENT REPORT ==========\n";


for (const Assignment& assignment : assignments) {

    cout << "\nIssue ID: " << assignment.issueId;

    if (assignment.developerId == -1) {
        cout << "\nStatus: " << assignment.status;
    }
    else {
        string developerName = "Unknown";

        // Find the developer's name using their ID
        for (const Developer& developer : developers) {
            if (developer.id == assignment.developerId) {
                developerName = developer.name;
                break;
            }
        }

        cout << "\nDeveloper: " << developerName;
        cout << "\nDeveloper ID: " << assignment.developerId;
        cout << "\nAssigned Hours: " << assignment.assignedHours;
        cout << "\nStatus: " << assignment.status;
    }

    cout << "\nReason: " << assignment.reason << "\n";
}



// Calculate assignment statistics
int totalIssues = issues.size();
int assignedCount = 0;
int unassignedCount = 0;

for (const Assignment& assignment : assignments) {
    if (assignment.developerId != -1) {
        assignedCount++;
    }
}

// If assignment was skipped, the loaded issues have not been evaluated
// by the assignment engine. Otherwise, count the issues it left unassigned.
if (dependenciesValid && !circularDependency) {
    unassignedCount = totalIssues - assignedCount;
}
else {
    unassignedCount = 0;
}

// Calculate assignment success percentage
double successPercentage = 0.0;

if (totalIssues > 0) {
    successPercentage =
        (static_cast<double>(assignedCount) / totalIssues) * 100;
}

// Display assignment summary
cout << "\n========== ASSIGNMENT SUMMARY ==========\n";

cout << "Total Issues: " << totalIssues << "\n";

if (dependenciesValid && !circularDependency) {
    cout << "Successfully Assigned: " << assignedCount << "\n";
    cout << "Unassigned Issues: " << unassignedCount << "\n";

    cout << "Assignment Success Rate: "
         << successPercentage << "%\n";
}
else {
    cout << "Assignment was not performed because dependency data "
         << "is invalid or contains a cycle.\n";
}



     
cout << "\n========== DEVELOPER WORKLOAD REPORT ==========\n";

for (const Developer& developer : developers) {

    int newlyAssignedHours = 0;

    // Calculate hours assigned to this developer
    for (const Assignment& assignment : assignments) {
        if (assignment.developerId == developer.id) {
            newlyAssignedHours += assignment.assignedHours;
        }
    }

    int totalWorkload = developer.currentWorkload + newlyAssignedHours;

    int remainingCapacity = developer.availHours - totalWorkload;

    cout << "\nDeveloper: " << developer.name;
    cout << "\nDeveloper ID: " << developer.id;
    cout << "\nPreviously Assigned Hours: " << developer.currentWorkload;
    cout << "\nNewly Assigned Hours: " << newlyAssignedHours;
    cout << "\nTotal Workload: " << totalWorkload;
    cout << "\nRemaining Capacity: " << remainingCapacity << " hours\n";
}


    return 0;
}


