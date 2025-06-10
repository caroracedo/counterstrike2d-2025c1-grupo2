#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/SDL2pp.hh>

class SoundManager {
private:
    SDL2pp::Mixer mixer;

    // para las balas xq tienen != sonidos.
    //  std::unordered_map<std::string, Sound> bullets;
    std::unordered_map<std::string, SDL2pp::Chunk> sounds;
    // SDL2pp::Music background_music;

    // Control de reproducción
    std::unordered_map<std::string, uint32_t> last_played_time;  // en ms

    const uint32_t step_cooldown_ms = 600;

public:
    SoundManager():
            mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048)
    // background_music()
    {
        // Cargar sonidos
        // sounds.emplace("bullet", SDL2pp::Chunk("../assets/sfx/weapons/glock18.wav"));
        // sounds.emplace("hit", SDL2pp::Chunk("../assets/sfx/player/hit1.wav"));
        // sounds.emplace("steps_1", SDL2pp::Chunk("../assets/sfx/player/pl_dirt1.wav"));
        // sounds.emplace("steps_2", SDL2pp::Chunk("../assets/sfx/player/pl_dirt2.wav"));
        // sounds.emplace("steps_3", SDL2pp::Chunk("../assets/sfx/player/pl_dirt3.wav"));
        // sounds.emplace("steps_4", SDL2pp::Chunk("../assets/sfx/player/pl_dirt4.wav"));
        // sounds.emplace("beep", SDL2pp::Chunk("../assets/sfx/weapons/c4.wav"));
        // sounds.emplace("explotion", SDL2pp::Chunk("../assets/sfx/weapons/c4_explode.wav"));
    }

    void play(const std::string& name, int loops = 0) {
        uint32_t now = SDL_GetTicks();  // tiempo actual en ms

        if (name.find("steps") != std::string::npos) {
            if (now - last_played_time[name] < step_cooldown_ms)
                return;  // todavía no pasó suficiente tiempo
            last_played_time[name] = now;
        }

        auto it = sounds.find(name);
        if (it != sounds.end()) {
            mixer.PlayChannel(-1, it->second, loops);
        }
    }
};


#endif  // SOUND_MANAGER_H
