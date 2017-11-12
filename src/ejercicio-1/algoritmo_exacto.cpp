#include "algoritmo_exacto.h"


#ifndef header_algoritmo_exacto
#define header_algoritmo_exacto


int calcularJugada(tablero& tab, int columnas, int c, int p){
    vector<int> posiblesResultados(tab.n);
    vector<tablero> opcionesTablero(tab.n);
    for(int i = 0; i < columnas; i++){
        opcionesTablero[i] = tab;
        actualizarTablero(opcionesTablero[i], i, true);
        if(opcionesTablero[i].matrizFichas[i].size() > tab.m){
            posiblesResultados[i] = -10;
            // le asginamos un -10 ya que no quiero tener en cuenta ese movimiento.
        }
        else{
            posiblesResultados[i] = evaluarJugada(opcionesTablero[i], columnas, false, c, p - 1);
            // esta funcion se encargaria de maximizar, por lo tanto llamo a evaluar jugada con MINIMIZAR.
            if(posiblesResultados[i] == 1){
                return i;
            }
        }

    }
    return posMax(posiblesResultados);
}

int evaluarJugada(tablero& tab, int columnas, bool maximizar, int c, int p){
    int resultado = validarVictoria(tab, c);
    if(resultado == 1){
        return 1;
    }
    else if(resultado == -1){
        return -1;
    }
    else{
        if(tableroLleno(tab)){
            return 0;
        }
        else{
            if(maximizar){
                if(p == 0) return 0;
                vector<tablero> opcionesTablero(tab.n);
                vector<int> posiblesResultados(tab.n);
                for(int i = 0; i < tab.n; i++){
                    if(tab.matrizFichas[i].size() < tab.m){
                        opcionesTablero[i] = tab; //creo la cantidad de tableros necesarios
                        opcionesTablero[i].matrizFichas[i].push_back((fichaAliada));

                        int resultado = evaluarJugada(opcionesTablero[i], columnas, !maximizar, c, p - 1);
                        posiblesResultados[i] = resultado;
                        if(resultado == 1){ //parte alfabeta: si la rama que acabo de calcular ya me da 1 lo devuelvo asi no calculo los siguientes
                            return 1;
                        }
                    }
                }
                return maxRes(posiblesResultados);//devolver el maximo de cada posibilidad
            }
            else{//minimizar
                vector<tablero> opcionesTablero(tab.n);
                vector<int> posiblesResultados(tab.n);
                for(int i = 0; i < tab.n; i++){
                    if(tab.matrizFichas[i].size() < tab.m){
                        opcionesTablero[i] = tab; //creo la cantidad de tableros necesarios
                        opcionesTablero[i].matrizFichas[i].push_back((fichaEnemiga));
                        int resultado = evaluarJugada(opcionesTablero[i], columnas, !maximizar, c, p);
                        posiblesResultados[i] = resultado;
                        if(resultado == -1){ //parte alfabeta
                            return -1;
                        }
                    }
                }
                return minRes(posiblesResultados);//devolver el minimo de cada posibilidad
            }
        }
    }
}

int validarVictoria(tablero& tab, int c){
    for (int i = 0; i < tab.n; ++i)             //i recorre columnas
    {
        for (int j = 0; j < tab.m; ++j)         //j recorre filas
        {
            if(i <= tab.n - c){//chequea si hay ganador en la fila de p fichas  empezando de (i,k)
                bool hayGanador = true;
                for (int k = 0; k < c; ++k)
                {
					if (hayFicha(tab, i + k, j)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i + k][j])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
                }
            }
            if(j <= tab.m - c){//chequea si hay ganador en la columna de p fichas  empezando de (i,j)
                bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, i, j + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}

                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
                }
            }
            if((i <= tab.n - c)&&(j <= tab.m - c)){//chequea si hay ganador en la diagonal hacia la derecha de p fichas empezando de (i,k)
                bool hayGanador = true;
				for (int k = 0; k < c; ++k)
				{
					if (hayFicha(tab, i + k, j + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i + k][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
				}
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
                }
            }
            if((i >= c - 1)&&(j <= tab.m - c)){//chequea si hay ganador en la diagonal hacia la izquierda de p fichas empezando de (i,k)
                bool hayGanador = true;
                for (int k = 0; k < c; ++k)
                {
					if (hayFicha(tab, i - k, j + k)) {
						if (tab.matrizFichas[i][j] != tab.matrizFichas[i - k][j + k])
						{
							hayGanador = false;
							break;
						}
					}
					else { hayGanador = false;  break; }
                }
                if (hayGanador){
                    if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
                }
            }
        }
    }
    return 0;
}


/*                          Auxiliares                    */

/* Creo un nuevo tablero. Este es un vector de vectores conde el tablero ser lo
representa vector[i] es una columna y las jugadas se agregan haciendo push_back
en las columnas para que esta se ubique arriba de la columna. */
tablero crearTablero(int n, int m){
    tablero tab;
    tab.m = m;
    tab.n = n;
    for(int i = 0; i < n; i++){
        vector<int> columnas;
        tab.matrizFichas.push_back(columnas);
    }
    return tab;
}

/* Consulto si queda lugar en el tablero. */
bool tableroLleno(tablero& tab){
    for (int i = 0; i < tab.n; ++i)
    {
        if(tab.matrizFichas[i].size() < tab.m){
            return false;
        }
    }
    return true;
}

/* Saco el mínimo sobre las jugadas en un nivel. */
int minRes(vector<int> resultados){
    int minRes = resultados[0];
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] < minRes){
            minRes = resultados[i];
        }
    }
    return minRes;
}

/* Saco el máximo sobre las jugadas en un nivel. */
int maxRes(vector<int> resultados){
    int maxRes = resultados[0];
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] > maxRes){
            maxRes = resultados[i];
        }
    }
    return maxRes;
}

/* Obtengo el índice del máximo en el vector. */
int posMax(vector<int> resultados){
    int maxPos = 0;
    for(int i = 0; i < resultados.size(); i++){
        if(resultados[i] > resultados[maxPos]){
            maxPos = i;
        }
    }
    return maxPos;
}

/* Actualizo el tablero con la ultima jugada realizada. */
void actualizarTablero(tablero& tab, int move, bool moveAliado){
    if(moveAliado){
        tab.matrizFichas[move].push_back(fichaAliada);
    }
    else{
        tab.matrizFichas[move].push_back(fichaEnemiga);
    }
}

/* Consulto si hay una ficha en la fila y columna o superior. */
bool hayFicha(tablero& tab, int columna, int fila) {
	return fila < tab.matrizFichas[columna].size();
}


void send(const string& msg) {
    cout << msg << endl;
}

void send(int msg) {
    cout << msg << endl;
}

int read_int() {
    string msg;
    cin >> msg;
    if (msg == "salir") {
        send("listo");
        exit(0);
    }
    return stoi(msg);
}

string read_str() {
    string msg;
    cin >> msg;
    if (msg == "salir") {
        send("listo");
        exit(0);
    }
    return msg;
}


#endif
