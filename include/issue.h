#pragma once
#include <string>
#include <vector>
using namespace std;

struct Skill {
    string name;
    string proficiency;
};

struct Issue {
    int id;
    string title;
    string desc;
    int priority;
    int estimatedHours;
    string deadline;
    string status;
    vector<Skill> requiredskills;
};

struct Developer {
    int id;
    string name;
    vector<Skill> skills;
    int availHours;
    int currentWorkload;
    string experience;
};

struct Assignment {
    int issueId;
    int developerId;
    int assignedHours;
    string status;
    string reason;
};

struct Dependency {
    int issueId;
    int dependsOnIssueId;
};

