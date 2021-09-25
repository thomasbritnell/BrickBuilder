#include "mathLib2D.h"
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <time.h>
#include <vector>

using namespace std;

int main(int argc, char *argv[]){

    //Point2D tests
    Point2D p1 = Point2D(3.0,4.0);
    cout << "p1 X: " << p1.mX << "\n";
    cout << "p1 Y: " << p1.mY << "\n";

    Point2D p2 = Point2D(0,0);
    cout << "distance between p2 and p1: " << p1.distanceTo(p2) << "\n";
    cout << "checking that its the same between p1 and p2: " << p2.distanceTo(p1) << "\n";
    cout << "distance from point to itself should be 0: " << p1.distanceTo(p1) << "\n";

    cout << "fast distance between p2 and p1: " << p1.fastDistanceTo(p2) << "\n";
    cout << "checking that its the same between p1 and p2: " << p2.fastDistanceTo(p1) << "\n";
    cout << "fast distance from point to itself should be 0: " << p1.fastDistanceTo(p1) << "\n";

    //Vec2D tests
    Vec2D v1 = Vec2D(-3.0,4.0);
    cout << "v1 X: " << v1.mX << "\n";
    cout << "v1 Y: " << v1.mY << "\n";

    cout << "length should be 5: " << v1.length() << "\n";

    Vec2D v1normal  = v1.normalize();

    cout << "v1 normal X: " << v1normal.mX << "\n";
    cout << "v1 normal Y: " << v1normal.mY << "\n";

    cout << "v1 normalized length should be 1: " << v1normal.length() << "\n";

    Vec2D v1restored = v1normal.multiply(v1.length());

    cout << "multiply normalized vector by length of original should restore original vector:\n" 
        << "X: " << v1restored.mX << "\nY: " << v1restored.mY<<"\n";

    Point2D p3 = v1.movePoint(p2);
    cout << "moving (0,0) along v1 should return the point (v1.X,v1.Y)\n";
    cout << "p3 X: " << p3.mX << "\n";
    cout << "p3 Y: " << p3.mY << "\n";

    Vec2D v2 = Vec2D::createVector(p1,p2);
    cout << "Create a vector using p1 and p2:\n";    
    cout << "v2 X: " << v2.mX << "\n";
    cout << "v2 Y: " << v2.mY << "\n";

    cout << "v2 length should be 5: " << v2.length() << "\n";



    int arrayLength,vectorLength;
    cout << "Enter the length of the array: ";
    cin >> arrayLength;
    cout << "Enter the length of the vector: ";
    cin >> vectorLength;

    Point2D * pointArray = new Point2D [arrayLength];

    srand (time(NULL));
    int x,y;

    for(int i = 0; i < arrayLength; i++){
        x= rand() %10 ;
        y = rand() %10 ;
        pointArray[i] = Point2D(x,y);
    }

    cout<<"array:\n";
    for (int i = 0; i < arrayLength; i++){
        cout<< pointArray[i].mX << ", " << pointArray[i].mY <<"\n";
    }

    

    vector<Point2D> pointVector;

    for(int i = 0; i< vectorLength; i++){
        x= rand() %10 ;
        y = rand() %10 ;
        pointVector.push_back(Point2D(x,y));
    }
    cout<<"vector:\n";
    for (int i = 0; i < vectorLength; i++){
        cout<< pointVector[i].mX << ", " << pointVector[i].mY <<"\n";
    }

    

}