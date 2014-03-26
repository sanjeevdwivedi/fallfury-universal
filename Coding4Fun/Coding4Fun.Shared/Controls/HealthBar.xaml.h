//
// HealthBar.xaml.h
// Declaration of the HealthBar class
//

#pragma once

#include "Controls\HealthBar.g.h"
using namespace Windows::UI::Xaml;
using namespace Platform;

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Controls
		{
			public ref class HealthBar sealed
			{
				public:
					HealthBar();
					void Update(double currentHealth, double maxHealth);

				private:
					static DependencyProperty^ _MaxHealthProperty;
					static DependencyProperty^ _CurrentHealthProperty;

					double rectangleHealthAllocation;
					double bars;
					int fullBars;
					double percentageOfLastBar;

				public:
					static property DependencyProperty^ MaxHealthProperty
					{
						DependencyProperty^ get() { return _MaxHealthProperty; }
					}

					property double MaxHealth
					{
						double get() { return (double)GetValue(MaxHealthProperty); }
						void set(double value) { SetValue(MaxHealthProperty, value); }
					}

					static property DependencyProperty^ CurrentHealthProperty
					{
						DependencyProperty^ get() { return _CurrentHealthProperty; }
					}

					property double CurrentHealth
					{
						double get() { return (double)GetValue(CurrentHealthProperty); }
						void set(double value) { SetValue(CurrentHealthProperty, value); }
					}

			};
		}
	}
}
