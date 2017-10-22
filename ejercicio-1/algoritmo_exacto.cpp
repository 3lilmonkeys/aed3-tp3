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

int calcularJugada(vector<vector<int>> tablero, int columnas, bool maximizar){
    int resultado = validarVictoria(tablero);
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
                vector<int> posiblesResultados(columnas);
                for(int i = 0; i < columnas; i++){
                    tablero[i].push_back(fichaRoja);
                    posiblesResultados[i] = calcularJugada(tablero, columnas, !maximizar);
                }
            }
        }

    }
}
