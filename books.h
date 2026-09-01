class Book
{
    int id, price;
    char name[20], author[30];

public:
    Book()
    {
    }
    Book(int r, char n[], char a[], int p)
    {
        id = r;
        strcpy(name, n);
        strcpy(author, a);
        price = p;
    };

    static void header()
    {
        cout << left << setw(20) << "BOOK ID" << setw(20) << "NAME" << setw(20) << "AUTHOR" << setw(20) << "PRICE" << "\n";
    }

    void display()
    {
        cout << left << setw(20) << id << setw(20) << name << setw(20) << author << setw(20) << price << endl;
    };

    friend void display_b(int t3);
    friend int search_b(int target);
    friend void dlt_b(int trgt);
    friend void update_b(int t1);
    friend void insert_b(int t2);
    friend int available_book();
};

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
    {
        cout << "\nNo book records found.\n";
    }

    fin.close();
}

void display_b(int t3)
{
    int found = 0;
    Book s1;

    ifstream fin("Book.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.id == t3)
        {
            cout << "\nRecord Found\n\n";
            found = 1;
            Book::header();
            s1.display();
            break;
        }
    }

    if (found == 0)
    {
        cout << "NO RECORD FOUND !!!";
    }

    fin.close();
}

int search_b(int target)
{
    Book s1;

    ifstream fin("Book.dat", ios::binary);

    while (fin.read((char *)&s1, sizeof(s1)))
    {
        if (s1.id == target)
        {
            fin.close();
            return 1;
        }
    }

    fin.close();

    return 0;
}

void dlt_b(int trgt)
{
    try
    {
        if (search_b(trgt) != 1)
        {
            throw "BOOK not found!";
        }

        Book s1;

        ifstream fin("Book.dat", ios::binary);
        ofstream fout("5.dat", ios::binary);

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.id != trgt)
            {
                fout.write((char *)&s1, sizeof(s1));
            }
        }

        fin.close();
        fout.close();

        Book::header();
        ifstream fin1("5.dat", ios::binary);
        while (fin1.read((char *)&s1, sizeof(s1)))
        {
            s1.display();
        }
        fin1.close();

        remove("Book.dat");
        rename("5.dat", "Book.dat");
    }

    catch (const char *e)
    {
        cout << "ERROR : " << e << endl;
    }
}

void update_b(int t1)
{

    try
    {
        int found = search_b(t1);

        if (found != 1)
        {
            throw "BOOK not found!";
        }

        Book s1;
        char nname[20], aname[20];
        int pri;

        ifstream fin("Book.dat", ios::binary);
        ofstream fout("6.dat", ios::binary);

        cout << "Enter New Name : ";
        cin >> nname;
        cout << "Enter Author : ";
        cin >> aname;
        cout << "Enter Price : ";
        cin >> pri;

        while (fin.read((char *)&s1, sizeof(s1)))
        {
            if (s1.id == t1)
            {
                strcpy(s1.name, nname);
                strcpy(s1.author, aname);
                s1.price = pri;
                cout << "Record Updated Successfully.\n";
            }
            fout.write((char *)&s1, sizeof(s1));
        }
        fout.close();
        fin.close();

        Book::header();
        ifstream fin2("6.dat", ios::binary);
        while (fin2.read((char *)&s1, sizeof(s1)))
        {
            s1.display();
        }
        fin2.close();

        remove("Book.dat");
        rename("6.dat", "Book.dat");
    }
    catch (const char *exp)
    {
        cout << "ERROR : " << exp << endl;
    }
}

void insert_b(int t2)
{
    Book s1;
    try
    {
        if (search_b(t2))
        {
            throw "This ID already exists!\n";
        }

        char n[20], c[30];
        int p;

        cout << "\nINSERT NEW RECORD\n";

        cout << "Enter Book Name: ";
        cin >> n;
        cout << "Enter Author Name: ";
        cin >> c;
        cout << "Enter Price: ";
        cin >> p;

        Book ns(t2, n, c, p);

        ifstream fin2("Book.dat", ios::binary);
        ofstream fout("7.dat", ios::binary);

        while (fin2.read((char *)&s1, sizeof(s1)))
        {
            fout.write((char *)&s1, sizeof(s1));
        }

        fout.write((char *)&ns, sizeof(ns));

        fin2.close();
        fout.close();

        cout << "Record inserted successfully!\n";

        Book::header();
        ifstream fin3("7.dat", ios::binary);
        while (fin3.read((char *)&ns, sizeof(ns)))
        {
            ns.display();
        }
        fin3.close();

        remove("Book.dat");
        rename("7.dat", "Book.dat");
    }
    catch (const char *ex)
    {
        cout << "ERROR : " << ex << endl;
    }
}

int num_b(const char *msg = "", int menu = 0)
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
            cout << "ERROR : Enter an integer between 1 to 5 !!";
        }
    }
    return roll;
}

void delay_b(int ms)
{
    clock_t start = clock();

    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms)
        ;
}

void loading_b()
{
    char spin[] = {'|', '/', '-', '\\'};

    cout << "\nLoading ";

    for (int i = 0; i < 20; i++)
    {
        cout << spin[i % 4];
        cout.flush();
        delay_b(100);
        cout << '\b';
    }
}

void clear_b()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_b()
{
#ifdef _WIN32
    system("pause");
#else
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

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
        choice = num_b("\n\nEnter your choice: ", 1);

        switch (choice)
        {
        case 1:

            displayAllBooks();

            break;

        case 2:

            id = num_b("\nEnter Book ID to Search: ");
            display_b(id);
            break;

        case 3:

            id = num_b("\nEnter Book ID to Delete: ");
            dlt_b(id);
            break;

        case 4:

            id = num_b("\nEnter Book ID to Update: ");
            update_b(id);
            break;

        case 5:

            id = num_b("Enter new Book ID to insert: ");
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
            pause_b();
            loading_b();
            clear_b();
        }
    } while (choice != 6);
}