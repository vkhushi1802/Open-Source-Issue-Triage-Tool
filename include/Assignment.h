
#pragma once

#include "issue.h"
#include <vector>

// Assign issues to suitable developers
std::vector<Assignment> assignDevelopers(
    const std::vector<Issue>& issues,
    const std::vector<Developer>& developers,
    const std::vector<int>& issueOrder
);
