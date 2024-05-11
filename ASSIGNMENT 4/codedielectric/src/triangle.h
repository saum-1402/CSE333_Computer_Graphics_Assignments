// triangle.h
#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "object.h"
#include "ray.h"
#include "vector3D.h"
#include "color.h"
#include "vector"
using namespace std;

class Triangle : public Object
{
private:
    Vector3D A;
    Vector3D B;
    Vector3D C;


public:
    Triangle(const Vector3D &positionA, const Vector3D &positionB, const Vector3D &positionC, Material *mat) 
    : Object(mat), A(positionA), B(positionB), C(positionC)
    {
        isSolid = true;
    }

    virtual bool intersect(Ray &r) const;
};
#endif
