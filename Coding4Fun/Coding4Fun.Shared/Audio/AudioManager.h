#pragma once
#include "pch.h"
#include "Audio\AudioEngine.h"
#include "Entities\LevelType.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Audio
		{
			using namespace Coding4Fun::FallFury::Entities;

			class AudioManager
			{
				public:
					static void Initialize();
					static void Update();

					static void SetGameOverMusic();
					static void SetMainMenuMusic();
					static void SetGameWonMusic();
					static void CheckStates();
					static void SetGamePlayMusic(LevelType levelType);
		
					static void SetMusicVolume(int percent);
					static void SetSFXVolume(int percent);

					static GameState			MusicCategory;
					static Audio				AudioEngineInstance;

					static bool IsMusicStarted;
					static bool IsSFXStarted;

					static int MusicVolume;
					static int SFXVolume;

				private:
					static void SetVolume();

			};
		}
	}
}
