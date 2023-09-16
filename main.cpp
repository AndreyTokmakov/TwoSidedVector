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

/*
namespace
{
    struct Fixture
    {
        Fixture() = default;

        // And here it is correct to finish working with it
        ~Fixture() = default;

        DVector::DVector<int> dVector;
    };
}

BOOST_AUTO_TEST_CASE(MyTestCase)
{
    // To simplify this example test, let's suppose we'll test 'float'.
    // Some test are stupid, but all should pass.
    float x = 9.5f;

    BOOST_CHECK(x != 0.0f);
    BOOST_CHECK_EQUAL((int)x, 9);
    BOOST_CHECK_CLOSE(x, 9.5f, 0.0001f); // Checks differ no more then 0.0001%
}
 */

BOOST_AUTO_TEST_SUITE(CreateBasicTests)

    BOOST_AUTO_TEST_CASE(CreateAndCheckSize)
    {
        DVector::DVector<int> dVector;
        BOOST_CHECK_EQUAL(0, dVector.Size());
    }

    BOOST_AUTO_TEST_CASE(CreateAndCheckDefaultCapacity)
    {
        DVector::DVector<int> dVector;
        BOOST_CHECK_EQUAL(10, dVector.Capacity());
    }

    BOOST_AUTO_TEST_CASE(CreateAndCheckCustomCapacity)
    {
        constexpr size_t capacity { 20 };
        DVector::DVector<int> dVector (capacity);
        BOOST_CHECK_EQUAL(capacity, dVector.Capacity());
    }

BOOST_AUTO_TEST_SUITE_END()




BOOST_AUTO_TEST_SUITE(PushBackTests)

    BOOST_AUTO_TEST_CASE(PushBackFewElements)
    {
        DVector::DVector<int> dVector;
        dVector.push_back(21);
        dVector.push_back(22);
        dVector.push_back(23);

        //vect.printInfo();
    }

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(ClearMethodTests)

    BOOST_AUTO_TEST_CASE(ClearVector)
    {
        DVector::DVector<int> dVector;
        for (int i = 0; i < 55; ++i)
            dVector.push_back(i);

        BOOST_CHECK_EQUAL(55, dVector.Size());
        BOOST_CHECK_EQUAL(false, dVector.Empty());

        dVector.Clear();

        BOOST_CHECK_EQUAL(0, dVector.Size());
        BOOST_CHECK_EQUAL(true, dVector.Empty());
    }

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(DataMethodTests)

    BOOST_AUTO_TEST_CASE(GetData_CheckValues)
    {
        DVector::DVector<int> dVector;

        const std::vector<int> testValues { 3, 4, 5, 6, 7};
        for (int v: testValues)
            dVector.push_back(v);

        const int* data = dVector.Data();
        for (size_t idx = 0; idx < dVector.Size(); ++idx)
        {
            BOOST_CHECK_EQUAL(testValues[idx], data[idx]);
        }
    }

BOOST_AUTO_TEST_SUITE_END()
