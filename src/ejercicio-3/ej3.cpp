#include <random>
#include "individuo.h"
#include "../ejercicio-2/ej2.h"

#define TAM_POBLACION 100
#define PROB_MUTACION 0.05
#define LIMITE_PARAM (11 + (4 - 2)*2)
#define CANT_PARTIDOS 100
#define CANT_ESTR (11 + (4 - 2)*2) //11 +(c-2)*2

#define cantParams 23
#define tamGrupos 6
#include <algorithm>
//std::random_device rd;
std::mt19937 gen(rd());


void inicializarJugadorConParametrosRandom(individuo &jugador)
{
	vector<int> vectorParametros(cantParams);
	jugador.parametros = vectorParametros;
	jugador.win_rate = 0;
	for (int indice = 0; indice < jugador.parametros.size(); indice++)
		jugador.parametros[indice] = rand() % LIMITE_PARAM;
}

individuo gridSearch(vector<individuo> oponentes)
{
	int columns = 6;
	int fitnessAnterior = 0;
	vector<int> paramsAnterior(cantParams);
	
	individuo jugador;
	inicializarJugadorConParametrosRandom(jugador);
	int no_cambio;


	while (jugador.win_rate < 0.8)
	{
		//Selecciono grupos
		for (int i = 0; i <= tamGrupos; i++)
		{
			no_cambio = 0;
			while (no_cambio < 2)
			{
				// Seteo todos los parametros del grupo.
				for(int j = i; j < max(i + 6, cantParams); j++)
					jugador.parametros[j] = rand() % LIMITE_PARAM;

				jugador.calcular_fitness(oponentes);

				// Pregunto si mejoré en las ultimas iteraciones.
				if (fitnessAnterior < jugador.win_rate)
				{
					no_cambio = 0;
					fitnessAnterior = jugador.win_rate;
					paramsAnterior = jugador.parametros;
				}
				else
				{
					no_cambio++;
					jugador.parametros = paramsAnterior;
				}
			}
		}
	}
	return jugador;
}


individuo genetico(vector<individuo> poblacion0, vector<individuo> oponentesFijos) {

	int generacion = 0;
	for (int i = 0; i < TAM_POBLACION; i++)
	{
		cout << "calculando fitness " << i << endl;
		poblacion0[i].calcular_fitness(oponentesFijos);

		for (int j = 0; j < poblacion0[i].parametros.size(); j++)
		{
			cout << poblacion0[i].parametros[j] << " ";
		}
		cout << "   " << poblacion0[i].win_rate << " " << poblacion0[i].rapidez << endl;

	}
	generacion++;
	poblacion_sort(poblacion0);
	vector<individuo> poblacionAux;

	while (poblacion0[0].win_rate < 0.99 && generacion <= 75) {  

		cout << "Esta es la generacion " << generacion << endl;
		for (int i = 0; i < poblacion0[0].parametros.size(); i++)
		{
			cout << poblacion0[0].parametros[i] << " ";
		}
		cout << "   " << poblacion0[0].win_rate << " " << poblacion0[0].rapidez << endl;

		poblacionAux.clear(); //Para cuando los hacemos jugar con sus anteriores generaciones...??

		for (int i = 0; i < (TAM_POBLACION / 2); i++)
		{
			individuo indA = seleccionarRandom(poblacion0);
			individuo indB = seleccionarPonderado(poblacion0);
			
			individuo indAux = indA;

			indAux.crossover(indB);
			indB.crossover(indA);
			
			indA  = indAux;

			indA.mutar();
			indB.mutar();

			indA.calcular_fitness(poblacion0);
			indB.calcular_fitness(poblacion0);

			poblacionAux.push_back(indA);
			poblacionAux.push_back(indB);
		}

		generacion++;
				
		poblacion_sort(poblacionAux);

		poblacion0 = poblacionAux;
	}

	return poblacion0[0];
}

individuo seleccionarPonderado(vector<individuo> poblacion) {
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100);

	while (true) {
		for (int i = 0; i < TAM_POBLACION; i++)
		{
			int capacidad = (int) (poblacion[i].win_rate * 100) + (poblacion[i].rapidez * 10);
			if (dis(gen) < (capacidad / 10)) {
				return poblacion[i];
			}
		}
	}
}

individuo seleccionarRandom(vector<individuo> poblacion) {
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, TAM_POBLACION/2);
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
	std::uniform_real_distribution<> dis(0, 100);

	for (int i = 0; i < parametros.size(); i++)
	{
		if (dis(gen) > 50) {
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



void individuo::calcular_fitness(vector<individuo> oponentes) {

	int filas = 6;
	int columnas = 7;
	int c = 4;

	win_rate = 0;
	rapidez = 1;

	list<estr> estrategias(CANT_ESTR);

	int i = 0;
	for (auto it = estrategias.begin(); it != estrategias.end(); it++) {
		it->estrategia = i+1;
		it->peso = parametros[i];
		for (int j = 0; j < columnas; j++)
		{
			it->susMovs.resultados.push_back(0);
		}
		it->susMovs.lineaMax = 0;
		i++;
	}


	for (int i = 0; i < CANT_PARTIDOS; i++) {
		tablero tab = crearTablero(columnas, filas);
		int terminado = 0;
		bool empiezo = false;

		list<estr> estrategiasOponente(CANT_ESTR);

		if (i % 2 == 0) {
			empiezo = true;
		}

		int k = 0;
		for (auto it = estrategiasOponente.begin(); it != estrategiasOponente.end(); it++) {
			it->estrategia = k + 1;
			it->peso = oponentes[i].parametros[k];
			for (int j = 0; j < columnas; j++)
			{
				it->susMovs.resultados.push_back(0);
			}
			it->susMovs.lineaMax = 0;
			k++;
		}

		int jugadas = 0;

		while(!tableroLleno(tab) && terminado == 0) {
			if(empiezo) {
				actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR, columnas, c), true);
				jugadas++;
			}
			// 	actualizarTablero(tab, jugadaCasiRandom(tab, 4), false);
			actualizarTablero(tab, jugadaGolosa(tab, estrategiasOponente, 11, (c - 2) * 2, CANT_ESTR, columnas, c), false);
			actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR, columnas, c), true);

			jugadas++;
			empiezo = false;
			terminado = validarVictoria(tab, c);
		}

		if (terminado == 1) {
			win_rate += 0.01;
			rapidez -= ((float) jugadas)/(21*CANT_PARTIDOS);			//21 es el nuero de jugadas que puede hacer el jugador en un partido.
		}
	}
}


void individuo::calcular_fitness_catedra() {

	int filas = 6;
	int columnas = 7;
	int c = 4;

	win_rate = 0;
	rapidez = 1;

	list<estr> estrategias(CANT_ESTR);

	int i = 0;
	for (auto it = estrategias.begin(); it != estrategias.end(); it++) {
		it->estrategia = i+1;
		it->peso = parametros[i];
		for (int j = 0; j < columnas; j++)
		{
			it->susMovs.resultados.push_back(0);
		}
		it->susMovs.lineaMax = 0;
		i++;
	}


	for (int i = 0; i < 1000; i++) {
		tablero tab = crearTablero(columnas, filas);
		int terminado = 0;
		bool empiezo = false;

		if (i % 2 == 0) {
			empiezo = true;
		}

		int jugadas = 0;

		while(!tableroLleno(tab) && terminado == 0) {
			if(empiezo) {
				actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR, columnas, c), true);
				jugadas++;
			}
			actualizarTablero(tab, jugadaCasiRandom(tab, 7), false);
			//actualizarTablero(tab, jugadaGolosa(tab, estrategiasOponente, 11, (c - 2) * 2, CANT_ESTR, columnas, c), false);
			actualizarTablero(tab, jugadaGolosa(tab, estrategias, 11, (c-2)*2, CANT_ESTR, columnas, c), true);

			jugadas++;
			empiezo = false;
			terminado = validarVictoria(tab, c);
		}

		if (terminado == 1) {
			win_rate += 0.001;
			rapidez -= ((float) jugadas)/(21*CANT_PARTIDOS);			//21 es el nuero de jugadas que puede hacer el jugador en un partido.
		}
	}
}



void poblacion_sort(vector<individuo>& poblacion) {
	int n = poblacion.size();
	int j;
	individuo ind;
	//insertion sort por win_rate
	for (int i = 0; i < n; i++)
	{
		ind = poblacion[i];
		j = i - 1;

		while (j >= 0 && poblacion[j].win_rate < ind.win_rate)
		{
			poblacion[j + 1] = poblacion[j];
			j = j - 1;
		}
		poblacion[j + 1] = ind;
	}
}


int main() {

	int columns = 7;
	int rows = 6;
	tablero tab = crearTablero(columns, rows);
    list<estr> estrategias = inicializarEstrategias(11, 15, columns);

    vector<int> misEstrs(16);
    int k = 0;
    for (auto it = estrategias.begin(); it != estrategias.end(); it++)
    {
    	misEstrs[k] = it->peso;
    	k++;
    }

    individuo individuo_a_testear;
    individuo_a_testear.parametros = misEstrs;
    individuo_a_testear.win_rate = 0;
    individuo_a_testear.rapidez = 0;

    individuo_a_testear.calcular_fitness_catedra();

    cout << "El win_rate es: " << individuo_a_testear.win_rate << endl;

	// vector<individuo> poblacion0;

	// vector<individuo> oponentes;

	// uniform_int_distribution<int> rndStrat(0, LIMITE_PARAM);
	// int k = 1;
	// for (int i = 0; i < TAM_POBLACION; ++i)
	// {
	// 	individuo ind0;
	// 	individuo ind1;
	// 	for (int j = 0; j < CANT_ESTR; ++j)
	// 	{
	// 		if(j == k) ind1.parametros.push_back(15);
	// 		else ind1.parametros.push_back(0);
		
	// 		ind0.parametros.push_back(rndStrat(gen));
	// 	}
	// 	k = (k+1)%11;
	// 	oponentes.push_back(ind1);
	// 	poblacion0.push_back(ind0);
	// 	ind0.parametros.clear();
	// 	ind1.parametros.clear();
	// }

	// individuo gen = genetico(poblacion0, oponentes);
	// for (int i = 0; i < gen.parametros.size(); i++)
	// {
	// 	cout << gen.parametros[i] << " ";
	// }
	// cout << "   " << gen.win_rate << endl;

	return 0;
}



int jugadaCasiRandom(tablero tab, int col) {
	int moveParaGanar = unoParaGanar(tab, 4);
    if(moveParaGanar >= 0){
        return moveParaGanar;
    }

    int moveParaPerder = unoParaPerder(tab, 4);
    if (moveParaPerder >= 0) {
        return moveParaPerder;
    }

	int jugada;
	uniform_int_distribution<int> do_move(0, col - 1);
	do {
    	jugada = do_move(gen);
	} while(hayFicha(tab, jugada, tab.m));
	return jugada;
}
