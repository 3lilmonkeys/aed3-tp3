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
            posibles_jugadas[i] = evaluarJugada(posibles_tableros[i], columnas,
            false, c_linea, cant_fichas - 1);
            // Si gano, salgo.
            if(posibles_jugadas[i] == 1) return i;
        }

    }
    // Maximizo y devuelvo la columna donde quiero jugar.
    return posMax(posibles_jugadas);
}

int evaluarJugada(tablero& tab, int columnas, bool maximizar, int c_linea, int cant_fichas){
    int resultado = validarVictoria(tab, c_linea);
    int evaluar;

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

int maximinar(tablero& tab, int columnas, int c_linea, int cant_fichas)
{
  if(cant_fichas == 0) return 0;
  vector<tablero> opcionesTablero(tab.n);
  vector<int> posiblesResultados(tab.n);
  for(int i = 0; i < tab.n; i++){
      if(tab.matrizFichas[i].size() < tab.m){
          opcionesTablero[i] = tab;
          opcionesTablero[i].matrizFichas[i].push_back((fichaAliada));

          int resultado = evaluarJugada(opcionesTablero[i], columnas,
          MINIMIZAR, c_linea, cant_fichas - 1);

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

          int resultado = evaluarJugada(opcionesTablero[i], columnas,
          MAXIMIZAR, c_linea, cant_fichas);

          posiblesResultados[i] = resultado;
          if(resultado == -1){ //parte alfabeta
              return -1;
          }
      }
  }
  // Devolver el minimo de cada posibilidad.
  return minRes(posiblesResultados);
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

#endif
