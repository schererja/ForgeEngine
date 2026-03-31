#define MINIAUDIO_IMPLEMENTATION

#include "AudioSystem.h"

#include <miniaudio.h>

#include <iostream>

namespace Forge {
AudioSystem::AudioSystem() = default;
AudioSystem::~AudioSystem() { shutdown(); }

bool AudioSystem::initialize() {
    engine = new ma_engine();
    ma_result result = ma_engine_init(nullptr, engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine: " << result
                  << std::endl;
        delete engine;
        engine = nullptr;
        return false;
    }
    initialized = true;
    std::cout << "Audio engine initialized successfully." << std::endl;
    return true;
}

void AudioSystem::playMusic(const std::string& filePath, float volume) {
    if (!initialized) {
        return;
    }
    // Stop current music if playing
    stopMusic();
    musicSound = new ma_sound();
    ma_result result =
        ma_sound_init_from_file(engine, filePath.c_str(), MA_SOUND_FLAG_STREAM,
                                nullptr, nullptr, musicSound);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to load music: " << filePath
                  << " Error: " << result << std::endl;
        delete musicSound;
        musicSound = nullptr;
        return;
    }

    ma_sound_set_looping(musicSound, MA_TRUE);
    ma_sound_set_volume(musicSound, volume);
    ma_sound_start(musicSound);

    std::cout << "Playing music: " << filePath << std::endl;
}

void AudioSystem::stopMusic() {
    if (musicSound) {
        ma_sound_uninit(musicSound);
        delete musicSound;
        musicSound = nullptr;
        std::cout << "Music stopped." << std::endl;
    }
}

void AudioSystem::setMusicVolume(float volume) {
    if (musicSound) {
        ma_sound_set_volume(musicSound, volume);
        std::cout << "Music volume set to: " << volume << std::endl;
    }
}

bool AudioSystem::isMusicPlaying() const {
    if (musicSound) {
        return ma_sound_is_playing(musicSound) == MA_TRUE;
    }
    return false;
}

void AudioSystem::playSoundEffect(const std::string& filePath) {
    if (!initialized) {
        return;
    }
    // ma_engine_play_sound() will handle loading and playing the sound effect
    ma_result result = ma_engine_play_sound(engine, filePath.c_str(), nullptr);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to play sound effect: " << filePath
                  << " Error: " << result << std::endl;
    } else {
        std::cout << "Playing sound effect: " << filePath << std::endl;
    }
}

void AudioSystem::setMasterVolume(float volume) {
    if (initialized) {
        ma_engine_set_volume(engine, volume);
        std::cout << "Master volume set to: " << volume << std::endl;
    }
}

void AudioSystem::shutdown() {
    if (musicSound) {
        ma_sound_uninit(musicSound);
        delete musicSound;
        musicSound = nullptr;
    }
    if (engine) {
        ma_engine_uninit(engine);
        delete engine;
        engine = nullptr;
    }
    initialized = false;
    std::cout << "Audio engine shut down." << std::endl;
}
}  // namespace Forge