#include "../ejercicio-2/ej2.h"
#include <set>

#define TAM_POBLACION 100
#define PROB_MUTACION 0.05
#define LIMITE_PARAM (11 + (4 - 2) * 2)
#define CANT_PARTIDOS 100
#define CANT_ESTR (11 + (4 - 2) * 2) //11 +(c-2)*2
#define cantParams 23
#define tamGrupos 6
#define ultimoParametroCoherente 15

std::random_device rd;
std::mt19937 generator(rd());
std::mt19937 gen(rd());
list<estr> cargarIndividuo();

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
individuo genetico(vector<individuo> poblacion0, vector<individuo> oponentes);
int jugadaCasiRandom(tablero tab, int col);

/* gridSearch */
void mostrarVector(vector<int> vec);
void inicializarJugadorConParametrosRandomDeNuevoPunto(individuo &jugador, int punto);
individuo gridSearch_optimizado_v1(vector<individuo> oponentes);
individuo gridSearch_optimizado_v2(vector<individuo> oponentes);
individuo gridSearch_Exaustivo(vector<individuo> oponentes);
void calcularParametrosRecursivo(int indiceEnParametros,
	set<vector<int> > &conjuntoParametros, vector<int> vectorParametros);
void inicializarJugadorConParametrosRandom(individuo &jugador);
int aumentarParametroHastaQueNoMejoreMas(individuo &jugador,
	int indiceEnParametros, vector<individuo> &oponentes, float &fitnessMasAlto,
	vector<int> &mejoresParametros, float &fitnessAumentado);
int disminuirParametroHastaQueNoMejoreMas(individuo &jugador,
	int indiceEnParametros, vector<individuo> &oponentes, float &fitnessMasAlto,
	vector<int> &mejoresParametros, float &fitnessReducido);
int mejorParametroDeEstaIteracion(int parametroAumentado, int parametroReducido,
	float fitnessAumentado, float fitnessReducido);