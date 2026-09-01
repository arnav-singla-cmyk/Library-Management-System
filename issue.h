#include "student.h"
#include "books.h"

class Issue
{
    int issue_id, student_id, book_id, days;
    char date[30], date2[30], date3[30];

public:
    Issue()
    {
    }
    Issue(int ii, int si, int bi, char d[], int da, char rda[], char dda[])
    {
        issue_id = ii;
        student_id = si;
        book_id = bi;
        strcpy(date, d);
        days = da;
        strcpy(date2, rda);
        strcpy(date3, dda);
    };

    static void header()
    {
        cout << left << setw(20) << "ISSUE ID"
             << setw(20) << "STUDENT ID"
             << setw(20) << "BOOK ID"
             << setw(20) << "DATE"
             << setw(20) << "DAYS ALLOWED"
             << setw(20) << "DUE DATE"
             << setw(20) << "RETURNING DATE"
             << setw(20) << "STATUS"
             << endl;
    }

    void display()
    {
        cout << left << setw(20)
             << issue_id << setw(20)
             << student_id << setw(20)
             << book_id << setw(20)
             << date << setw(20)
             << days << setw(20)
             << date3 << setw(20)
             << date2 << setw(20);

        if (strcmp(date2, "-----") == 0)
            cout << setw(20) << "Issued";
        else
            cout << setw(20) << "Returned";

        cout << endl;
    };

    friend void display(int t3);
    friend int search(int target);
    friend void dlt(int trgt);
    friend void update(int t1);
    friend void insert(int t2);
    friend void update_book(int t4);
    friend int issued_book(int id);
    friend void displayIssuedBooks();
    friend void displayReturnedBooks();
    friend int returned_book(int issueid);
};


int issued_book(int id)
{
    Issue s1;

    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.book_id == id && strcmp(s1.date2, "-----") == 0)
        {
            fin.close();
            return 1; // Book already issued
        }
    }

    fin.close();
    return 0; // Book available
}

int available_book()
{
    Book b;
    ifstream fin("Book.dat", ios::binary);

    while (fin.read((char *)&b, sizeof(b)))
    {
        if (!issued_book(b.id))
        {
            fin.close();
            return 1;
        }
    }

    fin.close();
    return 0;
}

int num(const char *msg = "", int menu = 0, char *date = NULL)
{
    int x;
    char ch;

    while (true)
    {
        try
        {
            cout << msg;

            // Date Input
            if (menu == 2)
            {
                cin >> date;

                int d, m, y;

                if (sscanf(date, "%d/%d/%d", &d, &m, &y) != 3)
                    throw "ENTER DATE IN dd/mm/yyyy FORMAT";

                tm t = {};
                t.tm_mday = d;
                t.tm_mon = m - 1;
                t.tm_year = y - 1900;
                t.tm_isdst = -1;

                mktime(&t);

                if (t.tm_mday != d || t.tm_mon != m - 1 || t.tm_year != y - 1900)
                    throw "INVALID DATE";

                break;
            }

            // Integer Input
            if (!(cin >> x))
                throw "ENTER VALID INTEGER";

            if (cin.get(ch) && ch != '\n')
                throw "EXTRA CHARACTERS FOUND";

            if (menu == 1 && (x < 1 || x > 9))
                throw 1.5;

            break;
        }

        catch (const char *e)
        {
            cout << "ERROR : " << e << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        catch (double)
        {
            cout << "ERROR : Enter an integer between 1 to 9 !!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return x;
}

int compareDate(char d1[], char d2[])
{
    int day, month, year;

    tm t1 = {}, t2 = {};

    sscanf(d1, "%d/%d/%d", &day, &month, &year);
    t1.tm_mday = day;
    t1.tm_mon = month - 1;
    t1.tm_year = year - 1900;

    sscanf(d2, "%d/%d/%d", &day, &month, &year);
    t2.tm_mday = day;
    t2.tm_mon = month - 1;
    t2.tm_year = year - 1900;

    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);

    if (time1 < time2)
        return -1;

    if (time1 > time2)
        return 1;

    return 0;
}

int validBookUpdate(int currentBook)
{
    int id;

    while (true)
    {
        id = num("Enter BOOK ID : ");

        if (!search_b(id))
        {
            cout << "ERROR : Invalid BOOK ID!\n";
            continue;
        }

        if (id != currentBook && issued_book(id))
        {
            cout << "ERROR : Book already issued!\n";
            continue;
        }

        return id;
    }
}

int returned_book(int issueid)
{
    Issue s1;

    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.issue_id == issueid)
        {
            fin.close();

            if (strcmp(s1.date2, "-----") == 0)
                return 0; // Not returned yet

            return 1; // Already returned
        }
    }

    fin.close();

    return -1; // Issue ID not found
}

int validDays()
{
    int d;

    while (true)
    {
        d = num("Enter ALLOWED DAYS : ");

        if (d > 0)
            return d;

        cout << "ERROR : Days must be greater than zero.\n";
    }
}

int validStudent()
{
    int id;

    while (true)
    {
        id = num("Enter STUDENT ID : ");

        if (search_s(id))
            return id;

        cout << "ERROR : Invalid STUDENT ID!\n";
    }
}

int validBook()
{
    int id;

    while (true)
    {
        id = num("Enter BOOK ID : ");

        if (!search_b(id))
        {
            cout << "ERROR : Invalid BOOK ID!\n";
            continue;
        }

        if (issued_book(id))
        {
            cout << "ERROR : Book already issued!\n";
            continue;
        }

        return id;
    }
}

void calculateDueDate(char issue[], int days, char due[])
{
    int d, m, y;

    sscanf(issue, "%d/%d/%d", &d, &m, &y);

    tm date = {};

    date.tm_mday = d;
    date.tm_mon = m - 1;
    date.tm_year = y - 1900;

    time_t t = mktime(&date);

    t += (long long)days * 24 * 60 * 60;

    tm *newDate = localtime(&t);

    sprintf(due, "%02d/%02d/%04d",
            newDate->tm_mday,
            newDate->tm_mon + 1,
            newDate->tm_year + 1900);
}

int fine(char due[], char ret[])
{
    int d1, m1, y1, d2, m2, y2;

    sscanf(due, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(ret, "%d/%d/%d", &d2, &m2, &y2);

    tm dueDate = {};
    dueDate.tm_mday = d1;
    dueDate.tm_mon = m1 - 1;
    dueDate.tm_year = y1 - 1900;

    tm returnDate = {};
    returnDate.tm_mday = d2;
    returnDate.tm_mon = m2 - 1;
    returnDate.tm_year = y2 - 1900;

    time_t t1 = mktime(&dueDate);
    time_t t2 = mktime(&returnDate);

    int late = (t2 - t1) / (24 * 60 * 60);

    if (late < 0)
        late = 0;

    return late;
}

void display(int t3)
{
    Issue s1;
    int found = 0;

    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.issue_id == t3)
        {
            cout << "\nRecord Found\n\n";
            Issue::header();
            found = 1;
            s1.display();
            break;
        }
    }

    if (found == 0)
    {
        cout << "\nNo Record Found\n\n";
    }

    fin.close();
}

int search(int target)
{
    Issue s1;

    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.issue_id == target)
        {
            fin.close();
            return 1;
        }
    }

    fin.close();

    return 0;
}

void dlt(int trgt)
{
    try
    {
        if (search(trgt) != 1)
        {
            throw "ISSUE ID not found!";
        }

        Issue s1;

        ifstream fin("Issue.dat", ios::binary);
        ofstream fout("5.dat", ios::binary);

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.issue_id != trgt)
            {
                fout.write((char *)&s1, sizeof(s1));
            }
        }

        fin.close();
        fout.close();

        Issue::header();
        ifstream fin1("5.dat", ios::binary);
        while (fin1.read((char *)&s1, sizeof(s1)))
        {
            s1.display();
        }
        fin1.close();

        remove("Issue.dat");
        rename("5.dat", "Issue.dat");
    }

    catch (const char *e)
    {
        cout << "ERROR : " << e << endl;
    }
}

void update(int t1)
{
    try
    {
        if (!search(t1))
            throw "ISSUE ID not found!";

        Issue s1;
        int currentBook = 0;

        // Find the current book ID of this issue
        ifstream fin("Issue.dat", ios::binary);

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.issue_id == t1)
            {
                currentBook = s1.book_id;
                break;
            }
        }

        fin.close();

        int nname = validStudent();
        int bname = validBookUpdate(currentBook);

        char a[30];
        num("Enter ISSUE DATE (dd/mm/yyyy) : ", 2, a);

        int n = validDays();

        ifstream fin2("Issue.dat", ios::binary);
        ofstream fout("6.dat", ios::binary);

        while (fin2.read((char *)&s1, sizeof(s1)))
        {
            if (s1.issue_id == t1)
            {
                s1.student_id = nname;
                s1.book_id = bname;
                strcpy(s1.date, a);
                s1.days = n;

                calculateDueDate(a, n, s1.date3);

                // Since issue details changed, mark as not returned
                strcpy(s1.date2, "-----");

                cout << "\nRecord Updated Successfully.\n";
            }

            fout.write((char *)&s1, sizeof(s1));
        }

        fin2.close();
        fout.close();

        Issue::header();

        ifstream fin3("6.dat", ios::binary);

        while (fin3.read((char *)&s1, sizeof(s1)))
        {
            s1.display();
        }

        fin3.close();

        remove("Issue.dat");
        rename("6.dat", "Issue.dat");
    }
    catch (const char *e)
    {
        cout << "ERROR : " << e << endl;
    }
}

void update_book(int t4)
{
    try
    {
        int status = returned_book(t4);

        if (status == -1)
            throw "ISSUE ID not found!";

        if (status == 1)
            throw "Book has already been returned!";

        Issue s1;
        char rdate[30];

        ifstream fin("Issue.dat", ios::binary);
        ofstream fout("8.dat", ios::binary);

        num("Enter RETURN DATE (dd/mm/yyyy) : ", 2, rdate);

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.issue_id == t4)
            {
                if (compareDate(rdate, s1.date) == -1)
                {
                    fin.close();
                    fout.close();
                    remove("8.dat");
                    throw "Return date cannot be before Issue date!";
                }

                break;
            }
        }

        fin.clear();
        fin.seekg(0, ios::beg);

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.issue_id == t4)
            {
                strcpy(s1.date2, rdate);

                int late = fine(s1.date3, rdate);

                cout << "\nBook Returned Successfully\n\n";

                cout << "Issue Date      : " << s1.date << endl;
                cout << "Due Date        : " << s1.date3 << endl;
                cout << "Return Date     : " << rdate << endl;

                if (late == 0)
                {
                    cout << "Status          : Returned On Time" << endl;
                }
                else
                {
                    cout << "Late By         : " << late << " day(s)" << endl;
                    cout << "Fine            : Rs. " << late * 100 << endl;
                }
            }

            fout.write((char *)&s1, sizeof(s1));
        }

        fin.close();
        fout.close();

        Issue::header();
        ifstream fin2("8.dat", ios::binary);

        while (fin2.read((char *)&s1, sizeof(s1)))
        {
            s1.display();
        }

        fin2.close();

        remove("Issue.dat");
        rename("8.dat", "Issue.dat");
    }

    catch (const char *e)
    {
        cout << "ERROR : " << e << endl;
    }
}

void insert(int t2)
{
    Issue s1;

    try
    {
        if (search(t2))
            throw "ISSUE ID already exists!";

        int s, b, da;
        char d[30], due[30], rda[30] = "-----";

        cout << "\nINSERT NEW RECORD\n";

        s = validStudent();
        b = validBook();

        num("Enter ISSUE DATE (dd/mm/yyyy) : ", 2, d);

        da = validDays();

        calculateDueDate(d, da, due);

        cout << "\nDUE DATE : " << due << endl;

        Issue ns(t2, s, b, d, da, rda, due);

        ifstream fin("Issue.dat", ios::binary);
        ofstream fout("7.dat", ios::binary);

        while (fin.read((char *)&s1, sizeof(s1)))
            fout.write((char *)&s1, sizeof(s1));

        fout.write((char *)&ns, sizeof(ns));

        fin.close();
        fout.close();

        cout << "\nRecord Inserted Successfully!\n";

        Issue::header();

        ifstream fin2("7.dat", ios::binary);

        while (fin2.read((char *)&s1, sizeof(s1)))
            s1.display();

        fin2.close();

        remove("Issue.dat");
        rename("7.dat", "Issue.dat");
    }

    catch (const char *e)
    {
        cout << "ERROR : " << e << endl;
    }
}

void delay(int ms)
{
    clock_t start = clock();

    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms)
        ;
}

void loading()
{
    char spin[] = {'|', '/', '-', '\\'};

    cout << "\nLoading ";

    for (int i = 0; i < 20; i++)
    {
        cout << spin[i % 4];
        cout.flush();
        delay(100);
        cout << '\b';
    }
}

void clear()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
#ifdef _WIN32
    system("pause");
#else
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

void displayAllIssue()
{
    Issue i;
    ifstream fin("Issue.dat", ios::binary);

    int found = 0;

    while (fin.read((char *)&i, sizeof(i)))
    {
        if (found == 0)
        {
            Issue::header();
            found = 1;
        }

        i.display();
    }

    if (found == 0)
    {
        cout << "\nNo issue records found.\n";
    }

    fin.close();
}

void displayIssuedBooks()
{
    Issue i;
    ifstream fin("Issue.dat", ios::binary);

    int found = 0;

    cout << "\nCURRENTLY ISSUED BOOKS\n\n";
    Issue::header();

    while (fin.read((char *)&i, sizeof(i)))
    {
        if (strcmp(i.date2, "-----") == 0)
        {
            found = 1;
            i.display();
        }
    }

    if (!found)
        cout << "\nNo books are currently issued.\n";

    fin.close();
}

void displayReturnedBooks()
{
    Issue i;
    ifstream fin("Issue.dat", ios::binary);

    int found = 0;

    cout << "\nRETURNED BOOKS\n\n";
    Issue::header();

    while (fin.read((char *)&i, sizeof(i)))
    {
        if (strcmp(i.date2, "-----") != 0)
        {
            found = 1;
            i.display();
        }
    }

    if (!found)
        cout << "\nNo returned books found.\n";

    fin.close();
}

void issue_menu()
{
    int choice;
    int issue_id;

    do
    {
        cout << "\n-----MENU-----";
        cout << "\n1. View All Records";
        cout << "\n2. Search Issue ID";
        cout << "\n3. Delete Issue ID";
        cout << "\n4. Update Issue Details";
        cout << "\n5. Update Return Date";
        cout << "\n6. Insert New Issue ID";
        cout << "\n7. View All Issued Books";
        cout << "\n8. View All Returned Books";
        cout << "\n9. Return to Main Menu";
        choice = num("\n\nEnter your choice: ", 1);

        switch (choice)
        {
        case 1:

            displayAllIssue();

            break;

        case 2:

            issue_id = num("\nEnter ISSUE ID to Search: ");
            display(issue_id);
            break;

        case 3:

            issue_id = num("\nEnter ISSUE ID to Delete: ");
            dlt(issue_id);
            break;

        case 4:

            issue_id = num("\nEnter ISSUE ID to Update: ");
            update(issue_id);
            break;

        case 5:
            issue_id = num("\nEnter ISSUE ID to add Returning Date: ");
            update_book(issue_id);
            break;

        case 6:

            if (!available_book())
            {
                cout << "\nERROR : No books are available for issue!\n";
                break;
            }

            issue_id = num("Enter new ISSUE ID to insert: ");
            insert(issue_id);
            break;

        case 7:

            displayIssuedBooks();
            break;

        case 8:

            displayReturnedBooks();
            break;

        case 9:
            cout << "Returning.....\n";
            break;

        default:
            cout << "\nInvalid!";
        }
        if (choice != 9)
        {
            pause();
            loading();
            clear();
        }
    } while (choice != 9);
}