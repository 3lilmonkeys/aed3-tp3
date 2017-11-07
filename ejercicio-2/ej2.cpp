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

struct estr{
    int estrategia;
    int peso;
    vector<bool> susMovs;

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


vector<bool> mejorHorizontal(bool ofensivo, tablero& tab, int p);
vector<bool> mejorVertical(bool ofensivo, tablero& tab, int p);
vector<bool> mejorDiagonal(bool ofensivo, tablero& tab, int p);
vector<bool> juegoAlCentroExacto(tablero& tab);
//vector<bool> juegoAlCentroAlrededor(tablero& tab); //vale la pena???
vector<bool> ataqueInmediato(bool ofensivo, tablero& tab, int p);


vector<bool> calcularMoves(int estrategia, tablero &tab, int p);
int buscarMejorJugada(vector<estr> posiblesJugadas);
bool hayFicha(tablero& tab, int columna, int fila);
vector<bool> posMaxOIguales(vector<int> resultados);
void actualizarTablero(tablero& tab, int move, bool moveAliado);
int minRes(vector<int> resultados);
int maxRes(vector<int> resultados);
int posMax(vector<int> resultados);
int validarVictoria(tablero& tab, int p);
bool tableroLleno(tablero& tab);
int evaluarJugada(tablero& tab, int columnas, bool maximizar, int c, int p);
void inicializarEstrategias(vector<int> misEstrategias, int cantEstrategias, int columnas);
bool estrategiaEsValida(vector<bool> jugadas);
vector<bool> calcularMoves(int estrategia);
bool sePuedeMoverAhi(tablero &tab, int columna, int fila);

//Como este tipo de estrategia va a variar segun el valor de C en linea, de 2 a C-1, voy a asumir que
// el vector con las estrategias va a tener las ultimas C-3 posiciones con estas estrategias.
// C-3 porque discrimino, C, C-1, y 1.
// Siendo la longitud del vector, ESTRATEGIAS + C-3 (de las estrategias dichas anteriormente).


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


//=====================================INICIO ALGORITMO CENTRAL 2A=================================================\\


int jugadaGolosa(tablero &tab, list<estr> estrs, int estrUnicas, int formarLineas, int bloquearLineas, unsigned int cantEstrTotal, unsigned int columnas, int p){

    for(auto it = estrs.begin(); it != estrs.end() ; it++) it->peso = it->peso * (-1);
    //ordeno las estrategias por su peso de forma descendente
    estrs.sort();
    // reestablezco su peso original
    for(auto it = estrs.begin(); it != estrs.end() ; it++) it->peso = it->peso * (-1);

    // se inicializan las posiblesJugadas y las mejoresJugadas en 0 para cada posicion.
    vector<estr> posiblesJugadas;
    vector<int> mejoresJugadas;

    bool yaTengoFormarLinea = false;
    bool yaTengoBloquearLinea = false;
    vector<bool> calcularJugadas(columnas);

    for(auto it = estrs.begin(); it != estrs.end(); it++){
        if(it->estrategia <= estrUnicas){                       // las estr arrancan de 1. Por eso el <=
            it->susMovs = calcularMoves(it->estrategia, tab,  p);    // calcularMoves falta
            if(estrategiaEsValida(calcularJugadas)){            // miro que realmente me haya dado al menos una jugada valida.
                estr estr1;
                estr1.susMovs = it->susMovs;
                estr1.estrategia = it->estrategia;
                estr1.peso = it->peso;
                posiblesJugadas.push_back(estr1);     // ya  que puede que no exista mov que satisfaga la estr
                // por las dudas me creo una copia del IT, puede ser que sea innecesario
            }
        }
        // calculo por separado ya que son excluyentes, los formar linea y bloquear linea
        else{
            // formarLinea
            if(it->estrategia > estrUnicas && it->estrategia <= formarLineas+estrUnicas && !yaTengoFormarLinea){
                //aca calculo lo de hacer, 2 en linea, 3 en linea ... Solo tomo el primero que me de alguna estr valida
                calcularJugadas = calcularMoves(it->estrategia, tab,  p);
                if(estrategiaEsValida(calcularJugadas)){
                    estr estr1;
                    estr1.susMovs = it->susMovs;
                    estr1.estrategia = it->estrategia;
                    estr1.peso = it->peso;
                    posiblesJugadas.push_back(estr1);
                    // por las dudas me creo una copia del IT, puede ser que sea innecesario
                    // si ya obtuve una estrategia de formarLineaDeXFichas, no quiero seguir mirando otras estrs
                    yaTengoFormarLinea = true;
                }
            }
            else{
                // version defensiva de la de arriba, ver si esta bien que se separen en dos grupos o hacer todo exluyentes.
                if(!yaTengoBloquearLinea){
                    //bloquearLinea
                    calcularJugadas = calcularMoves(it->estrategia, tab,  p);
                    if(estrategiaEsValida(calcularJugadas)){
                        estr estr1;
                        estr1.susMovs = it->susMovs;
                        estr1.estrategia = it->estrategia;
                        estr1.peso = it->peso;
                        posiblesJugadas.push_back(estr1);
                        // por las dudas me creo una copia del IT, puede ser que sea innecesario
                        // si ya obtuve una estrategia de bloquearLineaDeXFichas, no quiero seguir mirando otras estrs
                        yaTengoBloquearLinea = true;
                    }
                }
            }
        }
    }
    return buscarMejorJugada(posiblesJugadas);
    // una vez que tengo los posibles movimientos debo elegir el mejor de ellos segun algoritmo propuesto mas abajo.
    // basicamente voy a devolver la jugada que mas coincida en las estrs pero siempre dando prioridad maxima a primera
    // estr, luego a la segunda, y asi. (ya esta el pseudoCodigo)
}

//=====================================FIN ALGORITMO CENTRAL=================================================\\

//=====================================INICIO ALGORITMO SUBCENTRAL=================================================\\

vector<bool> calcularMoves(int estrategia, tablero &tab, int p){
    vector<bool> movimientos;
    // cuando llamo con TRUE a la funcion -> OFENSIVA, si no DEFENSIVA
    switch(estrategia) //donde opción es la variable a comparar
    {
        case 1:
            movimientos = mejorHorizontal(true, tab, p);
            return movimientos;

        case 2:
            movimientos = mejorVertical(true, tab, p);
            return movimientos;

        case 3:
            movimientos = mejorDiagonal(true, tab, p);
            return movimientos;

        case 4:
            movimientos = ataqueInmediato(true, tab, p);
            return movimientos;

//        case 5:
//            movimientos = conectarLineas(true, tab, p);
//            //falta funcion
//            return movimientos;

//        case 6:
//            movimientos = conectarLineas(false, tab, p);
//            //falta funcion
//            return movimientos;

        case 7:
            movimientos = mejorHorizontal(false, tab, p);
            return movimientos;

        case 8:
            movimientos = mejorVertical(false, tab, p);
            return movimientos;

        case 9:
            movimientos = mejorDiagonal(false, tab, p);
            return movimientos;

        case 10:
            movimientos = ataqueInmediato(false, tab, p);
            return movimientos;

        case 11:
            movimientos = juegoAlCentroExacto(tab);
            return movimientos;

            //falataria el resto que son bloquearLineasDeX y hacerLineaDeX

            //Nótese que valor 1 2 y 3 son los valores que puede tomar la opción
            //la instrucción break es necesaria, para no ejecutar todos los casos.
        default: exit(0);//Bloque de instrucciones por defecto;
            //default, es el bloque que se ejecuta en caso de que no se de ningún caso
    }
}

//=====================================FIN ALGORITMO SUBCENTRAL=================================================\\


//======================================AUXILIARES==============================================================\\

//dado un vector de estr, que al hacer vector[i].movimientos nos queda una especie de matriz de posibles jugadas
// debo buscar los mejores posibles movimientos
// EJEMPLO, si la primera estr, es decir la de mas peso, tiene tres posibles movimentos, y un moviento de esos
// coincide con uno de los movimientos de la segunda estr mas ponderada, elijo ese movimientos sobre los otros.
// VER SI CAMBIANDO LA FORMA DE ELEGIR A LA MEJOR JUGADA MEJORA EL JUGADOR(ej, devolver el mov tal que sumando los pesos
// de todas las estr que tienen ese mov sea mayor que cualquier otro, no importa si la primer estr no tiene ese mov)
int buscarMejorJugada(vector<estr> posiblesJugadas){

}


// chequeo que haya al menos un movimiento que pueda realizar para satisfacer la estrategia
bool estrategiaEsValida(vector<bool> jugadas){
    bool existeJugada = false;
    for(int i = 0; i < jugadas.size(); i++){
        if(jugadas[i]){
            return true;
        }
    }
    return existeJugada;
}


void inicializarEstrategias(vector<estr> misEstrategias, int cantEstrategias, int columnas){
    for(int i = 1; i <= cantEstrategias; i++){
        estr estr;
        estr.estrategia = i;
        estr.peso = 5;            // asignar peso
        vector<bool> movimientosPosibles(columnas);
        estr.susMovs = movimientosPosibles;
        misEstrategias.push_back(estr);
    }
}


vector<bool> ataqueInmediato(bool ofensivo, tablero& tab, int p){
    vector<bool> maxHorizontales = mejorHorizontal(ofensivo, tab, p);
    vector<bool> maxVerticales = mejorVertical(ofensivo, tab, p);
    vector<bool> maxDiagonales = mejorDiagonal(ofensivo, tab, p);
    int actual = 0;
    int mejor = 0;
    int mejorPos = -1;
    vector<int> mejoresJugadas(tab.n);

    if(ofensivo){
        for(int i = 0; i < tab.n; i++){
            bool ifCondition = true;
            int fila = (int) tab.matrizFichas[i].size();

            if(maxHorizontales[i]){                                     // veo cual es la mejor HORIZONTAL
                actual = 0;
                for(int k = 0; k < p && (i + k < tab.m); k++){
                    if(hayFicha(tab, i + k, fila) && ifCondition){             // la horizontal a la derecha
                        if (fichaAliada == tab.matrizFichas[i + k][fila])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0); k++){
                    if(hayFicha(tab, i - k, fila)  && ifCondition){
                        if (fichaAliada == tab.matrizFichas[i - k][fila])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
            }
            if(maxVerticales[i]){
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0); k++){
                    if(hayFicha(tab, i, fila - k && ifCondition)){
                        if (fichaAliada == tab.matrizFichas[i][fila-k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
            }
            if(maxDiagonales[i]){
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i + k < tab.m) && (fila + k < tab.n); k++){
                    if(hayFicha(tab, i + k, fila+k) && ifCondition){             // la horizontal a la derecha
                        if (fichaAliada == tab.matrizFichas[i + k][fila+k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0) && (fila + k < tab.n); k++){
                    if(hayFicha(tab, i - k, fila+k)  && ifCondition){
                        if (fichaAliada == tab.matrizFichas[i - k][fila+k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0) && (fila - k >= 0); k++){
                    if(hayFicha(tab, i - k, fila-k)  && ifCondition){
                        if (fichaAliada == tab.matrizFichas[i - k][fila-k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i + k < tab.n) && (fila - k >= 0); k++){
                    if(hayFicha(tab, i + k, fila-k)  && ifCondition){
                        if (fichaAliada == tab.matrizFichas[i + k][fila-k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
            }

        }
    }
    else{
        for(int i = 0; i < tab.n; i++){
            bool ifCondition = true;
            int fila = (int) tab.matrizFichas[i].size();

            if(maxHorizontales[i]){                                     // veo cual es la mejor HORIZONTAL
                actual = 0;
                for(int k = 0; k < p && (i + k < tab.m); k++){
                    if(hayFicha(tab, i + k, fila) && ifCondition){             // la horizontal a la derecha
                        if (fichaEnemiga == tab.matrizFichas[i + k][fila])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0); k++){
                    if(hayFicha(tab, i - k, fila)  && ifCondition){
                        if (fichaEnemiga == tab.matrizFichas[i - k][fila])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
            }
            if(maxVerticales[i]){
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0); k++){
                    if(hayFicha(tab, i, fila - k && ifCondition)){
                        if (fichaEnemiga == tab.matrizFichas[i][fila-k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
            }
            if(maxDiagonales[i]){
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i + k < tab.m) && (fila + k < tab.n); k++){
                    if(hayFicha(tab, i + k, fila+k) && ifCondition){             // la horizontal a la derecha
                        if (fichaEnemiga == tab.matrizFichas[i + k][fila+k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0) && (fila + k < tab.n); k++){
                    if(hayFicha(tab, i - k, fila+k)  && ifCondition){
                        if (fichaEnemiga == tab.matrizFichas[i - k][fila+k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i - k >= 0) && (fila - k >= 0); k++){
                    if(hayFicha(tab, i - k, fila-k)  && ifCondition){
                        if (fichaEnemiga == tab.matrizFichas[i - k][fila-k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
                actual = 0;
                ifCondition = true;
                for(int k = 0; k < p && (i + k < tab.n) && (fila - k >= 0); k++){
                    if(hayFicha(tab, i + k, fila-k)  && ifCondition){
                        if (fichaEnemiga == tab.matrizFichas[i + k][fila-k])
                        {
                            actual++;
                        }
                        else{
                            ifCondition = false;
                        }
                    }
                    else{
                        if (mejor <= actual){
                            mejor = actual;
                            if(mejoresJugadas[i] < mejor){
                                mejoresJugadas[i] = mejor;
                            }
                        }
                        else{
                            actual = 0;
                        }
                    }
                }
            }

        }
    }
    vector<bool> posiblesRes = posMaxOIguales(mejoresJugadas);
    return posiblesRes;
}


vector<bool> juegoAlCentroExacto(tablero& tab){
    vector<bool> jugadas(tab.n);
    if(tab.n%2==0){
        if(tab.matrizFichas[(tab.n/2) -1].size() < tab.m){
            jugadas[(tab.n/2) -1] = true;
        }
        if(tab.matrizFichas[tab.n/2].size() < tab.m){
            jugadas[tab.n/2] = true;
        }
    }
    else{
        if(tab.matrizFichas[tab.n/2].size() < tab.m){
            jugadas[tab.n/2] = true;
        }
    }
    return jugadas;
}



vector<bool> mejorHorizontal(bool ofensivo, tablero& tab, int p){
    int mejorH = 0;
    vector<bool> mejoresJugadas(tab.n);
    vector<int> mejoresPorFila(tab.n);
    int hActual = 0;

    if(ofensivo){
        for (int i = 0; i < tab.m; ++i)             //i recorre filas
        {
            hActual = 1;
            for (int j = 0; j < tab.n-1; ++j)         //j recorre columnas
            {
                if (hayFicha(tab, j, i) && tab.matrizFichas[j][i] == fichaAliada){ // pregunto por la sgt posicion si la primera es mi ficha
                    if (hayFicha(tab, j+1, i) && fichaAliada == tab.matrizFichas[j + 1][i]){
                        hActual++;
                    }
                    else{
                        if(!hayFicha(tab, j+1, i) && sePuedeMoverAhi(tab, j+1, i)){      //solo me interesa si la posicion esta disponible al sgt turno
                            if (mejorH <= hActual){
                                mejorH = hActual;
                                if(mejoresPorFila[j + 1] < mejorH){
                                    mejoresPorFila[j + 1] = mejorH;
                                }
                            }
                        }
                    }
                }
            }
        }
        vector<bool> posiblesRes = posMaxOIguales(mejoresPorFila);
        return posiblesRes;
    }
    else{   // caso de bloquearMejorHorizontal
        for (int i = 0; i < tab.m; ++i)             //i recorre filas
        {
            hActual = 1;
            for (int j = 0; j < tab.n-1; ++j)         //j recorre columnas
            {
                if (hayFicha(tab, j, i) && tab.matrizFichas[j][i] == fichaEnemiga){ // pregunto por la sgt posicion si la primera es mi ficha
                    if (hayFicha(tab, j+1, i) && fichaEnemiga == tab.matrizFichas[j + 1][i]){
                        hActual++;
                    }
                    else{
                        if(!hayFicha(tab, j+1, i) && sePuedeMoverAhi(tab, j+1, i)){      //solo me interesa si la posicion esta disponible al sgt turno
                            if (mejorH <= hActual){
                                mejorH = hActual;
                                if(mejoresPorFila[j + 1] < mejorH){
                                    mejoresPorFila[j + 1] = mejorH;
                                }
                            }
                        }
                    }
                }
            }
        }
        vector<bool> posiblesRes = posMaxOIguales(mejoresPorFila);
        return posiblesRes;
    }


}



vector<bool> mejorVertical(bool ofensivo, tablero& tab, int p){
    int mejorV = 0;
    vector<bool> mejoresJugadas(tab.n);
    vector<int> mejoresPorColumna(tab.n);
    int vActual = 0;

    if(ofensivo){
        for (int i = 0; i < tab.n; ++i)             //i recorre columnas
        {
            vActual = 1;
            for (int j = 0; j < tab.m-1; ++j)         //j recorre filas
            {
                if (hayFicha(tab, i, j) && tab.matrizFichas[i][j] == fichaAliada){ // pregunto por la sgt posicion si la primera es mi ficha
                    if (hayFicha(tab, i, j+1) && fichaAliada == tab.matrizFichas[i][j + 1]){
                        vActual++;
                    }
                    else{
                        if(!hayFicha(tab, i, j+1) && sePuedeMoverAhi(tab, i, j+1)){      //solo me interesa si la posicion esta disponible al sgt turno
                            if (mejorV <= vActual){
                                mejorV = vActual;
                                if(mejoresPorColumna[i] < mejorV){
                                    mejoresPorColumna[i] = mejorV;
                                }
                            }
                        }
                    }
                }
            }
        }
        vector<bool> posiblesRes = posMaxOIguales(mejoresPorColumna);
        return posiblesRes;
    }
    else{   // caso de bloquearMejorVertical
        for (int i = 0; i < tab.n; ++i)             //i recorre columnas
        {
            vActual = 1;
            for (int j = 0; j < tab.m-1; ++j)         //j recorre filas
            {
                if (hayFicha(tab, i, j) && tab.matrizFichas[i][j] == fichaEnemiga){ // pregunto por la sgt posicion si la primera es mi ficha
                    if (hayFicha(tab, i, j+1) && fichaEnemiga == tab.matrizFichas[i][j + 1]){
                        vActual++;
                    }
                    else{
                        if(!hayFicha(tab, i, j+1) && sePuedeMoverAhi(tab, i, j+1)){      //solo me interesa si la posicion esta disponible al sgt turno
                            if (mejorV <= vActual){
                                mejorV = vActual;
                                if(mejoresPorColumna[i] < mejorV){
                                    mejoresPorColumna[i] = mejorV;
                                }
                            }
                        }
                    }
                }
            }
        }
        vector<bool> posiblesRes = posMaxOIguales(mejoresPorColumna);
        return posiblesRes;
    }
}


vector<bool> mejorDiagonal(bool ofensivo, tablero& tab, int c){
    int mejorV = 0;
    vector<bool> mejoresJugadas(tab.n);
    vector<int> mejoresPorColumna(tab.n);
    int vActual = 0;

    if(ofensivo){
        for(int i = 0; i < tab.n-1; i++){           // recorro columnas
            for(int j = 0; j < tab.m -1; j++){

                    for (int k = 0; k < c; ++k)
                    {
                        if(hayFicha(tab, i + k, j + k)){
                            if (hayFicha(tab, i ,j) && tab.matrizFichas[i][j] == fichaAliada && fichaAliada == tab.matrizFichas[i + k][j + k])
                            {
                                vActual++;
                            }
                            else{
                                vActual = 0;
                            }
                        }
                        else{
                            if(sePuedeMoverAhi(tab, i+k, j+k)){
                                if(mejorV <= vActual){
                                    mejorV = vActual;
                                    if(mejoresPorColumna[i+k] < mejorV){
                                        mejoresPorColumna[i+k] = mejorV;
                                    }
                                }
                            }
                            else{
                                vActual = 0;
                            }
                        }
                    }
                    for (int k = 0; k < c; ++k)
                    {
                        if(hayFicha(tab, i - k, j + k)){
                            if (hayFicha(tab, i ,j) && tab.matrizFichas[i][j] == fichaAliada && fichaAliada == tab.matrizFichas[i - k][j + k])
                            {
                                vActual++;
                            }
                            else{
                                vActual = 0;
                            }
                        }
                        else{
                            if(sePuedeMoverAhi(tab, i-k, j+k)){
                                if(mejorV <= vActual){
                                    mejorV = vActual;
                                    if(mejoresPorColumna[i-k] < mejorV){
                                        mejoresPorColumna[i-k] = mejorV;
                                    }
                                }
                            }
                            else{
                                vActual = 0;
                            }
                        }
                    }
            }
        }

        vector<bool> posiblesRes = posMaxOIguales(mejoresPorColumna);
        return posiblesRes;
    }
    else{   // caso de bloquearMejorVertical
        for(int i = 0; i < tab.n-1; i++){           // recorro columnas
            for(int j = 0; j < tab.m -1; j++){

                    for (int k = 0; k < c; ++k)
                    {
                        if(hayFicha(tab, i + k, j + k)){
                            if (hayFicha(tab, i ,j) && tab.matrizFichas[i][j] == fichaEnemiga && fichaEnemiga == tab.matrizFichas[i + k][j + k])
                            {
                                vActual++;
                            }
                            else{
                                vActual = 0;
                            }
                        }
                        else{
                            if(sePuedeMoverAhi(tab, i+k, j+k)){
                                if(mejorV <= vActual){
                                    mejorV = vActual;
                                    if(mejoresPorColumna[i+k] < mejorV){
                                        mejoresPorColumna[i+k] = mejorV;
                                    }
                                }
                            }
                            else{
                                vActual = 0;
                            }
                        }
                    }
                    for (int k = 0; k < c; ++k)
                    {
                        if(hayFicha(tab, i - k, j + k)){
                            if (hayFicha(tab, i ,j) && tab.matrizFichas[i][j] == fichaEnemiga && fichaEnemiga == tab.matrizFichas[i - k][j + k])
                            {
                                vActual++;
                            }
                            else{
                                vActual = 0;
                            }
                        }
                        else{
                            if(sePuedeMoverAhi(tab, i-k, j+k)){
                                if(mejorV <= vActual){
                                    mejorV = vActual;
                                    if(mejoresPorColumna[i-k] < mejorV){
                                        mejoresPorColumna[i-k] = mejorV;
                                    }
                                }
                            }
                            else{
                                vActual = 0;
                            }
                        }
                    }
            }
        }
        vector<bool> posiblesRes = posMaxOIguales(mejoresPorColumna);
        return posiblesRes;
    }
}



bool sePuedeMoverAhi(tablero &tab, int columna, int fila){
    bool res = false;
    if(tab.m < fila || tab.n < columna) return false;
    if(fila == 0 || hayFicha(tab, columna, fila-1)) return true;
    return res;
}



int main(){
    int C = 4;
    int estrategiasUnicas = 11;
    int cantEstrategias = estrategiasUnicas + C - 2;
    vector<int> misEstrategias;
    //inicializarEstrategias(misEstrategias, cantEstrategias);

    int columnas = 4;
    int filas = 4;
    int cLinea = 4;
    int fichas = 6;

    tablero tab1 = crearTablero(columnas, filas);

    // para testear las funciones de forma individual!

    // mejorHorizontal esta testeada medianamente bien...
    // mejorVertical esta testeada medianamente bien...
    // jugarAlCentro no necesita mucho testeo...
    // mejorDiagonal esta testeada muy poco
    // ataqueInmediato le falta la parte de testeo.

    actualizarTablero(tab1, 1, true);
    actualizarTablero(tab1, 1, true);
    actualizarTablero(tab1, 1, false);
    actualizarTablero(tab1, 0, true);


    vector<bool> posibleRes = mejorDiagonal(true, tab1, cLinea);

    cout << "Move a realizar: \n " << posibleRes[0] << ", " << posibleRes[1] << ", "
         << posibleRes[2] << ", " << posibleRes[3];

    // Primero movi yo, luego el rival, cada columna tiene una sola ficha.
    // por lo tanto al evaluar jugada deberia devolverme que mueva a la columna 1 ya que en la 0 esta el rival y en la 1 yo.

//    int movimiento = calcularJugada(tab1, columnas, cLinea, fichas);

//    cout << "El movimiento a realizar es en la columna: " << movimiento << endl;


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

vector<bool> posMaxOIguales(vector<int> resultados){
    int maxPos = 0;
    bool hayMax = false;
    vector<bool> posiblesPos(resultados.size());
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] > maxPos && resultados[i] > 0){
            maxPos = i;
            hayMax = true;
        }
    }
    if(hayMax){
        for(int i = 0; i < resultados.size(); i++){
            if(resultados[i] == resultados[maxPos]){
                posiblesPos[i] = true;
            }else{
                posiblesPos[i] = false;
            }
        }
    }
    return posiblesPos;
}

void actualizarTablero(tablero& tab, int move, bool moveAliado){
    if(moveAliado){
        tab.matrizFichas[move].push_back(fichaAliada);
    }
    else{
        tab.matrizFichas[move].push_back(fichaEnemiga);
    }
}

bool hayFicha(tablero& tab, int columna, int fila){
    if(columna >= tab.n || fila >= tab.m) return false;
    if(columna < 0 || fila < 0) return false;
    return fila < tab.matrizFichas[columna].size();
}
