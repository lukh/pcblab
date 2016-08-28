#ifndef TEST_APERTUREPRIMITIVE_H
#define TEST_APERTUREPRIMITIVE_H

#include <cppunit/Test.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestResult.h>
#include <cppunit/extensions/HelperMacros.h>

#include <cppunit/ui/text/TestRunner.h>

#include "aperture/apertureprimitive.h"

class Test_APCircle: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(Test_APCircle);

    CPPUNIT_TEST(test_isValid);

    CPPUNIT_TEST(test_boundingBox_Centered);

    CPPUNIT_TEST(test_boundingBox_TopLeft);
    CPPUNIT_TEST(test_boundingBox_TopRight);
    CPPUNIT_TEST(test_boundingBox_BottomLeft);
    CPPUNIT_TEST(test_boundingBox_BottomRight);

    CPPUNIT_TEST(test_boundingBox_Rotation90);

    CPPUNIT_TEST_SUITE_END();

    public:
        Test_APCircle();

        void setUp();
        void tearDown();

        void test_isValid();

        void test_boundingBox_Centered();

        void test_boundingBox_TopLeft();
        void test_boundingBox_TopRight();
        void test_boundingBox_BottomLeft();
        void test_boundingBox_BottomRight();

        void test_boundingBox_Rotation90();


    private:
        APrimCircle mCircleCentered;

        APrimCircle mCircleTopLeft;
        APrimCircle mCircleTopRight;
        APrimCircle mCircleBottomLeft;
        APrimCircle mCircleBottomRight;

        APrimCircle mCircleRotation90;

};

class Test_APVectorLine: public CppUnit::TestFixture{
    CPPUNIT_TEST_SUITE(Test_APVectorLine);

    CPPUNIT_TEST(test_isValid);
    CPPUNIT_TEST(test_boundingBox_0Deg);
    CPPUNIT_TEST(test_boundingBox_0DegInv);

    CPPUNIT_TEST_SUITE_END();

    public:
        Test_APVectorLine();

        void setUp();
        void tearDown();

        void test_isValid();

        void test_boundingBox_0Deg();
        void test_boundingBox_0DegInv();

    private:
        // start at 0,0 finish at 10, 10
        APrimVectorLine mVL0deg;

        //start at 10,10, finish at 0,0
        APrimVectorLine mVL0degInv;
};

#endif // TEST_APERTUREPRIMITIVE_H