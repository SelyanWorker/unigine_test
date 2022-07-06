#pragma once

#include <cmath>
#include <iostream>

namespace ut
{
	class Vector
	{
	public:
		struct Comparator
		{
			bool operator()(const Vector& left, const Vector& right) const
			{
				if (left.x < right.x)
					return true;

				if (right.x < left.x)
					return false;

				if (left.y < left.y)
					return true;

				return false;
			}
		};

	public:
		explicit Vector(float value = 0)
			: x(value)
			, y(value)
		{}

		Vector(float x, float y)
			: x(x)
			, y(y)
		{}

		float Length() const { return std::sqrt(x * x + y * y); }

		float LengthQuad() const { return x * x + y * y; }

		float Dot(const Vector& v) const { return x * v.x + y * v.y; }

		Vector& Normalize()
		{
			float m_length = Length();
			x /= m_length;
			y /= m_length;
			return *this;
		}

		Vector& Mul(float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		Vector& Add(const Vector& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Vector& operator*=(float scalar) { return this->Mul(scalar); }

		Vector& operator+=(const Vector& v) { return this->Add(v); }

		Vector& operator-=(const Vector& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		Vector operator-() const { return Vector{ -x, -y }; }

		bool operator==(const Vector& v) const { return (x == v.x && y == v.y); }

		Vector& operator=(const Vector& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

	public:
		float x;
		float y;
	};

	std::ostream& operator<<(std::ostream& ostream, const Vector& vector);
	Vector operator+(const Vector& left, const Vector& right);
	Vector operator-(const Vector& left, const Vector& right);
}