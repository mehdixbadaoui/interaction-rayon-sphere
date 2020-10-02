
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

Vector3 color_calc(Sphere s, bool visibility, Vector3 intensity, Vector3 light, Vector3 ax, Vector3 contact) {

    Vector3 ret;
    Vector3 distance = contact - light;
    float dist = distance.length();
    distance = distance.normalize();

    Vector3 normale = contact - s.centre;
    normale.normalize();

    float dot = abs(normale.dot(distance));

    if (dist == 0) return Vector3(0, 0, 0);
    else if (visibility) {
        Vector3 norm = contact - s.centre;
        ret = (intensity * ax * dot) / (dist * dist * M_PI);
        if (ret.x < 0) ret.x = 0;
        if (ret.y < 0) ret.y = 0;
        if (ret.z < 0) ret.z = 0;
    }
    return ret;
}


int main()
{
    Vector3 white(255, 255, 255);
    RGBColor black(0, 0, 0);
    RGBColor red(255, 0, 0);
    

    int size = 512;
    Image img(size, size, "result.png", black);
    

    Sphere s1(Vector3(150, 0, 100), 90);
    Sphere s2(Vector3(0,200, 100), 60);
    Sphere s3(Vector3(300, 300, 100), 60);

    Sphere spheres[3] = { s1, s2, s3 };

    Vector3 d(0, 0, 1);
    //Vector3 c(0, 0, 100);
    float dis;

    Vector3 light(150, 100, 0);
    //Vector3 light2(200, 50, 100);
    float min_dist = INFINITY;


    for(Sphere s : spheres) {

        for (int x = -100; x <= 500; x++) {
            for (int y = -100; y <= 500; y++) {
                Vector3 contact(x, y, 0);
                RGBColor blue(0, 0, 255);
                RGBColor green(0, 255, 0);

                if (raySphereIntersect(contact, d, s.centre, s.radius) >= 0 ) {


                    Vector3 pixel_color = color_calc(s, true, Vector3(500, 500, 500), light, Vector3(0, 255, 255), contact);
                    RGBColor col(pixel_color.x, pixel_color.y, pixel_color.z);
                    img.SetPixel(x + 128, y + 128, col);
                    //if (pixel_color.x < 0 || pixel_color.y < 0 || pixel_color.z < 0) {

                    //    cout << "("<<pixel_color.x<<", "<<pixel_color.y<<", "<<pixel_color.z<<")" << endl;
                    //}

                }
            }
        }


    }


    img.SetFileName("result.png");
    img.Write();

}









