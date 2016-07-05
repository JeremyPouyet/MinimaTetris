#include "AudioManager.hh"

AudioManager::AudioManager() {
  Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
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

AudioManager::~AudioManager() {
  for (auto sound : sounds)
    Mix_FreeChunk(sound.second);
  Mix_CloseAudio();
}
