///////////////////////////////////////////////////////////////////////////////////////////////
//
// Name:        Sphere.h
//
// Author:      David Borland
//
// Description: Sphere class.  
//
/////////////////////////////////////////////////////////////////////////////////////////////// 


#ifndef SPHERE_H
#define SPHERE_H


#include "Vec3.h"


class Sphere {
public:
    // Constructors
    Sphere();                                       // Set to (0, 0, 0), 1
    Sphere(const Vec3& c, double r);                // Set with center and radius    

    // Use default copy constructor


    // Destructor
    virtual ~Sphere();


    // Use default assignment operator


    // Set values
    void MakeIdentity();                            // Set to (0, 0, 0), 1
    void Set(const Vec3& c, double r);              // Set with center and radius

        
    // Element access
    Vec3& c();                                      // Read/write access
    double& r();                                    // Read/write access

    Vec3 c() const;                                 // Just read access
    double r() const;                               // Just read access


    // Sphere-sphere intersection
    enum SSI_Type {
        SSI_EmptyOutside,
        SSI_EmptyInside,
        SSI_Point,
        SSI_Circle,
        SSI_Sphere
    };
    SSI_Type Intersection(const Sphere& s, Vec3& c, double& r, Vec3& n) const;

    // Closest point on the sphere
    virtual Vec3 ClosestPoint(const Vec3& p) const;

    // Is point on the sphere
    virtual bool IsValid(const Vec3& p) const;


    // Output to a stream
    friend std::ostream& operator<<(std::ostream& os, const Sphere& s);

protected:
    // The internal representation
    Vec3 _c;
    double _r;

    const static double epsilon;
};


#endif