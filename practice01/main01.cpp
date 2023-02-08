#include <iostream>
#include <iomanip>
#include <fstream>

struct Student {
    unsigned short group;
    unsigned short index;
    std::string fullName;
    bool isMale;
    unsigned short grades[8]; // 3 exams and 5 differentiated tests
};

int addStudent() {
    /*
     * return 0: No errors
     * return 1: TypeError in input
     * return 2: Error or abortion while saving to DB
     */

    Student student{};

    // Init values
    std::cout << std::setw(15) << "Group: ";
    if ((std::cin >> student.group).fail() || student.group < 1000 || student.group >= 10000) return 1;

    std::cout << std::setw(15) << "Index: ";
    if ((std::cin >> student.index).fail() || student.index < 1 || student.index >= 100) return 1;

    std::cout << std::setw(15) << "Full name: ";
    std::getline(std::cin.ignore(), student.fullName);

    std::cout << std::setw(15) << "Is male (1/0): ";
    if ((std::cin >> student.isMale).fail()) return 1;

    std::cout << std::setw(15) << "Grades: \n";
    std::string lessonsInGradeBook[] = {
            "algebra", "math analysis", "programming", "programming CW",
            "physics", "computer science", "philosophy", "languages"
    };
    for (int i = 0; i < 8; ++i) {
        std::cout << std::setw(18) << lessonsInGradeBook[i] << ": ";
        if ((std::cin >> student.grades[i]).fail() || student.grades[i] < 0 || student.grades[i] > 5) return 1;
        if (student.grades[i] <= 2) {
            std::cout << student.fullName << " will be expelled. Saving to DB aborted";
            return 2;
        }
    }

    // Save to the database.txt
    std::ofstream database("database.txt", std::ios::app);
    if (!database.is_open()) return 2;
    database << student.group << ',';
    database << student.index << ',';
    database << student.fullName << ',';
    database << student.isMale;
    for (auto grade : student.grades) database << ',' << grade;
    database << std::endl;
    database.close();

    return 0;
}

int main() {
    setlocale(LC_ALL, "ru");

    int response = addStudent();

    return 0;
}
