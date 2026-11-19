
#pragma once

#include "issue.h"
#include <vector>

using namespace std;

// Assign issues to suitable developers
vector<Assignment> assignDevelopers(
    const vector<Issue>& issues,
    const vector<Developer>& developers,
    const vector<int>& issueOrder
);
