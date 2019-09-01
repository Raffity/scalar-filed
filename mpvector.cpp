#include "MpVector.h"

MpVector::MpVector(float xx, float yy, float zz) :
    x(xx), y(yy), z(zz)
    { }

MpVector::MpVector() : x(0), y(0), z(0)
    { }
MpVector::MpVector(const MpVector& other) : x(other.x), y(other.y), z(other.z)
    { }

MpVector& MpVector::Normalize()
{
    float length = sqrt(x*x + y*y + z*z);
    if(!length) return *this;
    x /= length;
    y /= length;
    z /= length;
    return *this;
}


float MpVector::Magnitude()
{
    return sqrt(x*x + y*y + z*z);
}

MpVector MpVector::Cross(const MpVector& other)
{
    return MpVector(y*other.z-z*other.y, z*other.x-x*other.z, x*other.y-y*other.x);
}

MpVector MpVector::operator - (MpVector v)
{
    return MpVector(x - v.x, y - v.y, z - v.z);
}

MpVector MpVector::operator + (MpVector v)
{
    return MpVector(x + v.x, y + v.y, z + v.z);
}

float MpVector ::operator * (MpVector v)
{
    return x*v.x + y*v.y + z*v.z;
}
MpVector MpVector::operator - (float c)
{
    return MpVector(x-c, y-c, z-c);
}

MpVector MpVector::operator + (float c)
{
    return MpVector(x+c, y+c, z+c);
}

MpVector MpVector::operator / (float c)
{
    return MpVector(x/c, y/c, z/c);
}

MpVector MpVector::operator * (float c)
{
    return MpVector(x*c, y*c, z*c);
}

MpVector& MpVector::operator = (const MpVector& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
}

MpVector::operator mp4Vector() const
{
    return mp4Vector(*this);
}

mp4Vector::mp4Vector() : x(0), y(0), z(0), val(0)
{ }

mp4Vector::mp4Vector(float aa, float bb, float cc, float dd) :
    x(aa), y(bb), z(cc), val(dd)
{ }

mp4Vector::mp4Vector(const mp4Vector& other) :
    x(other.x), y(other.y), z(other.z), val(other.val)
{ }

mp4Vector::mp4Vector(const MpVector& v, const float value) :
    x(v.x), y(v.x), z(v.z), val(value)
{ }

void mp4Vector::operator = (const mp4Vector& v)
{
    x = v.x; y = v.y; z = v.z;
    val = v.val;
}

mp4Vector::operator MpVector() const
{
    return MpVector(x, y, z);
}
