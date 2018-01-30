#ifndef action
#define action

#include "action.cpp";

class Action {
private:
    State *state;
    string type;
    vector <string> args;
public:
    Action(State &state, string type, vector <string> args, Work &work) : state(state), type(type), args(args) {
        if (type == "NO") {//NEW ORDER
            int id_klienta = atoi(args[0]);

            W.exec("INSERT INTO ZAMOWIENIE (id_klienta,udane) VALUES ("
                   + id_klienta + ",false);");
            result r = W.exec("SELECT id_klienta from KLIENT WHERE udane = false;");
            return (*r.begin())[0]->as<int>;

        } else if (type == "AO") { //ADD TO ORDER
            int id_zamowienia, ilosc, id_pizzy;
            sscanf(args[0], "%d %d %d", &id_zamowienia, &id_pizzy, &ilosc);
            W.exec("INSERT INTO PolaczeniaPizzaZamowienie (id_zamowienia,id_pizzy,ilosc)"
                           "VALUES (" + id_zamowienia + "," + id_pizzy + "," + ilosc + ");");
            result r = W.exec("SELECT sprawdzSkladniki(" + orderId + ");");
            //TODO sprawdz to
            if (*r.begin()[0] == false) {
                W.exec("DELETE from PolaczeniePizzaZamowienie WHERE id_zamowienia = " + id_zamowienia
                       + " AND id_pizzy = " + id_pizzy + ";");
            }
        } else if (type == "FO") {//FINISH ORDER
            int id_zamowienia;
            sscanf(args[0],"%d" ,&id_zamowienia);
            W.exec("SELECT uczynZamowienie("+id_zamowienia+")");
            W.commit();
        } else if (type == "D") {//DELIVER
            W.exec("UPDATE Skladniki SET ilosc = ilosc+10");
        } else if (type == "M") {//MENU
            result r = W.exec("SELECT nazwa_pizzy,id_pizzy FROM Pizze;");
            for(auto raw: r){
                cout <<raw[1]<<" "<<raw[0]<<"\n";
            }

        } else if (type == "KI") { //gives klient id, may add him
            

        } else if (type == "END") {

        }
    };
};

