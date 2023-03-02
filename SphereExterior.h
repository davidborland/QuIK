///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        SphereExterior.h
//
// Author:      David Borland
//
// Description: Class that represents the interior of a sphere.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef SPHEREEXTERIOR_H
#define SPHEREEXTERIOR_H


#include "Sphere.h"


class SphereExterior : public Sphere {
public:
    // Constructors
    SphereExterior();                                       // Set to (0, 0, 0), 1
    SphereExterior(const Vec3& c, double r);                // Set with center and radius

    // Destructor
    virtual ~SphereExterior();

    // Closest point on or outside the sphere
    virtual Vec3 ClosestPoint(const Vec3& p) const;

    // Is point on or outside the sphere
    virtual bool IsValid(const Vec3& p) const;
};


#endif