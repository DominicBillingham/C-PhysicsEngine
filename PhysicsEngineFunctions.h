#pragma once

#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <SFML\Graphics.hpp>
#include "EntityClass.cpp"

using namespace std;
using namespace sf;

//Main intergration loop
void Intergrate(vector<Entity>& EntList, float FrameDelay, float ResCoeff);

//COLLISION RESPONSE FUNCTIONS

// Resolves new position based on forces, e.g gravity by using suvat equation
Vector2f FindNewPosition(Vector2f u, float t, Vector2f a);

// Function for calculating collision impulse, factoring in mass, speed and rescoeff. This impulse is then used to calculate the new speeds after a collision.
float Impulse(Vector2f Vel1, Vector2f Vel2, Vector2f VelNormal, float Mass1, float Mass2, float ResCoeff);

// These functions use impulse and caculate the speed both colliding objects should now move at.
Vector2f NewVel1(float Impluse, Vector2f InitSpeed, float Mass, Vector2f VelNormal);
Vector2f NewVel2(float Impluse, Vector2f InitSpeed, float Mass, Vector2f VelNormal);

// GENERIC MATHS FUNCTIONS
float DotProduct(Vector2f Vec1, Vector2f Vec2);

// Returns a unit vector of any vector fed into it
Vector2f VectorNormilisation(Vector2f Vec);

// COLLISION DETECTION

// Colliding squares figures out if two rectangles are colliding, but returns how much they are intersecting by
Vector2f CollidingSquares(Vector2f ShapeATopLeft, Vector2f ShapeABottomRight, Vector2f ShapeBTopLeft, Vector2f ShapeBBottomRight);

// Takes the intersection distance calculate by CollidingSquares() and uses it to find the middle of the intersection square
Vector2f IntersectingSquarePos(Vector2f IntersectingSquareSize, Vector2f ShapeATopLeft, Vector2f ShapeBTopLeft, Vector2f ShapeASize, Vector2f ShapeBSize);

