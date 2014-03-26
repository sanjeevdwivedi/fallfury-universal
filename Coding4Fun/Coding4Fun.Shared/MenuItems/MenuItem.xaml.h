//
// MenuItem.xaml.h
// Declaration of the MenuItem class
//

#pragma once

#include "MenuItems\MenuItem.g.h"
using namespace Windows::UI::Xaml;
using namespace Platform;

namespace Coding4Fun
{
	namespace FallFury
	{
		public ref class MenuItem sealed
		{
			public:
				MenuItem();
			private:
				static DependencyProperty^ _LabelProperty;	
				double initialBarWidth;
				double maxDeltaSize;
		

			public:
				static property DependencyProperty^ LabelProperty
				{
					DependencyProperty^ get() { return _LabelProperty; }
				}

				property String^ Label
				{
					String^ get() {  return (String^)GetValue(LabelProperty);  }
					void set(String^ value) { SetValue(LabelProperty, value); }
				}

			internal:
				delegate void ButtonSelected(MenuItem^ sender, Platform::String^ params);
				event ButtonSelected^ OnButtonSelected;

			private:
				void ResetElements();

				void Grid_ManipulationCompleted(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^ e);
				void Grid_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e);

				void Grid_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
				void Grid_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		};
	}
}
