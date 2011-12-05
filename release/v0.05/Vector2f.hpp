/** @file Vector2f.hpp
* Represents a 2-dimensional vector.
* @author Keeler Russell
* @date Oct 13, 2011
*/
#ifndef VECTOR_2F_HPP
#define VECTOR_2F_HPP

#include <iostream>

/** @class Vector2f
* A 2-dimensional vector class. Used for storing positions and velocities.
*/
class Vector2f
{
	public:
		/** Default constructor. Initializes a zero vector.
		* @pre None.
		* @post The object will represent the vector (0, 0).
		*/
		Vector2f();
		/** Typical constructor. Initializes a vector from the input values.
		* @param x The magnitude of the vector in the x dimension.
		* @param y The magnitude of the vector in the y dimension.
		* @pre None.
		* @post The object will represent the vector (x, y).
		*/
		Vector2f( double x, double y );

		/** Overloaded square bracket accessor. Used for setting the values of the vector.
		* @param index Indicates which value to set, the x value or the y value.
		* @pre The value of index must be 0 (for x) or 1 (for y).
		* @post A reference to the specified value will be returned, so that it may be changed.
		*/
		double & operator[]( int index );
		/** Overloaded square bracket accessor. Used for getting the values of the vector.
		* @param index Inicates which value to get, the x value or the y value.
		* @pre The value of index must be 0 (for x) or 1 (for y).
		* @post A copy of the data at the specified index will be returned, with no change to the vector.
		*/
		double operator[]( int index ) const;

		/** Overloaded scalar multiplication.
		* @param scale The value to multiply the vector by.
		* @pre None.
		* @post This object will be unchanged, and return a vector equal to (scale * x, scale * y).
		*/
		Vector2f operator*( double scale ) const;
		/** Overloaded scalar division.
		* @param scale The value to divide the vector by.
		* @pre The value of scale must not be 0..
		* @post This object will be unchanged, and return a vector equal to (x / scale, y / scale).
		*/
		Vector2f operator/( double scale ) const;
		/** Overloaded vector addition.
		* @param other The vector to add to this one.
		* @pre None.
		* @post This object will be unchanged, and return a vector equal the sum of the vectors.
		*/
		Vector2f operator+( const Vector2f & other ) const;
		/** Overloaded vector subtraction.
		* @param other The vector to subtract from this one.
		* @pre None.
		* @post This object will be unchanged, and return a vector equal to this vector minus the other.
		*/
		Vector2f operator-( const Vector2f & other ) const;
		/** Overloaded unary minus sign.
		* @pre None.
		* @post This object will be unchanged, and return a vector equal to (-1 * x, -1 * y).
		*/
		Vector2f operator-() const;

		/** Overloaded scalar multiplication with assignment.
		* @param scale The value to multiply the vector by.
		* @pre None.
		* @post This object will now equal (scale * x, scale * y).
		*/
		const Vector2f & operator*=( double scale );
		/** Overloaded scalar division with assignment.
		* @param scale The value to divide the vector by.
		* @pre The value of scale must not be 0.
		* @post This object will now equal (x / scale, y / scale).
		*/
		const Vector2f & operator/=( double scale );
		/** Overloaded vector addition with assignment.
		* @param other The vector to add to this one.
		* @pre None.
		* @post This object will now equal the sum of this and the other vector.
		*/
		const Vector2f & operator+=( const Vector2f & other );
		/** Overloaded vector subtraction with assignment.
		* @param other The vector to subtract from this one.
		* @pre None.
		* @post This object will now equal the defference of this minus the other vector.
		*/
		const Vector2f & operator-=( const Vector2f & other );

		/** Returns the magnitude of the vector, equal to \f$\sqrt{ x^2 + y^2 }\f$.
		* @pre None.
		* @post None.
		*/
		double magnitude() const;
		/** Returns the square of the magnitude of the vector, equal to \f$x^2 + y^2\f$.
		* @pre None.
		* @post None.
		*/
		double magnitudeSquared() const;
		/** Normalizes the vector by dividing it by its own magnitude.
		* @pre None.
		* @post None.
		*/
		Vector2f normalize() const;
		
	private:
		double mComponents[2];
};

/** Overloaded stream output operator.
* @param os The output stream to write the vector to.
* @param vector The vector to write to the output stream.
* @pre None.
* @post The vector will be printed out with no trailing newline.
*/
std::ostream &operator<< (std::ostream & os, const Vector2f & vector);

#endif