#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Constantes */

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PLAYER_WIDTH 32     ///////////////
#define PLAYER_HEIGHT 32    // SE USAN?? //
#define OBSTACLE_WIDTH 32   ///////////////
#define OBSTACLE_HEIGHT 32  ///////////////

// TODO: Automatizar el cálculo coherente de matrix size de lógica y editor!
#define MATRIX_SIZE 12            // posición máxima en la matriz (celdas)
#define MOVE_STEP 5               // paso de movimiento
#define CELL_SIZE 56              // tamaño de cada celda en la matriz
#define PLAYER_RADIUS 16          // radio del jugador
#define BULLET_RADIUS 2           // radio de la bala
#define DEACTIVATION_DISTANCE 50  // distancia máxima para desactivar la bomba

#define KILL_REWARD 100       // recompensa por matar a un jugador
#define MAX_PLAYER_AMOUNT 10  // cantidad máxima de jugadores

#endif  // CONSTANTS_H
