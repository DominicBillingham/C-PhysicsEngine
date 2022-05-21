
#include <SFML\Graphics.hpp>
#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <chrono>
#include <thread>
#include <list>
#include <vector>

#include "PhysicsEngineFunctions.h"

using namespace std;
using namespace sf;

void Intergrate(vector<Entity>& EntList, float FrameDelay, float ResCoeff) {

    // First, update all objects based of any constant forces applied such as gravity
    for (int i = 0; i < EntList.size(); ++i) {

        Vector2f NewPos = FindNewPosition(EntList[i].getVel(), (FrameDelay / 1000), EntList[i].getAcc());
        EntList[i].setPosition(EntList[i].getPosition() + NewPos);
        EntList[i].setVel(EntList[i].getVel() + EntList[i].getAcc() * (FrameDelay / 1000));

    }

    // Variables needed for collision detection and calculation during run time.
    Vector2f Clipping;
    Vector2f IntersectingSqaurePos;
    Vector2f VelNorm;
    Vector2f ShapeAOrigin;

    // Main loop, cycles through all entities in the entity list and then compares them.
    for (int i = 0; i < EntList.size(); ++i) { 

        for (int j = 0; j < EntList.size(); ++j) {

            // Avoids performing collision checks on the same entity.
            // Avoids performing calculations on static entities too.
            if (i == j) { continue; }
            if (EntList[i].IsStatic()) { continue; }

            // Clipping is how much another square is colliding with another square.
            Clipping = CollidingSquares(EntList[i].getPosition(), EntList[i].getPosition() + EntList[i].getSize(), EntList[j].getPosition(), EntList[j].getPosition() + EntList[j].getSize());
            IntersectingSqaurePos = IntersectingSquarePos(Clipping, EntList[i].getPosition(), EntList[j].getPosition(), EntList[i].getSize(), EntList[j].getSize());

            if (Clipping.x > 0 && Clipping.y > 0) {

                ShapeAOrigin.x = EntList[i].getPosition().x + (EntList[i].getSize().x / 2);
                ShapeAOrigin.y = EntList[i].getPosition().y + (EntList[i].getSize().y / 2);
                // Distance here is the distance between the middle of the colliding square AND the intersecting square's middle.
                Vector2f Distance = IntersectingSqaurePos - ShapeAOrigin;
                VelNorm = VectorNormilisation(Distance);

                // First, shunt any object that is colliding out of another object, so that it is accurate to newtonian physics.
                if (Clipping.x > Clipping.y) {

                    if (Distance.y > 0) { EntList[i].setPosition(EntList[i].getPosition().x, EntList[i].getPosition().y - Clipping.y); }
                    else { EntList[i].setPosition(EntList[i].getPosition().x, EntList[i].getPosition().y + Clipping.y); }

                }
                else {
                    if (Distance.x > 0) { EntList[i].setPosition(EntList[i].getPosition().x - Clipping.x, EntList[i].getPosition().y); }
                    else { EntList[i].setPosition(EntList[i].getPosition().x + Clipping.x, EntList[i].getPosition().y); }
                }

                // Now resolve collisions by calculating impluse and projecting along the normal of the collision.
                if (!EntList[j].IsStatic()) {
                    float Imp = Impulse(EntList[i].getVel(), EntList[j].getVel(), VelNorm, EntList[i].getMass(), EntList[j].getMass(), ResCoeff);
                    EntList[i].setVel(NewVel1(Imp, EntList[i].getVel(), EntList[i].getMass(), VelNorm));
                    EntList[j].setVel(NewVel2(Imp, EntList[j].getVel(), EntList[j].getMass(), VelNorm));
                }

                // However, if the other object is a stationary object do not attempt to move it
                if (EntList[j].IsStatic()) {

                    if (Clipping.x < Clipping.y) {

                        Vector2f Temp = EntList[i].getVel();
                        Temp.x = -(EntList[i].getVel().x) * ResCoeff;
                        Temp.y = (EntList[i].getVel().y) * ResCoeff;
                        EntList[i].setVel(Temp);

                    }
                    else {

                        Vector2f Temp = EntList[i].getVel();
                        Temp.y = -(EntList[i].getVel().y) * ResCoeff;
                        Temp.x = (EntList[i].getVel().x) * ResCoeff;
                        EntList[i].setVel(Temp);

                    }

                    // Prevents objects from pushing objects through a wall by over-riding their speed
                    break;

                }

            }
            // End of J loop
        }

        // End of I loop
    }

}

Vector2f FindNewPosition(Vector2f u, float t, Vector2f a) {

    Vector2f NewPos;
    NewPos.x = (u.x * t) + (0.5 * a.x * pow(t, 2));
    NewPos.y = (u.y * t) + (0.5 * a.y * pow(t, 2));
    return NewPos;

}

float Impulse(Vector2f Vel1, Vector2f Vel2, Vector2f VelNormal, float Mass1, float Mass2, float ResCoeff) {

    Vector2f RelVel = (Vel1 - Vel2);
    float RelVelDotNormal = DotProduct(RelVel, VelNormal);
    float TopPart = RelVelDotNormal * -(1 + ResCoeff);
    float BottomPart = DotProduct(VelNormal, VelNormal * ((1 / Mass1) + (1 / Mass2)));
    return (TopPart / BottomPart);

}

Vector2f NewVel1(float Impluse, Vector2f InitSpeed, float Mass, Vector2f VelNormal) {

    return InitSpeed + ((Impluse / Mass) * VelNormal);

}

Vector2f NewVel2(float Impluse, Vector2f InitSpeed, float Mass, Vector2f VelNormal) {

    return InitSpeed - ((Impluse / Mass) * VelNormal);

}

float DotProduct(Vector2f Vec1, Vector2f Vec2) {

    return ((Vec1.x * Vec2.x) + (Vec1.y * Vec2.y));

}

Vector2f VectorNormilisation(Vector2f Vec) {

    Vector2f NormalisedVector;

    float XSquare = Vec.x * Vec.x;
    float YSquare = Vec.y * Vec.y;
    float HypLength = sqrt(XSquare + YSquare);

    NormalisedVector.x = Vec.x / HypLength;
    NormalisedVector.y = Vec.y / HypLength;

    return NormalisedVector;
    
}

//Code adapated from (GeeksForGeeks, 2021), lines 161 through to 170
Vector2f CollidingSquares(Vector2f ShapeATopLeft, Vector2f ShapeABottomRight, Vector2f ShapeBTopLeft, Vector2f ShapeBBottomRight) {

    Vector2f IntersectionSize;
    IntersectionSize.x = min(ShapeABottomRight.x, ShapeBBottomRight.x) - max(ShapeATopLeft.x, ShapeBTopLeft.x);
    IntersectionSize.y = min(ShapeABottomRight.y, ShapeBBottomRight.y) - max(ShapeATopLeft.y, ShapeBTopLeft.y);
    return IntersectionSize;

}
//End of adapated code 

Vector2f IntersectingSquarePos(Vector2f IntersectingSquareSize, Vector2f ShapeATopLeft, Vector2f ShapeBTopLeft, Vector2f ShapeASize, Vector2f ShapeBSize ) {

    IntersectingSquareSize.x = IntersectingSquareSize.x / 2;
    IntersectingSquareSize.y = IntersectingSquareSize.y / 2;
        
    if (ShapeATopLeft.x < ShapeBTopLeft.x) {

        if (ShapeATopLeft.y < ShapeBTopLeft.y) {
            //To the top left
            return (ShapeBTopLeft + IntersectingSquareSize);
        }
        else {
            //to the bottom left
            Vector2f IntersectMiddle;
            IntersectMiddle.x = ShapeBTopLeft.x + IntersectingSquareSize.x;
            IntersectMiddle.y = ShapeBTopLeft.y + ShapeBSize.y - IntersectingSquareSize.y;
            return IntersectMiddle;
        }

    }
    else {

        if (ShapeATopLeft.y < ShapeBTopLeft.y) {
            //To the top right
            Vector2f IntersectMiddle;
            IntersectMiddle.x = ShapeATopLeft.x + IntersectingSquareSize.x;
            IntersectMiddle.y = ShapeATopLeft.y + ShapeASize.y - IntersectingSquareSize.y;
            return IntersectMiddle;

        }
        else {
            // To the bottom right
            return (ShapeATopLeft + IntersectingSquareSize);
        }

    }

}

//Used when engine only handled inelastic collisions
/*
Vector2f FindNewVelocity(Vector2f Mo1, Vector2f Mo2, float mass1, float mass2) {

    Vector2f Result;

    Result = Mo1 + Mo2;
    Result = Result / (mass1 + mass2);

    return Result;
}
*/

//Old way of calcuating collision intersection distances
/*
Vector2f CollidingSquares(sf::Vector2f Size1, sf::Vector2f CO1, sf::Vector2f Size2, sf::Vector2f CO2) {

    // Center the coordinates to the middle of the sqaure
    CO1.x = CO1.x + (Size1.x / 2);
    CO1.y = CO1.y + (Size1.y / 2);
    CO2.x = CO2.x + (Size2.x / 2);
    CO2.y = CO2.y + (Size2.y / 2);

    Vector2f Distance = CO2 - CO1;
    Distance.x = abs(Distance.x);
    Distance.y = abs(Distance.y);

    Vector2f Clipping;
    Clipping.x = Distance.x - ((Size1.x / 2) + (Size2.x) / 2);
    Clipping.y = Distance.y - ((Size1.y / 2) + (Size2.y) / 2);

    return Clipping;

};
*/