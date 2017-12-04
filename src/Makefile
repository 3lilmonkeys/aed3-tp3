CC = g++
CCFLAGS = -std=c++11 -O3

SOURCE_EJERCICIO_1 = ejercicio-1/main.cpp ejercicio-1/algoritmo_exacto.cpp\
ejercicio-1/algoritmo_exacto.h
TARGET_EJERCICIO_1 = ej1
SOURCE_EJERCICIO_2_GOLOSO = ejercicio-2/ej2Jugador.cpp
TARGET_EJERCICIO_2_GOLOSO = ej2
SOURCE_EJERCICIO_3 = ejercicio-3/ej3.cpp
TARGET_EJERCICIO_3 = ej3
SOURCE_RANDOM = random_player.cpp
TARGET_RANDOM = random
SOURCE_TEST_EJ1 = ejercicio-1/algoritmo_exacto.h ejercicio-1/test/testVerificarTablero.cpp \
ejercicio-1/algoritmo_exacto.cpp


TARGET_TEST_EJ1 = test_validar

all: ejercicio1 ejercicio2 random ejercicio3

ejemplo:
	/usr/bin/python c_linea.py --first azul --ui True --columns 7 --rows 6 --p 21 --c 4

ejemplo_jugador:
	/usr/bin/python c_linea.py --blue_player ./ej1 --first azul --ui True --columns 3 --rows 5 --p 10 --c 3

ejercicio1:
	$(CC) $(CCFLAGS) -g -o $(TARGET_EJERCICIO_1) $(SOURCE_EJERCICIO_1)

test_ej1_tablero:
	$(CC) $(CCFLAGS) -g -o $(TARGET_TEST_EJ1) $(SOURCE_TEST_EJ1)

ejercicio2:
	$(CC) $(CCFLAGS) -o $(TARGET_EJERCICIO_2_GOLOSO) $(SOURCE_EJERCICIO_2_GOLOSO)

random:
	$(CC) $(CCFLAGS) -o $(TARGET_RANDOM) $(SOURCE_RANDOM)

ejercicio3:
	$(CC) $(CCFLAGS) -o $(TARGET_EJERCICIO_3) $(SOURCE_EJERCICIO_3)

clean:
	$(RM) $(TARGET_EJERCICIO_1) $(TARGET_EJERCICIO_2_GOLOSO) \
	$(TARGET_RANDOM) $(TARGET_EJERCICIO_3) constants.pyc GraphicalBoard.pyc \
	LogicalBoard.pyc PlayerCommunicator.pyc Referee.pyc HumanPlayer.pyc \
	log/player_communicator_azul.log log/player_communicator_rojo.log \
	RandomPlayer_test.pyc
