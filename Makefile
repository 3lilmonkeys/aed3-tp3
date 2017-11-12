CC = g++
CCFLAGS = -std=c++11

SOURCE_EJERCICIO_1 = src/ejercicio-1/algoritmo_exacto.cpp
TARGET_EJERCICIO_1 = ej1
SOURCE_EJERCICIO_2_GOLOSO = src/ejercicio-2/ej2Jugador.cpp
TARGET_EJERCICIO_2_GOLOSO = ej2
SOURCE_EJERCICIO_3 = src/ejercicio-3/ej3.cpp
TARGET_EJERCICIO_3 = ej3
SOURCE_RANDOM = src/random_player.cpp
TARGET_RANDOM = random

all: ejercicio1 ejercicio2_goloso random ejercicio3

ejercicio1:
	$(CC) $(CCFLAGS) -o $(TARGET_EJERCICIO_1) $(SOURCE_EJERCICIO_1)

ejercicio2_goloso:
	$(CC) $(CCFLAGS) -o $(TARGET_EJERCICIO_2_GOLOSO) $(SOURCE_EJERCICIO_2_GOLOSO)

random:
	$(CC) $(CCFLAGS) -o $(TARGET_RANDOM) $(SOURCE_RANDOM)

ejercicio3:
	$(CC) $(CCFLAGS) -o $(TARGET_EJERCICIO_3) $(SOURCE_EJERCICIO_3)

clean:
	$(RM) $(TARGET_EJERCICIO_1) $(TARGET_EJERCICIO_2_GOLOSO) \
	$(TARGET_RANDOM) $(TARGET_EJERCICIO_3)
