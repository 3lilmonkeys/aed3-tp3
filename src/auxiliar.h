#pragma once 

#include <vector>
#include <iostream>

#define gane 1;
#define perdi -1;
#define empate 0;
#define fichaAliada 10
#define fichaEnemiga 20

using namespace std;

struct tablero {
	vector< vector<int> > matrizFichas;
	int n;//columnas
	int m;//filas
} tablero1;


tablero crearTablero(int n, int m) {
	tablero tab;
	tab.m = m;
	tab.n = n;
	for (int i = 0; i < n; i++) {
		vector<int> filas;
		tab.matrizFichas.push_back(filas);
	}
	return tab;
}


void actualizarTablero(tablero& tab, int move, bool moveAliado);
int validarVictoria(tablero& tab, int p);
bool tableroLleno(tablero& tab);
bool hayFicha(tablero& tab, int columna, int fila);



bool tableroLleno(tablero& tab) {
	for (int i = 0; i < tab.n; ++i)
	{
		if (tab.matrizFichas[i].size() < tab.m) {
			return false;
		}
	}
	return true;
}



int validarVictoria(tablero& tab, int c) {
	for (int i = 0; i < tab.n; ++i)             //i recorre columnas
	{
		for (int j = 0; j < tab.m; ++j)         //j recorre filas
		{
			if (i <= tab.n - c) {//chequea si hay ganador en la fila de p fichas  empezando de (i,k)
				bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, j, i + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i + k][j])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}
				if (hayGanador) {
					if (tab.matrizFichas[i][j] == fichaAliada) { return 1; }
					else { return -1; }
				}
			}
			if (j <= tab.m - c) {//chequea si hay ganador en la columna de p fichas  empezando de (i,j)
				bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, j + k, i)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}

				if (hayGanador) {
					if (tab.matrizFichas[i][j] == fichaAliada) { return 1; }
					else { return -1; }
				}
			}
			if ((i <= tab.n - c) && (j <= tab.m - c)) {//chequea si hay ganador en la diagonal hacia la derecha de p fichas empezando de (i,k)
				bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, j + k, i + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i + k][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}
				if (hayGanador) {
					if (tab.matrizFichas[i][j] == fichaAliada) { return 1; }
					else { return -1; }
				}
			}
			if ((i >= c - 1) && (j <= tab.m - c)) {//chequea si hay ganador en la diagonal hacia la izquierda de p fichas empezando de (i,k)
				bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, j + k, i - k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i - k][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}
				if (hayGanador) {
					if (tab.matrizFichas[i][j] == fichaAliada) { return 1; }
					else { return -1; }
				}
			}
		}
	}
	return 0;

}




void actualizarTablero(tablero& tab, int move, bool moveAliado) {
	if (moveAliado) {
		tab.matrizFichas[move].push_back(fichaAliada);
	}
	else {
		tab.matrizFichas[move].push_back(fichaEnemiga);
	}
}


bool hayFicha(tablero& tab, int columna, int fila) {
	return columna < tab.matrizFichas[fila].size();
}