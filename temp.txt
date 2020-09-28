
#include <iostream>
#include <fstream>
#include<optional>
#include<string>
#include <algorithm>

#include "vector3.h"
#include "EasyBMP.hpp"

using namespace EasyBMP;
using namespace std;


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

    float sol = (-b - sqrt((b* b) - 4.0 * a * c)) / (2.0 * a);
    if (sol <= 0) {
        //return -1.0;
        return - 1;
    }
    return sol;
    }


int main()
{
    Vector3 white(255, 255, 255);
    RGBColor red(255, 0, 0);
    

    int size = 512;
    Image img(size, size, "result.png", red);
    
    Vector3 d(0, 0, 1);
    Vector3 c(0, 0, 100);
    float dis;

    Vector3 light(0, 200, 100);
    float min_dist = INFINITY;
    //cout << (min_dist > 5);
    //cout << (light - c).length();
    for (int x = -100; x <= 100; x++) {
        for (int y = -100; y <= 100; y++) {
            Vector3 a(x, y, 0);
            if ((light - a).length() < min_dist) min_dist = (light - a).length();
        }
    }

    cout << min_dist;


    for (int x = -100; x <= 100; x++) {
        for (int y = -100; y <= 100; y++) {
            Vector3 a(x, y, 0);
            if ((light - a).length() < min_dist) min_dist = (light - a).length();
            RGBColor color(0, 0, 255);
            float r = 90;

            if (raySphereIntersect(a, d, c, r) >= 0 ) {
                if (abs((light - a).length()) < min_dist + r ) {

                dis = raySphereIntersect(a, d, c, r);
                //color.g = dis + 100;
                img.SetPixel(x + 128, y + 128, color);
                }
                else {
                    img.SetPixel(x + 128, y + 128, RGBColor(0, 0, 0));
                }

            }

        }
    }








    img.SetFileName("result.png");
    img.Write();

}









