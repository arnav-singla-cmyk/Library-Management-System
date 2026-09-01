#include "issue.h"

int num_f(const char *msg = "", int menu = 0)
{
    int x;
    while (true)
    {
        try
        {
            cout << msg;
            cin >> x;
            if (cin.fail())
            {
                throw "ENTER VALID DATA-TYPE";
            }
            if (menu == 1 && (x > 4 || x < 1))
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
            cout << "ERROR : Enter an integer between 1 to 4 !!";
        }
    }
    return x;
}

void delay_f(int ms)
{
    clock_t start = clock();

    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms)
        ;
}

void loading_f()
{
    char spin[] = {'|', '/', '-', '\\'};

    cout << "\nLoading ";

    for (int i = 0; i < 20; i++)
    {
        cout << spin[i % 4];
        cout.flush();
        delay_f(100);
        cout << '\b';
    }
}

void clear_f()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause_f()
{
#ifdef _WIN32
    system("pause");
#else
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

void final_menu()
{
    int choice;

    do
    {
        cout << "\n-----MENU-----";
        cout << "\n1. MANAGE Student";
        cout << "\n2. MANAGE Books";
        cout << "\n3. MANAGE Issue System";
        cout << "\n4. Exit";
        choice = num_f("\n\nEnter your choice: ", 1);

        switch (choice)
        {
        case 1:

            student_menu();

            break;

        case 2:

            book_menu();

            break;

        case 3:

            issue_menu();

            break;

        case 4:
            cout << "Thank You";
            break;

        default:
            cout << "\nInvalid!";
        }
        if (choice != 4)
        {
            pause_f();
            loading_f();
            clear_f();
        }
    } while (choice != 4);
}