#include "Validator.h"
#include <vector>
#include <algorithm>
#include <iostream>

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
    for (int i = 0; i < issueIDs.size() - 1; i++) {

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
    int right = issueIDs.size() - 1;

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