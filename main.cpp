/*
 *  This application will need the GNU library to encrypt the password
 *  however you can comment out the encryption code on passserver.cpp
 *  to mak eit work without encryption.
 */

#include "hashtable.h"
#include "passserver.h"
#include <string>
#include <utility>
#include <termios.h>

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

    //-----------Using termios.h to hide input------------

    //TODO: If there is time put this in a function for cleaner code

    //Got this information from the GNU libray(they provide an alternative
    //way to do something similar to the outdated getpass()) and termios.h
    //documentation.

    //This function sets the parameters associated with the terminal
    //tcsetattr(int, int, struct termios *);

    //Needs a structure as a paremeter
    //off will be for echo off and on for echo on
    struct termios t_off, t_on;

    //This function will put the associated parameters for termianl
    tcgetattr (fileno (stdin), &t_on);

    //sets t_off to be same as t_on, the only thing different is
    //that echo will be turned off
    t_off = t_on;
    t_off.c_lflag &= ~ECHO;

    //To turn off echo we will have to put this before getting password
    //TCSANOW will make sure changes occur immediately
    tcsetattr(fileno (stdin), TCSANOW, &t_off);

    //To turn back on we put this after
    tcsetattr(fileno (stdin), TCSANOW, &t_on);

    //----------------------------------------------------

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

            tcsetattr(fileno (stdin), TCSANOW, &t_off);
            cin >> pass;
            tcsetattr(fileno (stdin), TCSANOW, &t_on);

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

            tcsetattr(fileno (stdin), TCSANOW, &t_off);
            cin >> pass;

            cout << endl;

            cout << "Enter new password: ";

            cin >> newPass;
            tcsetattr(fileno (stdin), TCSANOW, &t_on);

            cout << endl;

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