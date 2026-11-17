
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "CSVreader.h"

using namespace std;

vector<Issue> loadIssues(const string& filename) {

    vector<Issue> issues;

    ifstream file(filename);                    //ifstream allows C++ to read data from a file.

    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return issues;
    }

    string line;

    // Skip the header row
    getline(file, line);

    while (getline(file, line)) {           //This keeps reading until the file ends.

        stringstream ss(line);
        string field;
        vector<string> fields;

        // Split each row using commas
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // Check whether the row has enough columns
        if (fields.size() < 8) {
            cout << "Invalid row found. Skipping..." << endl;
            continue;
        }

        Issue issue;

        issue.id = stoi(fields[0]);
        issue.title = fields[1];
        issue.desc = fields[2];

        // Store the required skill
        Skill skill;
        skill.name = fields[3];
        skill.proficiency = "Not specified";

        issue.requiredskills.push_back(skill);

        issue.priority = stoi(fields[4]);
        issue.estimatedHours = stoi(fields[5]);
        issue.deadline = fields[6];
        issue.status = fields[7];

        // Add the completed Issue object to our vector
        issues.push_back(issue);
    }

    file.close();

    return issues;
}



vector<Developer> loadDevelopers(const string& filename) {

    vector<Developer> developers;

    ifstream file(filename);                    //ifstream allows C++ to read data from a file.

    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return developers;
    }

    string line;

    // Skip the header row
    getline(file, line);

    while (getline(file, line)) {           //This keeps reading until the file ends.

        stringstream ss(line);
        string field;
        vector<string> fields;

        // Split each row using commas
        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        // Check whether the row has enough columns
        if (fields.size() < 5) {
            cout << "Invalid row found. Skipping..." << endl;
            continue;
        }

        Developer developer;

        developer.id = stoi(fields[0]);
        developer.name = fields[1];
        developer.availHours = stoi(fields[2]);
        developer.currentWorkload=stoi(fields[3]);
        developer.experience=fields[4];

       
        // Add the completed Developer object to our vector
        developers.push_back(developer);
    }




    file.close();

    return developers;
}

vector<Developer> loadDeveloperSkills(const string& filename, vector<Developer> developers) {

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return developers;
    }

    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line)) {

        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() < 3) {
            cout << "Invalid row found. Skipping..." << endl;
            continue;
        }

        int developerID = stoi(fields[0]);

        Skill skill;
        skill.name = fields[1];
        skill.proficiency = fields[2];

        // Find the developer with matching ID
        for (int i = 0; i < developers.size(); i++) {

            if (developers[i].id == developerID) {

                developers[i].skills.push_back(skill);
                break;
            }
        }
    }

    file.close();

    return developers;
}

vector<Dependency> loadDependencies(const string& filename) {

    vector<Dependency> dependencies;

    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return dependencies;
    }

    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line)) {

        stringstream ss(line);
        string field;
        vector<string> fields;

        while (getline(ss, field, ',')) {
            fields.push_back(field);
        }

        if (fields.size() < 2) {
            cout << "Invalid row found. Skipping..." << endl;
            continue;
        }

        Dependency dependency;

        dependency.issueId = stoi(fields[0]);
        dependency.dependsOnIssueId = stoi(fields[1]);

        dependencies.push_back(dependency);
    }

    file.close();

    return dependencies;
}
