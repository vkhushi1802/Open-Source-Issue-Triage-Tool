
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "CSVreader.h"

using namespace std;


int readInt(
    const string& value,
    const string& filename,
    int row,
    const string& column
) {
    try {
        return stoi(value);
    }
    catch (const exception& ) {
        cerr << "\nCSV ERROR!"
             << "\nFile: " << filename
             << "\nRow: " << row
             << "\nColumn: " << column
             << "\nInvalid value: [" << value << "]\n";

        throw;
    }
}


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

    int row = 1;

    while (getline(file, line)) {           //This keeps reading until the file ends.

        row++;

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

        issue.id = readInt(fields[0], filename, row, "id");
        issue.title = fields[1];
        issue.desc = fields[2];


        // Store multiple required skills
        stringstream skillStream(fields[3]);
        string skillName;

        while (getline(skillStream, skillName, ';')) {

            Skill skill;
            skill.name = skillName;
            skill.proficiency = "Not specified";

            issue.requiredskills.push_back(skill);
        }


        issue.priority = readInt(fields[4], filename, row, "priority");
        issue.estimatedHours = readInt(fields[5], filename, row, "estimated_hours");
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

    int row = 1;

    while (getline(file, line)) {           //This keeps reading until the file ends.

        row++;

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

        developer.id = readInt(fields[0], filename, row, "developer_id");
        developer.name = fields[1];
        developer.availHours = readInt(fields[2], filename, row, "available_hours");
        developer.currentWorkload = readInt(fields[3], filename, row, "current_workload");
        developer.experience = fields[4];


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

    int row = 1;

    while (getline(file, line)) {

        row++;

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

        int developerID = readInt(fields[0], filename, row, "developer_id");

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

    int row = 1;

    while (getline(file, line)) {

        row++;

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

        dependency.issueId = readInt(fields[0], filename, row, "issue_id");
        dependency.dependsOnIssueId = readInt(fields[1], filename, row, "depends_on_issue_id");

        dependencies.push_back(dependency);
    }

    file.close();

    return dependencies;
}
