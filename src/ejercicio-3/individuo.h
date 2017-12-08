#include <vector>
#include "../ejercicio-2/ej2.h"
#include "../ejercicio-2/auxiliar.h"
#include <set>
#include <random>
#include <algorithm>

#define gane 1;
#define perdi -1;
#define empate 0;
#define fichaAliada 10
#define fichaEnemiga 20

#define TAM_POBLACION 100
#define PROB_MUTACION 0.05
#define LIMITE_PARAM (11 + (4 - 2) * 2)
#define CANT_PARTIDOS 100
#define CANT_ESTR (11 + (4 - 2) * 2) //11 +(c-2)*2

#define cantParams 23
#define tamGrupos 6
//std::random_device rd;

/* Esto no se cual es el valor correcto */
#define ultimoParametroCoherente 20

using namespace std;

class individuo {
public:
	vector<int> parametros;
	float win_rate;
	float rapidez;

	void calcular_fitness(vector<individuo> oponentes);
	void crossover(individuo B);
	void mutar();
	void calcular_fitness_catedra();
};


individuo seleccionarPonderado(vector<individuo> poblacion);
individuo seleccionarRandom(vector<individuo> poblacion);
vector<individuo> inicializarOponentes();

void poblacion_sort(vector<individuo>& poblacion);

individuo genetico(vector<individuo> poblacion, vector<individuo> oponentes);

int jugadaCasiRandom(tablero tab, int col);






/* gridSearch */
individuo gridSearch_optimizado_v1(vector<individuo> oponentes);
individuo gridSearch_optimizado_v2(vector<individuo> oponentes);

individuo gridSearch_Exaustivo(vector<individuo> oponentes);

void calcularParametrosRecursivo(int indiceEnParametros,
	set<vector<int> > &conjuntoParametros, vector<int> vectorParametros);

void inicializarJugadorConParametrosRandom(individuo &jugador);
