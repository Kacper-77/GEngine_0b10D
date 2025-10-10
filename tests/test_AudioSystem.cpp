#include <gtest/gtest.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "systems/AudioSystem.h"

class AudioSystemTest : public ::testing::Test {
protected:
    EntityManager entityManager;
    AudioSystem audio{&entityManager};

    EntityID entity = 1;
    SoundTag tag = "test_sound";

    AudioType musicAudio;
    AudioType chunkAudio;

    void SetUp() override {
        ASSERT_EQ(SDL_Init(SDL_INIT_AUDIO), 0) << "SDL_Init failed";
        ASSERT_EQ(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048), 0) << "Mix_OpenAudio failed";

        // Load music
        Mix_Music* music = Mix_LoadMUS("../assets/sample.ogg");
        ASSERT_NE(music, nullptr) << "Mix_LoadMUS failed";

        musicAudio.type = AudioType::Type::Music;
        musicAudio.music = music;
        musicAudio.priority = 10;

        // load sound effect
        Mix_Chunk* chunk = Mix_LoadWAV("../assets/sample.wav");
        ASSERT_NE(chunk, nullptr) << "Mix_LoadWAV failed";

        chunkAudio.type = AudioType::Type::Chunk;
        chunkAudio.chunk = chunk;
        chunkAudio.priority = 5;
    }


    void TearDown() override {
        if (musicAudio.music) {
            Mix_FreeMusic(musicAudio.music);
            musicAudio.music = nullptr;
        }
        if (chunkAudio.chunk) {
            Mix_FreeChunk(chunkAudio.chunk);
            chunkAudio.chunk = nullptr;
        }
        Mix_CloseAudio();
        SDL_Quit();
    }
};

TEST_F(AudioSystemTest, RegisterAndPlayEntitySound) {
    audio.RegisterSound(entity, tag, chunkAudio);
    EXPECT_NO_THROW(audio.Play(entity, tag));
}

TEST_F(AudioSystemTest, RegisterAndPlayGlobalSound) {
    audio.RegisterGlobalSound(tag, musicAudio);
    EXPECT_NO_THROW(audio.PlayGlobal(tag));
    SDL_Delay(1000);
    audio.StopGlobal(tag);
}

TEST_F(AudioSystemTest, StopGlobalSoundDoesNotThrow) {
    EXPECT_NO_THROW(audio.StopGlobal(tag));
}

TEST_F(AudioSystemTest, LayerVolumeAndMuteControl) {
    audio.SetLayerVolume(AudioLayer::UI, 64);
    EXPECT_EQ(audio.m_layerVolumes[AudioLayer::UI], 64);

    audio.MuteLayer(AudioLayer::UI);
    EXPECT_TRUE(audio.m_layerMuted[AudioLayer::UI]);

    audio.UnmuteLayer(AudioLayer::UI);
    EXPECT_FALSE(audio.m_layerMuted[AudioLayer::UI]);
}

TEST_F(AudioSystemTest, RegisterLayeredSound) {
    audio.RegisterLayeredSound(tag, chunkAudio, AudioLayer::Gameplay);
    auto it = audio.m_layeredAudio.find(tag);
    ASSERT_NE(it, audio.m_layeredAudio.end());
    EXPECT_EQ(it->second.second, AudioLayer::Gameplay);
}

TEST_F(AudioSystemTest, SpatialAudioDoesNotThrow) {
    audio.RegisterSound(entity, tag, chunkAudio);
    audio.m_entityPositions[entity] = VectorInt{100, 0};
    VectorFloat listener{50.0f, 0.0f};
    EXPECT_NO_THROW(audio.PlaySpatial(entity, tag, listener));
}

TEST_F(AudioSystemTest, EnqueueAndUpdateQueue) {
    audio.EnqueueSound(entity, chunkAudio);
    audio.EnqueueSound(entity, musicAudio);

    EXPECT_EQ(audio.m_soundQueues[entity].sounds.size(), 2);

    audio.Update(0.016f);
    EXPECT_TRUE(audio.m_soundQueues[entity].isPlaying);
}

TEST_F(AudioSystemTest, CleanupUnusedEntities) {
    audio.RegisterSound(entity, tag, chunkAudio);
    audio.m_entityPositions[entity] = VectorFloat{0.0f, 0.0f};
    audio.EnqueueSound(entity, chunkAudio);

    entityManager.DestroyEntityFromList(entity);
    audio.CleanupUnused();

    EXPECT_EQ(audio.m_audioAndSounds.count(entity), 0);
    EXPECT_EQ(audio.m_entityPositions.count(entity), 0);
    EXPECT_EQ(audio.m_soundQueues.count(entity), 0);
}
