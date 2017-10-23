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
        std::exit(0);
    }
    return std::stoi(msg);
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
    vector<vector<int>> matrizFichas;
    int n;//columnas
    int m;//filas
} tablero

int calcularJugada(tablero& tab, int columnas, bool maximizar){
    int resultado = validarVictoria(tablero.matrizFichas);
    if(resultado == 1){
        return 1;
    }
    else if(resultado == -1){
        return -1;
    }
    else{
        if(tableroLleno(&tablero)){//lo paso por referencia
            return 0;
        }
        else{
            if(maximizar){
                vector<tablero *> opcionesTablero[tablero.n];
                vector<int> posiblesResultados[tablero.n];
                for(int i = 0; i < tablero.n; i++){
                    opcionesTablero[i] = new tablero; //creo la cantidad de tableros necesarios
                    opcionesTablero[i]->matrizFichas[i].push_back(fichaAliada);
                    int resultado = calcularJugada(*opcionesTablero[i], !maximizar);
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
                    opcionesTablero[i] = new tablero; //creo la cantidad de tableros necesarios
                    opcionesTablero[i]->matrizFichas[i].push_back(fichaEnemiga);
                    int resultado = calcularJugada(*opcionesTablero[i], maximizar);
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