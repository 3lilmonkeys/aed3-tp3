#include <vector>

using namespace std;

class individuo {	
public:
	vector<int> parametros;
	float win_rate;
	float rapidez;

	void calcular_fitness();
	void crossover(individuo B);
	void mutar();
};

	individuo seleccionarPonderado(vector<individuo> poblacion);
	individuo seleccionarRandom(vector<individuo> poblacion);
	
	void poblacion_sort(vector<individuo> poblacion);

	individuo genetico(vector<individuo> poblacion);