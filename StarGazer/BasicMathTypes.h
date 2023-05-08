#pragma once
#include <cstdint>
namespace sg
{
	namespace math
	{
		struct Size
		{
			uint32_t w, h;
		};
		struct Point2d
		{
			uint32_t x, y;
		};
		struct Rect
		{
			uint32_t x, y;
			uint32_t w, h;
		};
	}

}