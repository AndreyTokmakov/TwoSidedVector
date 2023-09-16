/**============================================================================
Name        : main.cpp
Created on  : 15.09.2023
Author      : Andrei Tokmakov
Version     : 1.0
Copyright   : Your copyright notice
Description : DVector.cpp
============================================================================**/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest

#include <cstdlib>
#include <iostream>
#include <utility>
#include "DVector.h"

#include <boost/version.hpp>
#include <boost/test/unit_test.hpp>


namespace
{
    struct Fixture
    {
        Fixture() = default;

        // And here it is correct to finish working with it
        ~Fixture() = default;

        /** And here is the test object itself **/
        DVector::DVector<int> dVector;
    };

}

/*
BOOST_AUTO_TEST_CASE(MyTestCase)
{
    // To simplify this example test, let's suppose we'll test 'float'.
    // Some test are stupid, but all should pass.
    float x = 9.5f;

    BOOST_CHECK(x != 0.0f);
    BOOST_CHECK_EQUAL((int)x, 9);
    BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}*/

BOOST_AUTO_TEST_SUITE(CreateBasicTests)

    BOOST_FIXTURE_TEST_CASE(CreateAndCheckSize, Fixture)
    {
        BOOST_CHECK(0 == dVector.Size());
    }

    BOOST_FIXTURE_TEST_CASE(CreateAndCheckDefaultCapacity, Fixture)
    {
        BOOST_CHECK(0 == dVector.Capacity());
    }

    BOOST_FIXTURE_TEST_CASE(CreateAndCheckCustomCapacity, Fixture)
    {
        BOOST_CHECK(0 == dVector.Capacity());
    }

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(PushBackTests)

    BOOST_FIXTURE_TEST_CASE(PushBackFewElements, Fixture)
    {
        dVector.push_back(21);
        dVector.push_back(22);
        dVector.push_back(23);

        //vect.printInfo();
    }

BOOST_AUTO_TEST_SUITE_END()

