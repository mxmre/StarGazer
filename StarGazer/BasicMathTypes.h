#pragma once
#include "stdafx.h"
namespace sg
{
	namespace math
	{
		template<typename type>
		struct tSize
		{
			type w, h;
		};
		
		template<typename type>
		struct tPoint2d
		{
			type x, y;
		};
		template<typename type>
		struct tScale2d
		{
			type xScale, yScale;
		};
		template<typename type>
		struct tRect
		{
			type x, y;
			type w, h;
		};
		using iSize = tSize<int32_t>;
		using uSize = tSize<uint32_t>;
		using fSize = tSize<float>;
		using dSize = tSize<double>;

		using iPoint2d = tPoint2d<int32_t>;
		using uPoint2d = tPoint2d<uint32_t>;
		using fPoint2d = tPoint2d<float>;
		using dPoint2d = tPoint2d<double>;

		using iRect = tRect<int32_t>;
		using uRect = tRect<uint32_t>;
		using fRect = tRect<float>;
		using dRect = tRect<double>;

		using iScale2d = tScale2d<int32_t>;
		using uScale2d = tScale2d<uint32_t>;
		using fScale2d = tScale2d<float>;
		using dScale2d = tScale2d<double>;
	}

}