#include "algoritmo_exacto.h"

#ifndef header_algoritmo_exacto
#define header_algoritmo_exacto

int calcularJugada(tablero& tab, int c_linea, int cant_fichas){
  vector<int> posibles_jugadas(tab.n);
  vector<tablero> posibles_tableros(tab.n);
  /* Comienzo la recursión por todas las "ramas". En este caso son una por
  columna. Me guardo un tablero para cada rama. La raiz toma el maximo, por lo
  que el de imediatamente abajo toma minimo. */
  for(int i = 0; i < tab.n; i++){
    posibles_jugadas[i] = 0;
    // Si la columna no está llena.
    if(tab.matrizFichas.at(i).size() < tab.m){
      posibles_tableros[i] = tab;
      actualizarTablero(posibles_tableros[i], i, MAXIMIZAR);
      bool jugada_ganadora = validar_jugada(posibles_tableros[i], c_linea, i);
      if(jugada_ganadora) return i;
      posibles_jugadas[i] = minimizar(posibles_tableros[i],c_linea,
      cant_fichas - 1, i);
      if(posibles_jugadas[i] == 1) return i;
    }
  }
  // Devuelvo la columna donde quiero jugar.
  return mejor_jugada(posibles_jugadas);
}

int maximizar(tablero& tab, int c_linea, int cant_fichas, int ultimo_movimiento)
{
  if(cant_fichas == 0) return 0;
  vector<tablero> opcionesTablero(tab.n);
  vector<int> posiblesResultados(tab.n);

  for(int i = 0; i < tab.n; i++){
    posiblesResultados[i] = 0;
    // Si la columna no está llena.
    if(tab.matrizFichas[i].size() < tab.m){
      opcionesTablero[i] = tab;
      actualizarTablero(opcionesTablero[i], i, MAXIMIZAR);
      bool jugada_ganadora = validar_jugada(opcionesTablero[i], c_linea, i);
      if(jugada_ganadora) return 1;
      if( tableroLleno(opcionesTablero[i]) ) return 0;

      posiblesResultados[i] = minimizar(opcionesTablero[i],c_linea,cant_fichas-1,i);
      if(posiblesResultados[i] == 1) return 1;
    }
  }
  // Devolver el maximo de cada posibilidad.
  return maxRes(posiblesResultados);
}

int minimizar(tablero& tab, int c_linea, int cant_fichas, int ultimo_movimiento)
{
  if(cant_fichas == 0) return 0;
  vector<tablero> opcionesTablero(tab.n);
  vector<int> posiblesResultados(tab.n);
  for(int i = 0; i < tab.n; i++){
    posiblesResultados[i] = 0;
    // Si la columna no está llena.
    if(tab.matrizFichas[i].size() < tab.m){
      opcionesTablero[i] = tab;
      actualizarTablero(opcionesTablero[i], i, MINIMIZAR);
      bool jugada_ganadora = validar_jugada(opcionesTablero[i], c_linea, i);
      if(jugada_ganadora) return 1;
      if( tableroLleno(opcionesTablero[i]) ) return 0;

      posiblesResultados[i] = maximizar(opcionesTablero[i],c_linea,cant_fichas-1,i);
      if(posiblesResultados[i] == -1) return -1;
    }
  }
  // Devolver el minimo de cada posibilidad.
  return minRes(posiblesResultados);
}

/* Como parametros le paso, el tablero, la cantidad necesaria para ganar y la
columna donde se puso la ultima ficha en el tablero. */
bool validar_jugada(tablero& tablero, int c_linea, int ultima_jugada)
{
  /* true igual gane, false igual no gane. */
  bool respuesta = false;
  if( complete_fila(tablero, c_linea, ultima_jugada) )
    respuesta = true;
  else if( complete_columna(tablero, c_linea, ultima_jugada) )
    respuesta = true;
  else if( complete_diagonal_45(tablero, c_linea, ultima_jugada) )
    respuesta = true;
  else if( complete_diagonal_135(tablero, c_linea, ultima_jugada) )
    respuesta= true;

  return respuesta;
}

bool complete_columna(tablero& tablero, int c_linea, int ultimo_movimiento)
{
  int cant_fichas = 1;
  int jugador_actual = ultimo_jugador(tablero, ultimo_movimiento);
  int cant_fichas_col = fichas_en_columna(tablero, ultimo_movimiento);

  int indice_columna =cant_fichas_col-1;
  while( (indice_columna >= 0) and (jugador_actual==
  tablero.matrizFichas.at(ultimo_movimiento).at(indice_columna)) )
  {
    cant_fichas++;
    indice_columna--;
  }
  return (cant_fichas >= c_linea);
}

bool complete_fila(tablero& tablero, int c_linea, int ultimo_movimiento)
{
  int cant_fichas= 1;

  int cant_fichas_col = fichas_en_columna(tablero, ultimo_movimiento);
  int jugador_actual = ultimo_jugador(tablero, ultimo_movimiento);

  /* Sumo a la izquierda */
  int indice_fila = ultimo_movimiento-1;
  while( (0 <= indice_fila) and (cant_fichas_col <=
  fichas_en_columna(tablero, indice_fila)) and
  (tablero.matrizFichas.at(indice_fila).at(cant_fichas_col)==jugador_actual) )
  {
    cant_fichas++;
    indice_fila--;
  }
  /* Sumo a la derecha */
  indice_fila = ultimo_movimiento+1;
  while( (indice_fila < tablero.n) and (cant_fichas_col <=
  fichas_en_columna(tablero, indice_fila)) and
  (tablero.matrizFichas.at(indice_fila).at(cant_fichas_col)==jugador_actual))
  {
    cant_fichas++;
    indice_fila++;
  }
  return (cant_fichas >= c_linea);
}

bool complete_diagonal_45(tablero& tablero, int c_linea, int ultimo_movimiento)
{
  int cant_fichas= 1;
  int jugador_actual = ultimo_jugador(tablero, ultimo_movimiento);
  int cant_fichas_col = fichas_en_columna(tablero, ultimo_movimiento);

  /* Sumo a izquierda */
  int indice_x = ultimo_movimiento-1;
  int indice_y = cant_fichas_col+1;
  while( (0<=indice_x) and (indice_y < tablero.m) and
  (indice_y < tablero.matrizFichas.at(indice_x).size()) and
  (jugador_actual == tablero.matrizFichas.at(indice_x).at(indice_y)) )
  {
    cant_fichas++;
    indice_y++;
    indice_x--;
  }
  /* Sumo a derecha */
  indice_x = ultimo_movimiento +1;
  indice_y = cant_fichas_col-1;
  while( (indice_x < tablero.n) and (0 <= indice_y) and
  (indice_y < tablero.matrizFichas.at(indice_x).size()) and
  (jugador_actual == tablero.matrizFichas.at(indice_x).at(indice_y)) )
  {
    cant_fichas++;
    indice_y--;
    indice_x++;
  }
  return (cant_fichas >= c_linea);
}

bool complete_diagonal_135(tablero& tablero, int c_linea, int ultimo_movimiento)
{
  int cant_fichas= 1;
  int jugador_actual = ultimo_jugador(tablero, ultimo_movimiento);
  int cant_fichas_col = fichas_en_columna(tablero, ultimo_movimiento);

  /* Sumo a derecha */
  int indice_x = ultimo_movimiento+1;
  int indice_y = cant_fichas_col+1;
  while( (indice_x < tablero.n) and (indice_y< tablero.m) and
  (indice_y < tablero.matrizFichas.at(indice_x).size()) and
  (jugador_actual == tablero.matrizFichas.at(indice_x).at(indice_y)) )
  {
    cant_fichas++;
    indice_y++;
    indice_x++;
  }
  /* Sumo a izquierda */
  indice_x = ultimo_movimiento -1;
  indice_y = cant_fichas_col-1;
  while( (indice_x >= 0) and (indice_y >= 0) and
  (indice_y < tablero.matrizFichas.at(indice_x).size()) and
  (jugador_actual == tablero.matrizFichas.at(indice_x).at(indice_y)) )
  {
    cant_fichas++;
    indice_y--;
    indice_x--;
  }
  return (cant_fichas >= c_linea);
}

/*------------------------------------Auxiliares----------------------------- */

/* Creo un nuevo tablero. Este es un vector de vectores conde el tablero ser lo
representa vector[i] es una columna y las jugadas se agregan haciendo push_back
en las columnas para que esta se ubique arriba de la columna. */
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

/* Consulto si la columna se lleno. */
bool columna_llena(tablero& tab, int columna)
{
  return tab.matrizFichas[columna].size() > tab.m;
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

/* Me devuelve el indice de la jugada con mayor valor asociado, aka, 1,0 o -1 */
int mejor_jugada(vector<int>& jugadas)
{
  int respuesta = 0;
  for(int indice = 1; indice < jugadas.size(); indice++)
  {
    if( jugadas.at(indice) > jugadas.at(respuesta) ) respuesta = indice;
  }
  return respuesta;
}

int fichas_en_columna(tablero& tablero, int ultimo_movimiento)
{
  return tablero.matrizFichas.at(ultimo_movimiento).size()-1;
}

int ultimo_jugador(tablero& tablero, int ultimo_movimiento)
{
  int cant_fichas_col = fichas_en_columna(tablero, ultimo_movimiento);
  if(cant_fichas_col<0) cant_fichas_col = 0;
  int jugador_actual = tablero.matrizFichas.at(ultimo_movimiento).at(cant_fichas_col);
  return jugador_actual;
}

#endif
