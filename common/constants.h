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
#define WEAPON_WIDTH 32
#define AK47_HEIGHT 17
#define M3_HEIGHT 16
#define AWP_HEIGHT 16
#define KICKBACK_DISTANCE 5.0f

#define TIME 1
#define SHOP_TIME 10
#define SNAPSHOT_TIME 33
#define STATS_TIME 5
#define WAIT_TIME 100
#define MESSAGE_TIME 5

// TODO: Automatizar el cálculo coherente de matrix size de lógica y editor!
#define MATRIX_SIZE 21    // posición máxima en la matriz (celdas)
#define MOVE_STEP 5       // paso de movimiento
#define BULLET_STEP 10    // paso de la bala
#define CELL_SIZE 32      // tamaño de cada celda en la matriz
#define PLAYER_RADIUS 16  // radio del jugador
#define BULLET_RADIUS 2   // radio de la bala
#define BOMB_SIZE 32
#define BOMB_COUNTDOWN 30 * 15
#define DEACTIVATION_DISTANCE 50  // distancia máxima para desactivar la bomba
#define INITIAL_PLAYER_ANGLE 90   // ángulo inicial del jugador

#define KILL_REWARD 100       // recompensa por matar a un jugador
#define ROUND_WON_REWARD 300  // recompensa por ganar una ronda

#define YAML_EXTENSION ".yaml"

#endif  // CONSTANTS_H
