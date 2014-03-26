#pragma once
#include "Audio\AudioManager.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			ref class GameWinScreen : public GameScreenBase
			{
			internal:
				GameWinScreen(void);
				GameWinScreen(ScreenManager^);

				virtual void											RenderScreen() override;
				virtual void											Load() override;
				virtual void											Update(float, float) override;
			};
		}
	}
}

