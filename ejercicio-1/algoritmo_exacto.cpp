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
        vector<int> filas;
        tab.matrizFichas.push_back(filas);
    }
    return tab;
}


void actualizarTablero(tablero& tab, int move, bool moveAliado);
int minRes(vector<int> resultados);
int maxRes(vector<int> resultados);
int posMax(vector<int> resultados);
int validarVictoria(tablero& tab, int p);
bool tableroLleno(tablero& tab);
int evaluarJugada(tablero& tab, int columnas, bool maximizar, int c, int p);
bool hayFicha(tablero& tab, int columna, int fila);

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


int calcularJugada(tablero& tab, int columnas, int c, int p){
    vector<int> posiblesResultados(tab.n);
    vector<tablero> opcionesTablero(tab.n);
    for(int i = 0; i < columnas; i++){
        opcionesTablero[i] = tab;
        actualizarTablero(opcionesTablero[i], i, true);
        if(opcionesTablero[i].matrizFichas[i].size() > tab.m){
            posiblesResultados[i] = -10;
            // le asginamos un -10 ya que no quiero tener en cuenta ese movimiento.
        }
        else{
            posiblesResultados[i] = evaluarJugada(opcionesTablero[i], columnas, false, c, p - 1);
            // esta funcion se encargaria de maximizar, por lo tanto llamo a evaluar jugada con MINIMIZAR.
            if(posiblesResultados[i] == 1){
                return i;
            }
        }

    }
    return posMax(posiblesResultados);
}

int evaluarJugada(tablero& tab, int columnas, bool maximizar, int c, int p){
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
                if(p == 0) return 0;
                vector<tablero> opcionesTablero(tab.n);
                vector<int> posiblesResultados(tab.n);
                for(int i = 0; i < tab.n; i++){
                    if(tab.matrizFichas[i].size() < tab.m){
                        opcionesTablero[i] = tab; //creo la cantidad de tableros necesarios
                        opcionesTablero[i].matrizFichas[i].push_back((fichaAliada));

                        int resultado = evaluarJugada(opcionesTablero[i], columnas, !maximizar, c, p - 1);
                        posiblesResultados[i] = resultado;
                        if(resultado == 1){ //parte alfabeta: si la rama que acabo de calcular ya me da 1 lo devuelvo asi no calculo los siguientes
                            return 1;
                        }
                    }
                }
                return maxRes(posiblesResultados);//devolver el maximo de cada posibilidad
            }
            else{//minimizar
                vector<tablero> opcionesTablero(tab.n);
                vector<int> posiblesResultados(tab.n);
                for(int i = 0; i < tab.n; i++){
                    if(tab.matrizFichas[i].size() < tab.m){
                        opcionesTablero[i] = tab; //creo la cantidad de tableros necesarios
                        opcionesTablero[i].matrizFichas[i].push_back((fichaEnemiga));
                        int resultado = evaluarJugada(opcionesTablero[i], columnas, !maximizar, c, p);
                        posiblesResultados[i] = resultado;
                        if(resultado == -1){ //parte alfabeta
                            return -1;
                        }
                    }
                }
                return minRes(posiblesResultados);//devolver el minimo de cada posibilidad
            }
        }
    }
}

bool tableroLleno(tablero& tab){
    for (int i = 0; i < tab.n; ++i)
    {
        if(tab.matrizFichas[i].size() < tab.m){
            return false;
        }
    }
    return true;
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
            move = calcularJugada(tab, columns, c, p);
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
   
            move = calcularJugada(tab, columns, c, p);
   
            //actualizar tablero con el movimiento, move mio.
            actualizarTablero(tab, move, true);
            send(move);
        }
    }

    return 0;
}

int validarVictoria(tablero& tab, int c){
    for (int i = 0; i < tab.n; ++i)             //i recorre columnas
    {
        for (int j = 0; j < tab.m; ++j)         //j recorre filas
        {
            if(i <= tab.n - c){//chequea si hay ganador en la fila de p fichas  empezando de (i,k)
                bool hayGanador = true;
                for (int k = 0; k < c; ++k)
                {
					if (hayFicha(tab, i + k, j)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i + k][j])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
                }
            }
            if(j <= tab.m - c){//chequea si hay ganador en la columna de p fichas  empezando de (i,j)
                bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, i, j + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}

                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
                }
            }
            if((i <= tab.n - c)&&(j <= tab.m - c)){//chequea si hay ganador en la diagonal hacia la derecha de p fichas empezando de (i,k)
                bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, i + k, j + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i + k][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
                }
            }
            if((i >= c - 1)&&(j <= tab.m - c)){//chequea si hay ganador en la diagonal hacia la izquierda de p fichas empezando de (i,k)
                bool hayGanador = true;
                for (int k = 0; k < c; ++k)
                {
					if (hayFicha(tab, i - k, j + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i - k][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
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
        if(resultados[i] > maxRes){
            maxRes = resultados[i];
        }
    }
    return maxRes;
}


int posMax(vector<int> resultados){
    int maxPos = 0;
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] > maxPos){
            maxPos = i;
        }
    }
    return maxPos;
}

void actualizarTablero(tablero& tab, int move, bool moveAliado){
    if(moveAliado){
        tab.matrizFichas[move].push_back(fichaAliada);
    }
    else{
        tab.matrizFichas[move].push_back(fichaEnemiga);
    }
}


bool hayFicha(tablero& tab, int columna, int fila) {
	return fila < tab.matrizFichas[columna].size();
}