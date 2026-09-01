#pragma once
#include "utils.h"

class Student
{
    int roll;
    char name[20], cls[30];

public:
    Student() {}

    Student(int r, const char *n, const char *c)
    {
        roll = r;
        strncpy(name, n, sizeof(name) - 1); name[sizeof(name) - 1] = '\0';
        strncpy(cls, c, sizeof(cls) - 1);   cls[sizeof(cls) - 1]   = '\0';
    }

    int getRoll() const { return roll; }
    const char *getName() const { return name; }
    const char *getCls() const { return cls; }

    void setName(const char *n)
    {
        strncpy(name, n, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }

    void setCls(const char *c)
    {
        strncpy(cls, c, sizeof(cls) - 1);
        cls[sizeof(cls) - 1] = '\0';
    }

    static void header()
    {
        cout << left << setw(20) << "Roll NO" << setw(20) << "Name" << setw(20) << "Class" << "\n";
    }

    void display() const
    {
        cout << left << setw(20) << roll << setw(20) << name << setw(20) << cls << endl;
    }
};

// ---------- Queries ----------

int search_s(int target)
{
    Student s1;
    ifstream fin("Student.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getRoll() == target)
        {
            fin.close();
            return 1;
        }
    }

    fin.close();
    return 0;
}

void displayAll()
{
    Student s;
    ifstream fin("Student.dat", ios::binary);
    int found = 0;

    while (fin.read((char *)&s, sizeof(s)))
    {
        if (found == 0)
        {
            Student::header();
            found = 1;
        }
        s.display();
    }

    if (found == 0)
        cout << "\nNo student records found.\n";

    fin.close();
}

void display_s(int t3)
{
    Student s1;
    int found = 0;
    ifstream fin("Student.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getRoll() == t3)
        {
            cout << "\nRecord Found\n\n";
            found = 1;
            Student::header();
            s1.display();
            break;
        }
    }

    if (found == 0)
        cout << "NO RECORD FOUND !!! ";

    fin.close();
}

// ---------- Mutations ----------

void dlt_s(int trgt)
{
    if (!search_s(trgt))
    {
        cout << "ERROR : Roll Number not found!\n";
        return;
    }

    rewriteFile<Student>("Student.dat", [trgt](const Student &s) {
        return s.getRoll() != trgt;
    });

    cout << "Record deleted successfully.\n";
    Student::header();
    displayAll();
}

void update_s(int t1)
{
    if (!search_s(t1))
    {
        cout << "ERROR : Roll Number not found!\n";
        return;
    }

    char nname[20];
    readStr("Enter New Name : ", nname, sizeof(nname));

    transformFile<Student>("Student.dat", [t1, &nname](Student &s) {
        if (s.getRoll() == t1)
            s.setName(nname);
    });

    cout << "Record Updated Successfully.\n";
    Student::header();
    displayAll();
}

void insert_s(int t2)
{
    if (search_s(t2))
    {
        cout << "ERROR : Student with Roll No already exists!\n";
        return;
    }

    char n[20], c[30];
    cout << "\nINSERT NEW RECORD\n";
    readStr("Enter Name: ", n, sizeof(n));
    readStr("Enter Class: ", c, sizeof(c));

    Student ns(t2, n, c);
    appendRecord("Student.dat", ns);

    cout << "Record inserted successfully!\n";
    Student::header();
    displayAll();
}

// ---------- Menu ----------

void student_menu()
{
    int choice;
    int roll;

    do
    {
        cout << "\n-----MENU-----";
        cout << "\n1. View All Records";
        cout << "\n2. Search Student";
        cout << "\n3. Delete Student";
        cout << "\n4. Update Student";
        cout << "\n5. Insert Student";
        cout << "\n6. Return to Main Menu";
        choice = readInt("\n\nEnter your choice: ", 1, 6);

        switch (choice)
        {
        case 1:
            displayAll();
            break;
        case 2:
            roll = readInt("\nEnter Roll No to Search: ");
            display_s(roll);
            break;
        case 3:
            roll = readInt("\nEnter Roll No to Delete: ");
            dlt_s(roll);
            break;
        case 4:
            roll = readInt("\nEnter Roll No to Update: ");
            update_s(roll);
            break;
        case 5:
            roll = readInt("Enter new Roll no to insert: ");
            insert_s(roll);
            break;
        case 6:
            cout << "Returning.....\n";
            break;
        default:
            cout << "\nInvalid!";
        }

        if (choice != 6)
        {
            pauseScreen();
            loading();
            clearScreen();
        }

    } while (choice != 6);
}
