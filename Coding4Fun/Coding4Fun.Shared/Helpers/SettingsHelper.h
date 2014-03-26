#pragma once

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Helpers
		{
			class SettingsHelper
			{
			public:
				SettingsHelper(void);

				static void Save(Platform::String^ key, Platform::Object^ value);
				static Platform::Object^ Read(Platform::String^ key);
				static bool Exists(Platform::String^ key);
			};
		}
	}
}
