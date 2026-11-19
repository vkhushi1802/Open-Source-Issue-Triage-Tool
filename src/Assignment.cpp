
#include "Assignment.h"
#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

// Convert proficiency into a numerical score
int getProficiencyScore(string proficiency) {

    if (proficiency == "Advanced") {
        return 3;
    }
    else if (proficiency == "Intermediate") {
        return 2;
    }
    else if (proficiency == "Beginner") {
        return 1;
    }

    return 0;
}

// Convert numerical score back into proficiency name
string getProficiencyName(int score) {

    if (score == 3) return "Advanced";
    if (score == 2) return "Intermediate";
    if (score == 1) return "Beginner";

    return "Unknown";
}

// Count how many skills are exclusive to this developer
int countExclusiveSkills(
    int developerIndex,
    const vector<Developer>& developers
) {

    int exclusiveSkills = 0;

    for (const Skill& skill : developers[developerIndex].skills) {

        bool foundElsewhere = false;

        for (int i = 0; i < developers.size(); i++) {

            if (i == developerIndex) {
                continue;
            }

            for (const Skill& otherSkill : developers[i].skills) {

                if (skill.name == otherSkill.name) {
                    foundElsewhere = true;
                    break;
                }
            }

            if (foundElsewhere) {
                break;
            }
        }

        if (!foundElsewhere) {
            exclusiveSkills++;
        }
    }

    return exclusiveSkills;
}




vector<Assignment> assignDevelopers(
    const vector<Issue>& issues,
    const vector<Developer>& developers,
    const vector<int>& issueOrder
) {

    vector<Assignment> assignments;

    // Track additional hours assigned to each developer
    vector<int> assignedHours(developers.size(), 0);

    // Process issues in priority order
    for (int issueID : issueOrder) {

        // Find the issue using its ID
        int issueIndex = -1;

        for (int i = 0; i < issues.size(); i++) {
            if (issues[i].id == issueID) {
                issueIndex = i;
                break;
            }
        }

        if (issueIndex == -1) {
            continue;
        }

        const Issue& issue = issues[issueIndex];

        // Check whether the issue has required skills
        if (issue.requiredskills.empty()) {

            assignments.push_back({
                issue.id,
                -1,
                0,
                "Unassigned",
                "Required skills not specified"
            });

            continue;
        }

        int bestDeveloperIndex = -1;
        int bestTotalProficiency = -1;
        int bestRemainingHours = -1;
        int bestExclusiveSkills = INT_MAX;

        bool skillMatchFound = false;
        bool enoughHoursFound = false;

        string bestSkillDetails;

        // Examine every developer
        for (int i = 0; i < developers.size(); i++) {

            const Developer& developer = developers[i];

            int totalProficiency = 0;
            bool hasAllSkills = true;

            string skillDetails = "";

            // Check every required skill
            for (const Skill& requiredSkill : issue.requiredskills) {

                bool skillFound = false;
                int proficiencyScore = -1;

                // Search for this skill in the developer's skills
                for (const Skill& developerSkill : developer.skills) {

                    if (developerSkill.name == requiredSkill.name) {

                        proficiencyScore =
                            getProficiencyScore(developerSkill.proficiency);

                        if (proficiencyScore != -1) {
                            skillFound = true;
                        }

                        break;
                    }
                }

                // Reject developer if a required skill is missing
                if (!skillFound) {
                    hasAllSkills = false;
                    break;
                }

                totalProficiency += proficiencyScore;

                if (!skillDetails.empty()) {
                    skillDetails += ", ";
                }

                skillDetails += requiredSkill.name +
                    " (" + getProficiencyName(proficiencyScore) + ")";
            }

            // Developer must possess ALL required skills
            if (!hasAllSkills) {
                continue;
            }

            skillMatchFound = true;

            // Calculate remaining available hours
            int remainingHours =
                developer.availHours
                - developer.currentWorkload
                - assignedHours[i];

            // Developer must have enough hours
            if (remainingHours < issue.estimatedHours) {
                continue;
            }

            enoughHoursFound = true;

            // Count skills only this developer possesses
            int exclusiveSkills =
                countExclusiveSkills(i, developers);

            // Selection rules:
            // 1. Fewer exclusive skills
            // 2. Higher combined proficiency
            // 3. More remaining hours

            if (
                exclusiveSkills < bestExclusiveSkills ||
                (
                    exclusiveSkills == bestExclusiveSkills &&
                    totalProficiency > bestTotalProficiency
                ) ||
                (
                    exclusiveSkills == bestExclusiveSkills &&
                    totalProficiency == bestTotalProficiency &&
                    remainingHours > bestRemainingHours
                )
            ) {

                bestDeveloperIndex = i;
                bestExclusiveSkills = exclusiveSkills;
                bestTotalProficiency = totalProficiency;
                bestRemainingHours = remainingHours;
                bestSkillDetails = skillDetails;
            }
        }

        // Handle issues that cannot be assigned
        if (bestDeveloperIndex == -1) {

            string reason;

            if (!skillMatchFound) {
                reason =
                    "No developer possesses all the required skills";
            }
            else if (!enoughHoursFound) {
                reason =
                    "No developer with all required skills has sufficient available hours";
            }

            assignments.push_back({
                issue.id,
                -1,
                0,
                "Unassigned",
                reason
            });

            continue;
        }

        // Assign the issue to the selected developer
        assignedHours[bestDeveloperIndex] += issue.estimatedHours;

        // Generate an explanation for the assignment
        string reason =
            "Matched skills: " + bestSkillDetails +
            " | Combined proficiency score: " +
            to_string(bestTotalProficiency) +
            " | Available hours before assignment: " +
            to_string(bestRemainingHours) +
            " | Exclusive skills: " +
            to_string(bestExclusiveSkills);

        assignments.push_back({
            issue.id,
            developers[bestDeveloperIndex].id,
            issue.estimatedHours,
            "Assigned",
            reason
        });
    }

    return assignments;
}

