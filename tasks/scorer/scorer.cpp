#include "scorer.h"

ScoreTable GetScoredStudents(const Events& events, time_t score_time) {
    TaskUpdates task_updates;

    for (const Event& event : events) {
        if (event.time <= score_time) {
            TaskInfo& student_task = task_updates[event.student_name][event.task_name];

            switch (event.event_type) {
                case EventType::CheckSuccess:
                    student_task.last_success_ci = std::max(student_task.last_success_ci, event.time);
                    break;
                case EventType::CheckFailed:
                    student_task.last_falied_ci = std::max(student_task.last_falied_ci, event.time);
                    break;
                case EventType::MergeRequestOpen:
                    student_task.opened_merge_requests_count++;
                    break;
                case EventType::MergeRequestClosed:
                    student_task.opened_merge_requests_count--;
                    break;
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
