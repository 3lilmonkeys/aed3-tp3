#include "individuo.h"

std::mt19937 gen(rd());


/*-----------------------GRID SEARCH---------------------------------------*/

individuo gridSearch_optimizado_v1(vector<individuo> oponentes)
{
	int columns = 6;
	float fitnessAnterior = 0;
	vector<int> paramsAnterior(cantParams);

	individuo jugador;
	inicializarJugadorConParametrosRandom(jugador);
	int cambio;

	while (fitnessAnterior < 0.99)
	{
		//Selecciono grupos
		for (int i = 0; i <= tamGrupos; i = i + 6)
		{
			cambio = 0;
			while (cambio < 2)
			{
				// cout << "Nueva iteración" << endl;
				// Seteo todos los parametros del grupo.
				for (int j = i; j < max(i + 6, cantParams); j++)
					jugador.parametros[j] = rand() % LIMITE_PARAM;

				jugador.calcular_fitness(oponentes);
				cout << "fitness actual : " << jugador.win_rate << endl;
				// Pregunto si mejoré en las ultimas iteraciones.
				if (fitnessAnterior < jugador.win_rate)
				{
					// cout << "if"<<endl;
					cambio = 0;
					fitnessAnterior = jugador.win_rate;
					paramsAnterior = jugador.parametros;
				}
				else
				{
					// cout << "else" << endl;
					cambio++;
					jugador.parametros = paramsAnterior;
				}
			}
			// cout << "Otra iteración" << endl;
		}
		cout << "winrate actual : " << jugador.win_rate << endl;
	}
	jugador.win_rate = fitnessAnterior;
	jugador.parametros = paramsAnterior;
	return jugador;
}

void inicializarJugadorConParametrosRandom(individuo &jugador)
{
	vector<int> vectorParametros(cantParams);
	jugador.parametros = vectorParametros;
	jugador.win_rate = 0;
	for (int indice = 0; indice < jugador.parametros.size(); indice++)
		jugador.parametros[indice] = rand() % LIMITE_PARAM;
}

/* Es demasiado grande el espacio de busqueda del exaustivo */

// individuo gridSearch_Exaustivo(vector<individuo> oponentes)
// {
// 	int columns = 6;
// 	float fitnessAnterior = 0;

// 	set<vector<int> > parametrosExaustivosSinRepetidos;

// 	individuo jugador;
// 	inicializarJugadorConParametrosRandom(jugador);
	
// 	cout<< "Comienzo parametros recursivo"<<endl;

// 	vector<int> parametros = jugador.parametros;
// 	calcularParametrosRecursivo(0, parametrosExaustivosSinRepetidos, parametros);

// 	float maximoFitness = 0;
// 	individuo jugadorTemporal;

// 	cout << "Comienzo a tomar tiempos" << endl;

// 	float fitnessTemporal = 0;
// 	for (set<vector<int> >::iterator iter = parametrosExaustivosSinRepetidos.begin();
// 		iter != parametrosExaustivosSinRepetidos.end(); ++iter)
// 	{
// 		jugadorTemporal.parametros = (*iter);
// 		jugador.calcular_fitness(oponentes);
// 		fitnessTemporal = jugador.win_rate;

// 		cout << "Fitness actual : " << fitnessTemporal << endl;

// 		if( maximoFitness < fitnessTemporal )
// 			jugador.parametros = jugadorTemporal.parametros;
// 	}

// 	return jugador;
// }


// void calcularParametrosRecursivo(int indiceEnParametros, set<vector<int> > &conjunto,
// 	vector<int> jugador)
// {
// 	if (indiceEnParametros == jugador.size())
// 		return;

// 	for (int indice = 0; indice < ultimoParametroCoherente; indice++)
// 	{
// 		jugador.at(indiceEnParametros) = indice;
// 		if (indiceEnParametros == jugador.size() - 1)
// 			conjunto.insert(jugador);

// 		cout << "Cantidad de vectores : " <<  conjunto.size() << endl;

// 		calcularParametrosRecursivo(indiceEnParametros + 1, conjunto, jugador);
// 	}
// 	return;
// }

individuo gridSearch_optimizado_v2(vector<individuo> oponentes)
{	
	int fitnessTemporal = 0;
	vector<int> parametros;
	individuo jugador;
	
	float mejorFitness = 0;
	vector<int> mejoresParametros(cantParams);

	int noMejorePorXIntentos;
	int valorOriginal;
	int valorMenor;
	float fitnessMenor;
	int valorMayor;
	float fitnessMayor;
	
	for(int cantPuntosAleatorios = 0; cantPuntosAleatorios < 10; cantPuntosAleatorios ++)
	{	
		cout << "Estoy en el punto : " << cantPuntosAleatorios << endl;
		inicializarJugadorConParametrosRandom(jugador);
		for(int indiceEnParametros = 0; indiceEnParametros < jugador.parametros.size(); indiceEnParametros++)
		{
			cout<< "Cambio de indice por : " << indiceEnParametros << endl;
			noMejorePorXIntentos = 0;
			while( noMejorePorXIntentos < 3 )
			{
				cout << "no mejore por cantidad de intentos : " << noMejorePorXIntentos << endl;
				valorOriginal = jugador.parametros.at(indiceEnParametros);

				valorMayor = valorOriginal+1;
				jugador.parametros.at(indiceEnParametros) = valorMenor;
				jugador.calcular_fitness(oponentes);
				fitnessMenor = jugador.win_rate;
				cout << "El fitness menor es : " << fitnessMenor << endl;
				if (mejorFitness < fitnessMenor)
				{
					mejorFitness = fitnessMenor;
					parametros = jugador.parametros;
					noMejorePorXIntentos = 0;
					continue;
				}

				valorMenor = valorOriginal-1;
				jugador.parametros.at(indiceEnParametros) = valorMayor;
				jugador.calcular_fitness(oponentes);
				fitnessMayor = jugador.win_rate;
				cout << "El fitness menor es : " << fitnessMayor << endl;
				if(mejorFitness < fitnessMayor){
					mejorFitness = fitnessMayor;
					parametros = jugador.parametros;
					noMejorePorXIntentos = 0;
					continue;
				}

				jugador.parametros.at(indiceEnParametros) = valorOriginal;
				noMejorePorXIntentos++;
				
			}
		}
	}
	jugador.parametros = parametros;
	return jugador;
}

/*-------------------------------------------FIN GRID SEARCH----------------*/

























/*--------------------------------------GENETICO-----------------------------*/
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

int jugadaCasiRandom(tablero tab, int col)
{
	int moveParaGanar = unoParaGanar(tab, 4);
	if (moveParaGanar >= 0)
	{
		return moveParaGanar;
	}

	int moveParaPerder = unoParaPerder(tab, 4);
	if (moveParaPerder >= 0)
	{
		return moveParaPerder;
	}

	int jugada;
	uniform_int_distribution<int> do_move(0, col - 1);
	do
	{
		jugada = do_move(gen);
	} while (hayFicha(tab, jugada, tab.m));
	return jugada;
}

















/*-------------------------------------------MAIN------------------------------*/

int main() {

	int columns = 7;
	int rows = 6;
	tablero tab = crearTablero(columns, rows);
    list<estr> estrategias = inicializarEstrategias(11, 15, columns);

    // vector<int> misEstrs(16);
    // int k = 0;
    // for (auto it = estrategias.begin(); it != estrategias.end(); it++)
    // {
    // 	misEstrs[k] = it->peso;
    // 	k++;
    // }

    // individuo individuo_a_testear;
    // individuo_a_testear.parametros = misEstrs;
    // individuo_a_testear.win_rate = 0;
    // individuo_a_testear.rapidez = 0;

    // individuo_a_testear.calcular_fitness_catedra();

    // cout << "El win_rate es: " << individuo_a_testear.win_rate << endl;

	vector<individuo> poblacion0;

	vector<individuo> oponentes;

	uniform_int_distribution<int> rndStrat(0, LIMITE_PARAM);
	int k = 1;
	for (int i = 0; i < TAM_POBLACION; ++i)
	{
		individuo ind0;
		individuo ind1;
		for (int j = 0; j < CANT_ESTR; ++j)
		{
			if(j == k) ind1.parametros.push_back(15);
			else ind1.parametros.push_back(0);
		
			ind0.parametros.push_back(rndStrat(gen));
		}
		k = (k+1)%11;
		oponentes.push_back(ind1);
		poblacion0.push_back(ind0);
		ind0.parametros.clear();
		ind1.parametros.clear();
	}

	// individuo gen = genetico(poblacion0, oponentes);
	// for (int i = 0; i < gen.parametros.size(); i++)
	// {
	// 	cout << gen.parametros[i] << " ";
	// }
	// cout << "   " << gen.win_rate << endl;

	cout << "Comienza grid" << endl;

	// individuo jugador = gridSearch_optimizado_v1(oponentes);
	individuo jugador = gridSearch_optimizado_v2(oponentes);
	cout << jugador.win_rate << endl;

	return 0;
}


