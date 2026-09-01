#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <limits>
#include <string>

using namespace std;

// ---------- Input helpers ----------

inline int readInt(const char *msg = "",
                    int minVal = numeric_limits<int>::min(),
                    int maxVal = numeric_limits<int>::max())
{
    int x;
    char ch;
    while (true)
    {
        try
        {
            cout << msg;
            if (!(cin >> x)) throw "Input is not a number.";
            if (cin.get(ch) && ch != '\n') throw "Extra characters found.";
            if (x < minVal || x > maxVal) throw "Value out of range.";
            return x;
        }
        catch (const char *e)
        {
            cout << "Error: " << e << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Bounded string read: caps input at (bufSize - 1) chars, always null-terminates.
inline void readStr(const char *msg, char *buf, int bufSize)
{
    cout << msg;
    cin >> setw(bufSize) >> buf;
}

// ---------- UI helpers ----------

inline void delayMs(int ms)
{
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms);
}

inline void loading()
{
    char spin[] = {'|', '/', '-', '\\'};
    cout << "\nLoading ";
    for (int i = 0; i < 20; i++)
    {
        cout << spin[i % 4];
        cout.flush();
        delayMs(100);
        cout << '\b';
    }
}

inline void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void pauseScreen()
{
#ifdef _WIN32
    system("pause");
#else
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
#endif
}

// ---------- Generic binary-file rewrite ----------
// Reads every record of type T from `filename`, keeps only those for which
// keep(record) returns true, writes them to a temp file, then atomically
// swaps it in. Used for delete/update-style rewrites.

template <typename T, typename Predicate>
inline void rewriteFile(const char *filename, Predicate keep)
{
    T record;
    string tmpName = string(filename) + ".tmp";

    ifstream fin(filename, ios::binary);
    ofstream fout(tmpName, ios::binary);

    while (fin.read((char *)&record, sizeof(record)))
    {
        if (keep(record))
        {
            fout.write((char *)&record, sizeof(record));
        }
    }

    fin.close();
    fout.close();

    remove(filename);
    rename(tmpName.c_str(), filename);
}

// Same idea, but transforms/mutates each kept record via `transform`
// instead of just filtering. Used for update-style rewrites where the
// matching record's fields change but every record is kept.
template <typename T, typename Transform>
inline void transformFile(const char *filename, Transform transform)
{
    T record;
    string tmpName = string(filename) + ".tmp";

    ifstream fin(filename, ios::binary);
    ofstream fout(tmpName, ios::binary);

    while (fin.read((char *)&record, sizeof(record)))
    {
        transform(record);
        fout.write((char *)&record, sizeof(record));
    }

    fin.close();
    fout.close();

    remove(filename);
    rename(tmpName.c_str(), filename);
}

// Appends a single record to a binary file.
template <typename T>
inline void appendRecord(const char *filename, const T &record)
{
    ofstream fout(filename, ios::binary | ios::app);
    fout.write((char *)&record, sizeof(record));
    fout.close();
}
