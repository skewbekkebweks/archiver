#include "tests_checking.h"
#include <deque>

std::vector<std::string> StudentsOrder(const std::vector<StudentAction>& student_actions,
                                       const std::vector<size_t>& queries) {
    std::deque<std::string> ordred_student_names_deque;
    for (const StudentAction& student_action : student_actions) {
        switch (student_action.side) {
            case Side::Top:
                ordred_student_names_deque.push_front(student_action.name);
                break;
            case Side::Bottom:
                ordred_student_names_deque.push_back(student_action.name);
                break;
        }
    }

    std::vector<std::string> ordered_student_names_vector(
        {ordred_student_names_deque.begin(), ordred_student_names_deque.end()});

    std::vector<std::string> response;
    for (const size_t& query : queries) {
        response.push_back(ordered_student_names_vector[query - 1]);
    }

    return response;
}
