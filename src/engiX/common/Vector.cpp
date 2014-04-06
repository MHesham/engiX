#include "Vector.h"

using namespace engiX;

/*
Uniformly. With homogeneous coordinates, we augment to 4-tuples and what we
place in the fourth w-coordinate depends on whether we are describing a point or vector. Specifically, we write:
1. (x, y, z, 0) for vectors
2. (x, y, z, 1) for points
Setting w = 1 for points allows translations of points to work correctly, and setting w = 0 for vectors
prevents the coordinates of vectors from being modified by translations
*/
typedef Vector3<real, 0> Vec3F;
typedef Vector3<int, 0> Vec3;

typedef Vector3<real, 1> Point3F;
typedef Vector3<int, 1> Point3;

const static Vec3F UP = Vec3F(0, 1.0, 0);
const static Vec3F ZERO = Vec3F(0, 0, 0);
const static Vec3F ONE = Vec3F(1.0, 1.0, 1.0);