class Student
{
    int roll;
    char name[20], cls[30];

public:
    Student()
    {
    }
    Student(int r, char n[], char c[])
    {
        roll = r;
        strcpy(name, n);
        strcpy(cls, c);
    };

    static void header()
    {
        cout << left << setw(20) << "Roll NO" << setw(20) << "Name" << setw(20) << "Class" << "\n";
    }

    void display()
    {
        cout << left << setw(20) << roll << setw(20) << name << setw(20) << cls << endl;
    };

    friend void display_s(int t3);
    friend int search_s(int target);
    friend void dlt_s(int trgt);
    friend void update_s(int t1);
    friend void insert_s(int t2);
};

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
    {
        cout << "\nNo student records found.\n";
    }

    fin.close();
}

void display_s(int t3)
{
    int found = 0;
    Student s1;

    ifstream fin("Student.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.roll == t3)
        {
            cout << "\nRecord Found\n\n";
            found = 1;
            Student::header();
            s1.display();
            break;
        }
    }

    if (found == 0)
    {
        cout << "NO RECORD FOUND !!! ";
    }

    fin.close();
}

int search_s(int target)
{
    Student s1;

    ifstream fin("Student.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.roll == target)
        {
            fin.close();
            return 1;
        }
    }

    fin.close();

    return 0;
}

void dlt_s(int trgt)
{
    try
    {
        if (search_s(trgt) != 1)
        {
            throw "Roll Number not found!";
        }

        Student s1;

        ifstream fin("Student.dat", ios::binary);
        ofstream fout("5.dat", ios::binary);

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.roll != trgt)
            {
                fout.write((char *)&s1, sizeof(s1));
            }
        }

        fin.close();
        fout.close();

        Student::header();
        ifstream fin1("5.dat", ios::binary);
        while (fin1.read((char *)&s1, sizeof(s1)))
        {
            s1.display();
        }
        fin1.close();

        remove("Student.dat");
        rename("5.dat", "Student.dat");
    }

    catch (const char *e)
    {
        cout << "ERROR : " << e << endl;
    }
}

void update_s(int t1)
{

    try
    {
        int found = search_s(t1);

        if (found != 1)
        {
            throw "Roll Number not found!";
        }

        Student s1;
        char nname[20];

        ifstream fin("Student.dat", ios::binary);
        ofstream fout("6.dat", ios::binary);

        cout << "Enter New Name : ";
        cin >> nname;

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.roll == t1)
            {
                strcpy(s1.name, nname);
                cout << "Record Updated Successfully.\n";
            }
            fout.write((char *)&s1, sizeof(s1));
        }
        fout.close();
        fin.close();

        Student::header();
        ifstream fin2("6.dat", ios::binary);
        while (fin2.read((char *)&s1, sizeof(s1)))
        {
            s1.display();
        }
        fin2.close();

        remove("Student.dat");
        rename("6.dat", "Student.dat");
    }
    catch (const char *exp)
    {
        cout << "ERROR : " << exp << endl;
    }
}

void insert_s(int t2)
{
    Student s1;
    try
    {
        if (search_s(t2))
        {
            throw "Student with Roll No already exists!\n";
        }

        char n[20], c[30];

        cout << "\nINSERT NEW RECORD\n";

        cout << "Enter Name: ";
        cin >> n;
        cout << "Enter Class: ";
        cin >> c;

        Student ns(t2, n, c);

        ifstream fin2("Student.dat", ios::binary);
        ofstream fout("7.dat", ios::binary);

        while (fin2.read((char *)&s1, sizeof(s1)))
        {
            fout.write((char *)&s1, sizeof(s1));
        }

        fout.write((char *)&ns, sizeof(ns));

        fin2.close();
        fout.close();

        cout << "Record inserted successfully!\n";

        Student::header();
        ifstream fin3("7.dat", ios::binary);
        while (fin3.read((char *)&ns, sizeof(ns)))
        {
            ns.display();
        }
        fin3.close();

        remove("Student.dat");
        rename("7.dat", "Student.dat");
    }
    catch (const char *ex)
    {
        cout << "ERROR : " << ex << endl;
    }
}

int num_s(const char *msg = "", int menu = 0)
{
    int roll;
    char ch;
    while (true)
    {
        try
        {
            cout << msg;
            if (!(cin >> roll))
                throw "Input is not a number.";

            if (cin.get(ch) && ch != '\n')
                throw "Extra characters found.";
            if (menu == 1 && (roll > 6 || roll < 1))
            {
                throw 1.5;
            }
            break;
        }

        catch (const char *e)
        {
            cout << "Error: " << e << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        catch (double e)
        {
            cout << "ERROR : Enter an integer between 1 to 6 !!";
        }
    }
    return roll;
}

void delay_s(int ms)
{
    clock_t start = clock();

    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms)
        ;
}

void loading_s()
{
    char spin[] = {'|', '/', '-', '\\'};

    cout << "\nLoading ";

    for (int i = 0; i < 20; i++)
    {
        cout << spin[i % 4];
        cout.flush();
        delay_s(100);
        cout << '\b';
    }
}

void clear_s()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_s()
{
#ifdef _WIN32
    system("pause");
#else
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

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
        choice = num_s("\n\nEnter your choice: ", 1);

        switch (choice)
        {
        case 1:

            displayAll();

            break;

        case 2:

            roll = num_s("\nEnter Roll No to Search: ");
            display_s(roll);
            break;

        case 3:

            roll = num_s("\nEnter Roll No to Delete: ");
            dlt_s(roll);
            break;

        case 4:

            roll = num_s("\nEnter Roll No to Update: ");
            update_s(roll);
            break;

        case 5:

            roll = num_s("Enter new Roll no to insert: ");
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
            pause_s();
            loading_s();
            clear_s();
        }
    } while (choice != 6);
}