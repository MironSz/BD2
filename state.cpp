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

//    connection C("dbname = " + dbname + " user = " + user + " password = " + password + " hostaddr = " + hostadrr +" port = " + port);
//    pqxx::connection C(port);
//    work W(C);
public:
    State() : C("postgres://hnmgputi:fA9vDEgP8zRS5VSGn1GIlTWIYylJB0HS@dumbo.db.elephantsql.com:5432/hnmgputi") {};

    bool shouldContinue() {
        return true;
    }

    void doRealAction(string type, string args) {
        if (type == "NO") {//NEW ORDER
            int id_klienta;
            sscanf(args.c_str(), "%d", &id_klienta);
            {
                work W(C);
                W.exec(string("INSERT INTO ZAMOWIENIE (id_klienta,udane) VALUES (")
                       + to_string(id_klienta) + string(",false);"));
                W.commit();
            }
            {
                nontransaction N(C);
//            result r = W.exec("SELECT id_klienta from KLIENT WHERE udane = false;");
                result r(N.exec("SELECT id_klienta from KLIENT WHERE udane = false;"));
                lastCreatedOrder = (r.front())[0].as<int>();
            }
        } else if (type == "AO") { //ADD TO ORDER
            work W(C);
            int id_zamowienia, ilosc, id_pizzy;
            sscanf(&args[0], "%d %d %d", &id_zamowienia, &id_pizzy, &ilosc);
            W.exec(string("INSERT INTO PolaczeniaPizzaZamowienie (id_zamowienia,id_pizzy,ilosc)"
                                  "VALUES (") + to_string(id_zamowienia) + string(",") + to_string(id_pizzy)
                   + string(",") + to_string(ilosc) + string(");"));
            result r = W.exec(string("SELECT sprawdzSkladniki(") + to_string(id_zamowienia) + string(");"));
            //TODO sprawdz to
            if (r.front()[0].as<bool>() == false) {
                W.exec("DELETE from PolaczeniePizzaZamowienie WHERE id_zamowienia = " + to_string(id_zamowienia)
                       + " AND id_pizzy = " + to_string(id_pizzy) + ";");
            } else{
                W.abort();
            }
        } else if (type == "FO") {//FINISH ORDER
            int id_zamowienia;
            work W(C);
            sscanf(&args[0], "%d", &id_zamowienia);
            W.exec("SELECT uczynZamowienie(" + to_string(id_zamowienia) + ")");
            W.commit();
        } else if (type == "D") {//DELIVER
            work W(C);
            W.exec("UPDATE Skladniki SET ilosc = ilosc+10");
        } else if (type == "M") {//MENU
            work W(C);
            result r = W.exec("SELECT nazwa_pizzy,id_pizzy FROM Pizze;");
            for (auto raw: r) {
                cout << raw[1] << " " << raw[0] << "\n";
            }

        } else if (type == "KI") { //gives klient id, may add him


        } else if (type == "END") {

        } else if (type == "H") {

        }
    }

    void doAction(string line) {
        char type[30], args[30];

        sscanf(line.c_str(), "%s %s", type, args);
        printf("((%s,%s))\n", type, args);
        cout << "type: (" << type << ") args: (" << args << ")\n";
        doRealAction(string(type), string(args));
    }

};

#endif// state