#include "admission.h"
#include <algorithm>
#include <unordered_map>

bool CmpApplicants(const Applicant* a1, const Applicant* a2) {
    const int neg_a1_points = -a1->points;
    const int neg_a2_points = -a2->points;

    const Student& s1 = a1->student;
    const Student& s2 = a2->student;

    const Date& d1 = s1.birth_date;
    const Date& d2 = s2.birth_date;

    return std::tie(neg_a1_points, d1.year, d1.month, d1.day, s1.name) <
           std::tie(neg_a2_points, d2.year, d2.month, d2.day, s2.name);
}

AdmissionTable FillUniversities(const std::vector<University>& universities, const std::vector<Applicant>& applicants) {
    std::vector<const Applicant*> sorted_applicants;

    for (const Applicant& applicant : applicants) {
        sorted_applicants.push_back(&applicant);
    }

    std::sort(sorted_applicants.begin(), sorted_applicants.end(), CmpApplicants);

    std::unordered_map<std::string, size_t> vacancies_left;
    for (const University& univerity : universities) {
        vacancies_left[univerity.name] = univerity.max_students;
    }

    AdmissionTable response;

    for (const Applicant* applicant : sorted_applicants) {
        for (const std::string& university_name : applicant->wish_list) {
            if (vacancies_left[university_name] > 0) {
                response[university_name].push_back(&applicant->student);
                vacancies_left[university_name]--;
                break;
            }
        }
    }

    return response;
}
