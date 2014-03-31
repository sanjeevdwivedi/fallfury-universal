#pragma once

#include "Helpers\BasicMath.h"
#include "ObstacleType.h"
#include <string>

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Entities
		{
			struct Obstacle
			{
				float2						Position;
				bool						InflictsDamage;
				float						Rotation;
				float						Scale;
				double						HealthDamage;
				float2						Size;
				ObstacleType				Type;

				double						PixelDiff;

				Windows::Foundation::Rect GetBoundingBox()
				{
					// NOTE: (sanjeevd) This is the place where the scale is defined.
					return Windows::Foundation::Rect(Position.x - (Size.x * Scale / 2.0f),
						Position.y - (Size.y * Scale / 2.0f), Size.x * Scale, Size.y * Scale);
				}
			};
		}
	}
}
