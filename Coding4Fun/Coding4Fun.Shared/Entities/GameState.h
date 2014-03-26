#pragma once

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Entities
		{
			enum class GameState
			{
				GS_PLAYING,
				GS_PAUSED,
				GS_MAIN_MENU,
				GS_LEVEL_SELECT_SINGLE,
				GS_LEVEL_SELECT_MULTI,
				GS_ABOUT_SCREEN,
				GS_GAME_OVER,
				GS_SUBMIT_SCORE,
				GS_TOP_SCORES,
				GS_WIN,
				GS_HOW_TO,
				GS_FULL_WIN
			};
		}
	}
}