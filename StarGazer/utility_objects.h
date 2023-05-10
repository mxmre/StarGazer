#pragma once
#include "BasicMathTypes.h"
namespace sg
{
	namespace utility
	{
		template<class _Type>
		class ResizableObject : protected sg::math::tSize<_Type>
		{
		public:
			ResizableObject(const _Type w, const _Type h) : sg::math::tSize<_Type>{w, h} {}

			virtual const _Type Width() const
			{
				return this->w;
			}
			virtual const _Type Height() const
			{
				return this->h;
			}
			virtual const sg::math::tSize<_Type> Size() const
			{
				return sg::math::tSize<_Type>{this->w, this->h};
			}
			virtual void SetSize(const sg::math::tSize<_Type>& size)
			{
				this->h = size.h;
				this->w = size.w;
			}
			virtual void SetWidth(const _Type w)
			{
				this->w = w;
			}
			virtual void SetHeight(const _Type h)
			{
				this->h = h;
			}
		};
		template<class _Type>
		class Coord2dObject : protected sg::math::tPoint2d<_Type>
		{
		public:
			Coord2dObject(const _Type x, const _Type y) : sg::math::tPoint2d<_Type>{ x, y } {}

			virtual const _Type X() const
			{
				return this->x;
			}
			virtual const _Type Y() const
			{
				return this->y;
			}
			virtual const sg::math::tSize<_Type> Point2d() const
			{
				return sg::math::tSize<_Type>{this->x, this->y};
			}
			virtual void SetPoint2d(const sg::math::tPoint2d<_Type>& point2d)
			{
				this->x = point2d.x;
				this->y = point2d.y;
			}
			virtual void SetX(const _Type x)
			{
				this->x = x;
			}
			virtual void SetY(const _Type y)
			{
				this->y = y;
			}
		};
		template<class _Type>
		class RectangularObject : public Coord2dObject<_Type>, public ResizableObject<_Type>
		{
		public:
			RectangularObject(const _Type x, const _Type y, const _Type w, const _Type h) 
				: Coord2dObject<_Type>(x,y), ResizableObject<_Type>(w, h) {}
			void SetRect(const sg::math::tRect<_Type>& rect)
			{
				this->x = rect.x;
				this->y = rect.y;
				this->w = rect.w;
				this->h = rect.h;
			}
			const sg::math::tRect<_Type> GetRect() const
			{
				return sg::math::tRect<_Type>{this->x, this->y, this->w, this->h};
			}
		};
	}
}