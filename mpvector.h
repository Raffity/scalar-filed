#ifndef MpVector_H
#define MpVector_H

#include <math.h>
#include <stdio.h>

class mp4Vector;

class MpVector
{
public:
    float x, y, z;

    MpVector();
    MpVector(float xx, float yy, float zz);
    MpVector(const MpVector& other);

    MpVector& Normalize();
    float Magnitude();
    MpVector Cross(const MpVector& other);
    MpVector operator - (MpVector v);
    MpVector operator + (MpVector v);
    float operator * (MpVector v);
    MpVector operator - (float c);
    MpVector operator + (float c);
    MpVector operator / (float c);
    MpVector operator * (float c);
    MpVector& operator = (const MpVector& other);
    operator mp4Vector() const;

    operator char*()  const;

};


class mp4Vector
{
public:
    float x, y, z, val;

    mp4Vector();
    mp4Vector(float aa, float bb, float cc, float dd);
    mp4Vector(const mp4Vector& other);
    mp4Vector(const MpVector& v, const float value);

    void operator = (const mp4Vector& v);

    operator MpVector() const;
};


#endif // MpVector_H
