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
    vector<vector<int> > matrizFichas;
    int n;//columnas
    int m;//filas
};

tablero crearTablero(int n, int m);
int calcularJugada(tablero& tab, int columnas, int c, int p);
void actualizarTablero(tablero& tab, int move, bool moveAliado);
int minRes(vector<int> resultados);
int maxRes(vector<int> resultados);
int posMax(vector<int> resultados);
int validarVictoria(tablero& tab, int p);
bool tableroLleno(tablero& tab);
int evaluarJugada(tablero& tab, int columnas, bool maximizar, int c, int p);
bool hayFicha(tablero& tab, int columna, int fila);
void send(const string& msg);
void send(int msg);
int read_int();
string read_str();
