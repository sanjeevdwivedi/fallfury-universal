#pragma once
#include "pch.h"

namespace Coding4Fun
{
	namespace FallFury
	{
		namespace Entities
		{
			ref class Button
			{
			internal:
				Button(void);

				float2		Position;
				float2		Size;
				float		PixelDiff;

				Windows::Foundation::Rect	GetBoundingBox()
				{
					return Windows::Foundation::Rect(Position.x - (Size.x / 2),
						Position.y - (Size.y / 2), Size.x, Size.y);
				}
			};
		}
	}
}

