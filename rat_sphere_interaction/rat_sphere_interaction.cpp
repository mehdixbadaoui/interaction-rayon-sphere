
#include <iostream>
#include <fstream>
#include<optional>
#include<string>
#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

#include "vector3.h"
#include "EasyBMP.hpp"

using namespace EasyBMP;
using namespace std;

class Sphere {
    public :
        Vector3 centre;
        float radius;

        Sphere(Vector3 c, float r) {
            centre = c;
            radius = r;
        }

};

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

class Ray
{
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

class Box3
{
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


float raySphereIntersect(Vector3 r0, Vector3 rd, Vector3 s0, float sr) {
    // - r0: ray origin
    // - rd: normalized ray direction
    // - s0: sphere center
    // - sr: sphere radius
    // - Returns distance from r0 to first intersecion with sphere,
    //   or -1.0 if no intersection.

    //std::optional<float> 


    float a = rd.dot(rd);
    Vector3 s0_r0 = r0 - s0;
    float b = 2.0 * rd.dot(s0_r0);
    float c = s0_r0.dot(s0_r0) - (sr * sr);

    float sol = (-b - sqrt((b*b) - 4.0 * a * c)) / (2.0 * a);
    if (sol <= 0) {
        //return -1.0;
        return - 1;
    }
    return sol;
    }

float rayAABIntersect(Ray r, Box3 b){
    float t0x = (b.bounds[0].x - r.orig.x) / r.dir.x;
    float t1x = (b.bounds[1].x - r.orig.x) / r.dir.x;
    float t0y = (b.bounds[0].y - r.orig.y) / r.dir.y;
    float t1y = (b.bounds[1].y - r.orig.y) / r.dir.y;
    float t0z = (b.bounds[0].z - r.orig.z) / r.dir.z;
    float t1z = (b.bounds[1].z - r.orig.z) / r.dir.z;

    float tmin = max(max(min(t0x, t1x), min(t0y, t1y)), min(t0z, t1z));
    float tmax = min(min(max(t0x, t1x), max(t0y, t1y)), max(t0z, t1z));

    if (tmax > 0 && tmin < tmax)
    {
        return tmin;
    }

    return -1;
}

RGBColor color_calc(Sphere s, bool visibility, Vector3 intensity, Vector3 light, Vector3 ax, Vector3 contact) {

    Vector3 ret;
    Vector3 distance = contact - light;
    float dist = distance.length();
    distance = distance.normalize();

    Vector3 normale = contact - s.centre;
    normale.normalize();

    float dot = distance.dot(normale);

    if (dist == 0) return RGBColor (0, 0, 0);
    else if (visibility) {
        Vector3 norm = contact - s.centre;
        ret = ( -1 * intensity * ax * dot) / (dist * dist * M_PI);
        if (ret.x < 0) ret.x = 0;
        if (ret.y < 0) ret.y = 0;
        if (ret.z < 0) ret.z = 0;

        if (ret.x > 255) ret.x = 255;
        if (ret.y > 255) ret.y = 255;
        if (ret.z > 255) ret.z = 255;

    }
    return RGBColor(ret.x, ret.y, ret.z);
}


int main()
{
    RGBColor white(255, 255, 255);
    RGBColor black(0, 0, 0);
    RGBColor red(255, 0, 0);
    

    int size = 512;
    Image img(size, size, "result.png", black);
    

    Sphere s1(Vector3(150, 100, 100), 90);
    Sphere s2(Vector3(100,200, 100), 60);
    Sphere s3(Vector3(300, 300, 100), 60);

    Box3 b1(s1);
    Box3 b2(s2);
    Box3 b3(s3);


    Sphere spheres[3] = { s1, s2, s3 };
    Box3 boxes[3] = { b1, b2, b3 };

    Vector3 d(0, 0, 1);
    //Vector3 c(0, 0, 100);
    float dis;

    //Vector3 light(100, 300, 0);
    Light light(Vector3(100, 300, 0), 4.5);
    //Vector3 light2(200, 50, 100);
    float min_dist = INFINITY;


    for (Sphere s : spheres) {
        //for (int i = 0; i < sizeof(spheres); i++) {

        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                Vector3 contact(x, y, 0);
                if (raySphereIntersect(contact, d, s.centre, s.radius) >= 0) {


                    //Vector3 pixel_color = color_calc(s, true, Vector3(1, 3000, 3000), light, Vector3(0, 255, 255), contact);
                    RGBColor col = color_calc(s, true, light.getint(), light.getpos(), Vector3(0, 255, 255), contact);
                    img.SetPixel(x + 128, y + 128, col);

                    //Ray rayon(contact, d);
                    ////RGBColor blue(0, 0, 255);
                    ////RGBColor green(0, 255, 0);

                    ////if (raySphereIntersect(contact, d, s.centre, s.radius) >= 0 ) {
                    //if (rayAABIntersect(rayon, boxes[i]) > 0){
                    //    //Vector3 pixel_color = color_calc(s, true, Vector3(500, 500, 500), light, Vector3(0, 255, 255), contact);
                    //    //RGBColor col(pixel_color.x, pixel_color.y, pixel_color.z);
                    //    img.SetPixel(x , y, white);
                    //}

                }
            }


        }
    }


    img.SetFileName("result.png");
    img.Write();

}









