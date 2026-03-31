#define MINIAUDIO_IMPLEMENTATION

#include "AudioSystem.h"

#include <miniaudio.h>

#include <iostream>

#include "Log.h"
namespace Forge {
AudioSystem::AudioSystem() = default;
AudioSystem::~AudioSystem() { shutdown(); }

bool AudioSystem::initialize() {
    engine = new ma_engine();
    ma_result result = ma_engine_init(nullptr, engine);
    if (result != MA_SUCCESS) {
        FORGE_ERROR("Failed to initialize audio engine: " +
                    std::to_string(result));
        delete engine;
        engine = nullptr;
        return false;
    }
    initialized = true;
    FORGE_INFO("Audio engine initialized successfully.");

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
        FORGE_ERROR("Failed to load music: {} Error: {}", filePath,
                    std::to_string(result));
        delete musicSound;
        musicSound = nullptr;
        return;
    }

    ma_sound_set_looping(musicSound, MA_TRUE);
    ma_sound_set_volume(musicSound, volume);
    ma_sound_start(musicSound);
    FORGE_INFO("Playing music: {}", filePath);
}

void AudioSystem::stopMusic() {
    if (musicSound) {
        ma_sound_uninit(musicSound);
        delete musicSound;
        musicSound = nullptr;
        FORGE_INFO("Music stopped.");
    }
}

void AudioSystem::setMusicVolume(float volume) {
    if (musicSound) {
        ma_sound_set_volume(musicSound, volume);
        FORGE_INFO("Music volume set to: {}", std::to_string(volume));
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
        FORGE_ERROR("Failed to play sound effect: {} Error: {}", filePath,
                    std::to_string(result));

    } else {
        FORGE_INFO("Playing sound effect: {}", filePath);
    }
}

void AudioSystem::setMasterVolume(float volume) {
    if (initialized) {
        ma_engine_set_volume(engine, volume);
        FORGE_INFO("Master volume set to: {}", std::to_string(volume));
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
    FORGE_INFO("Audio engine shut down.");
}
}  // namespace Forge