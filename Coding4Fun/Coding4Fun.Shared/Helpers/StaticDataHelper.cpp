#include "pch.h"
#include "StaticDataHelper.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace Coding4Fun::FallFury::Helpers;
using namespace Coding4Fun::FallFury::Models;

Platform::String^	StaticDataHelper::FileName;
bool				StaticDataHelper::IsPaused;
float				StaticDataHelper::SecondsTotal;
int					StaticDataHelper::ButtonsTotal;
int					StaticDataHelper::ButtonsCollected;
int					StaticDataHelper::MonstersKilled;
int					StaticDataHelper::CurrentLevelID;
float				StaticDataHelper::BACKGROUND_SCROLL_SPEED  = 4.0f;
int					StaticDataHelper::Score;

bool				StaticDataHelper::IsAccelerometerEnabled;
bool				StaticDataHelper::IsAccelerometerInverted;
bool				StaticDataHelper::IsMouseEnabled;

String^				StaticDataHelper::CurrentLevelName;

Microsoft::WRL::ComPtr<ID3D11Texture2D>				StaticDataHelper::FlameA;
Microsoft::WRL::ComPtr<ID3D11Texture2D>				StaticDataHelper::FlameB;
Microsoft::WRL::ComPtr<ID3D11Texture2D>				StaticDataHelper::FlameC;
Microsoft::WRL::ComPtr<ID3D11Texture2D>				StaticDataHelper::MonsterWeapon;

Level^		StaticDataHelper::CurrentLevel;
MedalType	StaticDataHelper::Medal;

Platform::String^ StaticDataHelper::GetTimeString(int totalSeconds)
{
	int minutes = static_cast<int>(totalSeconds / 60.0f);
	int seconds = static_cast<int>(totalSeconds - (minutes * 60.0f));
			
	std::stringstream sStream;

	sStream << std::setiosflags(std::ios_base::right) << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;

	return GetManagedStringFromNative(sStream.str());
}

Platform::String^ StaticDataHelper::GetManagedStringFromNative(std::string param)
{
	std::wstring local (param.length(),L'');
	std::copy(param.begin(),param.end(),local.begin());
  	return ref new String(local.c_str());
}
