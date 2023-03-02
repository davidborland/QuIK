///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Sphere.cpp
//
// Author:      David Borland
//
// Description: Sphere class.
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#include "Sphere.h"

#include <math.h>


const double Sphere::epsilon = 1e-10;


///////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////////////////////

Sphere::Sphere() {
    MakeIdentity();
}

Sphere::Sphere(const Vec3& c, double r) {
    Set(c, r);
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////////////////////

Sphere::~Sphere() {
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Set values
///////////////////////////////////////////////////////////////////////////////////////////////

void Sphere::MakeIdentity() {
    Set(Vec3(0.0, 0.0, 0.0), 1.0);
}

void Sphere::Set(const Vec3& c, double r) {
    _c = c;
    _r = r;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Element access
///////////////////////////////////////////////////////////////////////////////////////////////

Vec3& Sphere::c() {
    return _c;
}

double& Sphere::r() {
    return _r;
}


Vec3 Sphere::c() const {
    return _c;
}

double Sphere::r() const {
    return _r;
}


///////////////////////////////////////////////////////////////////////////////////////////////
// Utilities
///////////////////////////////////////////////////////////////////////////////////////////////

Sphere::SSI_Type Sphere::Intersection(const Sphere& s, Vec3& c, double& r, Vec3& n) const {
    // Calculate the direction and distance between the two centers
    n = s._c - _c;
    double d = n.Magnitude();
    n.Normalize();

    // Check for intersection
    if (d <= 0.0) {
        // Concentric spheres
        if (_r == r) {
            // Same sphere
            c = _c;
            r = _r;

            return SSI_Sphere;
        }
        else {
            // No intersection, return the smallest radius
            c = _c;
            r = std::min(_r, s._r);

            return SSI_EmptyInside;
        }
    }
    else if (d > _r + s._r) {
        // Too far away, no intersection
        c = _c;
        r = _r;

        return SSI_EmptyOutside;
    }
    else if (d + s._r < _r) {
        // Sphere 2 completely inside sphere 1
        c = s._c;
        r = s._r;

        return SSI_EmptyInside;
    }
    else if (d + _r < s._r) {
        // Sphere 1 completely inside sphere 2
        c = _c;
        r = _r;
        n = _c - s._c;
        n.Normalize();

        return SSI_EmptyInside;
    }
    else if (d == _r + s._r) {
        // Intersection is a point
        c = _c + n * _r;
        r = 0.0;

        return SSI_Point;
    }


    // The intersection is a circle.
	// For now, assume the first sphere is at the origin and
	// the second is at a distance of d along the x-axis, and
	// solve for the location of the intersection plane.
    double r1_2 = _r * _r;
    double r2_2 = s._r * s._r;
	double x = (d * d - r2_2 + r1_2) / (2.0 * d);


	if (fabs(x) > _r) {
        // Must be a floating point precision problem, set to length of _r
        x = x < 0.0 ? -_r : _r;
	}


	// Calculate the circle center, given the distance from sphere 1
    c = _c + n * x;

	// Compute the radius
	r = sqrt(r1_2 - x * x);

	return SSI_Circle;
}

Vec3 Sphere::ClosestPoint(const Vec3& p) const {
    Vec3 v = p - _c;
    v.Normalize();

    return _c + v * _r;
}

bool Sphere::IsValid(const Vec3 &p) const {
    double d = _c.Distance(p);

    return (d <= _r + epsilon) &&
           (d >= _r - epsilon);
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Output to a stream
///////////////////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, const Sphere& s) {
    return (os << s._c << ", " << s._r);
}