#include <random>
#include "individuo.h"

#define TAM_POBLACION 100
#define PROB_MUTACION 0.01
#define LIMITE_PARAM 20

individuo genetico(vector<individuo> poblacion0) {
	for (int i = 0; i < TAM_POBLACION; i++)
	{
		poblacion0[i].calcular_fitness();		
	}
	poblacion_sort(poblacion0);
	vector<individuo> poblacionAux;

	while (poblacion0[0].win_rate < 0.9) {
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
	std::random_device rd;
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
	std::random_device rd;
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
	std::random_device rd;
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
	std::random_device rd;
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
	//hacer

}


void poblacion_sort(vector<individuo> poblacion) {
	//hacer

}




int main() {
	//hacer


	return 0;
}