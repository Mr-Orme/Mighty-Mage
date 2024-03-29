#pragma once
//------------------------------------------------------------------------
//
//  Name:   Vector2D.h
//
//  Desc:   2D vector struct
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <cmath>
#include <algorithm>
#include <iosfwd>
#include <limits>


const float Pi = 3.14159f;
struct Vector2D
{
	int x{ 0 };
	int y{ 0 };

	//sets x and y to zero
	void Zero() { x = 0; y = 0; }

	//returns true if both x and y are zero
	bool isZero()const { return (x*x + y * y) ==0; }

	//returns the length of the vector
	inline int    Length()const;

	//returns the squared length of the vector (thereby avoiding the sqrt)
	inline int    LengthSq()const;

	inline void      Normalize();

	inline int    Dot(const Vector2D& v2)const;

	//returns positive if v2 is clockwise of this vector,
	//negative if anticlockwise (assuming the Y axis is pointing down,
	//X axis to right like a Window app)
	inline int       Sign(const Vector2D& v2)const;

	//returns the vector that is perpendicular to this one.
	inline Vector2D  Perp()const;

	//adjusts x and y so that the length of the vector does not exceed max
	inline void      Truncate(float max);

	//returns the distance between this vector and th one passed as a parameter
	inline int    Distance(const Vector2D &v2)const;

	//squared version of above.
	inline int    DistanceSq(const Vector2D &v2)const;

	inline void      Reflect(const Vector2D& norm);

	//returns the vector that is the reverse of this vector
	inline Vector2D  GetReverse()const;


	int getAngleRadians(const Vector2D & v2)
	{
		return (int)acos(Dot(v2));
	}
	int getAngleDegrees(const Vector2D & v2)
	{
		return (int)(getAngleRadians(v2) * 180 / Pi);
	}
	//we need some overloaded operators
	
	Vector2D& operator++()
	{
		x++;
		y++;
		
		return *this;
	}
	Vector2D operator++(int)
	{
		Vector2D temp = *this;
		++* this;
		return temp;
	}
	const Vector2D& operator+=(const Vector2D &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}

	const Vector2D& operator+=(const int& rhs)
	{
		x += rhs;
		y += rhs;

		return *this;
	}

	const Vector2D& operator-=(const Vector2D &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}

	const Vector2D& operator*=(const int& rhs)
	{
		x *= rhs;
		y *= rhs;

		return *this;
	}
	const Vector2D& operator*=(const float& rhs)
	{
		x = (int)(x * rhs);
		y = (int)(y * rhs);

		return *this;
	}

	const Vector2D& operator/=(const int& rhs)
	{
		x /= rhs;
		y /= rhs;

		return *this;
	}

	bool operator==(const Vector2D& rhs)const
	{
		return (x == rhs.x) && (y == rhs.y);
	}

	bool operator!=(const Vector2D& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y);
	}

};

//-----------------------------------------------------------------------some more operator overloads
inline Vector2D operator*(const Vector2D &lhs, int rhs);
inline Vector2D operator*(float lhs, const Vector2D &rhs);
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs);
inline Vector2D operator/(const Vector2D &lhs, int val);
std::ostream& operator<<(std::ostream& os, const Vector2D& rhs);
std::ifstream& operator>>(std::ifstream& is, Vector2D& lhs);

bool operator<(const Vector2D& srcL, const Vector2D& srcR);


//------------------------------------------------------------------------member functions

//------------------------- Length ---------------------------------------
//
//  returns the length of a 2D vector
//------------------------------------------------------------------------
inline int Vector2D::Length()const
{
	return (int)sqrt(x * x + y * y);
}


//------------------------- LengthSq -------------------------------------
//
//  returns the squared length of a 2D vector
//------------------------------------------------------------------------
inline int Vector2D::LengthSq()const
{
	return (x * x + y * y);
}


//------------------------- Vec2DDot -------------------------------------
//
//  calculates the dot product
//------------------------------------------------------------------------
inline int Vector2D::Dot(const Vector2D &v2)const
{
	return x * v2.x + y * v2.y;
}

//------------------------ Sign ------------------------------------------
//
//  returns positive if v2 is clockwise of this vector,
//  minus if anticlockwise (Y axis pointing down, X axis to right)
//------------------------------------------------------------------------
enum { clockwise = 1, anticlockwise = -1 };

inline int Vector2D::Sign(const Vector2D& v2)const
{
	if (y*v2.x > x*v2.y)
	{
		return anticlockwise;
	}
	else
	{
		return clockwise;
	}
}

//------------------------------ Perp ------------------------------------
//
//  Returns a vector perpendicular to this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::Perp()const
{
	return { -y, x };
}

//------------------------------ Distance --------------------------------
//
//  calculates the euclidean distance between two vectors
//------------------------------------------------------------------------
inline int Vector2D::Distance(const Vector2D &v2)const
{
	int ySeparation{ v2.y - y };
	int xSeparation{ v2.x - x };

	return (int)sqrt(ySeparation*ySeparation + xSeparation * xSeparation);
}


//------------------------------ DistanceSq ------------------------------
//
//  calculates the euclidean distance squared between two vectors 
//------------------------------------------------------------------------
inline int Vector2D::DistanceSq(const Vector2D &v2)const
{
	int ySeparation{ v2.y - y };
	int xSeparation{ v2.x - x };

	return ySeparation * ySeparation + xSeparation * xSeparation;
}

//----------------------------- Truncate ---------------------------------
//
//  truncates a vector so that its length does not exceed max
//------------------------------------------------------------------------
inline void Vector2D::Truncate(float max)
{
	if (this->Length() > max)
	{
		this->Normalize();

		*this *= max;
	}
}

//--------------------------- Reflect ------------------------------------
//
//  given a normalized vector this method reflects the vector it
//  is operating upon. (like the path of a ball bouncing off a wall)
//------------------------------------------------------------------------
inline void Vector2D::Reflect(const Vector2D& norm)
{
	*this += 2.0f * this->Dot(norm) * norm.GetReverse();
}

//----------------------- GetReverse ----------------------------------------
//
//  returns the vector that is the reverse of this vector
//------------------------------------------------------------------------
inline Vector2D Vector2D::GetReverse()const
{
	return { -this->x, -this->y };
}


//------------------------- Normalize ------------------------------------
//
//  normalizes a 2D Vector
//------------------------------------------------------------------------
inline void Vector2D::Normalize()
{
	int vector_length = this->Length();

	if (vector_length > std::numeric_limits<int>::epsilon())
	{
		this->x = (int)(this->x / vector_length);
		this->y = (int)(this->y / vector_length);
	}
}


//------------------------------------------------------------------------non member functions

inline Vector2D Vec2DNormalize(const Vector2D &v)
{
	Vector2D vec = v;

	int vector_length = (int)vec.Length();

	if (vector_length > std::numeric_limits<float>::epsilon())
	{
		vec.x /= vector_length;
		vec.y /= vector_length;
	}

	return vec;
}


inline double Vec2DDistance(const Vector2D &v1, const Vector2D &v2)
{

	int ySeparation = v2.y - v1.y;
	int xSeparation = v2.x - v1.x;

	return sqrt(ySeparation*ySeparation + xSeparation * xSeparation);
}

inline int Vec2DDistanceSq(const Vector2D &v1, const Vector2D &v2)
{

	int ySeparation = v2.y - v1.y;
	int xSeparation = v2.x - v1.x;

	return ySeparation * ySeparation + xSeparation * xSeparation;
}

inline double Vec2DLength(const Vector2D& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

inline int Vec2DLengthSq(const Vector2D& v)
{
	return (v.x*v.x + v.y*v.y);
}


//inline Vector2D POINTStoVector(const POINTS& p)
//{
	//return Vector2D(p.x, p.y);
//}

//inline Vector2D POINTtoVector(const POINT& p)
//{
	//return Vector2D((float)p.x, (float)p.y);
//}

//inline POINTS VectorToPOINTS(const Vector2D& v)
//{
	//POINTS p;
	//p.x = (short)v.x;
	//p.y = (short)v.y;

	//return p;
//}

//inline POINT VectorToPOINT(const Vector2D& v)
//{
	//POINT p;
	//p.x = (long)v.x;
	//p.y = (long)v.y;

	//return p;
//}



//------------------------------------------------------------------------operator overloads
inline Vector2D operator*(const Vector2D &lhs, int rhs)
{
	Vector2D result(lhs);
	result *= rhs;
	return result;
}

inline Vector2D operator*(float lhs, const Vector2D &rhs)
{
	Vector2D result(rhs);
	result *= lhs;
	return result;
}

//overload the - operator
inline Vector2D operator-(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x -= rhs.x;
	result.y -= rhs.y;

	return result;
}

//overload the + operator
inline Vector2D operator+(const Vector2D &lhs, const Vector2D &rhs)
{
	Vector2D result(lhs);
	result.x += rhs.x;
	result.y += rhs.y;

	return result;
}

//overload the / operator
inline Vector2D operator/(const Vector2D &lhs, int val)
{
	Vector2D result(lhs);
	result.x /= val;
	result.y /= val;

	return result;
}

///////////////////////////////////////////////////////////////////////////////


//treats a window as a toroid
inline void WrapAround(Vector2D &pos, int MaxX, int MaxY)
{
	if (pos.x > MaxX) { pos.x = 0; }

	if (pos.x < 0) { pos.x = MaxX; }

	if (pos.y < 0) { pos.y = MaxY; }

	if (pos.y > MaxY) { pos.y = 0; }
}

//returns true if the point p is not inside the region defined by top_left
//and bot_rgt
inline bool NotInsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return (p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y);
}

inline bool InsideRegion(Vector2D p,
	Vector2D top_left,
	Vector2D bot_rgt)
{
	return !((p.x < top_left.x) || (p.x > bot_rgt.x) ||
		(p.y < top_left.y) || (p.y > bot_rgt.y));
}

inline bool InsideRegion(Vector2D p, int left, int top, int right, int bottom)
{
	return !((p.x < left) || (p.x > right) || (p.y < top) || (p.y > bottom));
}

//------------------ isSecondInFOVOfFirst -------------------------------------
//
//  returns true if the target position is in the field of view of the entity
//  positioned at posFirst facing in facingFirst
//-----------------------------------------------------------------------------
inline bool isSecondInFOVOfFirst(Vector2D posFirst,
	Vector2D facingFirst,
	Vector2D posSecond,
	float    fov)
{
	Vector2D toTarget = Vec2DNormalize(posSecond - posFirst);

	return facingFirst.Dot(toTarget) >= cos(fov / 2.0);
}





