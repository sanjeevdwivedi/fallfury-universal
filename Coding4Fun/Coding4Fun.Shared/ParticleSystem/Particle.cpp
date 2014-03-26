#include "pch.h"
#include "Particle.h"
#include "Helpers\Geometry.h"

using namespace Coding4Fun::FallFury::Helpers;

Particle::Particle(float2 size, float4 shading)
{
	Size = size; 
	Shading = shading; 
	Position = float2(0, 0);
	Velocity = float2(RandFloat(-2.0f, 2.0f), RandFloat(-2.0f, 2.0f));
	Rotation = 0;
	RotationVelocity = 0.2f;
	Scale = 1.0f;
}

Particle::Particle(float2 size)
{
	Size = size;
	Shading = float4(1.0f, 1.0f, 1.0f, 1.0f);
	Position = float2(0, 0);
	Velocity = float2(RandFloat(-2.0f, 2.0f), RandFloat(-2.0f, 2.0f));
	Rotation = 0;
	RotationVelocity = 0.2f;
	Scale = 1.0f;
}

bool Particle::IsWithinScreenBoundaries(float x, float y, Windows::Foundation::Rect screenBounds)
{
	Windows::Foundation::Rect adjusted = Geometry::MoveRectangle(GetBoundingBox(), x, y);
	return screenBounds.IntersectsWith(adjusted);
}

Windows::Foundation::Rect Particle::GetBoundingBox()
{
	return Windows::Foundation::Rect(Position.x - (Size.x / 2.0f),
		Position.y - (Size.y / 2.0f), Size.x, Size.y);
}
