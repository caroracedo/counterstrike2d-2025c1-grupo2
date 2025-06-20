#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Constantes */

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 32
#define OBSTACLE_WIDTH 32
#define OBSTACLE_HEIGHT 32
#define GUN_WIDTH 32
#define GUN_HEIGHT 32
#define POINTER_WIDTH 46
#define POINTER_HEIGHT 46
#define SCREEN_MARGIN 20
#define ROW_MARGIN 24
#define WEAPON_SIZE 16

#define MATRIX_SIZE 12            // posición máxima en la matriz (celdas)
#define MOVE_STEP 5               // paso de movimiento
#define CELL_SIZE 56              // tamaño de cada celda en la matriz
#define PLAYER_RADIUS 16          // radio del jugador
#define BULLET_RADIUS 2           // radio de la bala
#define BOMB_RADIUS 8             // radio de la bomba
#define DEACTIVATION_DISTANCE 50  // distancia máxima para desactivar la bomba
#define INITIAL_PLAYER_ANGLE 90   // ángulo inicial del jugador

#define KILL_REWARD 100  // recompensa por matar a un jugador

#define YAML_EXTENSION ".yaml"

#endif  // CONSTANTS_H
