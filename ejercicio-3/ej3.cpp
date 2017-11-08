#include <random>
#include "individuo.h"
#include "../ejercicio-2/ej2.h"

#define TAM_POBLACION 100
#define PROB_MUTACION 0.01
#define LIMITE_PARAM 100
#define CANT_PARTIDOS 100
#define CANT_ESTR (11 + (4 - 2)*2) //11 +(c-2)*2

std::random_device rd;
std::mt19937 gen(rd());

individuo genetico(vector<individuo> poblacion0) {
	for (int i = 0; i < TAM_POBLACION; i++)
	{
		poblacion0[i].calcular_fitness();		
	}
	poblacion_sort(poblacion0);
	vector<individuo> poblacionAux;

	while (poblacion0[0].win_rate < 0.9) {
		cout << poblacion0[0].parametros << " " << poblacion0[0].win_rate << endl;
		for (int i = 0; i < (TAM_POBLACION / 2); i++)
		{	
			individuo indA = seleccionarRandom(poblacion0);
			individuo indB = seleccionarPonderado(poblacion0);

			indA.crossover( indB);
			indB.crossover(indA);

			indA.mutar();
			indB.mutar();

			indA.calcular_fitness();
			indB.calcular_fitness();

			poblacionAux.push_back(indA);
			poblacionAux.push_back(indB);
		}
		
		poblacion0 = poblacionAux;

		poblacion_sort(poblacion0);
	}

	return poblacion0[0];
}

individuo seleccionarPonderado(vector<individuo> poblacion) {
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100);

	while (true) {
		for (int i = 0; i < 100; i++)
		{
			int capacidad = (int) poblacion[i].win_rate * 100 + poblacion[i].rapidez * 10;
			if (dis(gen) < (capacidad / 4)) {
				return poblacion[i];
			}
		}
	}
}

individuo seleccionarRandom(vector<individuo> poblacion) {
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, TAM_POBLACION/5);
	while (true) {
		for (int i = 0; i < 100; i++)
		{
			if (dis(gen) == 1) {
				return poblacion[i];
			}
		}
	}
}

void individuo::crossover(individuo B) {
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, win_rate + B.win_rate);

	for (int i = 0; i < parametros.size(); i++)
	{
		if (dis(gen) > win_rate) {
			parametros[i] = B.parametros[i];
		}

	}
}

void individuo::mutar() {
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	std::uniform_int_distribution<> param(0, LIMITE_PARAM);

	for (int i = 0; i < parametros.size(); i++)
	{
		if (dis(gen) <= PROB_MUTACION) {
			parametros[i] = param(gen);
		}
	}
}


jugadaGolosa()


void individuo::calcular_fitness() {

	int filas = 6;
	int columnas = 7;
	int terminado = 0;
	int c = 4;

	vector<estr> estrategias(CANT_ESTR);
	vector<bool> movsVacio(columnas);
	for (int i = 0; i < estrategias.size(); ++i)
	{
		estrategias[i].estrategia = i;
		estrategias[i].peso = parametros[i];
		estrategias[i].susMovs = movsVacio;
	}

    uniform_int_distribution<int> do_move(0, columnas - 1);

	for (int i = 0; i < CANT_PARTIDOS) {
		tablero tab = crearTablero(columnas, filas);

		bool empiezo = true;

		while(!tableroLleno(tab) && terminado == 0) {
			if(empiezo) {
				actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR, columnas, c), true);
				actualizarTablero(tab, jugadaRandom(tab, do_move), false);
			}
			else {

				actualizarTablero(tab, jugadaRandom(tab, do_move), false);
				actualizarTablero(tab, actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR), columnas, c), true);
			}

			empiezo = !empiezo;
			terminado = validarVictoria(tab, c);
		}

		if (terminado = 1) {
			win_rate += 0.01;
		}
		//else if (terminado = 0) {
		//	win_rate += 0.005;
		//}
	}

	rapidez = 0;
}


void poblacion_sort(vector<individuo> poblacion) {
	int n = poblacion.size();
	int j;
	individuo ind;
	//insertion sort por win_rate
	for (int i = 0; i < n; i++)
	{
		ind = poblacion[i];
		j = i - 1;
	
		while (j >= 0 && poblacion[j].win_rate > ind.win_rate)
		{
			poblacion[j + 1] = poblacion[j];
			j = j - 1;
		}
		poblacion[j + 1] = ind;
	}

	//falta sortear por rapidez
}


int main() {
	
	vector<individuo> poblacion0;

	uniform_int_distribution<int> rndStrat(0, LIMITE_PARAM);

	for (int i = 0; i < TAM_POBLACION; ++i)
	{
		individuo ind0;
		for (int j = 0; j < CANT_ESTR; ++j)
		{
			ind0.parametros[j] = rndStrat(gen);
		}

		poblacion0.push_back(ind0);
	}

	individuo gen = genetico(poblacion0);
	cout << gen.parametros << "   " << gen.win_rate << endl

	return 0;
}



int jugadaRandom(tablero tab) {
	int jugada;
	do {
    	jugada = do_move(gen);
	} while(hayFicha(tab, jugada, tab.m));

	return jugada;
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
					if (hayFicha(tab, j, i+k)) {
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
					if (hayFicha(tab, j + k, i)) {
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
					if (hayFicha(tab, j + k, i + k)) {
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
					if (hayFicha(tab, j + k, i - k)) {
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


bool tableroLleno(tablero& tab){
    for (int i = 0; i < tab.n; ++i)
    {
        if(tab.matrizFichas[i].size() < tab.m){
            return false;
        }
    }
    return true;
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
	return columna < tab.matrizFichas[fila].size();
}