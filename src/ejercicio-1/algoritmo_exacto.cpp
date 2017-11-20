#include "algoritmo_exacto.h"


#ifndef header_algoritmo_exacto
#define header_algoritmo_exacto


int calcularJugada(tablero& tab, int columnas, int c_linea, int cant_fichas){
    vector<int> posibles_jugadas(tab.n);
    vector<tablero> posibles_tableros(tab.n);
    /* Comienzo la recursión por todas las "ramas". En este caso son una por
    columna. Me guardo un tablero para cada rama. */
    for(int i = 0; i < columnas; i++){
        posibles_tableros[i] = tab;
        actualizarTablero(posibles_tableros[i], i, true);
        // Le asginamos un -10 ya que no quiero tener en cuenta ese movimiento.
        if( columna_llena(posibles_tableros[i], i) ){
            posibles_jugadas[i] = -10;
        }else{
            /* Esta funcion se encarga de maximizar, por lo tanto llamo a evaluar
            jugada con MINIMIZAR. */
            // posibles_jugadas[i] = evaluarJugada(posibles_tableros[i], columnas,
            // false, c_linea, cant_fichas - 1);
            posibles_jugadas[i] = jugar_recursivo(posibles_tableros[i], columnas,
            false, c_linea, cant_fichas - 1, i);
            // Si gano, salgo.
            if(posibles_jugadas[i] == 1) return i;
        }

    }
    // Maximizo y devuelvo la columna donde quiero jugar.
    return posMax(posibles_jugadas);
}


int maximinar(tablero& tab, int columnas, int c_linea, int cant_fichas)
{
  if(cant_fichas == 0) return 0;
  vector<tablero> opcionesTablero(tab.n);
  vector<int> posiblesResultados(tab.n);
  for(int i = 0; i < tab.n; i++){
      if(tab.matrizFichas[i].size() < tab.m){
          opcionesTablero[i] = tab;
          opcionesTablero[i].matrizFichas[i].push_back((fichaAliada));

          // int resultado = evaluarJugada(opcionesTablero[i], columnas,
          // MINIMIZAR, c_linea, cant_fichas - 1);

          int resultado = jugar_recursivo(opcionesTablero[i], columnas,
          MINIMIZAR, c_linea, cant_fichas - 1, i);

          posiblesResultados[i] = resultado;
          /* Parte alfabeta: si la rama que acabo de calcular ya me da 1 lo
          devuelvo asi no calculo los siguientes. */
          if(resultado == 1){
              return 1;
          }
      }
  }
  // Devolver el maximo de cada posibilidad.
  return maxRes(posiblesResultados);
}

int minimizar(tablero& tab, int columnas, int c_linea, int cant_fichas)
{
  vector<tablero> opcionesTablero(tab.n);
  vector<int> posiblesResultados(tab.n);
  for(int i = 0; i < tab.n; i++){
      if(tab.matrizFichas[i].size() < tab.m){
          opcionesTablero[i] = tab;
          opcionesTablero[i].matrizFichas[i].push_back((fichaEnemiga));

          // int resultado = evaluarJugada(opcionesTablero[i], columnas,
          // MAXIMIZAR, c_linea, cant_fichas);

          int resultado = jugar_recursivo(opcionesTablero[i], columnas,
          MINIMIZAR, c_linea, cant_fichas - 1, i);

          posiblesResultados[i] = resultado;
          if(resultado == -1){ //parte alfabeta
              return -1;
          }
      }
  }
  // Devolver el minimo de cada posibilidad.
  return minRes(posiblesResultados);
}

int jugar_recursivo(tablero& tab, int columnas, bool maximizar, int c_linea,
  int cant_fichas, int ultimo_movimiento)
{
  int respuesta = 0;

  // ¿Quien fue el ultimo en jugar?
  bool jugue_ultimo = ( ultimo_jugador(tab, ultimo_movimiento) == fichaAliada);

  // valido la ultima jugada
  bool jugada_ganadora = validar_jugada(tab, c_linea, ultimo_movimiento);

  /* Si yo fui el ultimo en jugar y gane con la ultima ficha que puse */
  if( jugue_ultimo and jugada_ganadora )
    respuesta = 1;
  /* Si jugo el contrario y gano */
  else if( (!jugue_ultimo) and jugada_ganadora )
    respuesta = -1;
  else if( tableroLleno(tab) )
    respuesta = 0;
  else{
    if(maximizar)
        respuesta = maximinar(tab, columnas, c_linea, cant_fichas);
    else
        respuesta = minimizar(tab, columnas, c_linea, cant_fichas);
  }
  return respuesta;
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
  while( (indice_x < tablero.n) and (indice_y >= 0) and
  (indice_y <= tablero.matrizFichas.at(indice_x).size()) and
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
  (indice_y <= tablero.matrizFichas.at(indice_x).size()) and
  (jugador_actual == tablero.matrizFichas.at(indice_x).at(indice_y)) )
  {
    cant_fichas++;
    indice_y--;
    indice_x--;
  }
  return (cant_fichas >= c_linea);
}

int fichas_en_columna(tablero& tablero, int ultimo_movimiento)
{
  return tablero.matrizFichas.at(ultimo_movimiento).size()-1;
}

int ultimo_jugador(tablero& tablero, int ultimo_movimiento)
{
  int cant_fichas_col = fichas_en_columna(tablero, ultimo_movimiento);
  int jugador_actual = tablero.matrizFichas.at(ultimo_movimiento).at(cant_fichas_col);
  return jugador_actual;
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

int evaluarJugada(tablero& tab, int columnas, bool maximizar, int c_linea,
int cant_fichas)
{
    int resultado = validarVictoria(tab, c_linea);

    if(resultado == 1)
        return 1;
    else if(resultado == -1)
        return -1;
    else if( tableroLleno(tab) )
        return 0;
    // Con el tablero como está empato, entonces elijo donde jugar.
    else{
        if(maximizar){
            return maximinar(tab, columnas, c_linea, cant_fichas);
        }
        else{
            return minimizar(tab, columnas, c_linea, cant_fichas);
        }
    }
}

/* Yo fui el ultimo que jugó, entonces quiero saber si con esa jugada gano. */
int validarVictoria(tablero& tab, int c){
    int temporal;
    //i recorre columnas
    for (int i = 0; i < tab.n; ++i)
    {
        //j recorre filas
        for (int j = 0; j < tab.m; ++j)
        {
            // Chequea si hay ganador en la fila de p fichas  empezando de (i,k)
            if(i <= tab.n - c){
                temporal = auxiliar1( i, j, tab, c);
                if(temporal) return temporal;
            }
            //chequea si hay ganador en la columna de p fichas  empezando de (i,j)
            if(j <= tab.m - c){
                temporal = auxiliar2( i, j, tab, c);
                if(temporal) return temporal;
            }
            //chequea si hay ganador en la diagonal hacia la derecha de p fichas empezando de (i,k)
            if((i <= tab.n - c)&&(j <= tab.m - c)){
                temporal = auxiliar3( i, j, tab, c);
                if(temporal) return temporal;
            }
            //chequea si hay ganador en la diagonal hacia la izquierda de p fichas empezando de (i,k)
            if((i >= c - 1)&&(j <= tab.m - c)){
                temporal = auxiliar4( i, j, tab, c);
                if(temporal) return temporal;
            }
        }
    }
    return 0;
}

int auxiliar1(int i, int j, tablero& tab, int c_linea)
{
    bool hayGanador = true;
    for (int k = 0; k < c_linea; ++k)
    {
        if (hayFicha(tab, i + k, j)) {
            if (tab.matrizFichas[i][j] != tab.matrizFichas[i + k][j])
            {
                hayGanador = false;
                break;
            }
        }else{ hayGanador = false;  break; }
    }
    if (hayGanador){
        if(tab.matrizFichas[i][j] == fichaAliada) {return 1;} else {return -1;}
    }
    return 0;
}

int auxiliar2(int i, int j, tablero& tab, int c_linea)
{
    bool hayGanador = true;
    for (int k = 0; k < c_linea; ++k)
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
    return 0;
}

int auxiliar3(int i, int j, tablero& tab, int c_linea)
{
  bool hayGanador = true;
  for (int k = 0; k < c_linea; ++k)
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
  return 0;
}

int auxiliar4(int i, int j, tablero& tab, int c_linea)
{
  bool hayGanador = true;
  for (int k = 0; k < c_linea; ++k)
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
  return 0;
}

#endif
