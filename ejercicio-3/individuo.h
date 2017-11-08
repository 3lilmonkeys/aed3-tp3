#include <vector>

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

	void calcular_fitness();
	void crossover(individuo B);
	void mutar();
};


struct tablero{
    vector<vector<int>> matrizFichas;
    int n;//columnas
    int m;//filas
} tablero1;



tablero crearTablero(int n, int m){
    tablero tab;
    tab.m = m;
    tab.n = n;
    for(int i = 0; i < n; i++){
        vector<int> filas;
        tab.matrizFichas.push_back(filas);
    }
    return tab;
}


	individuo seleccionarPonderado(vector<individuo> poblacion);
	individuo seleccionarRandom(vector<individuo> poblacion);
	
	void poblacion_sort(vector<individuo> poblacion);

	individuo genetico(vector<individuo> poblacion);

	int jugadaRandom(tablero tab);

	int validarVictoria(tablero& tab, int c);
	void actualizarTablero(tablero& tab, int move, bool moveAliado);
	bool tableroLleno(tablero& tab);
	bool hayFicha(tablero& tab, int columna, int fila);