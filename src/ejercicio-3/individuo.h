#include <vector>
#include "../ejercicio-2/auxiliar.h"

#define gane 1;
#define perdi -1;
#define empate 0;
#define fichaAliada 10
#define fichaEnemiga 20

using namespace std;

class individuo {
public:
	vector<int> parametros;
	float win_rate;
	float rapidez;

	void calcular_fitness(vector<individuo> oponentes);
	void crossover(individuo B);
	void mutar();
};


individuo seleccionarPonderado(vector<individuo> poblacion);
individuo seleccionarRandom(vector<individuo> poblacion);

void poblacion_sort(vector<individuo>& poblacion);

individuo genetico(vector<individuo> poblacion);

int jugadaRandom(tablero tab, int col);
