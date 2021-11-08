#ifndef MATHLIB_3D_H
#define MATHLIB_3D_H

/**
 * A point in 3D space. (x,y,z)
 * Default is (0,0,0)
 */
class Point3D {
public:
    Point3D();
    Point3D(float inX, float inY, float inZ);
    float mX;
    float mY;
    float mZ;

    /**
     * Returns the distance to another point
     */
    float distanceTo(Point3D other);
    /**
     * @brief returns the relative distance to another point
     * 
     */
    float fastDistanceTo(Point3D other);
};

/**
 * A vector in 3D space. (x,y,z) from the origin
 * 
 */
class Vec3D {
public:
    Vec3D();
    Vec3D(float inX, float inY, float inZ);
    float mX;
    float mY;
    float mZ;
    /**
     * @brief Return the length of the vector. 
     */
    float length();
    /**
     * @brief Retunr a unit vector in the same direction this vector
     */
    Vec3D normalize();
    /**
     * @brief Return a this vector scaled by a factor. 
     */
    Vec3D multiply(float scalar);
    /**
     * @brief Returns the translated point from source over this vector. 
     */
    Point3D movePoint(Point3D source);
    /**
     * @brief Returns the sum of this and another vector. 
     */
    Vec3D add(Vec3D other);

    /**
     * @brief Create a Vector object from p1 to p2
     */
    static Vec3D createVector(Point3D p1, Point3D p2);
    /**
     * @brief Return the cross product vector from v1 to v2. Used for calculating normals
     */
    static Vec3D crossProduct(Vec3D v1, Vec3D v2);
};

/**
 * A container for 3 floats; rotation on x, y, z...
 * 
 */
struct Angle{
    float rX;
    float rY;
    float rZ;
};

#endif
