#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>


#define gane 1;
#define perdi -1;
#define empate 0;
#define fichaRoja 10
#define fichaAzul 20

using namespace std;

void send(const std::string& msg) {
    cout << msg << std::endl;
}

void send(int msg) {
    cout << msg << std::endl;
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

struct type_name{
    vector<vector<int>>* matrizFichas;
    int n;//columnas
    int m;//filas
} tablero

int calcularJugada(tablero& tab, int columnas, bool maximizar, int c, int p){
    int resultado = validarVictoria(tablero, c);
    if(resultado == 1){
        return 1;
    }
    else if(resultado == -1){
        return -1;
    }
    else{
        if(tableroLleno(tablero)){
            return 0;
        }
        else{
            if(maximizar){
                vector<tablero *> opcionesTablero[tablero.n];
                vector<int> posiblesResultados[tablero.n];
                for(int i = 0; i < tablero.n; i++){
                    opcionesTablero[i] = tablero; //creo la cantidad de tableros necesarios
                    opcionesTablero[i]->matrizFichas[i].push_back(fichaAliada);
                    int resultado = calcularJugada(*opcionesTablero[i], columnas, !maximizar, c, p);
                    posiblesResultados[i].push_back(resultado);
                    if(resultado == 1){ //parte alfabeta: si la rama que acabo de calcular ya me da 1 lo devuelvo asi no calculo los siguientes
                        return 1;
                    }

                }
                return max(posiblesResultados);//devolver el maximo de cada posibilidad
            }
            else{//minimizar
                vector<tablero *> opcionesTablero[tablero.n];
                vector<int> posiblesResultados[tablero.n];
                for(int i = 0; i < tablero.n; i++){
                    opcionesTablero[i] = tablero; //creo la cantidad de tableros necesarios
                    opcionesTablero[i]->matrizFichas[i].push_back(fichaEnemiga);
                    int resultado = calcularJugada(*opcionesTablero[i], columnas, maximizar, c, p);
                    posiblesResultados[i].push_back(resultado);
                    if(resultado == -1){ //parte alfabeta
                        return -1;
                    }
                }
                return min(posiblesResultados);//devolver el minimo de cada posibilidad
            }
        }

    }
}

bool tableroLleno(tablero& tab){
    bool lleno = true;
    for (int i = 0; i < tab->n; ++i)
    {
        if(tab->matrizFichas[i].length == tab->m){
            return false;
        }
    }
    return lleno;
}



int main() {
    string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;

    while (true) {
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();


        vector<int> board(columns);

        vector<int> filas(rows, 0);

        tablero tab();

        for(int i=0; i<columns; ++i) board[i] = 0;

        go_first = read_str();
        if (go_first == "vos") {
            move = calcularJugada(tab, columns, 1, c, p);
            board[move]++;
            send(move);
        }

        while (true) {
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }

            board[std::stoi(msg)]++;
            do {
                move = calcularJugada(tab, columns, 0);
            } while(board[move] >= rows);
            
            board[move]++;
            send(move);
        }
    }

    return 0;
}


int validarVictoria(tablero& tab, int p){
    for (int i = 0; i < tab.n; ++i)             //i recorre columnas
    {
        for (int j = 0; j < tab.m; ++j)         //j recorre filas
        {
            if(i < tab.n - p){//chequea si hay ganador en la fila de p fichas  empezando de (i,k)
                bool hayGanador = true;
                for (int k = 1; k < p; ++k)
                {
                    if (tab.matrizFichas[i][j] != tab.matrizFichas[i+k][j])
                    {
                        hayGanador = false;
                        break;
                    }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1} else {return -1}
                }
            }
            if(j < tab.m - p){//chequea si hay ganador en la columna de p fichas  empezando de (i,k)
                bool hayGanador = true;
                for (int k = 1; k < p; ++k)
                {
                    if (tab.matrizFichas[i][j] != tab.matrizFichas[i][j+k])
                    {
                        hayGanador = false;
                        break;
                    }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1} else {return -1}
                }
            }
            if((i < tab.n - p)&&(j < tab.m - p)){//chequea si hay ganador en la diagonal hacia la derecha de p fichas empezando de (i,k)
                bool hayGanador = true;
                for (int k = 1; k < p; ++k)
                {
                    if (tab.matrizFichas[i][j] != tab.matrizFichas[i+k][j+k])
                    {
                        hayGanador = false;
                        break;
                    }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1} else {return -1}
                }
            }
            if((i > p)&&(j < tab.m - p)){//chequea si hay ganador en la diagonal hacia la izquierda de p fichas empezando de (i,k)
                bool hayGanador = true;
                for (int k = 1; k < p; ++k)
                {
                    if (tab.matrizFichas[i][j] != tab.matrizFichas[i-k][j+k])
                    {
                        hayGanador = false;
                        break;
                    }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1} else {return -1}
                }
            }
        }
    }
    return 0;

}