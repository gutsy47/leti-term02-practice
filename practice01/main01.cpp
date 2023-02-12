#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include <vector>
#include <cmath>

const std::string lessonsInGradeBook[] = {
        "Algebra", "Maths", "Dev", "Dev CW", "Physics", "CS", "Philos", "English"
};

short getCursorPositionY() {
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo = {};
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBufferInfo);
    return screenBufferInfo.dwCursorPosition.Y;
}

void clearAfterCursor(short x = 0, short y = 0) {
    COORD destCoord = {x, y};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
            console, ' ', screen.dwSize.X * screen.dwSize.Y, destCoord, &written
    );
    SetConsoleCursorPosition(console, destCoord);
}

struct Student {
    unsigned short group{};
    unsigned short index{};
    std::string fullName;
    bool isMale{};
    unsigned short grades[8]{}; // 3 exams and 5 differentiated tests
    float average{};
};

Student stringToStudent(std::string data) {
    Student student;

    student.group = std::stoi(data.substr(0, 4));
    student.index = std::stoi(data.substr(5, 2));

    data = data.substr(student.index < 10 ? 7 : 8);
    student.fullName = "";
    for (int i = 0; i < data.length(); ++i) {
        if (data[i] == ',') {
            data = data.substr(i+1);
            break;
        }
        student.fullName += data[i];
    }

    student.isMale = std::stoi(data.substr(0, 1));

    for (int i = 0; i < 8; ++i) student.grades[i] = std::stoi(data.substr(2*(i+1), 1));

    student.average = std::stof(data.substr(18));

    return student;
}

unsigned getUuid(unsigned short group, unsigned short index) {
    return group * 100 + index;
}

float getAverage(const unsigned short (&grades)[8]) {
    float result = 0;
    for (auto grade : grades) result += float(grade);
    return std::round((result / 8 * 100)) / 100;
}

bool isUnique(std::vector<Student> &students, unsigned short group, unsigned short index) {
    // Not a std::any_of because it needs to create uuids vector
    for (auto &check : students) // NOLINT(readability-use-anyofallof)
        if (getUuid(check.group, check.index) == getUuid(group, index)) return false;
    return true;
}

void getStudents(std::vector<Student> &students) {
    std::ifstream database("database.txt");
    if (!database.is_open()) return;
    std::string line;
    while (!database.eof()) {
        std::getline(database, line);
        if (!line.empty()) students.push_back(stringToStudent(line));
    }
}

int addStudent(std::vector<Student> &students) {
    /*
     * return 0: No errors
     * return 1: TypeError in input
     * return 2: File read/write error
     * return 4: Student already exists
     */

    Student student;

    // Init values
    std::cout << std::setw(15) << "Group: ";
    if ((std::cin >> student.group).fail() || student.group < 1000 || student.group >= 10000) return 1;

    std::cout << std::setw(15) << "Index: ";
    if ((std::cin >> student.index).fail() || student.index < 1 || student.index >= 100) return 1;

    if (!isUnique(students, student.group, student.index)) return 4;

    std::cout << std::setw(15) << "Full name: ";
    std::getline(std::cin.ignore(), student.fullName);

    std::cout << std::setw(15) << "Is male (1/0): ";
    if ((std::cin >> student.isMale).fail()) return 1;

    std::cout << std::setw(15) << "Grades:\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << std::setw(14) << '|' << std::setw(8) << lessonsInGradeBook[i] << ": ";
        if ((std::cin >> student.grades[i]).fail() || student.grades[i] < 0 || student.grades[i] > 5) return 1;
        if (student.grades[i] <= 2) {
            std::cout << student.fullName << " will be expelled. Saving to DB aborted";
            return 2;
        }
    }

    student.average = getAverage(student.grades);
    std::cout << std::setw(15) << "|\n";
    std::cout << std::setw(14) << '|' << std::setw(10) << "Average: " << student.average << std::endl;

    // Push back and return
    students.push_back(student);
    return 0;
}

int updateStudent(std::vector<Student> &students, unsigned uuid) {
    /*
     * return 0: No errors
     * return 1: Wrong input
     * return 2: File read/write error
     * return 3: Student not found
     * return 4: Student already exists
     */

    // Input error handler
    if (uuid < 100000 || uuid > 999999) return 1;

    // Get student by UUID
    bool isFound = false;
    Student student;
    unsigned short studentInd;
    for (int i = 0; i < students.size(); ++i) {
        if ((getUuid(students[i].group, students[i].index)) == uuid) {
            isFound = true;
            student = students[i];
            studentInd = i;
        }
    }
    if (!isFound) return 3;

    // Update value while not interrupted by user
    unsigned short userInput = 0;
    short startCursorPosY = getCursorPositionY();
    do {
        // Print student info
        std::cout << "Student data" << (userInput == 0 ? "" : "(updated)") << std::endl;
        std::cout << "1. " << std::setw(8) << "Group: " << student.group << std::endl;
        std::cout << "2. " << std::setw(8) << "Index: " << student.index << std::endl;
        std::cout << "3. " << std::setw(8) << "Name: " << student.fullName << std::endl;
        std::cout << "4. " << std::setw(8) << "Sex: " << (student.isMale ? "Male" : "Female") << std::endl;
        std::cout << "5. " << std::setw(8) << "Grades: " << std::endl;
        for (int i = 0; i < 8; ++i) {
            std::cout << std::setw(10) << '|' << std::setw(8);
            std::cout << lessonsInGradeBook[i] << ": " << student.grades[i] << std::endl;
        }
        std::cout << std::setw(11) << "|\n";
        std::cout << std::setw(10) << '|' << std::setw(10) << "Average: " << student.average << std::endl;

        // Update value
        std::cout << "<< Enter number of value to update (or 0 to exit):\n>> ";
        std::cin >> userInput;
        switch (userInput) {
            case 0: break;
            case 1: {
                std::cout << "Group: ";
                if ((std::cin >> student.group).fail() || student.group < 1000 || student.group >= 10000) return 1;
                if (!isUnique(students, student.group, student.index)) return 4;
                break;
            }
            case 2: {
                std::cout << "Index: ";
                if ((std::cin >> student.index).fail() || student.index < 1 || student.index >= 100) return 1;
                if (!isUnique(students, student.group, student.index)) return 4;
                break;
            }
            case 3: {
                std::cout << "Full name (really?): ";
                std::getline(std::cin.ignore(), student.fullName);
                break;
            }
            case 4: {
                std::cout << "Is male (1/0) (ayo dude's from Turkey): ";
                if ((std::cin >> student.isMale).fail()) return 1;
                break;
            }
            case 5: {
                // Clear all after the start and write the grade book data
                clearAfterCursor(0, startCursorPosY);
                std::cout << "Student grades data" << std::endl;
                for (int i = 0; i < 8; ++i) {
                    std::cout << i+1 << '.' << std::setw(8);
                    std::cout << lessonsInGradeBook[i] << ": " << student.grades[i] << std::endl;
                }
                std::cout << std::setw(12) << "Average: " << student.average << std::endl;

                // Get input
                unsigned short gradeInd;
                std::cout << "<< Enter number of value to update (or 0 to exit):\n>> ";
                if ((std::cin >> gradeInd).fail() || gradeInd > 8) return 1;
                gradeInd--;
                std::cout << lessonsInGradeBook[gradeInd] << ": ";
                if ((std::cin >> student.grades[gradeInd]).fail()) return 1; // Not an uint
                if (student.grades[gradeInd] < 0 || student.grades[gradeInd] > 5) return 1; // Out of range
                if (student.grades[gradeInd] <= 2) { // Expelled
                    std::cout << student.fullName << " will be expelled. Saving to DB aborted";
                    return 2;
                }
                student.average = getAverage(student.grades);
                break;
            }
            default: return 1; // Not an uint or not in range
        }

        // Clear all after the start and replace cursor to the start line
        if (userInput != 0) clearAfterCursor(0, startCursorPosY);

    } while (userInput != 0);

    // Save the update and return
    students[studentInd] = student;
    return 0;
}

void printAllStudents(std::vector<Student> &students) {
    // Header
    std::cout << "| Group | Index |                      Full Name |    Sex |  Avg |          Grades |\n";
    std::cout << '|' << std::setw(84) << std::setfill('-') << "|\n" << std::setfill(' ');

    // Empty table check
    if (students.empty()) {
        std::cout << '|' << std::setw(44) << "No data." << std::setw(40) << "|\n";
        return;
    }

    // Table
    for (auto &student : students) {
        std::cout << '|' << std::setw(6) << student.group << ' ';
        std::cout << '|' << std::setw(6) << student.index << ' ';
        std::cout << '|' << std::setw(31) << student.fullName << ' ';
        std::cout << '|' << std::setw(7) << (student.isMale ? "Male" : "Female") << ' ';
        std::cout << '|' << std::setw(5) << getAverage(student.grades) << " |";
        for (auto grade : student.grades) std::cout << ' ' << grade;
        std::cout << " |\n";
    }
}

void printStudentsByGroup(std::vector<Student> &students, unsigned short group) {
    // Header
    std::cout << "| Index |                      Full Name |    Sex |  Avg |          Grades |\n";
    std::cout << '|' << std::setw(76) << std::setfill('-') << "|\n" << std::setfill(' ');

    // Wrong group -> table empty
    if (group < 1000 || group >= 10000) {
        std::cout << '|' << std::setw(40) << "No data." << std::setw(36) << "|\n";
        return;
    }

    // Table
    bool isPrinted = false;
    for (auto &student : students) {
        if (student.group == group) {
            std::cout << '|' << std::setw(6) << student.index << ' ';
            std::cout << '|' << std::setw(31) << student.fullName << ' ';
            std::cout << '|' << std::setw(7) << (student.isMale ? "Male" : "Female") << ' ';
            std::cout << '|' << std::setw(5) << student.average << " |";
            for (auto grade : student.grades) std::cout << ' ' << grade;
            std::cout << " |\n";
            isPrinted = true;
        }
    }
    if (!isPrinted) // Empty table
        std::cout << '|' << std::setw(40) << "No data." << std::setw(36) << "|\n";
}

bool compareGrades(const Student &st1, const Student &st2) {
    return (st1.average > st2.average);
}

void printNumberByGender(std::vector<Student> &students) {
    unsigned short maleAmount = 0, femaleAmount = 0;
    for (auto &student : students) {
        if (student.isMale) maleAmount++;
        else femaleAmount++;
    }
    std::cout << "  Male students: " << maleAmount << std::endl;
    std::cout << "Female students: " << femaleAmount << std::endl;
}

void printByScholarship(std::vector<Student> &students) {
    std::vector<Student> noScholarship, lowScholarship, midScholarship, highScholarship;

    // Filter by '3' | '4' | '4-5' | '5'
    for (auto &student : students) {
        bool any3 = false, any4 = false, any5 = false;
        for (auto grade : student.grades) {
            if (grade == 3) {
                any3 = true;
                break; // If '3' in grades then no scholarship
            }
            if (grade == 4) any4 = true;
            else any5 = true;
        }

        if (any3) {
            noScholarship.push_back(student); // '3' in grades
        } else if (any5) {
            if (any4) midScholarship.push_back(student); // '4' or '5'
            else highScholarship.push_back(student); // All '5'
        } else {
            lowScholarship.push_back(student); // All '4'
        }
    }

    // Print data
    std::cout << "High scholarship:\n";
    printAllStudents(highScholarship);
    std::cout << "\nMedium scholarship:\n";
    printAllStudents(midScholarship);
    std::cout << "\nDefault scholarship:\n";
    printAllStudents(lowScholarship);
    std::cout << "\nNo scholarship:\n";
    printAllStudents(noScholarship);
}

void printStudentsByIndex(std::vector<Student> &students, unsigned short index) {
    // Header
    std::cout << "| Group |                      Full Name |    Sex |  Avg |          Grades |\n";
    std::cout << '|' << std::setw(76) << std::setfill('-') << "|\n" << std::setfill(' ');

    // Wrong group -> table empty
    if (index < 0 || index >= 100) {
        std::cout << '|' << std::setw(40) << "No data." << std::setw(36) << "|\n";
        return;
    }

    // Table
    bool isPrinted = false;
    for (auto &student : students) {
        if (student.index == index) {
            std::cout << '|' << std::setw(6) << student.group << ' ';
            std::cout << '|' << std::setw(31) << student.fullName << ' ';
            std::cout << '|' << std::setw(7) << (student.isMale ? "Male" : "Female") << ' ';
            std::cout << '|' << std::setw(5) << student.average << " |";
            for (auto grade : student.grades) std::cout << ' ' << grade;
            std::cout << " |\n";
            isPrinted = true;
        }
    }
    if (!isPrinted) // Empty table
        std::cout << '|' << std::setw(40) << "No data." << std::setw(36) << "|\n";
}

int main() {
    setlocale(LC_ALL, "ru");

    // Get students from the DB
    std::vector<Student> students;
    getStudents(students);

    // Main part of the code (currently temp for functions test)
    int response = addStudent(students);
    std::cout << "addStudent: " << response << std::endl;
    std::cout << '\n';

    response = updateStudent(students, 237218);
    std::cout << "updStudent: " << response << std::endl;
    std::cout << '\n';

    printAllStudents(students);
    std::cout << '\n';

    printStudentsByGroup(students, 2379);
    std::cout << '\n';

    // Print students top
    std::sort(students.begin(), students.end(), compareGrades);
    printAllStudents(students);
    std::cout << '\n';

    printNumberByGender(students);
    std::cout << '\n';

    printByScholarship(students);
    std::cout << '\n';

    printStudentsByIndex(students, 2);
    std::cout << '\n';

    // Update the DB
    // ...

    return 0;
}
