#include "Validator.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <queue>

using namespace std;



bool hasDuplicateIssueIDs(const std::vector<Issue>& issues) {

    for (int i = 0; i < issues.size(); i++) {

        for (int j = i+1; j < issues.size(); j++) {

            if (issues[i].id == issues[j].id) {
                return true;
            }
        }
    }

    return false;
}






void merge(vector<int> &arr, int left, int mid, int right){
    vector<int>temp;

    int i=left;
    int j=mid+1;

    while(i<= mid && j<=right){

        if(arr[i] <= arr[j]){
            temp.push_back(arr[i]);
            i++;
        }
        else{
            temp.push_back(arr[j]);
            j++;
        }
    }

    while(i<= mid){
        temp.push_back(arr[i]);
        i++;
    }

    while(j<=right){
        temp.push_back(arr[j]);
        j++;
    }


        // Copy sorted elements back into the original array
    for (int k = 0; k < temp.size(); k++) {
        arr[left + k] = temp[k];
    }
}


void mergeSort(vector<int>& arr, int left, int right){
    if(left>=right){
        return;
    }

    int mid = left+ (right-left)/2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid+1, right);

    merge(arr, left, mid, right);
}
bool hasDuplicateIssueIDsMergeSort(const std::vector<Issue>& issues) {

    vector<int> issueIDs;

    for(int i=0; i<issues.size(); i++){
        issueIDs.push_back(issues[i].id);
    }

    // Sort the IDs
    if (!issueIDs.empty()) {
        mergeSort(issueIDs, 0, issueIDs.size() - 1);
    }

    // Check neighbouring elements
    for (int i = 0; i+1 < issueIDs.size(); i++) {

        if (issueIDs[i] == issueIDs[i + 1]) {
            return true;
        }
    }

    return false;

}







// Binary Search - O(log n)
// The vector must already be sorted.

bool binarySearchIssueID(const vector<int>& issueIDs, int target) {

    int left = 0;
int right = issueIDs.empty() ? -1 : issueIDs.size() - 1;

    while (left <= right) {

        int mid = left + (right - left) / 2;

        if (issueIDs[mid] == target) {
            return true;
        }

        else if (issueIDs[mid] < target) {
            left = mid + 1;
        }

        else {
            right = mid - 1;
        }
    }

    return false;
}



bool validateDependencies(
    const vector<Issue>& issues,
    const vector<Dependency>& dependencies
) {

    // Extract all issue IDs
    vector<int> issueIDs;

    for (int i = 0; i < issues.size(); i++) {
        issueIDs.push_back(issues[i].id);
    }

    // Sort issue IDs before Binary Search
    sort(issueIDs.begin(), issueIDs.end());

    bool allValid = true;

    // Check every dependency
    for (int i = 0; i < dependencies.size(); i++) {

        int issueID = dependencies[i].issueId;
        int prerequisiteID = dependencies[i].dependsOnIssueId;

        // Check whether both issues exist
        bool issueExists = binarySearchIssueID(issueIDs, issueID);
        bool prerequisiteExists = binarySearchIssueID(issueIDs, prerequisiteID);

        if (!issueExists) {
            cout << "Invalid: Issue " << issueID << " does not exist!" << endl;
            allValid = false;
        }

        if (!prerequisiteExists) {
            cout << "Invalid: Prerequisite issue " << prerequisiteID
                 << " does not exist!" << endl;
            allValid = false;
        }

        // Check self-dependency
        if (issueID == prerequisiteID) {
            cout << "Invalid: Issue " << issueID
                 << " cannot depend on itself!" << endl;
            allValid = false;
        }
    }



    return allValid;
}




bool hasCircularDependency(
    const vector<Issue>& issues,
    const vector<Dependency>& dependencies
) {
    // Step 1: Create a graph
    unordered_map<int, vector<int>> graph;

    for (int i = 0; i < dependencies.size(); i++) {
        graph[dependencies[i].issueId].push_back(
            dependencies[i].dependsOnIssueId
        );
    }

    // Step 2: Track the visiting state of every issue
    // 0 = Not visited
    // 1 = Currently visiting
    // 2 = Completely visited

    unordered_map<int, int> visited;

    // Step 3: Create DFS function
    function<bool(int)> dfs = [&](int node) {

        visited[node] = 1;

        // Explore all neighbouring issues
        for (int neighbour : graph[node]) {

            // If neighbour is already being explored,
            // a cycle exists
            if (visited[neighbour] == 1) {
                return true;
            }

            // If neighbour has not been visited,
            // explore it
            if (visited[neighbour] == 0) {

                if (dfs(neighbour)) {
                    return true;
                }
            }
        }

        // Mark current issue as completely visited
        visited[node] = 2;

        return false;
    };

    // Step 4: Run DFS for every issue
    for (int i = 0; i < issues.size(); i++) {

        int id = issues[i].id;

        if (visited[id] == 0) {

            if (dfs(id)) {
                return true;
            }
        }
    }

    return false;
}



vector<int> topologicalSort(
    const vector<Issue>& issues,
    const vector<Dependency>& dependencies
) {
    // Step 1: Create graph and indegree map

    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> indegree;

    // Initially, every issue has indegree 0
    for (int i = 0; i < issues.size(); i++) {
        indegree[issues[i].id] = 0;
    }

    // Step 2: Build graph
    // Edge direction: Prerequisite -> Dependent issue

    for (int i = 0; i < dependencies.size(); i++) {

        int issueID = dependencies[i].issueId;
        int prerequisiteID = dependencies[i].dependsOnIssueId;

        graph[prerequisiteID].push_back(issueID);

        indegree[issueID]++;
    }

    // Step 3: Create a queue
    queue<int> readyQueue;

    // Add all issues with indegree 0
    for (int i = 0; i < issues.size(); i++) {

        int id = issues[i].id;

        if (indegree[id] == 0) {
            readyQueue.push(id);
        }
    }

    // Step 4: Perform topological sorting

    vector<int> result;

    while (!readyQueue.empty()) {

        int current = readyQueue.front();

        readyQueue.pop();

        result.push_back(current);

        // Visit all dependent issues
        for (int neighbour : graph[current]) {

            indegree[neighbour]--;

            // If all prerequisites are completed,
            // this issue is now ready
            if (indegree[neighbour] == 0) {
                readyQueue.push(neighbour);
            }
        }
    }

    // Step 5: Check whether all issues were processed

    if (result.size() != issues.size()) {

        cout << "Cannot generate execution order: "
             << "Circular dependency exists!" << endl;

        return {};
    }

    return result;
}










struct IssuePriorityCompare {

    const unordered_map<int, Issue>* issueMap;

    bool operator()(int id1, int id2) const {

        const Issue& a = issueMap->at(id1);
        const Issue& b = issueMap->at(id2);

        // Rule 1: Higher priority comes first
        if (a.priority != b.priority) {
            return a.priority < b.priority;
        }

        // Rule 2: Fewer estimated hours comes first
        if (a.estimatedHours != b.estimatedHours) {
            return a.estimatedHours > b.estimatedHours;
        }

        // Rule 3: Earlier deadline comes first
        if (a.deadline != b.deadline) {
            return a.deadline > b.deadline;
        }

        // Rule 4: Smaller issue ID comes first
        return a.id > b.id;
    }
};


vector<int> greedyIssueOrder(
    const vector<Issue>& issues,
    const vector<Dependency>& dependencies
) {

    // Step 1: Store issues using their IDs
    unordered_map<int, Issue> issueMap;

    for (int i = 0; i < issues.size(); i++) {
        issueMap[issues[i].id] = issues[i];
    }

    // Step 2: Create dependency graph and indegree map
    unordered_map<int, vector<int>> graph;
    unordered_map<int, int> indegree;

    for (int i = 0; i < issues.size(); i++) {
        indegree[issues[i].id] = 0;
    }

    // Edge direction: Prerequisite -> Dependent issue
    for (int i = 0; i < dependencies.size(); i++) {

        int issueID = dependencies[i].issueId;
        int prerequisiteID = dependencies[i].dependsOnIssueId;

        graph[prerequisiteID].push_back(issueID);

        indegree[issueID]++;
    }

    // Step 3: Create priority queue
    priority_queue<
        int,
        vector<int>,
        IssuePriorityCompare
    > readyQueue((IssuePriorityCompare{&issueMap}));

    // Step 4: Add all issues with indegree 0
    for (int i = 0; i < issues.size(); i++) {

        int id = issues[i].id;

        if (indegree[id] == 0) {
            readyQueue.push(id);
        }
    }

    // Step 5: Select issues greedily
    vector<int> result;

    while (!readyQueue.empty()) {

        int current = readyQueue.top();

        readyQueue.pop();

        result.push_back(current);

        // Update dependent issues
        for (int neighbour : graph[current]) {

            indegree[neighbour]--;

            if (indegree[neighbour] == 0) {
                readyQueue.push(neighbour);
            }
        }
    }

    // Step 6: Check for circular dependencies
    if (result.size() != issues.size()) {
        return {};
    }

    return result;
}


