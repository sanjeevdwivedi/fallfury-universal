#pragma once
#include "GameScreens\GamePlayScreen.h"

using namespace Windows::Graphics::Display;

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Helpers
		{
			using namespace Coding4Fun::FallFury::Entities;
			using namespace Coding4Fun::FallFury::Characters;

			ref class LevelDataLoader
			{
			internal:
				LevelDataLoader(void);
				LevelDataLoader(Coding4Fun::FallFury::Models::Level^ level, Coding4Fun::FallFury::Screens::GamePlayScreen^ screen);

				property Coding4Fun::FallFury::Screens::GamePlayScreen^		PlayScreen;
				property Coding4Fun::FallFury::Models::Level^	CurrentLevel;
	
				void SetupBear(Bear^ gameBear);
				void SetupObstacles(std::list<Obstacle> &obstacles);
				void SetupMonsters(std::list<Monster^> &monsters);
				void SetupButtons(std::list<Button^> &buttons, int &buttonPrice);
				void SetupPowerups(std::list<Powerup^> &powerups);

			private:
				float GetXPosition(float percentage);
			};
		}
	}
}

