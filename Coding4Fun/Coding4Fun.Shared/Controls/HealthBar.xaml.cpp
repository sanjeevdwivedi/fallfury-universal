//
// HealthBar.xaml.cpp
// Implementation of the HealthBar class
//

#include "pch.h"
#include "HealthBar.xaml.h"

using namespace Coding4Fun::FallFury::Controls;

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
using namespace Windows::UI::Xaml::Shapes;

HealthBar::HealthBar()
{
	InitializeComponent();
}

DependencyProperty^ HealthBar::_MaxHealthProperty = 
    DependencyProperty::Register("MaxHealth", 
    double::typeid,
	HealthBar::typeid, 
    nullptr);

DependencyProperty^ HealthBar::_CurrentHealthProperty = 
    DependencyProperty::Register("CurrentHealth", 
    double::typeid,
	HealthBar::typeid, 
    nullptr);

void HealthBar::Update(double currentHealth, double maxHealth)
{
	CurrentHealth = currentHealth;
	MaxHealth = maxHealth;

	if (CurrentHealth >= 0)
	{
		OverlayStacker->Height = 400.0 - ((400.0 * CurrentHealth) / MaxHealth);
	}
}