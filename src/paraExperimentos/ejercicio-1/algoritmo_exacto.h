#include <iostream>
#include <fstream>
#include <cassert>
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
#define MAXIMIZAR true
#define MINIMIZAR false

using namespace std;

struct tablero{
    vector<vector<int> > matrizFichas;
    int n;//columnas
    int m;//filas
};


int mejor_jugada(vector<int>& jugadas);
int fichas_en_columna(tablero& tablero, int ultimo_movimiento);
bool validar_jugada(tablero& tablero, int c_linea, int ultima_ficha);
bool complete_fila(tablero& tablero, int c_linea, int ultima_ficha);
bool complete_columna(tablero& tablero, int c_linea, int ultima_ficha);
bool complete_diagonal_45(tablero& tablero, int c_linea, int ultima_ficha);
bool complete_diagonal_135(tablero& tablero, int c_linea, int ultima_ficha);

int ultimo_jugador(tablero& tablero, int ultimo_movimiento);
int jugar_recursivo(tablero& tab, int c_linea, int cant_fichas, int ultimo_movimiento,
    bool maximizo_o_minimizo);
bool columna_llena(tablero&, int);
tablero crearTablero(int n, int m);
int calcularJugada(tablero& tab, int c, int p);
void actualizarTablero(tablero& tab, int move, bool moveAliado);
int minRes(vector<int> resultados);
int maxRes(vector<int> resultados);
bool tableroLleno(tablero& tab);
bool hayFicha(tablero& tab, int columna, int fila);
void send(const string& msg);
void send(int msg);
int read_int();
string read_str();


/* Auxiliares */

bool test_validar_columna();
bool test_validar_fila();
bool test_validar_diagonal_45();
bool test_validar_diagonal_135();
