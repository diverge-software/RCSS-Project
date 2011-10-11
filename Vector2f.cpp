#include "Vector2f.hpp"
#include <iostream>
#include <cmath>

Vector2f::Vector2f()
{
	mComponents[0] = 0.0f;
	mComponents[1] = 0.0f;
}

Vector2f::Vector2f( double x, double y )
{
	mComponents[0] = x;
	mComponents[1] = y;
}

double & Vector2f::operator[]( int index )
{
	return mComponents[index];
}

double Vector2f::operator[]( int index ) const
{
	return mComponents[index];
}

Vector2f Vector2f::operator*( double scale ) const
{
	return Vector2f( mComponents[0] * scale, mComponents[1] * scale );
}

Vector2f Vector2f::operator/( double scale ) const
{
	return Vector2f( mComponents[0] / scale, mComponents[1] / scale );
}

Vector2f Vector2f::operator+( const Vector2f & other ) const
{
    return Vector2f( mComponents[0] + other.mComponents[0],
                     mComponents[1] + other.mComponents[1] );
}

Vector2f Vector2f::operator-( const Vector2f & other ) const
{
    return Vector2f( mComponents[0] - other.mComponents[0],
                     mComponents[1] - other.mComponents[1] );
}

Vector2f Vector2f::operator-() const
{
	return Vector2f( -mComponents[0], -mComponents[1] );
}

const Vector2f & Vector2f::operator*=( double scale )
{
	mComponents[0] *= scale;
	mComponents[1] *= scale;
	return *this;
}

const Vector2f & Vector2f::operator/=( double scale )
{
	mComponents[0] /= scale;
	mComponents[1] /= scale;
	return *this;
}

const Vector2f & Vector2f::operator+=( const Vector2f & other )
{
	mComponents[0] += other.mComponents[0];
	mComponents[1] += other.mComponents[1];
	return *this;
}

const Vector2f & Vector2f::operator-=( const Vector2f & other )
{
	mComponents[0] -= other.mComponents[0];
	mComponents[1] -= other.mComponents[1];
	return *this;
}

double Vector2f::magnitude() const
{
	return sqrt( mComponents[0] * mComponents[0] +
                 mComponents[1] * mComponents[1] );
}

double Vector2f::magnitudeSquared() const
{
	return mComponents[0] * mComponents[0] +
           mComponents[1] * mComponents[1];
}

Vector2f Vector2f::normalize() const
{
	double mag = sqrt( mComponents[0] * mComponents[0] +
                      mComponents[1] * mComponents[1] );
	return Vector2f( mComponents[0] / mag, mComponents[1] / mag );
}

std::ostream &operator<< (std::ostream & os, const Vector2f & vector)
{
    std::cout << '(' << vector[0] << ", " << vector[1] << ')';
	return os;
}

