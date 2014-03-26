#pragma once

#include "pch.h"

using namespace Coding4Fun::FallFury;

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Helpers
		{
			ref class TopScoreLoader
			{
			internal:
				TopScoreLoader(void);
				TopScoreLoader(Coding4Fun::FallFury::Models::TopScoreSet^ set);
				Windows::Foundation::Collections::IVector<TopScore^>^ Extract();

			private:
				Coding4Fun::FallFury::Models::TopScoreSet^ m_currentEntrySet;
			};
		}
	}
}
