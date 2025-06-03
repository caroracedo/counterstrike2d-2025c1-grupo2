#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2pp/Chunk.hh>

#include <unordered_map>
#include <random>
#include <vector>
#include <string>
using namespace SDL2pp;
class SoundManager {
private:
    std::mt19937 rand;

    std::unordered_map<std::string, SDL2pp::Chunk> single_sounds;
    std::unordered_map<std::string, std::vector<SDL2pp::Chunk>> soundGroups;

public:
    SoundManager() {
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("Failed to initialize SDL_mixer: %s", Mix_GetError());
            // Manejar error o lanzar excepción
        }

        // Cargar sonidos
        single_sounds.emplace("bullet", SDL2pp::Chunk("../assets/sfx/weapons/glock18.wav"));
        single_sounds.emplace("hit", SDL2pp::Chunk("../assets/sfx/player/hit1.wav"));

        std::vector<std::string> stepPaths = {
            "../assets/sfx/player/pl_dirt1.wav",
            "../assets/sfx/player/pl_dirt2.wav",
            "../assets/sfx/player/pl_dirt3.wav",
            "../assets/sfx/player/pl_dirt4.wav"
        };

        std::vector<SDL2pp::Chunk> steps;
        for (const auto& path : stepPaths) {
            steps.emplace_back(path);
        }
        soundGroups.emplace("steps", std::move(steps));
    }

    ~SoundManager() {
        Mix_CloseAudio();
    }

    void play(const std::string& name, int loops = 0) {
        auto it = single_sounds.find(name);
        if (it != single_sounds.end()) {
            Mix_PlayChannel(-1, it->second.Get(), loops);
        }
    }

    void playFromGroup(const std::string& groupName, int loops = 0) {
        auto it = soundGroups.find(groupName);
        if (it != soundGroups.end() && !it->second.empty()) {
            std::uniform_int_distribution<> dist(0, it->second.size() - 1);
            Mix_PlayChannel(-1, it->second[dist(rand)].Get(), loops);
        }
    }
};

#endif  // SOUND_MANAGER_H
