#ifndef state
#define state

#include <string>
using namespace std;

class State{
private:
    string  dbname = "bd";
    string user = "ms383504";
    string password = "x";
    string hostadrr = "10.1.0.5";
    string port = "5432";
    int lastCreatedOrder="-1";
public:
    bool shouldContinue(){
        return true;
    }

    Work giveWork(){

    }

    void setLastOrder(int id){
        lastCreatedOrder = id;
    }

};


#endif state