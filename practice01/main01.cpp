#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>

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
};

Student stringToStudent(std::string data) {
    Student student;

    student.group = std::stoi(data.substr(0, 4));
    student.index = std::stoi(data.substr(5, 2));

    data = data.substr(8);
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

    return student;
}

int saveStudent(Student &student, bool isNew = true) {
    /*
     * return 0: No errors
     * return 2: Error or abortion while saving to DB
     */

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

int addStudent() {
    /*
     * return 0: No errors
     * return 1: TypeError in input
     * return 2: File read/write error
     */

    Student student;

    // Init values
    std::cout << std::setw(15) << "Group: ";
    if ((std::cin >> student.group).fail() || student.group < 1000 || student.group >= 10000) return 1;

    std::cout << std::setw(15) << "Index: ";
    if ((std::cin >> student.index).fail() || student.index < 1 || student.index >= 100) return 1;

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

    // Save to the database.txt
    return saveStudent(student);
}

int updateStudent(unsigned uuid) {
    /*
     * return 0: No errors
     * return 1: Wrong input
     * return 2: File read/write error
     * return 3: Student not found
     */

    // Input error handler
    if (uuid < 100000 || uuid > 999999) return 1;

    // Get data as string from the DB
    std::ifstream database("database.txt");
    if (!database.is_open()) return 2;
    std::string line;
    while (!database.eof()) {
        std::getline(database, line);
        unsigned check = std::stoi(line.substr(0, 4) + line.substr(5, 2));
        if (!line.empty() && check == uuid) break;
    }
    database.close();

    // Parse student object from line if not empty
    if (line.empty()) return 3;
    Student student = stringToStudent(line);

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

        // Update value
        std::cout << "<< Enter number of value to update (or 0 to exit):\n>> ";
        std::cin >> userInput;
        switch (userInput) {
            case 0: break;
            case 1: {
                std::cout << "Group: ";
                if ((std::cin >> student.group).fail() || student.group < 1000 || student.group >= 10000) return 1;
                break;
            }
            case 2: {
                std::cout << "Index: ";
                if ((std::cin >> student.index).fail() || student.index < 1 || student.index >= 100) return 1;
                break;
            }
            case 3: {
                std::cout << "Full name (really?): ";
                std::getline(std::cin.ignore(), student.fullName);
                break;
            }
            case 4: {
                std::cout << "Is male (1/0) (is it even possible?): ";
                if ((std::cin >> student.isMale).fail()) return 1;
                break;
            }
            case 5: {
                // Clear all after the start and replace cursor to the start line
                clearAfterCursor(0, startCursorPosY);

                // Get input
                std::cout << "Student grades data" << std::endl;
                for (int i = 0; i < 8; ++i) {
                    std::cout << i+1 << '.' << std::setw(8);
                    std::cout << lessonsInGradeBook[i] << ": " << student.grades[i] << std::endl;
                }
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
                break;
            }
            default: return 1; // Not an uint or not in range
        }

        // Clear all after the start and replace cursor to the start line
        clearAfterCursor(0, startCursorPosY);

    } while (userInput != 0);

    // THERE IS NO SAVE TO THE DB
    // I've decided to work with array of Student{} cuz it's easier and faster

    return 0;
}

int main() {
    setlocale(LC_ALL, "ru");

    int response = addStudent();
    response = updateStudent(237218);

    return 0;
}
