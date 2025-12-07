#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.txt"
#define CRE_FILE  "credentials.txt"

char currentUser[50];
char currentRole[20];

int login() {
    char u[50], p[50], r[20];
    char inUser[50], inPass[50];

    printf("USERNAME:\n");
    scanf("%s", inUser);
    printf("PASSWORD:\n");
    scanf("%s", inPass);

    FILE *fp = fopen(CRE_FILE, "r");
    if (!fp) {
        printf("Credential file missing!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", u, p, r) == 3) {
        if (strcmp(inUser, u) == 0 && strcmp(inPass, p) == 0) {
            strcpy(currentUser, u);
            strcpy(currentRole, r);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addStudent() {
    int roll;
    char name[50];
    float mark;

    printf("Roll:\n");
    scanf("%d", &roll);
    printf("Name:\n");
    scanf(" %[^\n]", name);
    printf("Mark:\n");
    scanf("%f", &mark);

    FILE *fp = fopen(STUD_FILE, "a");
    if (!fp) {
        printf("Error opening student file!\n");
        return;
    }

    fprintf(fp, "%d %s %.2f\n", roll, name, mark);
    fclose(fp);

    printf("Student added!\n");
}

void displayStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    int roll;
    char name[50];
    float mark;

    printf("Roll\tName\tMark\n");
    printf("----\t----\t----\n");
    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        printf("%d\t%s\t%.2f\n", roll, name, mark);
    }

    fclose(fp);
}

void searchStudent() {
    int find, roll;
    char name[50];
    float mark;

    printf("Enter roll to search:\n");
    scanf("%d", &find);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == find) {
            printf("Found: %d %s %.2f\n", roll, name, mark);
            fclose(fp);
            return;
        }
    }
    fclose(fp);
    printf("Student not found!\n");
}

void deleteStudent() {
    int delRoll;
    printf("Enter roll to delete:\n");
    scanf("%d", &delRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll != delRoll) {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student deleted!\n");
    else printf("Roll not found!\n");
}

void updateStudent() {
    int updateRoll;
    printf("Enter roll to update:\n");
    scanf("%d", &updateRoll);

    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student file!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    if (!temp) {
        printf("Error creating temp file!\n");
        fclose(fp);
        return;
    }

    int roll;
    char name[50];
    float mark;
    int found = 0;

    while (fscanf(fp, "%d %s %f", &roll, name, &mark) == 3) {
        if (roll == updateRoll) {
            found = 1;
            char newName[50];
            float newMark;

            printf("New Name:\n");
            scanf(" %[^\n]", newName);
            printf("New Mark:\n");
            scanf("%f", &newMark);

            fprintf(temp, "%d %s %.2f\n", roll, newName, newMark);
        } else {
            fprintf(temp, "%d %s %.2f\n", roll, name, mark);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found) printf("Student updated!\n");
    else printf("Roll not found!\n");
}

void adminMenu() {
    int c;
    while (1) {
        printf("\nADMIN MENU\n");
        printf("1. Add\n");
        printf("2. Display\n");
        printf("3. Search\n");
        printf("4. Update\n");
        printf("5. Delete\n");
        printf("6. Logout\n");
        scanf("%d", &c);

        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else if (c == 5) deleteStudent();
        else return;
    }
}

void staffMenu() {
    int c;
    while (1) {
        printf("\nSTAFF MENU\n");
        printf("1. Add\n");
        printf("2. Display\n");
        printf("3. Search\n");
        printf("4. Update\n");
        printf("5. Logout\n");
        scanf("%d", &c);

        if (c == 1) addStudent();
        else if (c == 2) displayStudents();
        else if (c == 3) searchStudent();
        else if (c == 4) updateStudent();
        else return;
    }
}

void guestMenu() {
    int c;
    while (1) {
        printf("\nGUEST MENU\n");
        printf("1. Display\n");
        printf("2. Search\n");
        printf("3. Logout\n");
        scanf("%d", &c);

        if (c == 1) displayStudents();
        else if (c == 2) searchStudent();
        else return;
    }
}

int main() {
    if (!login()) {
        printf("Invalid login!\n");
        return 0;
    }

    printf("Logged in as: %s\n", currentRole);

    if (strcmp(currentRole, "admin") == 0) adminMenu();
    else if (strcmp(currentRole, "staff") == 0) staffMenu();
    else guestMenu();

    return 0;
}
