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
#include <random>
#include <time.h>

#include "auxiliar.h"

#define gane 1;
#define perdi -1;
#define empate 0;
#define fichaAliada 10
#define fichaEnemiga 20

using namespace std;


struct resultado{
    int lineaMax;
    vector<bool> resultados;
};


struct estr{
    int estrategia;
    int peso;
    resultado susMovs;

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

resultado conectarLineas(bool ofensivo, tablero& tab, int p);
resultado mejorHorizontal(bool ofensivo, tablero& tab, int p);
resultado mejorVertical(bool ofensivo, tablero& tab, int p);
resultado mejorDiagonal(bool ofensivo, tablero& tab, int p);
resultado juegoAlCentroExacto(tablero& tab);
resultado ataqueInmediato(bool ofensivo, tablero& tab, int p);
resultado lineasDeXFichas(int x1, bool ofensivo, tablero& tab, int p);

int unoParaPerder(tablero &tab, int p);
int unoParaGanar(tablero &tab, int p);
bool columnaLlena(tablero &tab, int columna);
resultado calcularMoves(int estrategia, tablero &tab, int p);
int buscarMejorJugada(vector<estr> posiblesJugadas);
//bool hayFicha(tablero& tab, int columna, int fila);
bool hayFichaAliada(tablero& tab, int columna, int fila);
bool hayFichaEnemiga(tablero& tab, int columna, int fila);
resultado posMaxOIguales(vector<int> resultados);
//void actualizarTablero(tablero& tab, int move, bool moveAliado);
list<estr> inicializarEstrategias(int estrUnicas, int estrTotales, int columnas);
bool estrategiaEsValida(vector<bool> jugadas);
int jugadaCasiRandom(tablero& tab, int c);

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


int jugadaGolosa(tablero &tab, list<estr> estrs, int estrUnicas, int formarYBloquearLineas, unsigned int cantEstrTotal, unsigned int columnas, int p){

    int moveParaGanar = unoParaGanar(tab,p);
    if(moveParaGanar >= 0){
        return moveParaGanar;
    }

	int moveParaPerder = unoParaPerder(tab, p);
	if (moveParaPerder >= 0) {
		return moveParaPerder;
	}

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

    for(auto it = estrs.begin(); it != estrs.end(); it++){
        int size = (int) posiblesJugadas.size();
        if(it->estrategia <= estrUnicas){                       // las estr arrancan de 1. Por eso el <=
            it->susMovs = calcularMoves(it->estrategia, tab,  p);    // calcularMoves falta
            if(estrategiaEsValida(it->susMovs.resultados)){            // miro que realmente me haya dado al menos una jugada valida.
                estr estr1;
                estr1.susMovs = it->susMovs;
                estr1.estrategia = it->estrategia;
                estr1.peso = it->peso;
                posiblesJugadas.push_back(estr1);     // ya  que puede que no exista mov que satisfaga la estr
                size = (int) posiblesJugadas.size();
                // por las dudas me creo una copia del IT, puede ser que sea innecesario
            }
        }
            // calculo por separado ya que son excluyentes, los formar linea y bloquear linea
        else{
            // formarLinea
            if(it->estrategia > estrUnicas && it->estrategia <= formarYBloquearLineas+estrUnicas && ((it->estrategia)%2==0) && !yaTengoFormarLinea){
                //aca calculo lo de hacer, 2 en linea, 3 en linea ... Solo tomo el primero que me de alguna estr  valida
                it->susMovs = calcularMoves(it->estrategia, tab,  p);
                if(estrategiaEsValida(it->susMovs.resultados)){
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
                    it->susMovs = calcularMoves(it->estrategia, tab,  p);
                    if(estrategiaEsValida(it->susMovs.resultados)){
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
    int jugada = buscarMejorJugada(posiblesJugadas);
    return jugada;
    // una vez que tengo los posibles movimientos debo elegir el mejor de ellos segun algoritmo propuesto mas abajo.
    // basicamente voy a devolver la jugada que mas coincida en las estrs pero siempre dando prioridad maxima a primera
    // estr, luego a la segunda, y asi. (ya esta el pseudoCodigo)
}

//=====================================FIN ALGORITMO CENTRAL=================================================\\

//=====================================INICIO ALGORITMO SUBCENTRAL=================================================\\

resultado calcularMoves(int estrategia, tablero &tab, int p){
    resultado movimientos;
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

        case 5:
            movimientos = conectarLineas(true, tab, p);
            return movimientos;

        case 6:
            movimientos = conectarLineas(false, tab, p);
            return movimientos;

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
        case 12:
            movimientos = lineasDeXFichas(estrategia-10, true, tab, p);
            return movimientos;
        case 13:
            lineasDeXFichas(estrategia-11, false, tab, p);
            return movimientos;
        case 14:
            lineasDeXFichas(estrategia-11, true, tab, p);
            return movimientos;
        case 15:
            lineasDeXFichas(estrategia-12, false, tab, p);
            return movimientos;
        case 16:
            lineasDeXFichas(estrategia-12, true, tab, p);
            return movimientos;
        case 17:
            lineasDeXFichas(estrategia-13, false, tab, p);
            return movimientos;
        case 18:
            lineasDeXFichas(estrategia-13, true, tab, p);
            return movimientos;
        case 19:
            lineasDeXFichas(estrategia-14, false, tab, p);
            return movimientos;
        case 20:
            lineasDeXFichas(estrategia-14, true, tab, p);
            return movimientos;
        case 21:
            lineasDeXFichas(estrategia-15, false, tab, p);
            return movimientos;
        case 22:
            lineasDeXFichas(estrategia-15, true, tab, p);
            return movimientos;
        case 23:
            lineasDeXFichas(estrategia-16, false, tab, p);
            return movimientos;


        default: cout << "estrategia no soportada";
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
int buscarMejorJugada(vector<estr> posiblesEstr){
    int rachaActual = 0;
    int mejorJugada = -1;
    int mejorMov = -1;
    bool movCumpleEstr = true;
    for(int i = 0; i < posiblesEstr[0].susMovs.resultados.size(); i++){
        movCumpleEstr = true;
        for(int j = 0; movCumpleEstr && j < posiblesEstr.size(); j++){
            if(posiblesEstr[j].susMovs.resultados[i]){
                rachaActual++;
                if(j == posiblesEstr.size()-1){
                    if(mejorJugada < rachaActual){
                        mejorJugada = rachaActual;
                        mejorMov = i;
                    }
                    rachaActual = 0;
                }

            }
            else{
                movCumpleEstr = false;
                if(mejorJugada < rachaActual){
                    mejorJugada = rachaActual;
                    mejorMov = i;
                }
                rachaActual = 0;
            }
        }
    }
    return mejorMov;
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


list<estr> inicializarEstrategias(int estrUnicas, int estrTotales, int columnas){
    list<estr> misEstrategias;
    for(int i = 1; i <= estrTotales; i++){
        estr estr;
        estr.estrategia = i;
        estr.peso = rand()%100;            // asignar peso
        vector<bool> movimientosPosibles(columnas);
        estr.susMovs.lineaMax = 0;
        estr.susMovs.resultados = movimientosPosibles;
        misEstrategias.push_back(estr);
    }
//2 0 0 15 15 10 3 8 3 11 11 11 13 14 5 
//1 0 11 15 15 4 9 11 4 14 7 15 3 5 4       (50 generaciones vs oponentes)
//0 9 9 12 15 11 11 0 9 1 5 9 4 8 14        (100 generaciones vs poblacion 0.93 WR)
//8 9 2 14 13 13 4 6 14 15 7 0 12 14 0      (50 generaciones vs poblacion 0.98 WR 0.55 Rapidez)
    auto it = misEstrategias.begin();
    it->peso = 8;
    it++;
    it->peso = 9;
    it++;
    it->peso = 2;
    it++;
    it->peso = 14;
    it++;
    it->peso = 13;
    it++;
    it->peso = 13;
    it++;
    it->peso = 4;
    it++;
    it->peso = 6;
    it++;
    it->peso = 14;
    it++;
    it->peso = 15;
    it++;
    it->peso = 7;
    it++;
    it->peso = 0;
    it++;
    it->peso = 12;
    it++;
    it->peso = 14;
    it++;
    it->peso = 0;
    it++;
    return misEstrategias;
}

vector<int> inicializarEstrategiaUnica(int estrUnicas, int estrTotales, int columnas, int miEstr){
    vector<int> misEstrategias(23);
    for(int i = 1; i <= estrTotales; i++){
        if(i == miEstr){
            misEstrategias[i] = 15;    
        }
        else{
            misEstrategias[i] = 0;    
        }
        
    }
    return misEstrategias;
}


resultado lineasDeXFichas(int x, bool ofensivo, tablero& tab, int p){
    vector<bool> posiblesJugadas(tab.n);
    vector<tablero> tabPosibles(tab.n);
    bool aliado = true;
    bool sigoMirando = true;
    int lineaActual = 0;

    bool sigoPorDerecha = true;
    bool sigoPorIzq = true;

    if(ofensivo){
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                posiblesJugadas[i] = false;
                continue;
            }
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco conexion de horizontales
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaAliada(tabPosibles[i], i+k, fila)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaAliada(tabPosibles[i], i-k, fila)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }

            lineaActual = 0;
            sigoPorDerecha = true;
            sigoPorIzq = true;
            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaAliada(tabPosibles[i], i+k, fila+k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaAliada(tabPosibles[i], i-k, fila-k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas diagonales (\) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaAliada(tabPosibles[i], i+k, fila-k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaAliada(tabPosibles[i], i-k, fila+k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas exactas de x fichas verticalmente, solo miro para abajo...
            for(int k = 1; k < p && sigoMirando; k++){
                if(hayFichaAliada(tabPosibles[i], i, fila-k)){
                    lineaActual++;
                }
                else break;
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }
        }
    }
    else{
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                posiblesJugadas[i] = false;
                continue;
            }
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;
            int mejorConexionParcial = 0;

            // busco conexion de horizontales
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i+k, fila)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i-k, fila)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }
            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i+k, fila+k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i-k, fila-k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas diagonales (\) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i+k, fila-k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i-k, fila+k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas exactas de x fichas verticalmente, solo miro para abajo...
            for(int k = 1; k < p && sigoMirando; k++){
                if(hayFichaEnemiga(tabPosibles[i], i, fila-k)){
                    lineaActual++;
                }
                else break;
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(lineaActual <= x-1) {
                if(lineaActual == x-1) posiblesJugadas[i] = true;
            }
            else{
                posiblesJugadas[i] = false;
                sigoMirando = false;
            }
        }
    }
    resultado res;
    res.lineaMax = x;
    res.resultados = posiblesJugadas;
    return res;
}

resultado conectarLineas(bool ofensivo, tablero& tab, int p){
    vector<int> mejoresConexionesColumna(tab.n);
    vector<tablero> tabPosibles(tab.n);
    bool aliado = true;

    //considerar que no tiene sentido ver si conecte una vertical...Es imposible
    //tomo que conectarLinea solo suceda si realmente conecte una linea, y por ej si alargue una linea pero no conecte
    // nada, eso no vale como conectar (caso de juegar en las columnas tab.n-1 y 0 no los cuento entonces)

    if(ofensivo){
        for(int i = 1; i < tab.n-1; i++){
            if(!columnaLlena(tab, i)){
                mejoresConexionesColumna[i] = -1;
                continue;
            }
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            int conexionActual = 0;
            int mejorConexionParcial = 0;
            bool expandoSoloPorIzq = false;
            bool expandoSoloPorDer = false;

            // busco conexion de horizontales
            for(int k = 1; k < p; k++){
                if(expandoSoloPorDer){
                    if(hayFichaAliada(tabPosibles[i], i+k, fila)){
                        conexionActual++;
                    }
                }
                else if(expandoSoloPorIzq){
                    if(hayFichaAliada(tabPosibles[i], i-k, fila)){
                        conexionActual++;
                    }
                }
                else{
                    if(hayFichaAliada(tabPosibles[i], i+k, fila) && hayFichaAliada(tabPosibles[i], i-k, fila) ){
                        conexionActual += 2;
                    }
                    else{
                        if(k > 1){
                            if(hayFichaAliada(tabPosibles[i], i+k, fila)){
                                conexionActual++;
                                expandoSoloPorDer = true;
                            }
                            if(hayFichaAliada(tabPosibles[i], i-k, fila)){
                                conexionActual++;
                                expandoSoloPorIzq = true;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            expandoSoloPorIzq = false;
            expandoSoloPorDer = false;
            mejorConexionParcial = conexionActual;
            conexionActual = 0;
            // busco conexion de DIAGONALES orientadas a la derecha es decir / asi...
            for(int k = 1; k < p; k++){
                if(expandoSoloPorDer){
                    if(hayFichaAliada(tabPosibles[i], i+k, fila+1)){
                        conexionActual++;
                    }
                }
                else if(expandoSoloPorIzq){
                    if(hayFichaAliada(tabPosibles[i], i-k, fila-1)){
                        conexionActual++;
                    }
                }
                else{
                    if(hayFichaAliada(tabPosibles[i], i+k, fila+k) && hayFichaAliada(tabPosibles[i], i-k, fila-k) ){
                        conexionActual += 2;
                    }
                    else{
                        if(k > 1){
                            if(hayFichaAliada(tabPosibles[i], i+k, fila+k)){
                                conexionActual++;
                                expandoSoloPorDer = true;
                            }
                            if(hayFichaAliada(tabPosibles[i], i-k, fila-k)){
                                conexionActual++;
                                expandoSoloPorIzq = true;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            expandoSoloPorIzq = false;
            expandoSoloPorDer = false;
            if(mejorConexionParcial < conexionActual){
                mejorConexionParcial = conexionActual;
            }
            conexionActual = 0;

            // busco conexion de DIAGONALES orientadas a la izquiera es decir \ asi...
            for(int k = 1; k < p; k++){
                if(expandoSoloPorDer){
                    if(hayFichaAliada(tabPosibles[i], i+k, fila-1)){
                        conexionActual++;
                    }
                }
                else if(expandoSoloPorIzq){
                    if(hayFichaAliada(tabPosibles[i], i-k, fila+1)){
                        conexionActual++;
                    }
                }
                else{
                    if(hayFichaAliada(tabPosibles[i], i+k, fila-k) && hayFichaAliada(tabPosibles[i], i-k, fila+k) ){
                        conexionActual += 2;
                    }
                    else{
                        if(k > 1){
                            if(hayFichaAliada(tabPosibles[i], i+k, fila-k)){
                                conexionActual++;
                                expandoSoloPorDer = true;
                            }
                            if(hayFichaAliada(tabPosibles[i], i-k, fila+k)){
                                conexionActual++;
                                expandoSoloPorIzq = true;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            if(mejorConexionParcial < conexionActual){
                mejorConexionParcial = conexionActual;
            }
            conexionActual = 0;

            mejoresConexionesColumna[i] = mejorConexionParcial;
        }
        return posMaxOIguales(mejoresConexionesColumna);
    }
    else{
        for(int i = 1; i < tab.n-1; i++){
            if(!columnaLlena(tab, i)){
                mejoresConexionesColumna[i] = -1;
                continue;
            }
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, desconecte alguna posible futura linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            int conexionActual = 0;
            int mejorConexionParcial = 0;
            bool expandoSoloPorIzq = false;
            bool expandoSoloPorDer = false;

            // busco conexion de horizontales
            for(int k = 1; k < p; k++){
                if(expandoSoloPorDer){
                    if(hayFichaEnemiga(tabPosibles[i], i+k, fila)){
                        conexionActual++;
                    }
                }
                else if(expandoSoloPorIzq){
                    if(hayFichaEnemiga(tabPosibles[i], i-k, fila)){
                        conexionActual++;
                    }
                }
                else{
                    if(hayFichaEnemiga(tabPosibles[i], i+k, fila) && hayFichaEnemiga(tabPosibles[i], i-k, fila) ){
                        conexionActual += 2;
                    }
                    else{
                        if(k > 1){
                            if(hayFichaEnemiga(tabPosibles[i], i+k, fila)){
                                conexionActual++;
                                expandoSoloPorDer = true;
                            }
                            if(hayFichaEnemiga(tabPosibles[i], i-k, fila)){
                                conexionActual++;
                                expandoSoloPorIzq = true;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            expandoSoloPorIzq = false;
            expandoSoloPorDer = false;
            mejorConexionParcial = conexionActual;
            conexionActual = 0;
            // busco conexion de DIAGONALES orientadas a la derecha es decir / asi...
            for(int k = 1; k < p; k++){
                if(expandoSoloPorDer){
                    if(hayFichaEnemiga(tabPosibles[i], i+k, fila+1)){
                        conexionActual++;
                    }
                }
                else if(expandoSoloPorIzq){
                    if(hayFichaEnemiga(tabPosibles[i], i-k, fila-1)){
                        conexionActual++;
                    }
                }
                else{
                    if(hayFichaEnemiga(tabPosibles[i], i+k, fila+k) && hayFichaEnemiga(tabPosibles[i], i-k, fila-k) ){
                        conexionActual += 2;
                    }
                    else{
                        if(k > 1){
                            if(hayFichaEnemiga(tabPosibles[i], i+k, fila+k)){
                                conexionActual++;
                                expandoSoloPorDer = true;
                            }
                            if(hayFichaEnemiga(tabPosibles[i], i-k, fila-k)){
                                conexionActual++;
                                expandoSoloPorIzq = true;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            expandoSoloPorIzq = false;
            expandoSoloPorDer = false;
            if(mejorConexionParcial < conexionActual){
                mejorConexionParcial = conexionActual;
            }
            conexionActual = 0;

            // busco conexion de DIAGONALES orientadas a la izquiera es decir \ asi...
            for(int k = 1; k < p; k++){
                if(expandoSoloPorDer){
                    if(hayFichaEnemiga(tabPosibles[i], i+k, fila-1)){
                        conexionActual++;
                    }
                }
                else if(expandoSoloPorIzq){
                    if(hayFichaEnemiga(tabPosibles[i], i-k, fila+1)){
                        conexionActual++;
                    }
                }
                else{
                    if(hayFichaEnemiga(tabPosibles[i], i+k, fila-k) && hayFichaEnemiga(tabPosibles[i], i-k, fila+k) ){
                        conexionActual += 2;
                    }
                    else{
                        if(k > 1){
                            if(hayFichaEnemiga(tabPosibles[i], i+k, fila-k)){
                                conexionActual++;
                                expandoSoloPorDer = true;
                            }
                            if(hayFichaEnemiga(tabPosibles[i], i-k, fila+k)){
                                conexionActual++;
                                expandoSoloPorIzq = true;
                            }
                        }
                        else{
                            break;
                        }
                    }
                }
            }
            if(mejorConexionParcial < conexionActual){
                mejorConexionParcial = conexionActual;
            }
            conexionActual = 0;

            mejoresConexionesColumna[i] = mejorConexionParcial;
        }
        return posMaxOIguales(mejoresConexionesColumna);
    }

}

resultado ataqueInmediato(bool ofensivo, tablero& tab, int p){
    vector<int> mejoresLineas(tab.n);
    vector<tablero> tabPosibles(tab.n);
    bool aliado = true;
    bool sigoMirando = true;
    int lineaActual = 0;

    bool sigoPorDerecha = true;
    bool sigoPorIzq = true;

    if(ofensivo){
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco conexion de horizontales
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaAliada(tabPosibles[i], i+k, fila)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaAliada(tabPosibles[i], i-k, fila)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            lineaActual = 0;
            sigoPorDerecha = true;
            sigoPorIzq = true;
            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaAliada(tabPosibles[i], i+k, fila+k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaAliada(tabPosibles[i], i-k, fila-k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas diagonales (\) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaAliada(tabPosibles[i], i+k, fila-k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaAliada(tabPosibles[i], i-k, fila+k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas exactas de x fichas verticalmente, solo miro para abajo...
            for(int k = 1; k < p && sigoMirando; k++){
                if(hayFichaAliada(tabPosibles[i], i, fila-k)){
                    lineaActual++;
                }
                else break;
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    else{
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;
            int mejorConexionParcial = 0;

            // busco conexion de horizontales
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i+k, fila)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i-k, fila)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i+k, fila+k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i-k, fila-k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas diagonales (\) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++){
                if(sigoPorDerecha || sigoPorIzq){
                    if(sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i+k, fila-k)) {
                        lineaActual++;
                    }
                    else{
                        sigoPorDerecha = false;
                    }
                    if(sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i-k, fila+k)){
                        lineaActual++;
                    }
                    else sigoPorIzq = false;
                }
            }

            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;
            // busco lineas exactas de x fichas verticalmente, solo miro para abajo...
            for(int k = 1; k < p && sigoMirando; k++){
                if(hayFichaEnemiga(tabPosibles[i], i, fila-k)){
                    lineaActual++;
                }
                else break;
            }
            // si ya encontre una linea que justo puedo formar una de x1 > x fichas, listo, paso a la sgt columna
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    return posMaxOIguales(mejoresLineas);
}



resultado juegoAlCentroExacto(tablero& tab){
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
    resultado res;
    res.lineaMax = 1;
    res.resultados = jugadas;
    return res;
}



resultado mejorHorizontal(bool ofensivo, tablero& tab, int p){
    vector<int> mejoresLineas(tab.n);
    vector<tablero> tabPosibles(tab.n);
    bool aliado = true;
    bool sigoMirando = true;
    int lineaActual = 0;

    bool sigoPorDerecha = true;
    bool sigoPorIzq = true;

    if(ofensivo){
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoPorDerecha = true;
            sigoPorIzq = true;
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaAliada(tabPosibles[i], i + k, fila)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            lineaActual = 0;
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorIzq && hayFichaAliada(tabPosibles[i], i - k, fila)) {
                    lineaActual++;
                } else {
                    sigoPorIzq = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    else{
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoPorDerecha = true;
            sigoPorIzq = true;
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i + k, fila)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            lineaActual = 0;
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i - k, fila)) {
                    lineaActual++;
                } else {
                    sigoPorIzq = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    return posMaxOIguales(mejoresLineas);
}



resultado mejorVertical(bool ofensivo, tablero& tab, int p){
    vector<int> mejoresLineas(tab.n);
    vector<tablero> tabPosibles(tab.n);
    bool aliado = true;
    bool sigoMirando = true;
    int lineaActual = 0;

    bool sigoPorDerecha = true;

    if(ofensivo){
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoPorDerecha = true;
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaAliada(tabPosibles[i], i, fila-k)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    else{
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoPorDerecha = true;
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i, fila-k)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    return posMaxOIguales(mejoresLineas);
}


resultado mejorDiagonal(bool ofensivo, tablero& tab, int p){
    vector<int> mejoresLineas(tab.n);
    vector<tablero> tabPosibles(tab.n);
    bool aliado = true;
    bool sigoMirando = true;
    int lineaActual = 0;

    bool sigoPorDerecha = true;
    bool sigoPorIzq = true;

    if(ofensivo){
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoPorDerecha = true;
            sigoPorIzq = true;
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaAliada(tabPosibles[i], i + k, fila + k)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            lineaActual = 0;
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorIzq && hayFichaAliada(tabPosibles[i], i - k, fila - k)) {
                    lineaActual++;
                } else {
                    sigoPorIzq = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;

            // busco lineas diagonales (\) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaAliada(tabPosibles[i], i + k, fila - k)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            lineaActual = 0;
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorIzq && hayFichaAliada(tabPosibles[i], i - k, fila + k)) {
                    lineaActual++;
                } else {
                    sigoPorIzq = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    else{
        for(int i = 0; i < tab.n; i++){
            if(!columnaLlena(tab, i)){
                mejoresLineas[i] = -1;
                continue;
            }
            sigoPorDerecha = true;
            sigoPorIzq = true;
            sigoMirando = true;
            tabPosibles[i] = tab;
            actualizarTablero(tabPosibles[i], i, aliado);
            //me fijo si al mover la ficha en la columna i, conecte alguna linea.

            // la fila donde jugue la ficha seria el nuevo size-1...
            int fila = (int) tabPosibles[i].matrizFichas[i].size()-1;

            lineaActual = 0;

            // busco lineas diagonales (/) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i + k, fila + k)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            lineaActual = 0;
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i - k, fila - k)) {
                    lineaActual++;
                } else {
                    sigoPorIzq = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            sigoPorDerecha = true;
            sigoPorIzq = true;
            lineaActual = 0;

            // busco lineas diagonales (\) si todavia no en encontre una linea que contradiga que se puede tener una linea de x
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorDerecha && hayFichaEnemiga(tabPosibles[i], i + k, fila - k)) {
                    lineaActual++;
                } else {
                    sigoPorDerecha = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }

            lineaActual = 0;
            for(int k = 1; k < p && sigoMirando; k++) {
                if (sigoPorIzq && hayFichaEnemiga(tabPosibles[i], i - k, fila + k)) {
                    lineaActual++;
                } else {
                    sigoPorIzq = false;
                }
            }
            if(mejoresLineas[i] < lineaActual) {
                mejoresLineas[i] = lineaActual;
                if(lineaActual == p-1){ // si ya obtuve la mejor linea posible, es decir p en linea - 1
                    sigoMirando = false;
                }
            }
        }
    }
    return posMaxOIguales(mejoresLineas);
}


bool columnaLlena(tablero &tab, int columna){
    if(tab.matrizFichas[columna].size() < tab.m){
        return true;
    }
    else return false;
}

int buscarTrue(vector<bool> jugadas){
    for(int i = 0; i < jugadas.size(); i++){
        if(jugadas[i]) return i;
    }
    return 0;
}



int unoParaGanar(tablero &tab, int p){
    resultado res;
    res = mejorHorizontal(true, tab, p);
    if(res.lineaMax == p-1) return buscarTrue(res.resultados);
    res = mejorVertical(true, tab, p);
    if(res.lineaMax == p-1) return buscarTrue(res.resultados);
    res = mejorDiagonal(true, tab, p);
    if(res.lineaMax == p-1) return buscarTrue(res.resultados);
    res = ataqueInmediato(true, tab, p);
    if(res.lineaMax == p-1) return buscarTrue(res.resultados);
    res = conectarLineas(true, tab, p);
    if(res.lineaMax == p-1) return buscarTrue(res.resultados);

	return -1;
}


int unoParaPerder(tablero &tab, int p) {
	resultado res;
	res = mejorHorizontal(false, tab, p);
	if (res.lineaMax == p-1) return buscarTrue(res.resultados);
	res = mejorVertical(false, tab, p);
	if (res.lineaMax == p-1) return buscarTrue(res.resultados);
	res = mejorDiagonal(false, tab, p);
	if (res.lineaMax == p-1) return buscarTrue(res.resultados);
	res = ataqueInmediato(false, tab, p);
	if (res.lineaMax == p-1) return buscarTrue(res.resultados);
	res = conectarLineas(false, tab, p);
	if (res.lineaMax == p-1) return buscarTrue(res.resultados);

	return -1;
}



//int main() {
//    vector<int> myVec = {111,2,1,111,111,122};
//    resultado res;
//    res = posMaxOIguales(myVec);
//    cout << res.resultados[0] << "," << res.resultados[1] << "," << res.resultados[2] << "," << res.resultados[3] << "," <<  res.resultados[4]<< "," <<  res.resultados[5];
//
//    int C = 64
//    int estrategiasUnicas = 11;
//    int estrTotales = estrategiasUnicas+ (C-2)*2;
//    int columnas = 6;
//    int filas = 6;
//    int fichas = 18;
//
//    list<estr> estrategias = inicializarEstrategias(estrategiasUnicas, estrTotales, columnas);
//    for(auto it = estrategias.begin(); it != estrategias.end(); it++){
//        int estr = it->estrategia;
//        cout << it->estrategia << "\n";
//    }
//    tablero tab1 = crearTablero(columnas, filas);
//
//    // para testear las funciones de forma individual!
//
//    // mejorHorizontal esta testeada medianamente bien...
//    // mejorVertical esta testeada medianamente bien...
//    // jugarAlCentro no necesita mucho testeo...
//    // mejorDiagonal esta testeada muy poco
//    // ataqueInmediato le falta la parte de testeo.
//
//    actualizarTablero(tab1, 0, true);
//    actualizarTablero(tab1, 0, false);
//    actualizarTablero(tab1, 1, true);
//    actualizarTablero(tab1, 1, true);
//    actualizarTablero(tab1, 2, true);
//    actualizarTablero(tab1, 2, false);
//    actualizarTablero(tab1, 3, true);
//    actualizarTablero(tab1, 4, false);
//    actualizarTablero(tab1, 4, true);
//    actualizarTablero(tab1, 4, true);
//    actualizarTablero(tab1, 5, false);
//    actualizarTablero(tab1, 5, true);
//    actualizarTablero(tab1, 5, false);
//    actualizarTablero(tab1, 5, true);
//
//
////    vector<bool> posibleRes = mejorHorizontal(false, tab1, cLinea);
////
////    cout << "Move a realizar: \n " << posibleRes[0] << ", " << posibleRes[1] << ", "
////         << posibleRes[2] << ", " << posibleRes[3] << ", " << posibleRes[4] << ", " << posibleRes[5];
//
//
//    int movimiento = jugadaGolosa(tab1, estrategias, estrategiasUnicas, (C-2)*2, estrTotales, columnas, C);
//
//    cout << "El movimiento a realizar es en la columna: " << movimiento << endl;
//
//
//}

//==========================COSAS DEL EJ1 QUE PUEDEN SERVIR=================================================//

resultado posMaxOIguales(vector<int> resultados){
    int maxPos = 0;
    bool hayMax = true;
    vector<bool> posiblesPos(resultados.size());
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] > resultados[maxPos]){
            maxPos = i;
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
    resultado res;
    res.lineaMax = resultados[maxPos];
    res.resultados = posiblesPos;
    return res;
}
/*
void actualizarTablero(tablero& tab, int move, bool moveAliado){
    //verificar que la columna move no esta llena!
    if(moveAliado && tab.matrizFichas[move].size() < tab.m){
        tab.matrizFichas[move].push_back(fichaAliada);
    }
    else{
        if(tab.matrizFichas[move].size() < tab.m){
            tab.matrizFichas[move].push_back(fichaEnemiga);
        }
    }
}

bool hayFicha(tablero& tab, int columna, int fila){
    if(columna >= tab.n || fila >= tab.m) return false;
    if(columna < 0 || fila < 0) return false;
    return fila < tab.matrizFichas[columna].size();
}*/

bool hayFichaAliada(tablero& tab, int columna, int fila){
    if(columna >= tab.n || fila >= tab.m) return false;
    if(columna < 0 || fila < 0) return false;
    return fila < tab.matrizFichas[columna].size() && tab.matrizFichas[columna][fila] == fichaAliada;
}

bool hayFichaEnemiga(tablero& tab, int columna, int fila){
    if(columna >= tab.n || fila >= tab.m) return false;
    if(columna < 0 || fila < 0) return false;
    return fila < tab.matrizFichas[columna].size() && tab.matrizFichas[columna][fila] == fichaEnemiga;
}


int jugadaCasiRandom(tablero& tab, int c) {
    int moveParaGanar = unoParaGanar(tab, c);
    if(moveParaGanar >= 0){
        return moveParaGanar;
    }

    int moveParaPerder = unoParaPerder(tab, c);
    if (moveParaPerder >= 0) {
        return moveParaPerder;
    }
    uniform_int_distribution<int> do_move(0, tab.n - 1);

    do {
        move = do_move(generator);
    } while(columnaLlena(tab, tab.n));
            
    return move;
}