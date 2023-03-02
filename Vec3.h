///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Vec3.h
//
// Author:      David Borland
//
// Description: 3D vector class.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef VEC3_H
#define VEC3_H


#include <iostream>


class Vec3 {
public:
    // Constructors
    Vec3();                                         // Set to (0, 0, 0)
    Vec3(double v[3]);                              // Set from array
    Vec3(double x, double y, double z);             // Set components


    // Use default copy constructor
    // Use default destructor
    // Use default assignment operator


    // Set values
    void MakeIdentity();                            // Set to (0, 0, 0)
    void Set(double v[3]);                          // Set from array
    void Set(double x, double y, double z);         // Set components

        
    // Element access
    double& x();                                    // Read/write access
    double& y();                                    // Read/write access
    double& z();                                    // Read/write access

    double x() const;                               // Just read access
    double y() const;                               // Just read access
    double z() const;                               // Just read access


    // Operators
    double operator[](int i) const;                 // Index

    const Vec3 operator+(const Vec3& v) const;      // Vector addition
    const Vec3 operator-(const Vec3& v) const;      // Vector subtraction

    const Vec3 operator*(const Vec3& v) const;      // Cross product
    const Vec3 operator*(double scale) const;       // Scale

    Vec3& operator+=(const Vec3& v);                // Vector addition
    Vec3& operator-=(const Vec3& v);                // Vector subtraction

    Vec3& operator*=(const Vec3& v);                // Cross product
    Vec3& operator*=(double scale);                 // Scale

    const Vec3 operator!() const;                   // Invert

    bool operator==(const Vec3& v) const;           // Equality
    bool operator!=(const Vec3& v) const;           // Inequality

    bool operator<(const Vec3& v) const;            // Magnitude less than
    bool operator>(const Vec3& v) const;            // Magnitude greater than
    bool operator<=(const Vec3& v) const;           // Magnitude less than or equal to
    bool operator>=(const Vec3& v) const;           // Magnitude greater than or equal to


    // Utilities
    double DotProduct(const Vec3& v) const;
    double Distance(const Vec3& v) const;
    double Magnitude() const;
    void Normalize();


    // Output to a stream
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);

protected:
    // The internal representation
    double _v[3];                    

    // Indeces
    const static int X;
    const static int Y;
    const static int Z;
};


#endif