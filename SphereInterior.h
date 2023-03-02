///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SphereInterior.h
//
// Author:      David Borland
//
// Description: Class that represents the interior of a sphere.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef SPHEREINTERIOR_H
#define SPHEREINTERIOR_H


#include "Sphere.h"


class SphereInterior : public Sphere {
public:
    // Constructors
    SphereInterior();                                       // Set to (0, 0, 0), 1
    SphereInterior(const Vec3& c, double r);                // Set with center and radius

    // Destructor
    virtual ~SphereInterior();

    // Closest point on or inside the sphere
    virtual Vec3 ClosestPoint(const Vec3& p) const;

    // Is point on or inside the sphere
    virtual bool IsValid(const Vec3& p) const;
};


#endif