#ifndef LX_VECTORS_H
#define LX_VECTORS_H

#include <math.h>
#include "lxHack.h"	// And includes lxTypes.h

namespace lx {

class Vec2D {
public:
	float x;
	float y;
	
	Vec2D(float value) 
	:x(value)
	,y(value)
	{ }

	Vec2D(float x, float y) 
	:x(x)
	,y(y)
	{ }

	Vec2D(const Vec2D& vec) 
	:x(vec.x)
	,y(vec.y)
	{ }

	inline Vec2D operator-(void) const {
		return Vec2D(-x, -y);
	}

	inline Vec2D operator+(const Vec2D& v) const {
		return Vec2D( x + v.x , y + v.y);
	}

	inline Vec2D operator-(const Vec2D& v) const {
		return Vec2D( x - v.x, y - v.y);
	}

	inline Vec2D operator*(const Vec2D& v) const {
		return Vec2D( x * v.x, y * v.y);
	}

	inline Vec2D operator/(const Vec2D& v) const {
		return Vec2D( x / v.x, y / v.y );
	}

	inline Vec2D operator+=(const Vec2D& v) {
		x += v.x;
		y += v.y;
		return (*this);
	}

	inline Vec2D operator-=(const Vec2D& v) {
		x -= v.x;
		y -= v.y;
		return (*this);
	}

	inline Vec2D operator*=(const Vec2D& v) {
		x *= v.x;
		y *= v.y;
		return (*this);
	}

	inline Vec2D operator/=(const Vec2D& v) {
		x /= v.x;
		y /= v.y;
		return (*this);
	}

	inline Vec2D operator+(float f) const {
		return Vec2D( x + f, y + f );
	}

	inline Vec2D operator-(float f) const {
		return Vec2D( x - f, y - f );
	}

	inline Vec2D operator*(float f) const {
		return Vec2D( x * f, y * f );
	}

	inline Vec2D operator/(float f) const {
		float invF = 1.0f / f;
		return Vec2D( x * invF, y * invF );
	}
	
	inline Vec2D operator+=(float f) {
		x += f;
		y += f;
		return (*this);
	}

	inline Vec2D operator-=(float f) {
		x -= f;
		y -= f;
		return (*this);
	}

	inline Vec2D operator*=(float f) {
		x *= f;
		y *= f;
		return (*this);
	}

	inline Vec2D operator/=(float f) {
		float invF = 1.0f / f;
		x *= invF;
		y *= invF;
		return (*this);
	}

	inline float Dot(const Vec2D& v) {
		return (x * v.x) + (y * v.y);
	}

	inline Vec2D Cross(const Vec2D& v) {
		return Vec2D(x * v.y, y * v.x);
	}
    
	inline float Length() {
		return sqrt(LengthSqr());
	}

	inline float LengthSqr() {
		return this->Dot(*this);
	}

	inline Vec2D NormalizeApprox() {
		float lengthSqr = (x * x) + (y * y);
		float invLength = lxFloat::ApproxReciproqualSQRT(lengthSqr);
		return Vec2D( x * invLength, y * invLength );
	}

	inline Vec2D Normalize() {
		float lengthSqr = (x * x) + (y * y);
		float invLength = 1.0f / sqrt(lengthSqr);
		return Vec2D( x * invLength, y * invLength );
	}

	inline static
	float SqrDistancePointPoint	(const Vec2D& pointA, const Vec2D& pointB) {
		Vec2D dif = pointA - pointB;
		return dif.LengthSqr();
	}

	inline static
	/** Return value between [0..1] if point perpendicular projection is inside segment */
	float GetProjectionPosFactor(const Vec2D& point, const Vec2D& startSegment, const Vec2D& endSegment) {
		Vec2D segment = endSegment - startSegment;
		return ((point - startSegment).Dot(segment)) / (segment.LengthSqr());	
	}

	inline static
	Vec2D GetPoint				(const Vec2D& startSegment, const Vec2D& endSegment, float factor) {
		Vec2D segment = endSegment - startSegment;
		return startSegment + (segment * factor);
	}

};

class Vec3D {
public:
	float x;
	float y;
	float z;
	
	Vec3D(float value) 
	:x(value)
	,y(value)
	,z(value)
	{ }

	Vec3D(float x, float y, float z) 
	:x(x)
	,y(y)
	,z(z)
	{ }

	Vec3D(const Vec3D& vec) 
	:x(vec.x)
	,y(vec.y)
	,z(vec.z)
	{ }

	inline static
	float SqrDistancePointPoint	(const Vec3D& pointA, const Vec3D& pointB) {
		Vec3D dif = pointA - pointB;
		return dif.LengthSqr();
	}

	inline static
	/** Return value between [0..1] if point perpendicular projection is inside segment */
	float GetProjectionPosFactor(const Vec3D& point, const Vec3D& startSegment, const Vec3D& endSegment) {
		Vec3D segment = endSegment - startSegment;
		return ((point - startSegment).Dot(segment)) / (segment.LengthSqr());	
	}

	inline static
	Vec3D GetPoint				(const Vec3D& startSegment, const Vec3D& endSegment, float factor) {
		Vec3D segment = endSegment - startSegment;
		return startSegment + (segment * factor);
	}

	inline Vec3D operator-(void) const {
		return Vec3D(-x, -y, -z);
	}

	inline Vec3D operator+(const Vec3D& v) const {
		return Vec3D( x + v.x , y + v.y, z + v.z);
	}

	inline Vec3D operator-(const Vec3D& v) const {
		return Vec3D( x - v.x, y - v.y, z - v.z);
	}

	inline Vec3D operator*(const Vec3D& v) const {
		return Vec3D( x * v.x, y * v.y, z * v.z);
	}

	inline Vec3D operator/(const Vec3D& v) const {
		return Vec3D( x / v.x, y / v.y, z / v.z);
	}

	inline Vec3D operator+=(const Vec3D& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return (*this);
	}

	inline Vec3D operator-=(const Vec3D& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return (*this);
	}

	inline Vec3D operator*=(const Vec3D& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return (*this);
	}

	inline Vec3D operator/=(const Vec3D& v) {
		x /= v.x;
		y /= v.y;
		return (*this);
	}

	inline Vec3D operator+(float f) const {
		return Vec3D( x + f, y + f , z + f);
	}

	inline Vec3D operator-(float f) const {
		return Vec3D( x - f, y - f , z - f);
	}

	inline Vec3D operator*(float f) const {
		return Vec3D( x * f, y * f , z * f);
	}

	inline Vec3D operator/(float f) const {
		float invF = 1.0f / f;
		return Vec3D( x * invF, y * invF , z * invF );
	}
	
	inline Vec3D operator+=(float f) {
		x += f;
		y += f;
		z += f;
		return (*this);
	}

	inline Vec3D operator-=(float f) {
		x -= f;
		y -= f;
		z -= f;
		return (*this);
	}

	inline Vec3D operator*=(float f) {
		x *= f;
		y *= f;
		z *= f;
		return (*this);
	}

	inline Vec3D operator/=(float f) {
		float invF = 1.0f / f;
		x *= invF;
		y *= invF;
		z *= invF;
		return (*this);
	}

	inline float Dot(const Vec3D& v) {
		return (x * v.x) + (y * v.y) + (z * v.z);
	}

	inline Vec3D Cross(const Vec3D& v) {
        float t, u;
        t = (y*v.z) - (z*v.y);
        u = (z*v.x) - (x*v.z);
        z = (x*v.y) - (y*v.x);
        x = t;
        y = u;    
        return (*this);
	}
    
	inline float Length() {
		return sqrt(LengthSqr());
	}

	inline float LengthSqr() {
		return this->Dot(*this);
	}

	inline Vec3D NormalizeApprox() {
		float lengthSqr = (x * x) + (y * y);
		float invLength = lx::lxFloat::ApproxReciproqualSQRT(lengthSqr);
		return Vec3D( x * invLength, y * invLength, z * invLength );
	}

	inline Vec3D Normalize() {
		float lengthSqr = (x * x) + (y * y);
		float invLength = 1.0f / sqrt(lengthSqr);
		return Vec3D( x * invLength, y * invLength , z * invLength );
	}
};

}

#endif // LX_VECTORS_H
