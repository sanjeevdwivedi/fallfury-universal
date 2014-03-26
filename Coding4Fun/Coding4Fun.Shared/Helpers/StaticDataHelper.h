#pragma once
#include <collection.h>

using namespace Coding4Fun::FallFury::Models;

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Helpers
		{
			ref class StaticDataHelper abstract
			{
			internal:
				static float BACKGROUND_SCROLL_SPEED;

				static Platform::String^ FileName;
				static bool IsPaused;

				// These flags mirror the properties in DXPage to 
				// allow in-game tracking of the accelerometer state.
				static bool IsAccelerometerEnabled;
				static bool IsAccelerometerInverted;
				static bool IsMouseEnabled;


				static float SecondsTotal;
				static int ButtonsTotal;
				static int ButtonsCollected;
				static int MonstersKilled;
				static int Score;
				static MedalType Medal;

				static Platform::String^ GetTimeString(int);
				static Platform::String^ GetManagedStringFromNative(std::string);
		
				static int CurrentLevelID;
				static int TotalLevelCount;

				static Microsoft::WRL::ComPtr<ID3D11Texture2D>					FlameA;
				static Microsoft::WRL::ComPtr<ID3D11Texture2D>					FlameB;
				static Microsoft::WRL::ComPtr<ID3D11Texture2D>					FlameC;

				// Weapon texture set
				static Microsoft::WRL::ComPtr<ID3D11Texture2D>					MonsterWeapon;

				static Coding4Fun::FallFury::Models::Level^			CurrentLevel;
				static Platform::String^										CurrentLevelName;
			};
		}
	}
}

