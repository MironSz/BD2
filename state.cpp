#ifndef state
#define state

#include <pqxx/pqxx>
#include <string>

using namespace std;
using namespace pqxx;

class State {
private:
    string dbname = "bd";
    string user = "ms383504";
    string password = "x";
    string hostadrr = "10.1.0.5";
    string port = "5432";
    int lastCreatedOrder = "-1";

    connection C("dbname = " + dbname + " user = " + user + " password = " + password + " hostaddr = " + hostadrr +" port = " + port);
    work W(C);
public:
    bool shouldContinue() {
        return true;
    }

    Work giveWork() {

    }

    void setLastOrder(int id) {
        lastCreatedOrder = id;
    }

    void doAction(string &line) {
        string type, args;

        sscanf(line, "%s%[^\n]s", type, args);
        cout << "type: " << type << " args: (" << args << ")\n";
        Action action(*this, type, args,W);

    }

};


#endif state