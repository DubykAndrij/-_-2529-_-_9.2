#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

enum Speciality { COMPUTER_SCIENCE, INFORMATICS, MATH_ECON, PHYS_INF, LABOR_TRAINING };
string specialityStr[] = { "Computer Science", "Informatics", "Math & Econ", "Phys & Inf", "Labor Training" };

union ThirdGrade {
    int programming;
    int numericalMethods;
    int pedagogy;
};

struct Student {
    string surname;
    int course;
    Speciality speciality;
    int physicsGrade;
    int mathGrade;
    ThirdGrade thirdGrade;
};

void create(Student* p, const int n);
void print(Student* p, const int n);
void printIndexed(Student* p, int* indexArray, const int n);
void physicalSort(Student* p, const int n);
int* createIndexArray(Student* p, const int n);
int binarySearch(Student* p, const int n, string surname, Speciality spec, int course);

int main() {
    int n;
    cout << "Enter number of students: ";
    cin >> n;

    Student* students = new Student[n];
    int* indexArray = nullptr;
    int choice;

    do {
        cout << "\n--- MENU ---\n";
        cout << "1. Enter data\n";
        cout << "2. Display table\n";
        cout << "3. Physical sort (Spec -> Course -> Surname)\n";
        cout << "4. Create and display index sort\n";
        cout << "5. Binary search\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            create(students, n);
            break;
        case 2:
            print(students, n);
            break;
        case 3:
            physicalSort(students, n);
            cout << "Physically sorted.\n";
            print(students, n);
            break;
        case 4:
            if (indexArray) delete[] indexArray;
            indexArray = createIndexArray(students, n);
            printIndexed(students, indexArray, n);
            break;
        case 5: {
            string s;
            int c, sp;
            cout << "Enter surname: "; cin >> s;
            cout << "Enter course: "; cin >> c;
            cout << "Enter speciality (0-CS, 1-Inf, 2-MathEcon, 3-PhysInf, 4-Labor): "; cin >> sp;

            physicalSort(students, n);
            int idx = binarySearch(students, n, s, (Speciality)sp, c);
            if (idx != -1)
                cout << "Student found at index " << idx + 1 << endl;
            else
                cout << "Student not found.\n";
            break;
        }
        }
    } while (choice != 0);

    delete[] students;
    if (indexArray) delete[] indexArray;
    return 0;
}

void create(Student* p, const int n) {
    for (int i = 0; i < n; i++) {
        cout << "\nStudent #" << i + 1 << ":\n";
        cout << " Surname: "; cin >> p[i].surname;
        cout << " Course: "; cin >> p[i].course;
        int spec;
        cout << " Speciality (0-CS, 1-Inf, 2-MathEcon, 3-PhysInf, 4-Labor): ";
        cin >> spec;
        p[i].speciality = (Speciality)spec;
        cout << " Physics grade: "; cin >> p[i].physicsGrade;
        cout << " Math grade: "; cin >> p[i].mathGrade;

        if (p[i].speciality == COMPUTER_SCIENCE) {
            cout << " Programming grade: "; cin >> p[i].thirdGrade.programming;
        }
        else if (p[i].speciality == INFORMATICS) {
            cout << " Numerical Methods grade: "; cin >> p[i].thirdGrade.numericalMethods;
        }
        else {
            cout << " Pedagogy grade: "; cin >> p[i].thirdGrade.pedagogy;
        }
    }
}

void printHeader() {
    cout << "==========================================================================================\n";
    cout << "| No | Surname    | Course | Speciality      | Phys | Math | Prog | NumM | Pedagogy |\n";
    cout << "------------------------------------------------------------------------------------------\n";
}

void printRow(Student s, int i) {
    cout << "| " << setw(2) << i + 1
        << " | " << setw(10) << s.surname
        << " | " << setw(6) << s.course
        << " | " << setw(15) << specialityStr[s.speciality]
        << " | " << setw(4) << s.physicsGrade
        << " | " << setw(4) << s.mathGrade;

    if (s.speciality == COMPUTER_SCIENCE)
        cout << " | " << setw(4) << s.thirdGrade.programming << " |      |          |";
    else if (s.speciality == INFORMATICS)
        cout << " |      | " << setw(4) << s.thirdGrade.numericalMethods << " |          |";
    else
        cout << " |      |      | " << setw(8) << s.thirdGrade.pedagogy << " |";
    cout << endl;
}

void print(Student* p, const int n) {
    printHeader();
    for (int i = 0; i < n; i++) printRow(p[i], i);
    cout << "==========================================================================================\n";
}

void physicalSort(Student* p, const int n) {
    sort(p, p + n, [](Student a, Student b) {
        if (a.speciality != b.speciality) return a.speciality < b.speciality;
        if (a.course != b.course) return a.course < b.course;
        return a.surname < b.surname;
        });
}

int* createIndexArray(Student* p, const int n) {
    int* idx = new int[n];
    for (int i = 0; i < n; i++) idx[i] = i;

    sort(idx, idx + n, [&](int i, int j) {
        if (p[i].speciality != p[j].speciality) return p[i].speciality < p[j].speciality;
        if (p[i].course != p[j].course) return p[i].course < p[j].course;
        return p[i].surname < p[j].surname;
        });
    return idx;
}

void printIndexed(Student* p, int* indexArray, const int n) {
    printHeader();
    for (int i = 0; i < n; i++) printRow(p[indexArray[i]], i);
    cout << "==========================================================================================\n";
}

int binarySearch(Student* p, const int n, string surname, Speciality spec, int course) {
    int L = 0, R = n - 1;
    while (L <= R) {
        int m = L + (R - L) / 2;
        if (p[m].surname == surname && p[m].speciality == spec && p[m].course == course)
            return m;

        if ((p[m].speciality < spec) ||
            (p[m].speciality == spec && p[m].course < course) ||
            (p[m].speciality == spec && p[m].course == course && p[m].surname < surname))
            L = m + 1;
        else
            R = m - 1;
    }
    return -1;
}