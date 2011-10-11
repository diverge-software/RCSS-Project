#ifndef VECTOR_2F_HPP
#define VECTOR_2F_HPP

#include <iostream>

class Vector2f
{
	public:
		Vector2f();
		Vector2f( double x, double y );

		double & operator[]( int index );
		double operator[]( int index ) const;

		Vector2f operator*( double scale ) const;
		Vector2f operator/( double scale ) const;
		Vector2f operator+( const Vector2f & other ) const;
		Vector2f operator-( const Vector2f & other ) const;
		Vector2f operator-() const;

		const Vector2f & operator*=( double scale );
		const Vector2f & operator/=( double scale );
		const Vector2f & operator+=( const Vector2f & other );
		const Vector2f & operator-=( const Vector2f & other );

		double magnitude() const;
		double magnitudeSquared() const;
		Vector2f normalize() const;
		
	private:
		double mComponents[2];
};

std::ostream &operator<< (std::ostream & os, const Vector2f & vector);

#endif