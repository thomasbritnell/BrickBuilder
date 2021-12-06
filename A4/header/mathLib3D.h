#ifndef MATHLIB_3D_H
#define MATHLIB_3D_H

class Point3D {
public:
    Point3D();
    Point3D(float inX, float inY, float inZ);
    float mX;
    float mY;
    float mZ;

    Point3D multiply(float scalar);
    float distanceTo(Point3D other);
    float fastDistanceTo(Point3D other);
    bool isEqualTo(Point3D point);
};

class Vec3D {
public:
    Vec3D();
    Vec3D(float inX, float inY, float inZ);
    float mX;
    float mY;
    float mZ;
    Point3D start; // added to be a ray as well
    float length();
    Vec3D normalize();
    Vec3D multiply(float scalar);
    Point3D movePoint(Point3D source);
    Vec3D add(Vec3D other);

    static Vec3D createVector(Point3D p1, Point3D p2);
    static Vec3D crossProduct(Vec3D v1, Vec3D v2);
    static float dotProduct(Vec3D v1, Vec3D v2);
    static float dotProduct(Vec3D v, Point3D p);
};

struct Angle{
    float rX;
    float rY;
    float rZ;
};

#endif
