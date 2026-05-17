#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 200
#define NAME_SIZE 80
#define FILE_NAME "students.dat"

typedef struct {
    int id;
    char name[NAME_SIZE];
    double midterm;
    double finalExam;
    double project;
} Student;

Student students[MAX_STUDENTS];
int studentCount = 0;

void clearInputBuffer();
void readLine(char *buffer, int size);
void loadStudents();
void saveStudents();
int findStudentIndexById(int id);
double calculateAverage(Student student);
const char *letterGrade(double average);
void addStudent();
void updateStudent();
void deleteStudent();
void listStudents();
void searchStudent();
void classReport();
void exportReport();

int main() {
    int choice;

    loadStudents();

    while (1) {
        printf("\n=== C Student Records Pro ===\n");
        printf("1. Add student\n");
        printf("2. Update student\n");
        printf("3. Delete student\n");
        printf("4. List students\n");
        printf("5. Search student\n");
        printf("6. Class report\n");
        printf("7. Export text report\n");
        printf("8. Save and exit\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            clearInputBuffer();
            continue;
        }

        clearInputBuffer();

        if (choice == 1) {
            addStudent();
        } else if (choice == 2) {
            updateStudent();
        } else if (choice == 3) {
            deleteStudent();
        } else if (choice == 4) {
            listStudents();
        } else if (choice == 5) {
            searchStudent();
        } else if (choice == 6) {
            classReport();
        } else if (choice == 7) {
            exportReport();
        } else if (choice == 8) {
            saveStudents();
            printf("Saved. Goodbye.\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    return 0;
}

void clearInputBuffer() {
    int character;
    while ((character = getchar()) != '\n' && character != EOF) {
    }
}

void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

void loadStudents() {
    FILE *file = fopen(FILE_NAME, "rb");

    if (file == NULL) {
        return;
    }

    fread(&studentCount, sizeof(int), 1, file);

    if (studentCount > MAX_STUDENTS) {
        studentCount = 0;
        fclose(file);
        return;
    }

    fread(students, sizeof(Student), studentCount, file);
    fclose(file);
}

void saveStudents() {
    FILE *file = fopen(FILE_NAME, "wb");

    if (file == NULL) {
        printf("Error: Could not save data file.\n");
        return;
    }

    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(Student), studentCount, file);
    fclose(file);
}

int findStudentIndexById(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].id == id) {
            return i;
        }
    }

    return -1;
}

double calculateAverage(Student student) {
    return student.midterm * 0.30 + student.finalExam * 0.40 + student.project * 0.30;
}

const char *letterGrade(double average) {
    if (average >= 90) return "A";
    if (average >= 80) return "B";
    if (average >= 70) return "C";
    if (average >= 60) return "D";
    return "F";
}

void addStudent() {
    Student student;

    if (studentCount >= MAX_STUDENTS) {
        printf("Student storage is full.\n");
        return;
    }

    printf("Student ID: ");
    scanf("%d", &student.id);
    clearInputBuffer();

    if (findStudentIndexById(student.id) != -1) {
        printf("Student ID already exists.\n");
        return;
    }

    printf("Student name: ");
    readLine(student.name, NAME_SIZE);

    printf("Midterm grade: ");
    scanf("%lf", &student.midterm);

    printf("Final exam grade: ");
    scanf("%lf", &student.finalExam);

    printf("Project grade: ");
    scanf("%lf", &student.project);
    clearInputBuffer();

    if (student.midterm < 0 || student.midterm > 100 ||
        student.finalExam < 0 || student.finalExam > 100 ||
        student.project < 0 || student.project > 100) {
        printf("Grades must be between 0 and 100.\n");
        return;
    }

    students[studentCount] = student;
    studentCount++;
    saveStudents();

    printf("Student added successfully.\n");
}

void updateStudent() {
    int id;
    printf("Student ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    int index = findStudentIndexById(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    printf("New name: ");
    readLine(students[index].name, NAME_SIZE);

    printf("New midterm grade: ");
    scanf("%lf", &students[index].midterm);

    printf("New final exam grade: ");
    scanf("%lf", &students[index].finalExam);

    printf("New project grade: ");
    scanf("%lf", &students[index].project);
    clearInputBuffer();

    saveStudents();
    printf("Student updated successfully.\n");
}

void deleteStudent() {
    int id;
    printf("Student ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    int index = findStudentIndexById(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    for (int i = index; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }

    studentCount--;
    saveStudents();

    printf("Student deleted successfully.\n");
}

void listStudents() {
    if (studentCount == 0) {
        printf("No students found.\n");
        return;
    }

    printf("\nID | Name | Midterm | Final | Project | Average | Grade\n");
    printf("--------------------------------------------------------\n");

    for (int i = 0; i < studentCount; i++) {
        double average = calculateAverage(students[i]);

        printf("%d | %s | %.2f | %.2f | %.2f | %.2f | %s\n",
               students[i].id,
               students[i].name,
               students[i].midterm,
               students[i].finalExam,
               students[i].project,
               average,
               letterGrade(average));
    }
}

void searchStudent() {
    int id;
    printf("Student ID: ");
    scanf("%d", &id);
    clearInputBuffer();

    int index = findStudentIndexById(id);

    if (index == -1) {
        printf("Student not found.\n");
        return;
    }

    Student student = students[index];
    double average = calculateAverage(student);

    printf("\nStudent found:\n");
    printf("ID: %d\n", student.id);
    printf("Name: %s\n", student.name);
    printf("Average: %.2f\n", average);
    printf("Letter grade: %s\n", letterGrade(average));
}

void classReport() {
    if (studentCount == 0) {
        printf("No students found.\n");
        return;
    }

    double total = 0;
    int passCount = 0;
    int topIndex = 0;

    for (int i = 0; i < studentCount; i++) {
        double average = calculateAverage(students[i]);
        total += average;

        if (average >= 60) {
            passCount++;
        }

        if (average > calculateAverage(students[topIndex])) {
            topIndex = i;
        }
    }

    printf("\n=== Class Report ===\n");
    printf("Students: %d\n", studentCount);
    printf("Class average: %.2f\n", total / studentCount);
    printf("Passed: %d\n", passCount);
    printf("Failed: %d\n", studentCount - passCount);
    printf("Top student: %s with %.2f\n", students[topIndex].name, calculateAverage(students[topIndex]));
}

void exportReport() {
    FILE *file = fopen("student_report.txt", "w");

    if (file == NULL) {
        printf("Could not create report file.\n");
        return;
    }

    fprintf(file, "Student Report\n");
    fprintf(file, "==============\n");

    for (int i = 0; i < studentCount; i++) {
        double average = calculateAverage(students[i]);

        fprintf(file, "%d | %s | Average: %.2f | Grade: %s\n",
                students[i].id,
                students[i].name,
                average,
                letterGrade(average));
    }

    fclose(file);
    printf("Report exported to student_report.txt\n");
}
