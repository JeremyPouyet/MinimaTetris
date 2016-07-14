#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <map>

class AudioManager {
public:
  AudioManager();
  ~AudioManager();

  bool	play(const std::string &name);
  void	increaseVolume();
  void	decreaseVolume();

private:
  void	setSound();

  std::map<const std::string, Mix_Chunk *> sounds = {
    {"destroy", nullptr}, {"pose", nullptr},
    {"move", nullptr}, {"rotate", nullptr}
  };

  const std::string soundDirectory = "./sounds/";
  int	_currentVolume;
};
