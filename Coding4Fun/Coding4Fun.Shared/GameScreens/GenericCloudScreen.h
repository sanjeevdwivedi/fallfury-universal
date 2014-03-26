#pragma once
#include "Audio\AudioManager.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Screens
		{
			ref class GenericCloudScreen : public GameScreenBase
			{
			internal:
				GenericCloudScreen(void);
				GenericCloudScreen(ScreenManager^ parent);

				virtual void											RenderScreen() override;
				virtual void											Load() override;
				virtual void											Update(float, float) override;
			};
		}
	}
}

