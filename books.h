#pragma once
#include "utils.h"

class Book
{
    int id, price;
    char name[20], author[30];

public:
    Book() {}

    Book(int r, const char *n, const char *a, int p)
    {
        id = r;
        strncpy(name, n, sizeof(name) - 1);     name[sizeof(name) - 1] = '\0';
        strncpy(author, a, sizeof(author) - 1); author[sizeof(author) - 1] = '\0';
        price = p;
    }

    int getId() const { return id; }
    int getPrice() const { return price; }
    const char *getName() const { return name; }
    const char *getAuthor() const { return author; }

    void setName(const char *n)   { strncpy(name, n, sizeof(name) - 1);     name[sizeof(name) - 1] = '\0'; }
    void setAuthor(const char *a) { strncpy(author, a, sizeof(author) - 1); author[sizeof(author) - 1] = '\0'; }
    void setPrice(int p)          { price = p; }

    static void header()
    {
        cout << left << setw(20) << "BOOK ID" << setw(20) << "NAME" << setw(20) << "AUTHOR" << setw(20) << "PRICE" << "\n";
    }

    void display() const
    {
        cout << left << setw(20) << id << setw(20) << name << setw(20) << author << setw(20) << price << endl;
    }
};

// ---------- Queries ----------

int search_b(int target)
{
    Book s1;
    ifstream fin("Book.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getId() == target)
        {
            fin.close();
            return 1;
        }
    }

    fin.close();
    return 0;
}

void displayAllBooks()
{
    Book b;
    ifstream fin("Book.dat", ios::binary);
    int found = 0;

    while (fin.read((char *)&b, sizeof(b)))
    {
        if (found == 0)
        {
            Book::header();
            found = 1;
        }
        b.display();
    }

    if (found == 0)
        cout << "\nNo book records found.\n";

    fin.close();
}

void display_b(int t3)
{
    Book s1;
    int found = 0;
    ifstream fin("Book.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.getId() == t3)
        {
            cout << "\nRecord Found\n\n";
            found = 1;
            Book::header();
            s1.display();
            break;
        }
    }

    if (found == 0)
        cout << "NO RECORD FOUND !!!";

    fin.close();
}

// ---------- Mutations ----------

void dlt_b(int trgt)
{
    if (!search_b(trgt))
    {
        cout << "ERROR : BOOK not found!\n";
        return;
    }

    rewriteFile<Book>("Book.dat", [trgt](const Book &b) {
        return b.getId() != trgt;
    });

    cout << "Record deleted successfully.\n";
    Book::header();
    displayAllBooks();
}

void update_b(int t1)
{
    if (!search_b(t1))
    {
        cout << "ERROR : BOOK not found!\n";
        return;
    }

    char nname[20], aname[30];
    int pri;

    readStr("Enter New Name : ", nname, sizeof(nname));
    readStr("Enter Author : ", aname, sizeof(aname));
    pri = readInt("Enter Price : ");

    transformFile<Book>("Book.dat", [t1, &nname, &aname, pri](Book &b) {
        if (b.getId() == t1)
        {
            b.setName(nname);
            b.setAuthor(aname);
            b.setPrice(pri);
        }
    });

    cout << "Record Updated Successfully.\n";
    Book::header();
    displayAllBooks();
}

void insert_b(int t2)
{
    if (search_b(t2))
    {
        cout << "ERROR : This ID already exists!\n";
        return;
    }

    char n[20], a[30];
    int p;

    cout << "\nINSERT NEW RECORD\n";
    readStr("Enter Book Name: ", n, sizeof(n));
    readStr("Enter Author Name: ", a, sizeof(a));
    p = readInt("Enter Price: ");

    Book ns(t2, n, a, p);
    appendRecord("Book.dat", ns);

    cout << "Record inserted successfully!\n";
    Book::header();
    displayAllBooks();
}

// ---------- Menu ----------

void book_menu()
{
    int choice;
    int id;

    do
    {
        cout << "\n-----MENU-----";
        cout << "\n1. View All Records";
        cout << "\n2. Search Book";
        cout << "\n3. Delete Book";
        cout << "\n4. Update Book";
        cout << "\n5. Insert Book";
        cout << "\n6. Return to Main Menu";
        choice = readInt("\n\nEnter your choice: ", 1, 6);

        switch (choice)
        {
        case 1:
            displayAllBooks();
            break;
        case 2:
            id = readInt("\nEnter Book ID to Search: ");
            display_b(id);
            break;
        case 3:
            id = readInt("\nEnter Book ID to Delete: ");
            dlt_b(id);
            break;
        case 4:
            id = readInt("\nEnter Book ID to Update: ");
            update_b(id);
            break;
        case 5:
            id = readInt("Enter new Book ID to insert: ");
            insert_b(id);
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
