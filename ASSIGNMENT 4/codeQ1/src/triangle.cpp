// triangle.cpp

#include "triangle.h"
#include <math.h>
#include <vector>
#include <iostream>
using namespace std;
// e is origin of ray
// d in direction of ray
// d=1
// centre vector = e-c
// position is position of point

// Vector3D a = {-14.0, -13.0, -17.0}; // right
// Vector3D b = {5.0, -11.0, -25.0};
// Vector3D c = {-7.0, 5.0, -25.0};

double determinant(vector<vector<double>> matrix) //function to get the determinant of a 3x3 matrix
{
    double a1 = matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
    double b1 = matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
    double c1 = matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);

    return a1 - b1 + c1;
}

bool Triangle::intersect(Ray &r) const
{
    Vector3D a = A;
    Vector3D b = B;
    Vector3D c = C;
    vector<vector<double>> A1 = {{a[0] - b[0], a[0] - c[0], r.getDirection()[0]},
                                {a[1] - b[1], a[1] - c[1], r.getDirection()[1]},
                                {a[2] - b[2], a[2] - c[2], r.getDirection()[2]}}; // 3x3 matrix A

    vector<vector<double>> beta = {{a[0] - r.getOrigin()[0], a[0] - c[0], r.getDirection()[0]},
                                   {a[1] - r.getOrigin()[1], a[1] - c[1], r.getDirection()[1]},
                                   {a[2] - r.getOrigin()[2], a[2] - c[2], r.getDirection()[2]}}; // 3x3 matrix beta

    vector<vector<double>> gamma = {{a[0] - b[0], a[0] - r.getOrigin()[0], r.getDirection()[0]},
                                    {a[1] - b[1], a[1] - r.getOrigin()[1], r.getDirection()[1]},
                                    {a[2] - b[2], a[2] - r.getOrigin()[2], r.getDirection()[2]}}; // 3x3 matrix gamma

    vector<vector<double>> t = {{a[0] - b[0], a[0] - c[0], a[0] - r.getOrigin()[0]},
                                {a[1] - b[1], a[1] - c[1], a[1] - r.getOrigin()[1]},
                                {a[2] - b[2], a[2] - c[2], a[2] - r.getOrigin()[2]}}; // 3x3 matrix t

    //Cramer's rule
    double detA = determinant(A1);   // determinant of A
    double detBeta = determinant(beta) / detA;  // determinant of beta/ determinant of A
    double detGamma = determinant(gamma) / detA;  // determinant of gamma/ determinant of A
    double detT = determinant(t) / detA;    // determinant of t/ determinant of A


    if (detBeta + detGamma < 1 && detBeta > 0 && detGamma > 0 && detT > 0)  //condition to check if the ray intersects the triangle
    {
        bool tf = r.setParameter(detT, this);
        return true;
    }
    return false;

   
}
