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

public:
    SoundManager():
            mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048)
    // background_music()
    {
        // Cargar sonidos
        sounds.emplace("bullet", SDL2pp::Chunk("../assets/sfx/weapons/glock18.wav"));
        sounds.emplace("hit", SDL2pp::Chunk("../assets/sfx/player/hit1.wav"));
        sounds.emplace("steps_1", SDL2pp::Chunk("../assets/sfx/player/pl_dirt1.wav"));
        sounds.emplace("steps_2", SDL2pp::Chunk("../assets/sfx/player/pl_dirt2.wav"));
        sounds.emplace("steps_3", SDL2pp::Chunk("../assets/sfx/player/pl_dirt3.wav"));
        sounds.emplace("steps_4", SDL2pp::Chunk("../assets/sfx/player/pl_dirt4.wav"));
    }

    void play(const std::string& name, int loops = 0) {
        auto it = sounds.find(name);
        if (it != sounds.end()) {
            mixer.PlayChannel(-1, it->second, loops);
        }
    }
};


#endif  // SOUND_MANAGER_H
