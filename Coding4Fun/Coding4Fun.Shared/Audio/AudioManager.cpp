#include "pch.h"
#include "AudioManager.h"

using namespace Coding4Fun::FallFury::Audio;

GameState			AudioManager::MusicCategory;
Audio				AudioManager::AudioEngineInstance;
bool				AudioManager::IsMusicStarted;
bool				AudioManager::IsSFXStarted;

int					AudioManager::MusicVolume;
int					AudioManager::SFXVolume;

void AudioManager::Initialize()
{
	MusicVolume = 100;
	SFXVolume = 100;
	AudioEngineInstance.Initialize();
}

void AudioManager::Update()
{
	AudioEngineInstance.Render();
}

void AudioManager::SetGameOverMusic()
{
	MusicCategory = GameState::GS_GAME_OVER;
	AudioEngineInstance.~Audio();
	AudioEngineInstance.Initialize();
	AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\game_over.wma");
	
	SetVolume();
	AudioEngineInstance.Start();

	CheckStates();
}

void AudioManager::SetMainMenuMusic()
{
	MusicCategory = GameState::GS_MAIN_MENU;
	AudioEngineInstance.~Audio();
	AudioEngineInstance.Initialize();

	AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\maintheme.wma");
	
	SetVolume();
	AudioEngineInstance.Start();

	CheckStates();
}

void AudioManager::SetGameWonMusic()
{
	MusicCategory = GameState::GS_WIN;
	AudioEngineInstance.~Audio();
	AudioEngineInstance.Initialize();

	AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\game_won.wma");
	
	SetVolume();
	AudioEngineInstance.Start();

	CheckStates();
}

void AudioManager::SetGamePlayMusic(Coding4Fun::FallFury::Entities::LevelType levelType)
{
	AudioManager::MusicCategory = GameState::GS_PLAYING;
	AudioEngineInstance.~Audio();
	AudioEngineInstance.Initialize();

	switch (levelType)
	{
	case LevelType::NIGHTMARE:
			{
			AudioManager::AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\nightmare_theme.wma");
			break;
			}
	case LevelType::MAGIC_BEANS:
			{
			AudioManager::AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\magic_bean_theme.wma");
			break;
			}
	case LevelType::SPACE:
			{
			AudioManager::AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\magic_bean_theme.wma");
			break;
			}
	case LevelType::DREAM:
			{
			AudioManager::AudioEngineInstance.ReInitializeStreamer(L"Assets\\Music\\candyland_theme.wma");
			break;
			}
	 }
	
	SetVolume();
    AudioManager::AudioEngineInstance.Start();	

	CheckStates();
}

void AudioManager::CheckStates()
{
	if (IsMusicStarted)
		AudioEngineInstance.StartMusic();
	else
		AudioEngineInstance.SuspendMusic();

	if (IsSFXStarted)
		AudioEngineInstance.StartSFX();
	else
		AudioEngineInstance.SuspendSFX();
}

void AudioManager::SetMusicVolume(int percent)
{
	MusicVolume = percent;
	AudioEngineInstance.SetMusicVolume((percent/100.f) * 1.0f);
}

void AudioManager::SetSFXVolume(int percent)
{
	SFXVolume = percent;
	AudioEngineInstance.SetAllSoundEffectVolume((percent/100.f) * 1.0f);
}

void AudioManager::SetVolume()
{
	SetMusicVolume(MusicVolume);
	SetSFXVolume(SFXVolume);
}