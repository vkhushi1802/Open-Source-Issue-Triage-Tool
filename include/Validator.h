#pragma once

#include <vector>
#include "issue.h"

bool hasDuplicateIssueIDs(const std::vector<Issue>& issues);

//- & means we pass the original vector by reference instead of making a copy.
//- const means our function is not allowed to modify the original vector.

bool hasDuplicateIssueIDsMergeSort(const std::vector<Issue>& issues);





bool binarySearchIssueID(const std::vector<int>& issueIDs, int target);

bool validateDependencies(
    const std::vector<Issue>& issues,
    const std::vector<Dependency>& dependencies
);