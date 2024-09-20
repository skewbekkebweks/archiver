#include "tests_checking.h"
#include <string>
#include <vector>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    int count_top_actions = 0;
    for (const StudentAction& student_action : student_actions) {
        if (student_action.side == Side::Top) {
            count_top_actions++;
        }
    }

    std::vector<std::string> student_names_in_order(student_actions.size());
    int next_top_index = count_top_actions - 1;
    int next_bottom_index = count_top_actions;

    for (const StudentAction& student_action : student_actions) {
        if (student_action.side == Side::Top) {
            student_names_in_order[next_top_index] = student_action.name;
            next_top_index--;
        } else {
            student_names_in_order[next_bottom_index] = student_action.name;
            next_bottom_index--;
        }
    }

    std::vector<std::string> response;
    for (const size_t& query : queries) {
        response.push_back(student_names_in_order[query - 1]);
    }

    return response;
}
