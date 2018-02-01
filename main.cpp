#include <iostream>
#include "state.cpp"
#include "action.cpp"
#include <string>
using namespace std;
int main() {
    State state1;


    while(state1.shouldContinue()){
        string line;
        string type;
        getline(cin,line);
        state1.doAction(line);
    }
    return 0;
}