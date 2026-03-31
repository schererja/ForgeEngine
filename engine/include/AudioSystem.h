#pragma once

#include <string>
#include <unordered_map>

// Forward declaration
struct ma_engine;
struct ma_sound;

namespace Forge {
class AudioSystem {
   public:
    AudioSystem();
    ~AudioSystem();

    // No Copying
    AudioSystem(const AudioSystem&) = delete;
    AudioSystem& operator=(const AudioSystem&) = delete;

    bool initialize();
    void shutdown();

    // Music - One track at a time, loops by default
    void playMusic(const std::string& filePath, float volume = 1.0f);
    void stopMusic();
    void setMusicVolume(float volume);
    bool isMusicPlaying() const;

    // Sound Effects - Multiple can play simultaneously
    void playSoundEffect(const std::string& filePath);

    // Master Volume
    void setMasterVolume(float volume);

   private:
    ma_engine* engine = nullptr;
    ma_sound* musicSound = nullptr;
    bool initialized = false;
};
}  // namespace Forge