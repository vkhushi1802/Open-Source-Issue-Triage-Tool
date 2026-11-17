#pragma once
#include <string>
#include <vector>
#include "issue.h"

std::vector<Issue> loadIssues(const std::string& filename);



std::vector<Developer> loadDevelopers(const string& filename);



std::vector<Developer> loadDeveloperSkills(
    const string& filename,
    vector<Developer> developers
);
//Notice that we aren't creating new developers for every skill. We're adding skills to existing developers.



std:: vector<Dependency> loadDependencies(const string& filename);


