#pragma once

#include <vector>
#include "issue.h"

using namespace std;

bool hasDuplicateIssueIDs(const std::vector<Issue>& issues);

//- & means we pass the original vector by reference instead of making a copy.
//- const means our function is not allowed to modify the original vector.

bool hasDuplicateIssueIDsMergeSort(const std::vector<Issue>& issues);





bool binarySearchIssueID(const std::vector<int>& issueIDs, int target);

bool validateDependencies(
    const std::vector<Issue>& issues,
    const std::vector<Dependency>& dependencies
);


bool hasCircularDependency(
    const std::vector<Issue>& issues,
    const std::vector<Dependency>& dependencies
);



vector<int> topologicalSort(
    const vector<Issue>& issues,
    const vector<Dependency>& dependencies
);



vector<int> greedyIssueOrder(
    const vector<Issue>& issues,
    const vector<Dependency>& dependencies
);


vector<Activity> activitySelection(
    vector<Activity>& activities
);


vector<vector<Activity>> intervalPartitioning(
    vector<Activity> activities
);

int knapsack(
    const vector<Issue>& issues,
    int capacity,
    vector<int>& selectedIssueIDs
);