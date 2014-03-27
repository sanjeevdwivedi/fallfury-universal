//
// MenuItem.xaml.cpp
// Implementation of the MenuItem class
//

#include "pch.h"
#include "MenuItem.xaml.h"

using namespace Coding4Fun::FallFury;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Media::Animation;


MenuItem::MenuItem()
{
	InitializeComponent();

	maxDeltaSize = 150;
	initialBarWidth = 350;

	ResetElements();
}

DependencyProperty^ MenuItem::_LabelProperty = 
    DependencyProperty::Register("Label", 
    Platform::String::typeid,
    MenuItem::typeid, 
    nullptr);

void MenuItem::ResetElements()
{
	BitmapImage^ image = ref new BitmapImage(ref new Uri("ms-appx:///MenuItems/single_arrow.png"));
	MenuImage->Source = image;

	((Storyboard^)ControlContainer->Resources->Lookup("ArrowAnimator"))->Stop();

	coverRectangle->Width = initialBarWidth;
	coverActiveRectangle->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
}

void MenuItem::Grid_ManipulationCompleted(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationCompletedRoutedEventArgs^ e)
{
	float diff = e->Cumulative.Translation.X;
	
	if (diff > maxDeltaSize - 50) // slight buffer
	{
#ifndef WP8
		slideMenuMedia->Play();
#endif
		OnButtonSelected(this, Label);
	}

	ResetElements();
}

void MenuItem::Grid_ManipulationDelta(Platform::Object^ sender, Windows::UI::Xaml::Input::ManipulationDeltaRoutedEventArgs^ e)
{
	double diff = e->Cumulative.Translation.X;
	
	if (diff > maxDeltaSize)
	{
		diff = maxDeltaSize;
	}
	else if (diff < 0)
	{
		diff = 0;
	}

	coverRectangle->Width = initialBarWidth + diff;
}

void MenuItem::Grid_PointerPressed(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	BitmapImage^ image = ref new BitmapImage(ref new Uri("ms-appx:///MenuItems/double_arrow.png"));
	MenuImage->Source = image;

	coverActiveRectangle->Visibility = Windows::UI::Xaml::Visibility::Visible;
	((Storyboard^)ControlContainer->Resources->Lookup("ArrowAnimator"))->Begin();

// TODO: (sanjeevd) FIX this in WP8
#ifndef WP8
	coreMenuMedia->Play();
#endif
}

void MenuItem::Grid_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	ResetElements();
}
