#include "algoritmo_exacto.h"

int main() {
    string msg, color, oponent_color, go_first;
    int columns, rows, c, p, move;

    while (true){
        color = read_str();
        oponent_color = read_str();

        columns = read_int();
        rows = read_int();
        c = read_int();
        p = read_int();

        tablero tab = crearTablero(columns, rows);

        go_first = read_str();
        if (go_first == "vos") {
            move = calcularJugada(tab, columns, c, p);
            actualizarTablero(tab, move, true);
            send(move);
        }

        while (true){
            msg = read_str();
            if (msg == "ganaste" || msg == "perdiste" || msg == "empataron") {
                break;
            }

            //actualizar tablero con el movimiento del enemigo
            actualizarTablero(tab, stoi(msg), false);

            move = calcularJugada(tab, columns, c, p);

            //actualizar tablero con el movimiento, move mio.
            actualizarTablero(tab, move, true);
            send(move);
        }
    }

    return 0;
}
