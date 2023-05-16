#pragma once
#include "stdafx.h"
#include "BasicMathObjects.h"
namespace sg
{
	namespace graphics
	{
		class Graphical2dObject : public sg::math::RectangularObject<float>, public sg::math::Scalable2dObject<float>
		{
		public:
			Graphical2dObject(const float w = 0.f, const float h = 0.f,
				const float x = 0.f, const float y = 0.f, 
				const float xScale = 0.f, const float yScale = 0.f) : sg::math::RectangularObject<float>(w, h, x, y), 
				sg::math::Scalable2dObject<float>(xScale, yScale){}
		};
		struct fColorRGBA
		{
			float r, g, b, a;
		};
		struct ColorRGBA8
		{
			uint8_t r, g, b, a;
			operator fColorRGBA()
			{
				return fColorRGBA{ static_cast<float>(r) / 255.f, static_cast<float>(g) / 255.f,
					static_cast<float>(b) / 255.f, static_cast<float>(a) / 255.f };
			}
		};
	}
}