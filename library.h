#pragma once
#include "utils.h"
#include "issue.h"

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
        choice = readInt("\n\nEnter your choice: ", 1, 4);

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
            pauseScreen();
            loading();
            clearScreen();
        }

    } while (choice != 4);
}
