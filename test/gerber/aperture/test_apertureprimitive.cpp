#include "test_apertureprimitive.h"

#include <iostream>

using namespace std;

Test_APCircle::Test_APCircle()
{

}

void Test_APCircle::setUp()
{
    // --- centered ---
    //exposure
    mCircleCentered.addModifier(1.0);
    //diameter
    mCircleCentered.addModifier(10.0);
    //X
    mCircleCentered.addModifier(0.0);
    //Y
    mCircleCentered.addModifier(0.0);

    // --- top left ---
    //exposure
    mCircleTopLeft.addModifier(1.0);
    //diameter
    mCircleTopLeft.addModifier(10.0);
    //X
    mCircleTopLeft.addModifier(-5.0);
    //Y
    mCircleTopLeft.addModifier(5.0);

    // --- top right ---
    //exposure
    mCircleTopRight.addModifier(1.0);
    //diameter
    mCircleTopRight.addModifier(10.0);
    //X
    mCircleTopRight.addModifier(5.0);
    //Y
    mCircleTopRight.addModifier(5.0);

    // --- Bottom left ---
    //exposure
    mCircleBottomLeft.addModifier(1.0);
    //diameter
    mCircleBottomLeft.addModifier(10.0);
    //X
    mCircleBottomLeft.addModifier(-5.0);
    //Y
    mCircleBottomLeft.addModifier(-5.0);

    // --- Bottom right ---
    //exposure
    mCircleBottomRight.addModifier(1.0);
    //diameter
    mCircleBottomRight.addModifier(10.0);
    //X
    mCircleBottomRight.addModifier(5.0);
    //Y
    mCircleBottomRight.addModifier(-5.0);


    // --- rotation 90 ---
    //exposure
    mCircleRotation90.addModifier(1.0);
    //diameter
    mCircleRotation90.addModifier(1.0);
    //X
    mCircleRotation90.addModifier(5.0);
    //Y
    mCircleRotation90.addModifier(0.0);
    //ROT
    mCircleRotation90.addModifier(90.0);
}

void Test_APCircle::tearDown()
{

}

void Test_APCircle::test_isValid()
{
    CPPUNIT_ASSERT(mCircleCentered.isValid());

    CPPUNIT_ASSERT(mCircleTopLeft.isValid());
    CPPUNIT_ASSERT(mCircleTopRight.isValid());
    CPPUNIT_ASSERT(mCircleBottomLeft.isValid());
    CPPUNIT_ASSERT(mCircleBottomRight.isValid());

    CPPUNIT_ASSERT(mCircleRotation90.isValid());

}

void Test_APCircle::test_boundingBox_Centered()
{
    Rectangle bb = mCircleCentered.getBoundingBox();
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mX, -5.0));
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mY, -5.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mX, +5.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mY, +5.0));
}



void Test_APCircle::test_boundingBox_TopLeft()
{
    Rectangle bb = mCircleTopLeft.getBoundingBox();
    //cout << "x1:" << bb.getBottomLeft().mX << " y1: " << bb.getTopRight().mY << " x2: " << bb.getTopRight().mX << " y2: " << bb.getBottomLeft().mY << endl;
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mX, -10.0));
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mY, 0.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mX, 0.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mY, +10.0));
}

void Test_APCircle::test_boundingBox_TopRight()
{
    Rectangle bb = mCircleTopRight.getBoundingBox();
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mX, 0.0));
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mY, 0.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mX, +10.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mY, +10.0));
}

void Test_APCircle::test_boundingBox_BottomLeft()
{
    Rectangle bb = mCircleBottomLeft.getBoundingBox();
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mX, -10.0));
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mY, -10.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mX, 0.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mY, 0.0));
}

void Test_APCircle::test_boundingBox_BottomRight()
{
    Rectangle bb = mCircleBottomRight.getBoundingBox();
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mX, 0.0));
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mY, -10.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mX, +10.0));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mY, 0.0));
}

void Test_APCircle::test_boundingBox_Rotation90()
{
    Rectangle bb = mCircleRotation90.getBoundingBox();
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mX, -0.5));
    CPPUNIT_ASSERT(isEqual(bb.getBottomLeft().mY, 4.5));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mX, +0.5));
    CPPUNIT_ASSERT(isEqual(bb.getTopRight().mY, +5.5));
}



int main(int argc, char **argv){
    CppUnit::TextUi::TestRunner runner;
    runner.addTest( Test_APCircle::suite() );

    runner.run();
    return 0;
}
