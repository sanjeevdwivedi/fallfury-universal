#pragma once
#include "pch.h"

struct Particle
{
	Particle(float2 size);
	Particle(float2 size, float4 shading);
	float2										Size;
	float2										Position;
	float2										Velocity;
	float4										Shading;
	float										Rotation;
	float										RotationVelocity;
	float										Scale;

	bool IsWithinScreenBoundaries(float x, float y, Windows::Foundation::Rect screenBounds);
	Windows::Foundation::Rect GetBoundingBox();
};

