
#include <iostream>
#include "CSVreader.h"
#include "Validator.h"

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




cout << "\n========== DEPENDENCY VALIDATION ==========" << endl;

if (validateDependencies(issues, dependencies)) {
    cout << "All dependencies are valid!" << endl;
}
else {
    cout << "Some dependencies are invalid!" << endl;
}



    return 0;
}


