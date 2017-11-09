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


		for (int i = 0; i < poblacion0[0].parametros.size(); i++)
		{
			cout << poblacion0[0].parametros[i];
		}
		cout << " " << poblacion0[0].win_rate << endl;



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



void individuo::calcular_fitness() {

	int filas = 6;
	int columnas = 7;
	int terminado = 0;
	int c = 4;

	list<estr> estrategias(CANT_ESTR);
	vector<bool> movsVacio(columnas);

	int i = 0;
	for (auto it = estrategias.begin(); it != estrategias.end(); it++) {
		it->estrategia = i;
		it->peso = parametros[i];		
		it->susMovs = movsVacio;
		i++;
	}


	for (int i = 0; i < CANT_PARTIDOS; i++) {
		tablero tab = crearTablero(columnas, filas);

		bool empiezo = true;

		while(!tableroLleno(tab) && terminado == 0) {
			if(empiezo) {
				actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR, columnas, c), true);
				actualizarTablero(tab, jugadaRandom(tab, columnas), false);
			}
			else {

				actualizarTablero(tab, jugadaRandom(tab, columnas), false);
				actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR, columnas, c), true);
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
	for (int i = 0; i < gen.parametros.size(); i++)
	{
		cout << gen.parametros[i];
	}
	cout << " " << gen.win_rate << endl;

	return 0;
}



int jugadaRandom(tablero tab, int col) {
	int jugada;
	uniform_int_distribution<int> do_move(0, col - 1);
	do {
    	jugada = do_move(gen);
	} while(hayFicha(tab, jugada, tab.m));

	return jugada;
}