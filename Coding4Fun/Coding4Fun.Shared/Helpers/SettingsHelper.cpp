#include "pch.h"
#include "SettingsHelper.h"

using namespace Windows::Storage;
using namespace Coding4Fun::FallFury::Helpers;

SettingsHelper::SettingsHelper(void)
{
}

void SettingsHelper::Save(Platform::String^ key, Platform::Object^ value)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	values->Insert(key, value);
}

Platform::Object^ SettingsHelper::Read(Platform::String^ key)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	return values->Lookup(key);
}

bool SettingsHelper::Exists(Platform::String^ key)
{
	ApplicationDataContainer^ localSettings = ApplicationData::Current->LocalSettings;
	auto values = localSettings->Values;
	return values->HasKey(key);
}

