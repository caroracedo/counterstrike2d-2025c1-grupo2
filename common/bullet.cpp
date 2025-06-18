#include "bullet.h"

#include <cmath>
#include <iostream>
#include <random>

Bullet::Bullet(const uint16_t id, const uint16_t _player_id,
               const std::vector<uint16_t>& player_position, uint16_t _range, uint16_t _min_damage,
               uint16_t _max_damage, float _precision,
               const std::vector<uint16_t>& desired_position):
        Object(ObjectType::BULLET, id, {player_position[0], player_position[1]}, BULLET_RADIUS * 2,
               BULLET_RADIUS * 2),
        player_id(_player_id),
        range(_range),
        min_damage(_min_damage),
        max_damage(_max_damage),
        target_position(player_position),
        precision(_precision) {
    set_target_position(desired_position, player_position);
    set_starting_position(player_position);
}

ObjectDTO Bullet::get_dto() const { return ObjectDTO(object_type, position); }

bool Bullet::is_alive() const {
    /*
        Una bala está viva si tiene rango restante o si no ha alcanzado su posición objetivo.
    */
    float cx = position[0];
    float cy = position[1];
    bool reached_target = (static_cast<int>(cx) == static_cast<int>(target_position[0]) &&
                           static_cast<int>(cy) == static_cast<int>(target_position[1]));
    return range > 0 || !reached_target;
}

uint16_t Bullet::get_range() const { return range; }

void Bullet::decrement_range(uint16_t dist) {
    /*
        Decrementa el rango de la bala según la distancia recorrida.
    */
    range = (dist >= range) ? 0 : range - dist;
    distance_moved += dist;
}

uint16_t Bullet::get_player_id() const { return player_id; }

uint16_t Bullet::get_damage() const {
    /*
        Calcula el daño de la bala basado en la distancia recorrida, el rango, y la precisión.
        Devuelve 0 si el disparo falla.
        El daño se calcula como un valor aleatorio entre min_damage y max_damage, modificado por
        un factor de distancia. Si la bala está muy lejos del objetivo, el daño disminuye.
    */
    // Calcula distancia recorrida desde el origen de la bala
    float px = static_cast<float>(position[0]);
    float py = static_cast<float>(position[1]);
    float sx = static_cast<float>(start_position[0]);
    float sy = static_cast<float>(start_position[1]);
    float distance = std::sqrt((px - sx) * (px - sx) + (py - sy) * (py - sy));

    // Factor de daño por distancia (más lejos, menos daño)
    float distance_factor = 1.0f - std::min(distance / ((float)range + distance), 1.0f);

    // Daño aleatorio entre min y max
    static thread_local std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<uint16_t> dmg_dist(min_damage, max_damage);
    uint16_t base_damage = dmg_dist(rng);

    // Aplica el factor de distancia
    uint16_t final_damage = static_cast<uint16_t>(base_damage * distance_factor);

    // Probabilidad de acertar según precisión
    std::uniform_real_distribution<float> prob(0.0f, 1.0f);
    if (prob(rng) > precision) {
        return 0;  // Falló el disparo
    }

    std::cout << "\tBullet ID: " << id << ", Position: (" << position[0] << ", " << position[1]
              << ")"
              << ", Distance: " << distance << ", Base Damage: " << base_damage
              << ", Final Damage: " << final_damage << std::endl;

    // Siempre al menos 1 de daño si acertó y el daño no es cero
    return std::max<uint16_t>(final_damage, 1);
}

std::vector<uint16_t> Bullet::get_next_position() const {
    /*
        Calcula la siguiente posición de la bala basada en su posición actual, el rango restante,
        y la posición objetivo.
        Si la bala está en su posición objetivo, devuelve la posición actual.
    */
    if (target_position.size() != 2)
        return position;

    float cx = position[0];
    float cy = position[1];

    float dx = static_cast<float>(target_position[0]) - cx;
    float dy = static_cast<float>(target_position[1]) - cy;

    float dist_to_target = std::sqrt(dx * dx + dy * dy);
    if (dist_to_target == 0) {
        return position;
    }

    float step = std::min(static_cast<float>(MOVE_STEP), static_cast<float>(range));
    if (dist_to_target <= step) {
        // Si el paso es mayor o igual a la distancia restante, ir directo al target
        return {static_cast<uint16_t>(std::round(target_position[0])),
                static_cast<uint16_t>(std::round(target_position[1]))};
    }

    float v_step = step / dist_to_target;
    float step_dx = dx * v_step;
    float step_dy = dy * v_step;

    float nx_c = cx + step_dx;
    float ny_c = cy + step_dy;

    float min_pos = radius;
    float max_pos = MATRIX_SIZE * CELL_SIZE - radius;
    nx_c = std::max(min_pos, std::min(nx_c, max_pos));
    ny_c = std::max(min_pos, std::min(ny_c, max_pos));

    return {static_cast<uint16_t>(std::round(nx_c)), static_cast<uint16_t>(std::round(ny_c))};
}

void Bullet::set_target_position(const std::vector<uint16_t>& desired_position,
                                 const std::vector<uint16_t>& player_position) {
    /*
        Calcula la posición objetivo de la bala basada en la posición deseada y la posición del
        jugador. Asegura que la bala no salga del mapa y respeta el rango máximo.
        Si la posición deseada está fuera del rango, ajusta la posición objetivo al borde del mapa.
    */
    if (desired_position.size() != 2 || player_position.size() != 2)
        return;

    float px = static_cast<float>(player_position[0]);
    float py = static_cast<float>(player_position[1]);
    float dx = static_cast<float>(desired_position[0]) - px;
    float dy = static_cast<float>(desired_position[1]) - py;
    float magnitude = std::sqrt(dx * dx + dy * dy);

    float min_pos = radius;
    float max_pos = MATRIX_SIZE * CELL_SIZE - radius;

    float t_max = (magnitude == 0) ? 0.0f : static_cast<float>(range + PLAYER_RADIUS) / magnitude;

    // Calcula el t permitido por los bordes del mapa
    if (dx != 0) {
        float t1 = (min_pos - px) / dx;
        float t2 = (max_pos - px) / dx;
        float t_edge = dx > 0 ? t2 : t1;
        if (t_edge >= 0)
            t_max = std::min(t_max, t_edge);
    }
    if (dy != 0) {
        float t1 = (min_pos - py) / dy;
        float t2 = (max_pos - py) / dy;
        float t_edge = dy > 0 ? t2 : t1;
        if (t_edge >= 0)
            t_max = std::min(t_max, t_edge);
    }
    t_max = std::max(0.0f, t_max);

    float tx = px + dx * t_max;
    float ty = py + dy * t_max;

    target_position = {static_cast<uint16_t>(std::round(tx)),
                       static_cast<uint16_t>(std::round(ty))};
}

void Bullet::set_starting_position(const std::vector<uint16_t>& player_center) {
    /*
        Calcula la posición inicial de la bala para que esté fuera del radio del jugador.
        La posición se calcula en la dirección del objetivo, asegurando que la bala no colisione
        con el jugador al ser disparada.
    */
    float player_radius = static_cast<float>(PLAYER_RADIUS);
    float bullet_radius = static_cast<float>(BULLET_RADIUS);

    float cx = static_cast<float>(player_center[0]);
    float cy = static_cast<float>(player_center[1]);

    float dx = static_cast<float>(target_position[0]) - cx;
    float dy = static_cast<float>(target_position[1]) - cy;
    float mag = std::sqrt(dx * dx + dy * dy);

    float x_bullet, y_bullet;
    if (mag == 0) {
        x_bullet = cx + player_radius + bullet_radius;
        y_bullet = cy;
    } else {
        float min_dist = player_radius + bullet_radius + 1.0f;
        float offset = min_dist;
        do {
            x_bullet = cx + dx * (offset / mag);
            y_bullet = cy + dy * (offset / mag);
            offset += 1.0f;
        } while (std::sqrt((x_bullet - cx) * (x_bullet - cx) + (y_bullet - cy) * (y_bullet - cy)) <
                 min_dist);
    }

    // Clamp para que el centro de la bala nunca salga del mapa
    float min_pos = bullet_radius;
    float max_pos = MATRIX_SIZE * CELL_SIZE - bullet_radius;
    x_bullet = std::max(min_pos, std::min(x_bullet, max_pos));
    y_bullet = std::max(min_pos, std::min(y_bullet, max_pos));

    position = {static_cast<uint16_t>(std::round(x_bullet)),
                static_cast<uint16_t>(std::round(y_bullet))};

    start_position = {static_cast<uint16_t>(std::round(x_bullet)),
                      static_cast<uint16_t>(std::round(y_bullet))};
}
