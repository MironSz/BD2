#include <iostream>
#include "state.cpp"
#include <string>
using namespace std;
int main() {



    try {
        connection C("postgres://hnmgputi:fA9vDEgP8zRS5VSGn1GIlTWIYylJB0HS@dumbo.db.elephantsql.com:5432/hnmgputi");

        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }
        C.disconnect();
    }
        catch (const std::exception &e) {
            cerr << e.what() << std::endl;
            return 1;
        }


    State state1;
    while(state1.shouldContinue()){
        cout <<"Type command (type H for help)\n";
        string line;
        string type;
        getline(cin,line);
        cout <<"["+line+"]\n";
        state1.doAction(line);
    }
    return 0;
}