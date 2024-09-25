#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

enum class EventType {
    CheckFailed,
    CheckSuccess,
    MergeRequestOpen,
    MergeRequestClosed,
};

using StudentName = std::string;
using TaskName = std::string;

struct Event {
    StudentName student_name;
    TaskName task_name;
    time_t time;
    EventType event_type;
};

struct TaskInfo {
    time_t last_success_ci = 0;
    time_t last_falied_ci = 0;
    uint32_t opened_merge_requests_count = 0;
};

using Events = std::vector<Event>;
using ScoreTable = std::map<StudentName, std::set<TaskName>>;
using TaskUpdates = std::unordered_map<StudentName, std::unordered_map<TaskName, TaskInfo>>;

ScoreTable GetScoredStudents(const Events& events, time_t score_time);
