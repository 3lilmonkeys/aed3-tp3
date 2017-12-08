#include "../algoritmo_exacto.h"

bool test_validar_columna()
{
  int n = 5;
  int m = 5;
  tablero tab = crearTablero( n, m);
  int c_linea = 3;

  tab.matrizFichas.at(4).push_back(fichaEnemiga);
  tab.matrizFichas.at(4).push_back(fichaAliada);
  tab.matrizFichas.at(4).push_back(fichaAliada);
  tab.matrizFichas.at(4).push_back(fichaAliada);

  int ultimo_movimiento = 4;

  return validar_jugada(tab, c_linea, ultimo_movimiento);
}

bool test_validar_fila()
{
  int n = 5;
  int m = 5;
  tablero tab = crearTablero( n, m);
  int c_linea = 3;

  tab.matrizFichas.at(1).push_back(fichaEnemiga);
  tab.matrizFichas.at(2).push_back(fichaEnemiga);
  tab.matrizFichas.at(3).push_back(fichaEnemiga);
  tab.matrizFichas.at(4).push_back(fichaEnemiga);

  tab.matrizFichas.at(1).push_back(fichaAliada);
  tab.matrizFichas.at(2).push_back(fichaAliada);
  tab.matrizFichas.at(3).push_back(fichaAliada);
  tab.matrizFichas.at(4).push_back(fichaAliada);

  int ultimo_movimiento = 1;

  return validar_jugada(tab, c_linea, ultimo_movimiento);
}

bool test_validar_diagonal_45()
{
  int n = 5;
  int m = 5;
  tablero tab = crearTablero( n, m);
  int c_linea = 3;

  tab.matrizFichas.at(1).push_back(fichaEnemiga);
  tab.matrizFichas.at(2).push_back(fichaEnemiga);
  tab.matrizFichas.at(3).push_back(fichaEnemiga);
  tab.matrizFichas.at(4).push_back(fichaAliada);

  tab.matrizFichas.at(1).push_back(fichaEnemiga);
  tab.matrizFichas.at(2).push_back(fichaEnemiga);
  tab.matrizFichas.at(3).push_back(fichaAliada);
  tab.matrizFichas.at(4).push_back(fichaEnemiga);

  tab.matrizFichas.at(1).push_back(fichaEnemiga);
  tab.matrizFichas.at(2).push_back(fichaAliada);
  tab.matrizFichas.at(3).push_back(fichaEnemiga);
  tab.matrizFichas.at(4).push_back(fichaEnemiga);

  tab.matrizFichas.at(1).push_back(fichaAliada);
  // tab.matrizFichas.at(2).push_back(fichaAliada);
  tab.matrizFichas.at(3).push_back(fichaEnemiga);
  tab.matrizFichas.at(4).push_back(fichaEnemiga);

  int ultimo_movimiento = 2;

  return validar_jugada(tab, c_linea, ultimo_movimiento);
}

bool test_validar_diagonal_135()
{
  int n = 5;
  int m = 5;
  tablero tab = crearTablero( n, m);
  int c_linea = 3;

  tab.matrizFichas.at(1).push_back(fichaEnemiga);
  tab.matrizFichas.at(2).push_back(fichaAliada);
  tab.matrizFichas.at(3).push_back(fichaAliada);
  tab.matrizFichas.at(4).push_back(fichaAliada);

  tab.matrizFichas.at(1).push_back(fichaAliada);
  tab.matrizFichas.at(2).push_back(fichaEnemiga);
  tab.matrizFichas.at(3).push_back(fichaAliada);
  tab.matrizFichas.at(4).push_back(fichaAliada);

  tab.matrizFichas.at(1).push_back(fichaAliada);
  tab.matrizFichas.at(2).push_back(fichaAliada);
  tab.matrizFichas.at(3).push_back(fichaEnemiga);
  tab.matrizFichas.at(4).push_back(fichaAliada);

  tab.matrizFichas.at(1).push_back(fichaAliada);
  tab.matrizFichas.at(2).push_back(fichaAliada);
  // tab.matrizFichas.at(3).push_back(fichaAliada);
  tab.matrizFichas.at(4).push_back(fichaEnemiga);

  int ultimo_movimiento = 3;

  return validar_jugada(tab, c_linea, ultimo_movimiento);
}

int main()
{
  assert( test_validar_columna() );
  assert( test_validar_fila() );
  assert( test_validar_diagonal_45() );
  assert( test_validar_diagonal_135() );

  return 0;
}
