#include "sort_students.h"
#include <algorithm>

bool CmpStudents(Student& s1, Student& s2, SortKind sort_kind) {
    switch (sort_kind) {
        case SortKind::Name:
            return std::tie(s1.last_name, s1.name, s1.birth_date.year, s1.birth_date.month, s1.birth_date.day) <
                   std::tie(s2.last_name, s2.name, s2.birth_date.year, s2.birth_date.month, s2.birth_date.day);
        case SortKind::Date:
            return std::tie(s1.birth_date.year, s1.birth_date.month, s1.birth_date.day, s1.last_name, s1.name) <
                   std::tie(s2.birth_date.year, s2.birth_date.month, s2.birth_date.day, s2.last_name, s2.name);
    }
}

void SortStudents(std::vector<Student>& students, SortKind sort_kind) {
    std::sort(students.begin(), students.end(),
              [&sort_kind](Student& s1, Student& s2) -> bool { return CmpStudents(s1, s2, sort_kind); });
}
