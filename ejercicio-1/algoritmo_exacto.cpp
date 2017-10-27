#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <algorithm>


#define gane 1;
#define perdi -1;
#define empate 0;
#define fichaAliada 10
#define fichaEnemiga 20

using namespace std;

struct tablero{
    vector<vector<int>> matrizFichas;
    int n;//columnas
    int m;//filas
} tablero1;

tablero crearTablero(int n, int m){
    tablero tab;
    tab.m = m;
    tab.n = n;
    for(int i = 0; i < n; i++){
        vector<int> filas(m);
        tab.matrizFichas[i] = filas;
    }
    return tab;
}


void actualizarTablero(tablero& tab, int move, bool moveAliado);
int minRes(vector<int> resultados);
int maxRes(vector<int> resultados);
int validarVictoria(tablero& tab, int p);
bool tableroLleno(tablero& tab);

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


int calcularJugada(tablero& tab, int columnas, bool maximizar, int c, int p){
    int resultado = validarVictoria(tab, c);
    if(resultado == 1){
        return 1;
    }
    else if(resultado == -1){
        return -1;
    }
    else{
        if(tableroLleno(tab)){
            return 0;
        }
        else{
            if(maximizar){
                vector<tablero> opcionesTablero(tab.n);
                vector<int> posiblesResultados(tab.n);
                for(int i = 0; i < tab.n; i++){
                    opcionesTablero[i] = tab; //creo la cantidad de tableros necesarios
                    opcionesTablero[i].matrizFichas[i].push_back((fichaAliada));
                    int resultado = calcularJugada(opcionesTablero[i], columnas, !maximizar, c, p);
                    posiblesResultados[i] = resultado;
                    if(resultado == 1){ //parte alfabeta: si la rama que acabo de calcular ya me da 1 lo devuelvo asi no calculo los siguientes
                        return 1;
                    }

                }
                return maxRes(posiblesResultados);//devolver el maximo de cada posibilidad
            }
            else{//minimizar
                vector<tablero> opcionesTablero(tab.n);
                vector<int> posiblesResultados(tab.n);
                for(int i = 0; i < tab.n; i++){
                    opcionesTablero[i] = tab; //creo la cantidad de tableros necesarios
                    opcionesTablero[i].matrizFichas[i].push_back((fichaEnemiga));
                    int resultado = calcularJugada(opcionesTablero[i], columnas, maximizar, c, p);
                    posiblesResultados[i] = resultado;
                    if(resultado == -1){ //parte alfabeta
                        return -1;
                    }
                }
                return minRes(posiblesResultados);//devolver el minimo de cada posibilidad
            }
        }

    }
}

bool tableroLleno(tablero& tab){
    bool lleno = true;
    for (int i = 0; i < tab.n; ++i)
    {
        if(tab.matrizFichas[i].size() == tab.m){
            return false;
        }
    }
    return lleno;
}


int main() {
    string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;

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
            move = calcularJugada(tab, columns, true, c, p);
            actualizarTablero(tab, move, true);
            send(move);
        }

        while (true){
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }

            //actualizar tablero con el movimiento del enemigo
            actualizarTablero(tab, std::stoi(msg), false);

            do {
                move = calcularJugada(tab, columns, false, c, p);
            } while(tableroLleno(tab));
            //porque esta instruccion???

            //actualizar tablero con el movimiento, move mio.
            actualizarTablero(tab, move, true);
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
                    if(tab.matrizFichas[i][j] == fichaAliada) {return gane;} else {return perdi;}
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
                    if(tab.matrizFichas[i][j] == fichaAliada) {return gane;} else {return perdi;}
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
                    if(tab.matrizFichas[i][j] == fichaAliada) {return gane;} else {return perdi;}
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
                    if(tab.matrizFichas[i][j] == fichaAliada) {return gane;} else {return perdi;}
                }
            }
        }
    }
    return 0;

}

int minRes(vector<int> resultados){
    int minRes = resultados[0];
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] < minRes){
            minRes = resultados[i];
        }
    }
    return minRes;
}

int maxRes(vector<int> resultados){
    int maxRes = resultados[0];
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] < maxRes){
            maxRes = resultados[i];
        }
    }
    return maxRes;
}

void actualizarTablero(tablero& tab, int move, bool moveAliado){
    if(moveAliado){
        tab.matrizFichas[move].push_back(fichaAliada);
    }
    else{
        tab.matrizFichas[move].push_back(fichaEnemiga);
    }
}
