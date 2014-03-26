#include "pch.h"
#include "LivingEntity.h"
#include "Helpers\Geometry.h"

using namespace Coding4Fun::FallFury::Characters;
using namespace Coding4Fun::FallFury::Screens;
using namespace Coding4Fun::FallFury::Helpers;

LivingEntity::LivingEntity(GameScreenBase^ container)
{
	DamageDivider = 1.0f;
	HostContainer = container;
}

void LivingEntity::Update(float timeTotal, float timeDelta, float2 position)
{
}

bool LivingEntity::IsWithinScreenBoundaries(float x, float y, Windows::Foundation::Rect screenBounds)
{
	Windows::Foundation::Rect adjusted = Geometry::MoveRectangle(GetBoundingBox(), x, y);

	return screenBounds.IntersectsWith(adjusted);
}

void LivingEntity::ShootAtTarget(float2)
{

}

Windows::Foundation::Rect LivingEntity::GetBoundingBox()
{
	return Windows::Foundation::Rect(Position.x - (Size.x * Scale / 2),
		Position.y - (Size.y * Scale / 2), Size.x * Scale, Size.y * Scale);
}

void LivingEntity::Load()
{

}

void LivingEntity::Render()
{

}

void LivingEntity::InflictDamage(float damage)
{
	CurrentHealth -= (damage / DamageDivider);
}

float2 LivingEntity::GetVelocityLegs(float2 lastTargetTrace)
{
	float bottomLeg = 0.0f;
	float sideLeg = (Position.y - lastTargetTrace.y) / 100.0f;

	bottomLeg = (Position.x - lastTargetTrace.x) / 100.0f;

	float requiredVelocity = 10.0f;

	float hypothenuse = sqrt(bottomLeg * bottomLeg + sideLeg * sideLeg);
	

	float proportionalX = 0.0f;
	float proportionalY = 0.0f;

	proportionalX = (requiredVelocity * bottomLeg) / hypothenuse;
	proportionalY = (sideLeg < 0.0f ? -1 : 1) * sqrt(requiredVelocity * requiredVelocity - proportionalX * proportionalX);
	return float2(proportionalX,proportionalY);
}
