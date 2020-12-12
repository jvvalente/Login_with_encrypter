/*
 *  This application will need the GNU library to encrypt the password
 *  however you can comment out the encryption code on passserver.cpp
 *  to mak eit work without encryption.
 */

#include "hashtable.h"
#include "passserver.h"
#include <string>
#include <utility>

using namespace std;
using namespace Joao;

void Menu()
{

    cout << "\n\n";
    cout << "l - Load From File" << endl;
    cout << "a - Add User" << endl;
    cout << "r - Remove User" << endl;
    cout << "c - Change User Password" << endl;
    cout << "f - Find User" << endl;
    cout << "d - Dump HashTable" << endl;
    cout << "s - HashTable Size" << endl;
    cout << "w - Write to Password File" << endl;
    cout << "x - Exit program" << endl;
    cout << "\nEnter choice : ";

}

int main() {

    int size;

    char choice;

    string user, pass;

    pair<string, string> p1;

    cout << "Enter preferred hash table capacity: ";

    cin >> size;

    PassServer hf(size);

    choice = 't';

    while(choice != 'x') {

        Menu();

        cin >> choice;

        choice = tolower(choice);

        if (choice == 'l') {

            char *filen = new char[100];

            cout << "Enter password file name to load from: ";

            cin >> filen;

            hf.load(filen);

        }
        else if (choice == 'a') {

            cout << "Enter username: ";

            //getline(cin, user, '\n');

            cin >> user;

            cout << "Enter password: ";

            cin >> pass;

            p1 = make_pair(user, pass);

            if(hf.addUser(p1))
                cout << endl <<  "User " << p1.first << " added." << endl;

        }
        else if (choice == 'r') {

            cout << "Enter username: ";

            cin >> user;

            if(hf.removeUser(user))
                cout << "User " << user << " deleted." << endl;
            else
                cout << "User " << user << " not deleted." << endl;

        }
        else if (choice == 'c') {

            string newPass;

            cout << "Enter username: ";

            cin >> user;

            cout << "Enter password: ";

            cin >> pass;

            cout << "Enter new password: ";

            cin >> newPass;

            p1 = make_pair(user, pass);

            if(hf.changePassword(p1, newPass))
                cout << "Password changed for user " << user << endl;

        }
        else if (choice == 'f') {

            string compare;

            cout << "Enter username : ";

            cin >> compare;

            cout << endl;

            if(hf.find(compare))
                cout << "User '" << compare << "' found" << endl;
            else
                cout << "User '" << compare << "' not found" << endl;

        }
        else if (choice == 'd') {

            hf.dump();

        }
        else if (choice == 's') {

            cout << "Size of hashtable: " << hf.size() << endl;

        }
        else if (choice == 'w') {

            char *file = new char[100];

            cout << "Enter password file name to write to: ";

            cin >> file;

            hf.write_to_file(file);

        }
        else if (choice == 'x') {
            break;
        }

    }

}