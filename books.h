#pragma once
#include "student.h"
#include "books.h"

class Issue
{
    int issue_id, student_id, book_id, days;
    char date[30], date2[30], date3[30];

public:
    Issue() {}

    Issue(int ii, int si, int bi, const char *d, int da, const char *rda, const char *dda)
    {
        issue_id = ii;
        student_id = si;
        book_id = bi;
        strncpy(date, d, sizeof(date) - 1);     date[sizeof(date) - 1] = '\0';
        days = da;
        strncpy(date2, rda, sizeof(date2) - 1); date2[sizeof(date2) - 1] = '\0';
        strncpy(date3, dda, sizeof(date3) - 1); date3[sizeof(date3) - 1] = '\0';
    }

    int getIssueId() const   { return issue_id; }
    int getStudentId() const { return student_id; }
    int getBookId() const    { return book_id; }
    int getDays() const      { return days; }
    const char *getDate() const  { return date; }  // issue date
    const char *getDate2() const { return date2; } // return date ("-----" if not returned)
    const char *getDate3() const { return date3; } // due date

    void setStudentId(int s) { student_id = s; }
    void setBookId(int b)    { book_id = b; }
    void setDays(int d)      { days = d; }
    void setDate(const char *d)  { strncpy(date, d, sizeof(date) - 1);   date[sizeof(date) - 1] = '\0'; }
    void setDate2(const char *d) { strncpy(date2, d, sizeof(date2) - 1); date2[sizeof(date2) - 1] = '\0'; }
    void setDate3(const char *d) { strncpy(date3, d, sizeof(date3) - 1); date3[sizeof(date3) - 1] = '\0'; }

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

    void display() const
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
    }
};

// ---------- Date helpers ----------

int compareDate(const char *d1, const char *d2)
{
    int day, month, year;

    tm t1 = {}, t2 = {};

    sscanf(d1, "%d/%d/%d", &day, &month, &year);
    t1.tm_mday = day; t1.tm_mon = month - 1; t1.tm_year = year - 1900;

    sscanf(d2, "%d/%d/%d", &day, &month, &year);
    t2.tm_mday = day; t2.tm_mon = month - 1; t2.tm_year = year - 1900;

    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);

    if (time1 < time2) return -1;
    if (time1 > time2) return 1;
    return 0;
}

void calculateDueDate(const char *issue, int days, char *due)
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
            newDate->tm_mday, newDate->tm_mon + 1, newDate->tm_year + 1900);
}

int fine(const char *due, const char *ret)
{
    int d1, m1, y1, d2, m2, y2;

    sscanf(due, "%d/%d/%d", &d1, &m1, &y1);
    sscanf(ret, "%d/%d/%d", &d2, &m2, &y2);

    tm dueDate = {};
    dueDate.tm_mday = d1; dueDate.tm_mon = m1 - 1; dueDate.tm_year = y1 - 1900;

    tm returnDate = {};
    returnDate.tm_mday = d2; returnDate.tm_mon = m2 - 1; returnDate.tm_year = y2 - 1900;

    time_t t1 = mktime(&dueDate);
    time_t t2 = mktime(&returnDate);

    int late = (t2 - t1) / (24 * 60 * 60);
    return late < 0 ? 0 : late;
}

// ---------- Queries ----------

int search(int target)
{
    Issue s1;
    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getIssueId() == target)
        {
            fin.close();
            return 1;
        }
    }

    fin.close();
    return 0;
}

int issued_book(int id)
{
    Issue s1;
    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getBookId() == id && strcmp(s1.getDate2(), "-----") == 0)
        {
            fin.close();
            return 1; // already issued
        }
    }

    fin.close();
    return 0; // available
}

int available_book()
{
    Book b;
    ifstream fin("Book.dat", ios::binary);

    while (fin.read((char *)&b, sizeof(b)))
    {
        if (!issued_book(b.getId()))
        {
            fin.close();
            return 1;
        }
    }

    fin.close();
    return 0;
}

// Returns: 1 = already returned, 0 = still issued, -1 = issue ID not found
int returned_book(int issueid)
{
    Issue s1;
    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getIssueId() == issueid)
        {
            fin.close();
            return strcmp(s1.getDate2(), "-----") == 0 ? 0 : 1;
        }
    }

    fin.close();
    return -1;
}

// ---------- Validated prompts ----------

int validStudent()
{
    int id;
    while (true)
    {
        id = readInt("Enter STUDENT ID : ");
        if (search_s(id)) return id;
        cout << "ERROR : Invalid STUDENT ID!\n";
    }
}

int validBook()
{
    int id;
    while (true)
    {
        id = readInt("Enter BOOK ID : ");
        if (!search_b(id)) { cout << "ERROR : Invalid BOOK ID!\n"; continue; }
        if (issued_book(id)) { cout << "ERROR : Book already issued!\n"; continue; }
        return id;
    }
}

int validBookUpdate(int currentBook)
{
    int id;
    while (true)
    {
        id = readInt("Enter BOOK ID : ");
        if (!search_b(id)) { cout << "ERROR : Invalid BOOK ID!\n"; continue; }
        if (id != currentBook && issued_book(id)) { cout << "ERROR : Book already issued!\n"; continue; }
        return id;
    }
}

int validDays()
{
    int d;
    while (true)
    {
        d = readInt("Enter ALLOWED DAYS : ");
        if (d > 0) return d;
        cout << "ERROR : Days must be greater than zero.\n";
    }
}

// ---------- Display ----------

void displayAllIssue()
{
    Issue i;
    ifstream fin("Issue.dat", ios::binary);
    int found = 0;

    while (fin.read((char *)&i, sizeof(i)))
    {
        if (found == 0) { Issue::header(); found = 1; }
        i.display();
    }

    if (found == 0)
        cout << "\nNo issue records found.\n";

    fin.close();
}

void display(int t3)
{
    Issue s1;
    int found = 0;
    ifstream fin("Issue.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getIssueId() == t3)
        {
            cout << "\nRecord Found\n\n";
            Issue::header();
            found = 1;
            s1.display();
            break;
        }
    }

    if (found == 0)
        cout << "\nNo Record Found\n\n";

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
        if (strcmp(i.getDate2(), "-----") == 0)
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
        if (strcmp(i.getDate2(), "-----") != 0)
        {
            found = 1;
            i.display();
        }
    }

    if (!found)
        cout << "\nNo returned books found.\n";

    fin.close();
}

// ---------- Mutations ----------

void dlt(int trgt)
{
    if (!search(trgt))
    {
        cout << "ERROR : ISSUE ID not found!\n";
        return;
    }

    rewriteFile<Issue>("Issue.dat", [trgt](const Issue &i) {
        return i.getIssueId() != trgt;
    });

    cout << "Record deleted successfully.\n";
    displayAllIssue();
}

void update(int t1)
{
    if (!search(t1))
    {
        cout << "ERROR : ISSUE ID not found!\n";
        return;
    }

    // Find the current book ID for this issue (needed by validBookUpdate)
    int currentBook = 0;
    {
        Issue s1;
        ifstream fin("Issue.dat", ios::binary);
        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.getIssueId() == t1)
            {
                currentBook = s1.getBookId();
                break;
            }
        }
        fin.close();
    }

    int nname = validStudent();
    int bname = validBookUpdate(currentBook);

    char a[30];
    readDate("Enter ISSUE DATE (dd/mm/yyyy) : ", a, sizeof(a));

    int n = validDays();

    char due[30];
    calculateDueDate(a, n, due);

    transformFile<Issue>("Issue.dat", [t1, nname, bname, &a, n, &due](Issue &s1) {
        if (s1.getIssueId() == t1)
        {
            s1.setStudentId(nname);
            s1.setBookId(bname);
            s1.setDate(a);
            s1.setDays(n);
            s1.setDate3(due);
            s1.setDate2("-----"); // details changed, mark as not returned
        }
    });

    cout << "\nRecord Updated Successfully.\n";
    displayAllIssue();
}

void update_book(int t4)
{
    int status = returned_book(t4);

    if (status == -1) { cout << "ERROR : ISSUE ID not found!\n"; return; }
    if (status == 1)  { cout << "ERROR : Book has already been returned!\n"; return; }

    // Fetch the issue's own dates before prompting, so we can validate the return date
    char issueDate[30], dueDate[30];
    {
        Issue s1;
        ifstream fin("Issue.dat", ios::binary);
        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.getIssueId() == t4)
            {
                strncpy(issueDate, s1.getDate(), sizeof(issueDate));
                strncpy(dueDate, s1.getDate3(), sizeof(dueDate));
                break;
            }
        }
        fin.close();
    }

    char rdate[30];
    while (true)
    {
        readDate("Enter RETURN DATE (dd/mm/yyyy) : ", rdate, sizeof(rdate));
        if (compareDate(rdate, issueDate) == -1)
        {
            cout << "ERROR : Return date cannot be before Issue date!\n";
            continue;
        }
        break;
    }

    transformFile<Issue>("Issue.dat", [t4, &rdate, &dueDate](Issue &s1) {
        if (s1.getIssueId() == t4)
        {
            s1.setDate2(rdate);

            int late = fine(dueDate, rdate);

            cout << "\nBook Returned Successfully\n\n";
            cout << "Issue Date      : " << s1.getDate() << endl;
            cout << "Due Date        : " << dueDate << endl;
            cout << "Return Date     : " << rdate << endl;

            if (late == 0)
                cout << "Status          : Returned On Time" << endl;
            else
            {
                cout << "Late By         : " << late << " day(s)" << endl;
                cout << "Fine            : Rs. " << late * 100 << endl;
            }
        }
    });

    displayAllIssue();
}

void insert(int t2)
{
    if (search(t2))
    {
        cout << "ERROR : ISSUE ID already exists!\n";
        return;
    }

    cout << "\nINSERT NEW RECORD\n";

    int s = validStudent();
    int b = validBook();

    char d[30], due[30];
    readDate("Enter ISSUE DATE (dd/mm/yyyy) : ", d, sizeof(d));

    int da = validDays();
    calculateDueDate(d, da, due);

    cout << "\nDUE DATE : " << due << endl;

    Issue ns(t2, s, b, d, da, "-----", due);
    appendRecord("Issue.dat", ns);

    cout << "\nRecord Inserted Successfully!\n";
    displayAllIssue();
}

// ---------- Menu ----------

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
        choice = readInt("\n\nEnter your choice: ", 1, 9);

        switch (choice)
        {
        case 1:
            displayAllIssue();
            break;
        case 2:
            issue_id = readInt("\nEnter ISSUE ID to Search: ");
            display(issue_id);
            break;
        case 3:
            issue_id = readInt("\nEnter ISSUE ID to Delete: ");
            dlt(issue_id);
            break;
        case 4:
            issue_id = readInt("\nEnter ISSUE ID to Update: ");
            update(issue_id);
            break;
        case 5:
            issue_id = readInt("\nEnter ISSUE ID to add Returning Date: ");
            update_book(issue_id);
            break;
        case 6:
            if (!available_book())
            {
                cout << "\nERROR : No books are available for issue!\n";
                break;
            }
            issue_id = readInt("Enter new ISSUE ID to insert: ");
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
            pauseScreen();
            loading();
            clearScreen();
        }

    } while (choice != 9);
}
