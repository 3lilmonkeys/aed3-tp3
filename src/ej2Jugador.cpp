//
// Created by reivaj on 08/11/17.
//

#include "ej2.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <algorithm>



void send(const std::string& msg) {
    std::cout << msg << std::endl;
}

void send(int msg) {
    std::cout << msg << std::endl;
}

int read_int() {
    std::string msg;
    std::cin >> msg;
    if (msg == "salir") {
        send("listo");
        exit(0);
    }
    return stoi(msg);
}

string read_str() {
    string msg;
    cin >> msg;
    if (msg == "salir") {
        send("listo");
        exit(0);
    }
    return msg;
}



int main(){
    int tiempo = (time(NULL));
    srand(tiempo);

    string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;



    //deberia ir dentro del while para que cambien las estrategias??
    // list<estr> estrategias = inicializarEstrategias(estrategiasUnicas, estrTotales, columnas);
    // for(auto it = estrategias.begin(); it != estrategias.end(); it++){
    //        int estr = it->peso;
    //        cout << "Peso Estr " << it->estrategia << ":" << it->peso << "\n";
    //    }

    while (true){
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();

        int estrategiasUnicas = 11;
        unsigned int estrTotales = (unsigned int) estrategiasUnicas+ (c-2)*2;
        int filas = 6;

        tablero tab = crearTablero(columns, rows);
        list<estr> estrategias = inicializarEstrategias(estrategiasUnicas, estrTotales, columns);

        go_first = read_str();
        if (go_first == "vos") {
            move = jugadaGolosa(tab, estrategias, estrategiasUnicas, (c-2)*2, estrTotales, columns, c);
            actualizarTablero(tab, move, true);
            send(move);
        }

        while (true){
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }

            //actualizar tablero con el movimiento del enemigo
            actualizarTablero(tab, stoi(msg), false);

            move = jugadaGolosa(tab, estrategias, estrategiasUnicas, (c-2)*2, estrTotales, columns, c);

            //actualizar tablero con el movimiento, move mio.
            actualizarTablero(tab, move, true);
            send(move);
        }
    }

    return 0;
}
