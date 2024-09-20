#include "scorer.h"
#include <cstdint>
#include <unordered_map>

struct TaskInfo {
    time_t last_success_ci = 0;
    time_t last_falied_ci = 0;
    uint32_t opened_merge_requests_count = 0;
};

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    std::unordered_map<StudentName, std::unordered_map<TaskName, TaskInfo>> task_updates;

    for (const Event& event : events) {
        if (event.time <= score_time) {
            TaskInfo& student_task = task_updates[event.student_name][event.task_name];

            if (event.event_type == EventType::CheckSuccess) {
                student_task.last_success_ci = std::max(student_task.last_success_ci, event.time);
            } else if (event.event_type == EventType::CheckFailed) {
                student_task.last_falied_ci = std::max(student_task.last_falied_ci, event.time);
            } else if (event.event_type == EventType::MergeRequestOpen) {
                student_task.opened_merge_requests_count++;
            } else {
                student_task.opened_merge_requests_count--;
            }
        }
    }

    ScoreTable response;

    for (const auto& [student, tasks] : task_updates) {
        for (const auto& [task, info] : tasks) {
            if (info.last_success_ci >= info.last_falied_ci && info.opened_merge_requests_count == 0) {
                response[student].insert(task);
            }
        }
    }

    return response;
}
