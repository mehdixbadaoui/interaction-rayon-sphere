#pragma once
#include "vector3.h"

class Light {
private:
    Vector3 pos;
    Vector3 intensity;

public:
    Light(Vector3 c, float i) {
        pos = c;
        intensity = Vector3(i * 100, i * 100, i * 100);
    }
    Vector3 getpos() {
        return pos;
    }

    Vector3 getint() {
        return intensity;
    }
};

class Sphere {
public:
    Vector3 centre;
    float radius;

    Sphere(Vector3 c, float r) {
        centre = c;
        radius = r;
    }

};

class Ray {
public:
    Ray(const Vector3& orig, const Vector3& dir) : orig(orig), dir(dir)
    {
        invdir = 1 / dir;
        sign[0] = (invdir.x < 0);
        sign[1] = (invdir.y < 0);
        sign[2] = (invdir.z < 0);
    }
    Vector3 orig, dir;       // ray orig and dir 
    Vector3 invdir;
    int sign[3];
};

class Box3 {
public:
    Box3(const Vector3& vmin, const Vector3& vmax)
    {
        bounds[0] = vmin;
        bounds[1] = vmax;
    }

    Box3(Sphere s) {
        bounds[0] = Vector3(s.centre.x - s.radius, s.centre.y - s.radius, s.centre.z - s.radius);
        bounds[1] = Vector3(s.centre.x + s.radius, s.centre.y + s.radius, s.centre.z + s.radius);
    }
    Vector3 bounds[2];
};
