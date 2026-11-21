
#include <iostream>
#include "CSVreader.h"
#include "Validator.h"
#include <vector>
#include "Assignment.h"

using namespace std;

int main() {

    vector<Issue> issues = loadIssues("data/issues.csv");

    cout << "\n========== DUPLICATE DETECTION ==========" << endl;

    bool duplicateIDs = false;

    // Select duplicate detection algorithm based on input size
    if (issues.size() <= 50) {

        cout << "Selected Algorithm: Nested Loop" << endl;

        duplicateIDs = hasDuplicateIssueIDs(issues);
    }
    else {

        cout << "Selected Algorithm: Merge Sort" << endl;

        duplicateIDs = hasDuplicateIssueIDsMergeSort(issues);
    }

    if (duplicateIDs) {
        cout << "Duplicate Issue IDs found!" << endl;
    }
    else {
        cout << "No duplicate Issue IDs found!" << endl;
    }

    cout << "\nTotal Issues Loaded: " << issues.size() << endl;

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
    developers = loadDeveloperSkills("data/developer_skills.csv", developers);


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

    cout << "\nTotal dependencies Loaded: " << dependencies.size() << endl;

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


    // Generate dependency-aware issue order only when dependency data is valid
    // and does not contain a cycle.
    vector<int> issueOrder;
    vector<Assignment> assignments;

    if (dependenciesValid && !circularDependency) {

        // Check whether issue priorities are different
        bool prioritiesDiffer = false;

        for (int i = 1; i < issues.size(); i++) {

            if (issues[i].priority != issues[0].priority) {
                prioritiesDiffer = true;
                break;
            }
        }

        cout << "\n========== ADAPTIVE ISSUE ORDERING ==========" << endl;

        // Select the best ordering algorithm based on input priorities
        if (prioritiesDiffer) {

            cout << "Selected Algorithm: Greedy Priority Ordering" << endl;

            issueOrder = greedyIssueOrder(issues, dependencies);
        }
        else {

            cout << "Selected Algorithm: Topological Sorting" << endl;

            issueOrder = topologicalSort(issues, dependencies);
        }

        if (!issueOrder.empty()) {

            cout << "\nRecommended Issue Execution Order:" << endl;

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

            // Assign issues to suitable developers
            assignments = assignDevelopers(issues, developers, issueOrder);
        }
        else {
            cout << "Unable to generate issue order." << endl;
        }
    }
    else {
        cout << "\nAssignment skipped because dependency data is invalid "
             << "or contains a circular dependency." << endl;
    }


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
