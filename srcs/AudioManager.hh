#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <map>

class AudioManager {
public:
  AudioManager();

  bool play(const std::string &name);

  ~AudioManager();
private:

  std::map<std::string, Mix_Chunk *> sounds = {
    {"destroy", nullptr}, {"pose", nullptr},
    {"move", nullptr}, {"rotate", nullptr}
  };

  const std::string soundDirectory = "./sounds/";
};
