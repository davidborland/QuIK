///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Vec3.cpp
//
// Author:      David Borland
//
// Description: 3D vector class for C++ version of quatlib.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Vec3.h"

#include <math.h>


// Set indeces
const int Vec3::X = 0;
const int Vec3::Y = 1;
const int Vec3::Z = 2;


///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////////////////////

Vec3::Vec3() {
    MakeIdentity();
}

Vec3::Vec3(double v[3]) {
    Set(v[X], v[Y], v[Z]);
}

Vec3::Vec3(double x, double y, double z) {
    Set(x, y, z);
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Set values
///////////////////////////////////////////////////////////////////////////////////////////////

void Vec3::MakeIdentity() {
    Set(0.0, 0.0, 0.0);
}

void Vec3::Set(double v[3]) {
    Set(v[X], v[Y], v[Z]);
}

void Vec3::Set(double x, double y, double z) {
    _v[X] = x;
    _v[Y] = y;
    _v[Z] = z;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Element access
///////////////////////////////////////////////////////////////////////////////////////////////

double& Vec3::x() {
    return _v[X];
}

double& Vec3::y() {
    return _v[Y];
}

double& Vec3::z() {
    return _v[Z];
}


double Vec3::x() const {
    return _v[X];
}

double Vec3::y() const {
    return _v[Y];
}

double Vec3::z() const {
    return _v[Z];
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////////////////////////

double Vec3::operator[](int i) const {
    return _v[i];
}

const Vec3 Vec3::operator+(const Vec3& v) const {
    return Vec3(_v[X] + v._v[X],
                _v[Y] + v._v[Y],
                _v[Z] + v._v[Z]);
}

const Vec3 Vec3::operator-(const Vec3& v) const {
    return Vec3(_v[X] - v._v[X],
                _v[Y] - v._v[Y],
                _v[Z] - v._v[Z]);
}


const Vec3 Vec3::operator*(const Vec3& v) const {
    return Vec3(_v[Y] * v._v[Z] - _v[Z] * v._v[Y],
                _v[Z] * v._v[X] - _v[X] * v._v[Z],
                _v[X] * v._v[Y] - _v[Y] * v._v[X]);
}

const Vec3 Vec3::operator*(double scale) const {
    return Vec3(_v[X] * scale, 
                _v[Y] * scale,
                _v[Z] * scale);
}


Vec3& Vec3::operator+=(const Vec3& v) {
    return (*this = *this + v);
}

Vec3& Vec3::operator-=(const Vec3& v) {
    return (*this = *this - v);
}


Vec3& Vec3::operator*=(const Vec3& v) {
    return (*this = *this * v);
}

Vec3& Vec3::operator*=(double scale) {
    return (*this = *this * scale);
}


const Vec3 Vec3::operator!() const {
    return Vec3(-_v[X], -_v[Y], -_v[Z]);
}


bool Vec3::operator==(const Vec3& v) const {
    return (_v[X] == v._v[X] &&
            _v[Y] == v._v[Y] &&
            _v[Z] == v._v[Z]);
}

bool Vec3::operator!=(const Vec3& v) const {
    return (_v[X] != v._v[X] &&
            _v[Y] != v._v[Y] &&
            _v[Z] != v._v[Z]);
}


bool Vec3::operator<(const Vec3& v) const {
    return (Magnitude() < v.Magnitude());
}

bool Vec3::operator>(const Vec3& v) const {
    return (Magnitude() > v.Magnitude());
}

bool Vec3::operator<=(const Vec3& v) const {
    return (Magnitude() <= v.Magnitude());
}

bool Vec3::operator>=(const Vec3& v) const {
    return (Magnitude() >= v.Magnitude());
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////////////////////

double Vec3::DotProduct(const Vec3& v) const {
    return (_v[X] * v._v[X] +
            _v[Y] * v._v[Y] +
            _v[Z] * v._v[Z]);
}

double Vec3::Distance(const Vec3& v) const {
    Vec3 diff = *this - v;
    return diff.Magnitude();
}

double Vec3::Magnitude() const {
    return sqrt(_v[X] * _v[X] +
                _v[Y] * _v[Y] + 
                _v[Z] * _v[Z]);
}

void Vec3::Normalize() {
    double magnitude = Magnitude();

    if (magnitude <= 0.0) {
//        std::cout << "Vec3::Normalize() : Vector has 0 magnitude." << std::endl;
        return;
    }

    double scale = 1.0 / magnitude;

    _v[X] *= scale;
    _v[Y] *= scale;
    _v[Z] *= scale;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Output to a stream
///////////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    return (os << "(" 
               << v._v[Vec3::X] << ", "
               << v._v[Vec3::Y] << ", "
               << v._v[Vec3::Z] 
               << ")");
}