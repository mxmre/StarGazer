#pragma once
#include "BasicMathTypes.h"
namespace sg
{
	namespace math
	{
		template<class _Type>
		class ResizableObject : protected sg::math::tSize<_Type>
		{
		public:
			explicit ResizableObject(const _Type w = static_cast<_Type>(0), const _Type h = static_cast<_Type>(0)) : sg::math::tSize<_Type>{w, h} {}

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
			explicit Coord2dObject(const _Type x = static_cast<_Type>(0), const _Type y = static_cast<_Type>(0)) : sg::math::tPoint2d<_Type>{ x, y } {}

			virtual const _Type X() const
			{
				return this->x;
			}
			virtual const _Type Y() const
			{
				return this->y;
			}
			virtual const sg::math::tPoint2d<_Type> Point2d() const
			{
				return sg::math::tPoint2d<_Type>{this->x, this->y};
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
		class Scalable2dObject : protected sg::math::tScale2d<_Type>
		{
		public:
			explicit Scalable2dObject(const _Type xScale = static_cast<_Type>(0), const _Type yScale = static_cast<_Type>(0)) : 
				sg::math::tPoint2d<_Type>{ xScale, yScale } {}

			virtual const _Type XScale() const
			{
				return this->x;
			}
			virtual const _Type YScale() const
			{
				return this->y;
			}
			virtual const sg::math::tScale2d<_Type> Scale2d() const
			{
				return sg::math::tScale2d<_Type>{this->xScale, this->yScale};
			}
			virtual void SetPoint2d(const sg::math::tScale2d<_Type>& scale2d)
			{
				this->xScale = scale2d.xScale;
				this->yScale = scale2d.yScale;
			}
			virtual void SetXScale(const _Type xScale)
			{
				this->xScale = xScale;
			}
			virtual void SetYScale(const _Type yScale)
			{
				this->yScale = yScale;
			}
		};
		template<class _Type>
		class ZRotatableObject
		{
		public:
			explicit ZRotatableObject(const _Type zAngle = static_cast<_Type>(0)) : zAngle_{ zAngle } {}

			virtual const _Type ZAngle() const
			{
				return this->zAngle_;
			}
			virtual void SetZAngle(const _Type zAngle)
			{
				this->zAngle_ = zAngle;
			}
			virtual void ZRotate(const _Type zAddAngle)
			{
				this->zAngle_ += zAddAngle;
			}
		protected:
			_Type zAngle_;
		};
		template<class _Type>
		class RectangularObject : public Coord2dObject<_Type>, public ResizableObject<_Type>
		{
		public:
			explicit RectangularObject(const _Type w = static_cast<_Type>(0), const _Type h = static_cast<_Type>(0), 
				const _Type x = static_cast<_Type>(0),
				const _Type y = static_cast<_Type>(0))
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