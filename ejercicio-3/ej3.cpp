#include <random>


#define CANT_PARAM 10
#define TAM_POBLACION 100
#define PROB_MUTACION 0.01
#define LIMITE_PARAM 20


struct individuo {
	int parametros[CANT_PARAM];
	float win_rate;
	float rapidez;
};

individuo genetico(individuo poblacion0[]);
individuo seleccionarPonderado(individuo poblacion[]);
individuo seleccionarRandom(individuo poblacion[]);
individuo crossover(individuo A, individuo B);
individuo mutar(individuo A);


individuo genetico(individuo poblacion0[]) {
	for (int i = 0; i < TAM_POBLACION; i++)
	{
		//poblacion0[i].win_rate = fitness;
		//poblacion0[i].rapidez = fitness;
	}
	//merge_win_sort(poblacion0);
	//merge_rap_sort(poblacion0);
	individuo poblacionAux[TAM_POBLACION];

	while (poblacion0[0].win_rate < 0.9) {
		for (int i = 0; i < (TAM_POBLACION / 2); i++)
		{
			individuo indA = seleccionarRandom(poblacion0);
			individuo indB = seleccionarPonderado(poblacion0);
			indA = crossover(indA, indB);
			indB = crossover(indA, indB);
			indA = mutar(indA);
			indB = mutar(indB);
			//indA.win_rate = fitness;
			//indA.rapidez = fitness;
			//indB.win_rate = fitness;
			//indB.rapidez = fitness;
			poblacionAux[2 * i] = indA;
			poblacionAux[2 * i + 1] = indB;
		}
		
		poblacion0 = poblacionAux;

		//merge_win_sort(poblacion0);
		//merge_rap_sort(poblacion0);
	}

	return poblacion0[0];
}

individuo seleccionarPonderado(individuo poblacion[]) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(1, 100);

	while (true) {
		for (int i = 0; i < 100; i++)
		{
			int capacidad = (int) poblacion[i].win_rate * 100 + poblacion[i].rapidez* 10;
			if (dis(gen) < capacidad) {
				return poblacion[i];
			}
		}
	}
}

individuo seleccionarRandom(individuo poblacion[]) {
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

individuo crossover(individuo A, individuo B) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, A.win_rate + B.win_rate);

	individuo res; 
	for (int i = 0; i < CANT_PARAM; i++)
	{
		if (dis(gen) > A.win_rate) {
			res.parametros[i] = B.parametros[i];
		}
		else {
			res.parametros[i] = A.parametros[i];
		}
	}
	return res;
}

individuo mutar(individuo A) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 1);

	std::uniform_int_distribution<> param(0, LIMITE_PARAM);

	individuo res;
	for (int i = 0; i < CANT_PARAM; i++)
	{
		if (dis(gen) <= PROB_MUTACION) {
			res.parametros[i] = param(gen);
		}
		else {
			res.parametros[i] = A.parametros[i];
		}
	}
	return res;
}