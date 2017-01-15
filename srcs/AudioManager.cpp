#include "AudioManager.hh"

AudioManager::AudioManager() : _currentVolume(MIX_MAX_VOLUME / 4) {
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
  setSound();
  std::string path;
  for (auto sound : sounds) {
    path = soundDirectory + sound.first + ".wav";
    sounds[sound.first] = Mix_LoadWAV(path.c_str());
  }
}

bool AudioManager::play(const std::string &name) {
  if (sounds.count(name) < 1)
    return false;
  return (Mix_PlayChannel(-1, sounds[name], 0) != -1);
}

void	AudioManager::increaseVolume() {
  if (_currentVolume + 1 <= MIX_MAX_VOLUME) {
    ++_currentVolume;
    setSound();
  }
}

void	AudioManager::decreaseVolume() {
  if (_currentVolume - 1 >= 0) {
    --_currentVolume;
    setSound();
  }
}

void	AudioManager::setSound() {
  Mix_Volume(-1, _currentVolume);
}

AudioManager::~AudioManager() {
  for (auto sound : sounds)
    Mix_FreeChunk(sound.second);
  Mix_CloseAudio();
}
