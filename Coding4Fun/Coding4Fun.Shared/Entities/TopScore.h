#pragma once

#include "pch.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Data;
using namespace Coding4Fun::FallFury::Models;

namespace Coding4Fun
{
	namespace FallFury
	{
		public ref class TopScore sealed
		{
		internal:
			TopScore(void);

		public:
			property String^ Name
			{
				String^ get()
				{ 
					return _name;
				}
				void set(String^ value)
				{
					_name = value;
				}
			}     

			property int Score
			{
				int get()
				{ 
					return _score;
				}
				void set(int value)
				{
					_score = value;
				}
			}     

			property String^ Level
			{
				String^ get()
				{ 
					return _level;
				}
				void set(String^ value)
				{
					_level = value;
				}
			}     

			property Models::MedalType ScoreMedalType
			{
				Models::MedalType get()
				{ 
					return _medal;
				}
				void set(Models::MedalType value)
				{
					_medal = value;
				}
			}

		private:
			String^ _name;
			int _score;
			String^ _level;
			Models::MedalType _medal;
		};
	}
}