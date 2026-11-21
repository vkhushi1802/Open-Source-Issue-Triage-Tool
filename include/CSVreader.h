
#pragma once
#include <string>
#include <vector>
#include "issue.h"

std::vector<Issue> loadIssues(const std::string& filename);

std::vector<Developer> loadDevelopers(const std::string& filename);

std::vector<Developer> loadDeveloperSkills(
    const std::string& filename,
    std::vector<Developer> developers
);

//Notice that we aren't creating new developers for every skill. We're adding skills to existing developers.

std::vector<Dependency> loadDependencies(const std::string& filename);
