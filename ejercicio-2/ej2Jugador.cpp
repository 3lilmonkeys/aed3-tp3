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

    string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;

    int C = 6;
    int estrategiasUnicas = 11;
    unsigned int estrTotales = (unsigned int) estrategiasUnicas+ (C-2)*2;
    int filas = 6;
    unsigned int columnas = (unsigned int) 6;
    int fichas = 18;

    list<estr> estrategias = inicializarEstrategias(estrategiasUnicas, estrTotales, columnas);

    while (true){
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();

        tablero tab = crearTablero(columns, rows);

        go_first = read_str();
        if (go_first == "vos") {
            move = jugadaGolosa(tab, estrategias, estrategiasUnicas, (C-2)*2, estrTotales, columnas, C);
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

            move = jugadaGolosa(tab, estrategias, estrategiasUnicas, (C-2)*2, estrTotales, columnas, C);

            //actualizar tablero con el movimiento, move mio.
            actualizarTablero(tab, move, true);
            send(move);
        }
    }

    return 0;
}



