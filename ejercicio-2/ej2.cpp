//
// Created by reivaj on 01/11/17.
//
#include <utility>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <sys/stat.h>
#include <algorithm>

using namespace std;

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
};



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
void inicializarEstrategias(vector<int> misEstrategias, int cantEstrategias);
bool estrategiaEsValida(vector<bool> jugadas);
vector<bool> calcularMoves(int estrategia);

//Como este tipo de estrategia va a variar segun el valor de C en linea, de 2 a C-1, voy a asumir que
// el vector con las estrategias va a tener las ultimas C-3 posiciones con estas estrategias.
// C-3 porque discrimino, C, C-1, y 1.
// Siendo la longitud del vector, ESTRATEGIAS + C-3 (de las estrategias dichas anteriormente).

struct estr{
    int estrategia;
    int peso;

    bool operator<(const estr& s){
        return peso < s.peso;
    }

    bool operator>(const estr& s){
        return peso > s.peso;
    }

    bool operator==(const estr& s){
        return estrategia == s.estrategia;
    }

    bool operator!=(const estr& s){
        return estrategia != s.estrategia;
    }
};

#define mejorHorizontalOf 1;

#define mejorVerticalOf 2;

#define mejorDiagonalOf 3;

#define ofOdefInmediata 4;

#define conectandoLineas 5;

#define desconectandoLineas 6;

#define mejorHorizontalDef 7;

#define mejorVertcialDef 8;

#define mejorDiagonalDef 9;

#define defInmediata 10;

#define jugarAlMedio 11;

#define formarLineaDe2fichas 12;

#define formarLineaDe3fichas 13;

#define formarLineaDe4fichas 14;

#define formarLineaDe5fichas 15;

#define formarLineaDe6fichas 16;

#define formarLineaDe7fichas 17;

#define bloquearLineaDe2fichas 18;

#define bloquearLineaDe3fichas 19;

#define bloquearLineaDe4fichas 20;

#define bloquearLineaDe5fichas 21;

#define bloquearLineaDe6fichas 22;

#define bloquearLineaDe7fichas 23;


int jugadaGolosa(list<estr> estrs, int estrUnicas, int formarLineas, int bloquearLineas, unsigned int cantEstrTotal, unsigned int columnas){

    for(auto it = estrs.begin(); it != estrs.end() ; it++) it->peso = it->peso * (-1);
    //ordeno las estrategias por su peso de forma descendente
    estrs.sort();
    // reestablezco su peso original
    for(auto it = estrs.begin(); it != estrs.end() ; it++) it->peso = it->peso * (-1);

    // se inicializan las posiblesJugadas y las mejoresJugadas en 0 para cada posicion.
    vector<vector<bool>> posiblesJugadas(cantEstrTotal);
    vector<int> mejoresJugadas(cantEstrTotal);

    bool yaTengoFormarLinea = false;
    bool yaTengoBloquearLinea = false;
    vector<bool> calcularJugadas(columnas);

    for(auto it = estrs.begin(); it != estrs.end(); it++){
        if(it->estrategia <= estrUnicas){
            calcularJugadas = calcularMoves(it->estrategia);
            if(estrategiaEsValida(calcularJugadas)){
                posiblesJugadas.push_back(calcularJugadas);
            }
        }
        // calculo por separado ya que son excluyentes, los formar linea y bloquear linea
        else{
            // formarLinea
            if(it->estrategia > estrUnicas && it->estrategia <= formarLineas+estrUnicas && !yaTengoFormarLinea){
                calcularJugadas = calcularMoves(it->estrategia);
                if(estrategiaEsValida(calcularJugadas)){
                    posiblesJugadas.push_back(calcularJugadas);
                    // si ya obtuve una estrategia de formarLineaDeXFichas, no quiero seguir mirando otras estrs
                    yaTengoFormarLinea = true;
                }
            }
            else{
                if(!yaTengoBloquearLinea){
                    //bloquearLinea
                    calcularJugadas = calcularMoves(it->estrategia);
                    if(estrategiaEsValida(calcularJugadas)){
                        posiblesJugadas.push_back(calcularJugadas);
                        // si ya obtuve una estrategia de bloquearLineaDeXFichas, no quiero seguir mirando otras estrs
                        yaTengoBloquearLinea = true;
                    }
                }
            }
        }
    }
}

// chequeo que haya al menos un movimiento que pueda realizar para satisfacer la estrategia
bool estrategiaEsValida(vector<bool> jugadas){
    bool existaJugada = false;
    for(int i = 0; i < jugadas.size(); i++){
        if(jugadas[i]){
            return true;
        }
    }
    return existaJugada;
}

vector<bool> calcularMoves(int estrategia){

}


void inicializarEstrategias(vector<estr> misEstrategias, int cantEstrategias){
    for(int i = 1; i <= cantEstrategias; i++){
        estr estr;
        estr.estrategia = i;
        estr.peso = 5;
        misEstrategias.push_back(estr);
    }
}

int main(){
    int C = 4;
    int estrategiasUnicas = 11;
    int cantEstrategias = estrategiasUnicas + C - 2;
    vector<int> misEstrategias;
    //inicializarEstrategias(misEstrategias, cantEstrategias);

}

//==========================COSAS DEL EJ1 QUE PUEDEN SERVIR=================================================//

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


int validarVictoria(tablero& tab, int p){
    for (int i = 0; i < tab.n; ++i)             //i recorre columnas
    {
        for (int j = 0; j < tab.m; ++j)         //j recorre filas
        {
            if(i <= tab.n - p){//chequea si hay ganador en la fila de p fichas  empezando de (i,k)
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
            if(j <= tab.m - p){//chequea si hay ganador en la columna de p fichas  empezando de (i,k)
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
            if((i <= tab.n - p)&&(j <= tab.m - p)){//chequea si hay ganador en la diagonal hacia la derecha de p fichas empezando de (i,k)
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
            if((i >= p)&&(j <= tab.m - p)){//chequea si hay ganador en la diagonal hacia la izquierda de p fichas empezando de (i,k)
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