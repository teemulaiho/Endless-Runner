/*
Taken from Helium Math
*/

#pragma once

#include <math.h>
#include <cmath>

#define PI    3.14159265358979323846264338327950288f
#define RAD2DEG 180.0f / PI


namespace Helium
{
	// @math-types
	struct Vector2
	{
		Vector2();
		Vector2(const Vector2& rhs);
		Vector2(const float x, const float y);

		Vector2 operator-() const;
		Vector2& operator=(const Vector2& rhs);
		Vector2 operator+(const Vector2& rhs) const;
		Vector2 operator-(const Vector2& rhs) const;
		Vector2 operator*(const Vector2& rhs) const;
		Vector2 operator*(const float rhs) const;
		Vector2 operator/(const float rhs) const;

		[[nodiscard]] Vector2 perp_right() const;
		[[nodiscard]] Vector2 perp_left() const;
		[[nodiscard]] Vector2 unit() const;
		[[nodiscard]] float length() const;
		[[nodiscard]] float length_squared() const;
		[[nodiscard]] float dot(const Vector2& rhs) const;
		// note: this is also called perp-product
		[[nodiscard]] float cross(const Vector2& rhs) const;
		void normalize();

		float x_;
		float y_;
	};

	struct Matrix3
	{
		static Matrix3 translate(const Vector2& position);
		static Matrix3 translate(const float x, const float y);
		static Matrix3 scale(const Vector2& axis);
		static Matrix3 scale(const float x, const float y);
		static Matrix3 rotate(const float degrees);

		Matrix3();
		Matrix3(const Matrix3& rhs);
		Matrix3(const float m00, const float m01, const float m02,
			const float m10, const float m11, const float m12,
			const float m20, const float m21, const float m22);

		Matrix3& operator=(const Matrix3& rhs);
		Matrix3 operator*(const Matrix3& rhs) const;

		[[nodiscard]] Vector2 transform(const Vector2& rhs) const;
		[[nodiscard]] Vector2 transform_direction(const Vector2& rhs) const;
		[[nodiscard]] float transform_rotation(const float radians) const;

		float m00_, m01_, m02_;
		float m10_, m11_, m12_;
		float m20_, m21_, m22_;
	};


	inline Vector2::Vector2()
		: x_(0.0f)
		, y_(0.0f)
	{
	}

	inline Vector2::Vector2(const Vector2& rhs)
		: x_(rhs.x_)
		, y_(rhs.y_)
	{
	}

	inline Vector2::Vector2(const float x,
		const float y)
		: x_(x)
		, y_(y)
	{
	}

	inline Vector2& Vector2::operator=(const Vector2& rhs)
	{
		x_ = rhs.x_;
		y_ = rhs.y_;

		return *this;
	}

	inline Vector2 Vector2::operator-() const
	{
		return Vector2(-x_, -y_);
	}

	inline Vector2 Vector2::operator+(const Vector2& rhs) const
	{
		return Vector2(x_ + rhs.x_, y_ + rhs.y_);
	}

	inline Vector2 Vector2::operator-(const Vector2& rhs) const
	{
		return Vector2(x_ - rhs.x_, y_ - rhs.y_);
	}

	inline Vector2 Vector2::operator*(const Vector2& rhs) const
	{
		return Vector2(x_ * rhs.x_, y_ * rhs.y_);
	}

	inline Vector2 Vector2::operator*(const float rhs) const
	{
		return Vector2(x_ * rhs, y_ * rhs);
	}

	inline Vector2 Vector2::operator/(const float rhs) const
	{
		return Vector2(x_ / rhs, y_ / rhs);
	}

	inline Vector2 Vector2::perp_right() const
	{
		return Vector2(-y_, x_);
	}

	inline Vector2 Vector2::perp_left() const
	{
		return Vector2(y_, -x_);
	}

	inline Vector2 Vector2::unit() const
	{
		Vector2 result(*this);
		result.normalize();
		return result;
	}

	inline float Vector2::length() const
	{
		return sqrtf(x_ * x_ + y_ * y_);
	}

	inline float Vector2::length_squared() const
	{
		return x_ * x_ + y_ * y_;
	}

	inline float Vector2::dot(const Vector2& rhs) const
	{
		return x_ * rhs.x_ + y_ * rhs.y_;
	}

	inline float Vector2::cross(const Vector2& rhs) const
	{
		return x_ * rhs.y_ - y_ * rhs.x_;
	}

	inline void Vector2::normalize()
	{
		float len = length();
		if (len > 0.0f)
		{
			x_ /= len;
			y_ /= len;
		}
	}

	inline Matrix3 Matrix3::translate(const Vector2& position)
	{
		return Matrix3(1.0f, 0.0f, position.x_,
			0.0f, 1.0f, position.y_,
			0.0f, 0.0f, 1.0f);
	}

	inline Matrix3 Matrix3::translate(const float x, const float y)
	{
		return Matrix3(1.0f, 0.0f, x,
			0.0f, 1.0f, y,
			0.0f, 0.0f, 1.0f);
	}

	inline Matrix3 Matrix3::scale(const Vector2& axis)
	{
		return Matrix3(axis.x_, 0.0f, 0.0f,
			0.0f, axis.y_, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	inline Matrix3 Matrix3::scale(const float x, const float y)
	{
		return Matrix3(x, 0.0f, 0.0f,
			0.0f, y, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	inline Matrix3 Matrix3::rotate(const float degrees)
	{
		const float radians = degrees * 3.141592f / 180.0f;
		const float c = cosf(radians);
		const float s = sinf(radians);

		return Matrix3(c, -s, 0.0f,
			s, c, 0.0f,
			0.0f, 0.0f, 1.0f);
	}

	inline Matrix3::Matrix3()
		: m00_(1.0f), m01_(0.0f), m02_(0.0f)
		, m10_(0.0f), m11_(1.0f), m12_(0.0f)
		, m20_(0.0f), m21_(0.0f), m22_(1.0f)
	{
	}

	inline Matrix3::Matrix3(const Matrix3& rhs)
		: m00_(rhs.m00_), m01_(rhs.m01_), m02_(rhs.m02_)
		, m10_(rhs.m10_), m11_(rhs.m11_), m12_(rhs.m12_)
		, m20_(rhs.m20_), m21_(rhs.m21_), m22_(rhs.m22_)
	{
	}

	inline Matrix3::Matrix3(const float m00, const float m01, const float m02,
		const float m10, const float m11, const float m12,
		const float m20, const float m21, const float m22)
		: m00_(m00), m01_(m01), m02_(m02)
		, m10_(m10), m11_(m11), m12_(m12)
		, m20_(m20), m21_(m21), m22_(m22)
	{
	}

	inline Matrix3& Matrix3::operator=(const Matrix3& rhs)
	{
		m00_ = rhs.m00_; m01_ = rhs.m01_; m02_ = rhs.m02_;
		m10_ = rhs.m10_; m11_ = rhs.m11_; m12_ = rhs.m12_;
		m20_ = rhs.m20_; m21_ = rhs.m21_; m22_ = rhs.m22_;

		return *this;
	}

	inline Matrix3 Matrix3::operator*(const Matrix3& rhs) const
	{
		Matrix3 result;

		result.m00_ = m00_ * rhs.m00_ + m01_ * rhs.m10_ + m02_ * rhs.m20_;
		result.m01_ = m00_ * rhs.m01_ + m01_ * rhs.m11_ + m02_ * rhs.m21_;
		result.m02_ = m00_ * rhs.m02_ + m01_ * rhs.m12_ + m02_ * rhs.m22_;

		result.m10_ = m10_ * rhs.m00_ + m11_ * rhs.m10_ + m12_ * rhs.m20_;
		result.m11_ = m10_ * rhs.m01_ + m11_ * rhs.m11_ + m12_ * rhs.m21_;
		result.m12_ = m10_ * rhs.m02_ + m11_ * rhs.m12_ + m12_ * rhs.m22_;

		result.m20_ = m20_ * rhs.m00_ + m21_ * rhs.m10_ + m22_ * rhs.m20_;
		result.m21_ = m20_ * rhs.m01_ + m21_ * rhs.m11_ + m22_ * rhs.m21_;
		result.m22_ = m20_ * rhs.m02_ + m21_ * rhs.m12_ + m22_ * rhs.m22_;

		return result;
	}

	/*
	m00_ m01_ m02_
	m10_ m11_ m12_
	m20_ m21_ m22_
 */

	inline Vector2 Matrix3::transform(const Vector2& rhs) const
	{
		const float x = m00_ * rhs.x_ + m01_ * rhs.y_ + m02_ * 1.0f;
		const float y = m10_ * rhs.x_ + m11_ * rhs.y_ + m12_ * 1.0f;

		return Vector2(x, y);
	}

	inline Vector2 Matrix3::transform_direction(const Vector2& rhs) const
	{
		const float x = m00_ * rhs.x_ + m01_ * rhs.y_ + m02_ * 0.0f;
		const float y = m10_ * rhs.x_ + m11_ * rhs.y_ + m12_ * 0.0f;

		return Vector2(x, y);
	}
	inline float Matrix3::transform_rotation(const float degrees) const
	{
		const float radians = degrees * 3.141592f / 180.0f;
		auto direction = Vector2(cos(radians), sin(radians));
		auto transformedDirection = transform_direction(direction);

		return atan2(transformedDirection.y_, transformedDirection.x_);

	}
} // Helium
