#include "PhysicsEngineFunctions.h"
#define BOOST_TEST_MODULE PhysicsEngineFunctionTests
#include <boost/test/included/unit_test.hpp>

// New Position / Force Tests
// Vector2f FindNewPosition(Vector2f u, float t, Vector2f a);
BOOST_AUTO_TEST_CASE(testNoAccNoSpeed)
{
	Vector2f TestCase = FindNewPosition(Vector2f(0, 0), 100, Vector2f(0, 0));
	Vector2f Answer = Vector2f(0, 0);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}

BOOST_AUTO_TEST_CASE(testNoAccInitialSpeed)
{
	Vector2f TestCase = FindNewPosition(Vector2f(5, 10), 100, Vector2f(0, 0));
	Vector2f Answer = Vector2f(500, 1000);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}

BOOST_AUTO_TEST_CASE(testGravity)
{
	Vector2f TestCase = FindNewPosition(Vector2f(0, 0), 5, Vector2f(0, 9.8));
	Vector2f Answer = Vector2f(0, 122.5);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}

// Dot Product Unit Tests
// float DotProduct(Vector2f Vec1, Vector2f Vec2);
BOOST_AUTO_TEST_CASE(testDotProductZero)
{
	float TestCase = DotProduct(Vector2f(0, 0), Vector2f(0, 0));
	float Answer = 0;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

BOOST_AUTO_TEST_CASE(testDotProductCancel)
{
	float TestCase = DotProduct(Vector2f(4, 0), Vector2f(0, 4));
	float Answer = 0;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

BOOST_AUTO_TEST_CASE(testDotProduct64)
{
	float TestCase = DotProduct(Vector2f(4, 3), Vector2f(4, 3));
	float Answer = 25;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

// Unit Vector Tests
// Vector2f VectorNormilisation(Vector2f Vec);
BOOST_AUTO_TEST_CASE(testVectorNormilisation)
{
	Vector2f TestCase = VectorNormilisation(Vector2f(4, 3));
	Vector2f Answer = Vector2f(0.8, 0.6);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}

// Collision Detection Tests
// Vector2f CollidingSquares(Vector2f ShapeATopLeft, Vector2f ShapeABottomRight, Vector2f ShapeBTopLeft, Vector2f ShapeBBottomRight);
BOOST_AUTO_TEST_CASE(testIntersectionFalsePositive1)
{
	bool TestCase = false;
	Vector2f Intersection = CollidingSquares(Vector2f(0, 0), Vector2f(4, 4), Vector2f(5, 5), Vector2f(100, 100));
	if (Intersection.x > 0 && Intersection.y > 0) { TestCase = true; }

	bool Answer = false;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

BOOST_AUTO_TEST_CASE(testIntersectionAllZeros)
{
	bool TestCase = false;
	Vector2f Intersection = CollidingSquares(Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0), Vector2f(0, 0));
	if (Intersection.x > 0 && Intersection.y > 0) { TestCase = true; }

	bool Answer = false;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

BOOST_AUTO_TEST_CASE(testIntersectionInvertedSquares)
{
	bool TestCase = false;
	Vector2f Intersection = CollidingSquares(Vector2f(0, 0), Vector2f(100, 100), Vector2f(5, 5), Vector2f(6, 6));
	if (Intersection.x > 0 && Intersection.y > 0) { TestCase = true; }

	bool Answer = true;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

BOOST_AUTO_TEST_CASE(testIntersectionLengths)
{
	Vector2f TestCase = CollidingSquares(Vector2f(0,0), Vector2f(4,4), Vector2f(3,3), Vector2f(100,100));
	Vector2f Answer = Vector2f(1,1);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}

// Intersecting Square Origin Tests
// Vector2f IntersectingSquarePos(Vector2f IntersectingSquareSize, Vector2f ShapeATopLeft, Vector2f ShapeBTopLeft, Vector2f ShapeASize, Vector2f ShapeBSize);

BOOST_AUTO_TEST_CASE(testGiganticIntersect)
{
	Vector2f TestCase = IntersectingSquarePos(Vector2f(100, 100), Vector2f(0, 0), Vector2f(0, 0), Vector2f(100, 100), Vector2f(100, 100));
	Vector2f Answer = Vector2f(50, 50);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}


BOOST_AUTO_TEST_CASE(testTinyIntersect)
{
	Vector2f TestCase = IntersectingSquarePos(Vector2f(1, 1), Vector2f(4, 4), Vector2f(5, 5), Vector2f(2, 2), Vector2f(1, 1));
	Vector2f Answer = Vector2f(5.5, 5.5);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}

// Impulse Calculation Tests
// float Impulse(Vector2f Vel1, Vector2f Vel2, Vector2f VelNormal, float Mass1, float Mass2, float ResCoeff);
BOOST_AUTO_TEST_CASE(testImpulseStandard)
{
	float TestCase = Impulse(Vector2f(100, 0), Vector2f(-100, 0), Vector2f(1, 0), 50, 50, 0.5);
	float Answer = -7500;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

BOOST_AUTO_TEST_CASE(testImpulseInverted)
{
	float TestCase = Impulse(Vector2f(100, 0), Vector2f(-100, 0), Vector2f(-1, 0), 50, 50, 0.5);
	float Answer = 7500;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

BOOST_AUTO_TEST_CASE(testImpulseZeroCoeff)
{
	float TestCase = Impulse(Vector2f(100, 0), Vector2f(-100, 0), Vector2f(1, 0), 50, 50, 0.0);
	float Answer = -5000;
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer << " NOT " << TestCase);
}

// Resultant Velocity of First Object Tests 
//Vector2f NewVel1(float Impluse, Vector2f InitSpeed, float Mass, Vector2f VelNormal);
BOOST_AUTO_TEST_CASE(testResVelocityShapeA)
{
	Vector2f TestCase = NewVel1(-7500, Vector2f(100, 0), 50, Vector2f(1, 0));
	Vector2f Answer = Vector2f(-50,0);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}

// Resultant Velocity of Second Object Tests 
//Vector2f NewVel2(float Impluse, Vector2f InitSpeed, float Mass, Vector2f VelNormal);
BOOST_AUTO_TEST_CASE(testResVelocityShapeB)
{
	Vector2f TestCase = NewVel2(-7500, Vector2f(-100, 0), 50, Vector2f(1, 0));
	Vector2f Answer = Vector2f(50, 0);
	BOOST_CHECK_MESSAGE(TestCase == Answer, "Should be " << Answer.x << "," << Answer.y << " NOT " << TestCase.x << "," << TestCase.y);
}