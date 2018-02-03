#ifndef state
#define state

#include <stdlib.h>
#include <string>
#include <cstring>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class State {
private:
    string dbname = "bd";
    string user = "ms383504";
    string password = "x";
    string hostadrr = "10.1.0.5";
    string port = "5432";
    int lastCreatedOrder = -1;
    connection C;
    bool shouldContinueVar = true;

//    connection C("dbname = " + dbname + " user = " + user + " password = " + password + " hostaddr = " + hostadrr +" port = " + port);
//    pqxx::connection C(port);
    shared_ptr <work> currentOrder;
public:
    State() : C("postgres://hnmgputi:fA9vDEgP8zRS5VSGn1GIlTWIYylJB0HS@dumbo.db.elephantsql.com:5432/hnmgputi") {};

    bool shouldContinue() {
        return shouldContinueVar;
    }

    void doRealAction(string type, string args) {
        if (type == "NO") {//NEW ORDer
            int id_klienta;
            {
                nontransaction N(C);
                result r(N.exec("SELECT czyscZamowienia();"));
            }
            sscanf(args.c_str(), "%d", &id_klienta);
            {
                work W(C);
                W.exec(string("INSERT INTO ZAMOWIENIE (id_klienta,udane) VALUES (")
                       + to_string(id_klienta) + string(",false);"));
                W.commit();
            }
            {
                nontransaction N(C);
                result r(N.exec("SELECT Zamowienie.id_zamowienia from KLIENT JOIN Zamowienie On "
                                        "Klient.id_klienta = Zamowienie.id_klienta WHERE udane = false;"));
                lastCreatedOrder = (r.front())[0].as<int>();
                cout << "Order initiated succesfuly, your order's id is: " << lastCreatedOrder << endl;
            }
        } else if (type == "AO") { //ADD TO ORDER
            work W(C);
            int id_zamowienia = lastCreatedOrder, ilosc, id_pizzy;
            sscanf(&args[0], "%d %d", &id_pizzy, &ilosc);
            printf("   (id_pizzy = %d, ilosc = %d)\n", id_pizzy, ilosc);
            W.exec(string("INSERT INTO PolaczaniePizzaZamowienie (id_zamowienia,id_pizzy,ilosc)"
                                  "VALUES (") + to_string(id_zamowienia) + string(",") + to_string(id_pizzy)
                   + string(",") + to_string(ilosc) + string(");"));

            result r = W.exec(string("SELECT sprawdzSkladniki(") + to_string(id_zamowienia) + string(");"));
            if (r.front()[0].as<bool>() == false) {
                W.abort();
                cout << "Sorry, we do not have enough ingredients :(\n";
            } else {
                W.commit();
            }
        } else if (type == "FO") {//FINISH ORDER
            int id_zamowienia = lastCreatedOrder;
            work W(C);
            W.exec("SELECT uczynZamowienie(" + to_string(id_zamowienia) + ")");
            W.commit();
        } else if (type == "D") {//DELIVER
            work W(C);
            W.exec("UPDATE Skladnik SET ilosc = ilosc+10");
        } else if (type == "M") {//MENU
            work W(C);
            result r = W.exec("SELECT nazwa_pizzy,id_pizzy FROM Pizze;");
            for (auto raw: r) {
                cout << raw[1] << " " << raw[0] << "\n";
            }

        } else if (type == "KI") { //gives klient id, may add him
            char name[100],surname[100];
            sscanf(args.c_str(),"%99s %99s" ,&name,&surname);
            nontransaction N(C);
            result r(N.exec("SELECT (id_klienta) FROM Klient WHERE imie = '"+string(name)+"' AND nazwisko = '"+string(surname)+"';"));
            N.close();
            if(r.empty()){
                work W(C);
                W.exec("INSERT INTO Klient (imie,nazwisko) VALUES ('"+string(name)+"','"+string(surname)+"');");
            }

        } else if (type == "END") {
            shouldContinueVar = false;

        } else if (type == "H") {
            cout << "Those are available commends:\n" << "[H]-list all commends\n"
                 << "[NO c]-begin new order for client c, will abort unfinished orders\n"
                 << "[AO p n]-adds n pizzas of type p to current order\n"
                 << "[FO]-finishes current order\n"
                 << "[KI name surname]-gives id of client, adds him to database if he is not in it\n"
                 << "[END]-close aplication\n" << "[MENU]-displays menu\n\n\n";

        } else if(type == "MENU"){
            nontransaction N(C);
            result r(N.exec("SELECT * FROM Pizza;"));
            cout<<"id   name   prize(in PLN)\n";
            for(auto raw : r){
                cout<<raw[0].as<int>()<<" "<<raw[1].as<string>() <<"   "<<raw[2]<<endl;
            }
        } else{
            cout <<"unknown command :(\n";
        }
    }

    void doAction(string line) {
        char type[30], args[30];

        sscanf(line.c_str(), "%s %[^\n]s", type, args);
        doRealAction(string(type), string(args));
    }

};

#endif// state