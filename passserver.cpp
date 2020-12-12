//
// Created by Joao on 11/14/20.
//

#include "passserver.h"
#include "hashtable.h"

using namespace Joao;

PassServer::PassServer(size_t size) : hashPass(size) {

}

PassServer::~PassServer() {

    hashPass.clear();

}

bool PassServer::load(const char *filename) {

    //creates file oject
    ofstream file1;

    //creates temp text file to store encrypted pairs
    file1.open("temp.txt");

    //Creates ifstream object
    ifstream f1;

    //Opens and checks if file opens
    f1.open(filename);

    if (!f1.is_open()) {
        cout << "File " << filename << " missing!\n\n";
        return false;
    }

    //Until the end of object f, will keep getting username and passwords and adding to file
    while(!f1.eof()){

        string username, password;

        f1 >> username >> password;

        file1 << username << " " << encrypt(password) << endl;

    }

    f1.close();

    return hashPass.load("temp.txt");

}

bool PassServer::addUser(std::pair<string, string> &kv) {

    //Has to encrypt password first
    kv.second = encrypt(kv.second);

    return hashPass.insert(kv);

}

bool PassServer::addUser(std::pair<string, string> &&kv) {

    //Has to encrypt password first
    kv.second = encrypt(kv.second);

    return hashPass.insert(kv);

}

bool PassServer::removeUser(const string &k) {

    return hashPass.remove(k);

}

bool PassServer::changePassword(const std::pair<string, string> &p, const string &newpassword) {

    // change an existing user's password. Note that both passwords passed in are in plaintext. They should be
    // encrypted before you interact with the hash table. If the user is not in the hash table, return false.
    // If p.second does not match the current password, return false. Also return false if the new password and
    // the old password are the same (i.e., we cannot update the password).

    if(!hashPass.contains(p.first)) {

        std::cout << "User " << p.first << " not found" << std::endl;

        return false;

    }

    //Will encrypt and create a new pair with the encrypted password
    string hashedPass = encrypt(p.second);

    pair<string, string> p1(p.first, hashedPass);

    //Has to check if pair value is in hashtable first
    if(!hashPass.match(p1)) {

        std::cout << "*****Error: Could not change user password" << std::endl;

        return false;

    }

    //will encrypt the new password and pass it in to be updated
    p1 = make_pair(p1.first, encrypt(newpassword));

    hashPass.insert(p1);

    return true;

}

bool PassServer::find(const string &user) const{

    return hashPass.contains(user);

}

void PassServer::dump(){

    hashPass.dump();

}

size_t PassServer::size() const{

    return hashPass.size();

}

bool PassServer::write_to_file(const char *filename) {

    return hashPass.write_to_file(filename);

}

string PassServer::encrypt(const string &str) {

    //Need to install GNU C library to use crypt password
    //If you can't install the library just comment out the following code and uncomment return str

    char salt[] = "$1$########";

    char *key = new char[100];

    strcpy(key, str.c_str());

    string encpassword = crypt(key, salt);

    //Only need the last 22 characters, have to subtract the salt

    encpassword.erase(0, 12);

    std::cout << encpassword << endl;

    return encpassword;

    //return str;

}


