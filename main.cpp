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

#include <iostream>
#include <ranges>
#include <utility>
#include <deque>

#include "DVector.h"

/** For testing only: **/
#include <chrono>
#include <unordered_set>
#include <random>

#include <boost/test/unit_test.hpp>


// TODO: Create lib/module for TestUtils
namespace Utilities
{
    std::random_device randomDevice {};
    std::mt19937 generator = std::mt19937 { randomDevice() };

    int getRandomIntInRange(int start = 0, int end = 100)
    {
        std::uniform_int_distribution intDistribution { start, end  };
        return intDistribution(generator);
    }

    int getRandomUniqueInt(int start = 0, int end = 100)
    {
        static std::unordered_set<int> uniqueInts;
        std::uniform_int_distribution intDistribution { start, end  };

        while (true) {
            const int number = intDistribution(generator);
            if (uniqueInts.insert(number).second)
                return number;
        }
    }

    [[nodiscard]]
    std::deque<int> getRandomIntegerDeque(size_t size)
    {
        std::deque<int> numbers;
        while (size--)
            numbers.push_back(getRandomIntInRange(0, static_cast<int>(size * 2)));
        return numbers;
    }

    [[nodiscard]]
    std::deque<int> getRandomIntegerUniqueDeque(size_t size)
    {
        std::deque<int> numbers;
        for (size_t i = 0; i < size; ++i)
            numbers.push_back(getRandomUniqueInt(0, static_cast<int>(size * 2)));
        return numbers;
    }

    template<typename Ty>
    void assertContent(const std::deque<Ty>& contentExpected,
                       const DVector::DVector<Ty>& vector)
    {
        BOOST_CHECK_EQUAL(contentExpected.size(), vector.Size());
        for (size_t idx = 0; idx < contentExpected.size(); ++idx)
            BOOST_CHECK_EQUAL(vector[idx],  contentExpected[idx]);
    }

    template<typename Ty>
    void assertEquals(const DVector::DVector<Ty>& first,
                      const DVector::DVector<Ty>& second)
    {
        BOOST_CHECK_EQUAL(first.Size(), second.Size());
        BOOST_CHECK_EQUAL(first.Capacity(), second.Capacity());
        BOOST_CHECK_EQUAL(first.Empty(), second.Empty());
        BOOST_CHECK_EQUAL(first.FrontCapacity(), second.FrontCapacity());
        BOOST_CHECK_EQUAL(first.BackCapacity(), second.BackCapacity());

        for (size_t idx = 0; idx < first.Size(); ++idx)
            BOOST_CHECK_EQUAL(first[idx],  second[idx]);
    }
}



/**    **/
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

/**    **/
BOOST_AUTO_TEST_SUITE(CapacityTests)

    BOOST_AUTO_TEST_CASE(TestInitialCapacity)
    {
        DVector::DVector<int> dVector;

        BOOST_CHECK_EQUAL(10UL, dVector.Capacity());
        BOOST_CHECK_EQUAL(5UL, dVector.FrontCapacity());
        BOOST_CHECK_EQUAL(5UL, dVector.BackCapacity());
    }

    BOOST_AUTO_TEST_CASE(TestCapacityWithConstructor)
    {
        DVector::DVector<int> dVector(30);
        BOOST_CHECK_EQUAL(30UL, dVector.Capacity());
        BOOST_CHECK_EQUAL(15UL, dVector.FrontCapacity());
        BOOST_CHECK_EQUAL(15UL, dVector.BackCapacity());
    }

    BOOST_AUTO_TEST_CASE(CapacityAfterReallocation_PushBack)
    {
        DVector::DVector<int> dVector;
        for (size_t i = 0; i < 15; ++i)
            dVector.push_back(i);

        BOOST_CHECK_EQUAL(40UL, dVector.Capacity());
        BOOST_CHECK_EQUAL(20UL, dVector.FrontCapacity());
        BOOST_CHECK_EQUAL(5UL, dVector.BackCapacity());
    }

    BOOST_AUTO_TEST_CASE(CapacityAfterReallocation_PushFront)
    {
        DVector::DVector<int> dVector;
        for (size_t i = 0; i < 15; ++i)
            dVector.push_front(i);

        BOOST_CHECK_EQUAL(40UL, dVector.Capacity());
        BOOST_CHECK_EQUAL(5UL, dVector.FrontCapacity());
        BOOST_CHECK_EQUAL(20UL, dVector.BackCapacity());
    }

BOOST_AUTO_TEST_SUITE_END()

/**    **/
BOOST_AUTO_TEST_SUITE(PopBackMethodTests)

    BOOST_AUTO_TEST_CASE(PopBack)
    {
        std::deque<int> testValues {1, 2, 3, 4, 5};
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        dVector.pop_back();
        testValues.pop_back();

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
        BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PopBack_Reallocation)
    {
        std::deque<int> testValues {1,2,3,4,5,7,8,9,10,11,12,13,14,15};
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        dVector.pop_back();
        testValues.pop_back();

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
        BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PopBack_UntilEmpty)
    {
        std::deque<int> testValues {1,2,3,4,5,7,8,9,10,11,12,13,14,15};
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        while (!dVector.Empty())
        {
            BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
            BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
            BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
            BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
            Utilities::assertContent(testValues, dVector);

            dVector.pop_back();
            testValues.pop_back();
        }
    }

BOOST_AUTO_TEST_SUITE_END()


/**    **/
BOOST_AUTO_TEST_SUITE(BackMethodTests)

    BOOST_AUTO_TEST_CASE(CheckBack_AfterPushBack)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(4);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
    }

    BOOST_AUTO_TEST_CASE(CheckBack_AfterPushFront)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(4);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_front(v);

        BOOST_CHECK_EQUAL(testValues.front(), dVector.Back());
    }

    BOOST_AUTO_TEST_CASE(CheckBack_AfterPushBack_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(45);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
    }

    BOOST_AUTO_TEST_CASE(CheckBack_AfterPushFront_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(45);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_front(v);

        BOOST_CHECK_EQUAL(testValues.front(), dVector.Back());
    }

BOOST_AUTO_TEST_SUITE_END()


/**    **/
BOOST_AUTO_TEST_SUITE(FrontMethodTests)

    BOOST_AUTO_TEST_CASE(CheckFront_AfterPushBack)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(4);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
    }

    BOOST_AUTO_TEST_CASE(CheckFront_AfterPushFront)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(4);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_front(v);

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Front());
    }

    BOOST_AUTO_TEST_CASE(CheckFront_AfterPushBack_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(45);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
    }

    BOOST_AUTO_TEST_CASE(CheckFront_AfterPushFront_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(45);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_front(v);

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Front());
    }

BOOST_AUTO_TEST_SUITE_END()


/**    **/
BOOST_AUTO_TEST_SUITE(PopFrontMethodTests)

    BOOST_AUTO_TEST_CASE(PopFront)
    {
        std::deque<int> testValues {1, 2, 3, 4, 5};
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        dVector.pop_front();
        testValues.pop_front();

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
        BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PopFront_Reallocation)
    {
        std::deque<int> testValues {1,2,3,4,5,7,8,9,10,11,12,13,14,15};
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        dVector.pop_front();
        testValues.pop_front();

        BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
        BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PopFront_UntilEmpty)
    {
        std::deque<int> testValues {1,2,3,4,5,7,8,9,10,11,12,13,14,15};
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        while (!dVector.Empty())
        {
            BOOST_CHECK_EQUAL(testValues.back(), dVector.Back());
            BOOST_CHECK_EQUAL(testValues.front(), dVector.Front());
            BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
            BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
            Utilities::assertContent(testValues, dVector);

            dVector.pop_front();
            testValues.pop_front();
        }
    }

BOOST_AUTO_TEST_SUITE_END()


/** PushBack tests **/
BOOST_AUTO_TEST_SUITE(PushBackTests)

    BOOST_AUTO_TEST_CASE(PushBack)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(4);
        DVector::DVector<int> dVector;
        for (const auto &v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        BOOST_CHECK_EQUAL(10UL, dVector.Capacity());

        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PushBack_Realloc)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(15);
        DVector::DVector<int> dVector;
        for (const auto &v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        BOOST_CHECK_EQUAL(40UL, dVector.Capacity());

        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PushBack_RValue)
    {
        const std::deque<std::string> testValues { "I", "II", "III", "IV"};
        DVector::DVector<std::string> dVector;
        for (std::string v: testValues)
            dVector.push_back(std::move(v));

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        BOOST_CHECK_EQUAL(10UL, dVector.Capacity());

        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PushBack_CustomTypes_TODO)
    {
        const std::deque<std::string> testValues { "I", "II", "III", "IV"};
        DVector::DVector<std::string> dVector;
        for (const auto &v: testValues)
            dVector.push_back(v);


        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        BOOST_CHECK_EQUAL(10UL, dVector.Capacity());

        Utilities::assertContent(testValues, dVector);
    }

BOOST_AUTO_TEST_SUITE_END()

/** PushFront tests **/
BOOST_AUTO_TEST_SUITE(PushFrontTests)

    BOOST_AUTO_TEST_CASE(PushFront)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(4);
        DVector::DVector<int> dVector;
        for (int val : std::ranges::reverse_view(testValues)) {
            dVector.push_front(val);
        }

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        BOOST_CHECK_EQUAL(10UL, dVector.Capacity());

        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PushFront_Realloc)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(15);
        DVector::DVector<int> dVector;
        for (int val : std::ranges::reverse_view(testValues)) {
            dVector.push_front(val);
        }

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        BOOST_CHECK_EQUAL(40UL, dVector.Capacity());

        Utilities::assertContent(testValues, dVector);
    }

    BOOST_AUTO_TEST_CASE(PushFront_CustomTypes_TODO)
    {
        const std::deque<std::string> testValues { "I", "II", "III", "IV"};
        DVector::DVector<std::string> dVector;
        for (const std::string& val : std::ranges::reverse_view(testValues)) {
            dVector.push_front(val);
        }

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        BOOST_CHECK_EQUAL(testValues.empty(), dVector.Empty());
        BOOST_CHECK_EQUAL(10UL, dVector.Capacity());

        Utilities::assertContent(testValues, dVector);
    }

BOOST_AUTO_TEST_SUITE_END()

/**  Constructor tests  **/
BOOST_AUTO_TEST_SUITE(ConstructorTests)

    BOOST_AUTO_TEST_CASE(CreateVectorTest)
    {
        DVector::DVector<int> dVector;
        BOOST_CHECK_EQUAL(0UL, dVector.Size());
        BOOST_CHECK_EQUAL(true, dVector.Empty());
        BOOST_CHECK_EQUAL(10UL, dVector.Capacity());
    }

    BOOST_AUTO_TEST_CASE(CreateVector_CustomCapacity)
    {
        DVector::DVector<int> dVector (100);
        BOOST_CHECK_EQUAL(0UL, dVector.Size());
        BOOST_CHECK_EQUAL(true, dVector.Empty());
        BOOST_CHECK_EQUAL(100UL, dVector.Capacity());
    }

BOOST_AUTO_TEST_SUITE_END()


/**  CopyConstructor tests  **/
BOOST_AUTO_TEST_SUITE(CopyConstructor)

    BOOST_AUTO_TEST_CASE(CopyConstructorTests)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(7);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        const DVector::DVector<int> dVectorCopy (dVectorOrig);
        Utilities::assertEquals(dVectorOrig, dVectorCopy);
    }

    BOOST_AUTO_TEST_CASE(CopyConstructorTests_Realloc)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(50);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        const DVector::DVector<int> dVectorCopy (dVectorOrig);
        Utilities::assertEquals(dVectorOrig, dVectorCopy);
    }

BOOST_AUTO_TEST_SUITE_END()


/**  CopyAssignmentOperator tests  **/
BOOST_AUTO_TEST_SUITE(CopyAssignmentOperator)

    BOOST_AUTO_TEST_CASE(CopyAssignmentTests)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(7);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        DVector::DVector<int> dVectorCopy;
        dVectorCopy = dVectorOrig;

        Utilities::assertEquals(dVectorOrig, dVectorCopy);
    }

    BOOST_AUTO_TEST_CASE(CopyAssignmentTests_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(50);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        DVector::DVector<int> dVectorCopy;
        dVectorCopy = dVectorOrig;

        Utilities::assertEquals(dVectorOrig, dVectorCopy);
    }

BOOST_AUTO_TEST_SUITE_END()

/**  MoveConstructor tests  **/
BOOST_AUTO_TEST_SUITE(MoveConstructorTests)

    BOOST_AUTO_TEST_CASE(MoveConstruction_Basic)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(7);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        DVector::DVector<int> dVectorDest = std::move(dVectorOrig);

        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Size());
        BOOST_CHECK_EQUAL(true, dVectorOrig.Empty());
        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Capacity());

        BOOST_CHECK_EQUAL(testValues.size(), dVectorDest.Size());
        BOOST_CHECK_EQUAL(false, dVectorDest.Empty());
        BOOST_CHECK_EQUAL(40UL, dVectorDest.Capacity());

        Utilities::assertContent(testValues, dVectorDest);
    }

    BOOST_AUTO_TEST_CASE(MoveConstruction_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(57);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        DVector::DVector<int> dVectorDest = std::move(dVectorOrig);

        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Size());
        BOOST_CHECK_EQUAL(true, dVectorOrig.Empty());
        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Capacity());

        BOOST_CHECK_EQUAL(testValues.size(), dVectorDest.Size());
        BOOST_CHECK_EQUAL(false, dVectorDest.Empty());
        BOOST_CHECK_EQUAL(160UL, dVectorDest.Capacity());

        Utilities::assertContent(testValues, dVectorDest);
    }

BOOST_AUTO_TEST_SUITE_END()

/**  MoveAssignmentOperator tests  **/
BOOST_AUTO_TEST_SUITE(MoveAssignmentOperatorTests)

    BOOST_AUTO_TEST_CASE(MoveAssignment)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(7);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        DVector::DVector<int> dVectorDest;
        dVectorDest = std::move(dVectorOrig);

        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Size());
        BOOST_CHECK_EQUAL(true, dVectorOrig.Empty());
        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Capacity());

        BOOST_CHECK_EQUAL(testValues.size(), dVectorDest.Size());
        BOOST_CHECK_EQUAL(false, dVectorDest.Empty());
        BOOST_CHECK_EQUAL(40UL, dVectorDest.Capacity());

        Utilities::assertContent(testValues, dVectorDest);
    }

    BOOST_AUTO_TEST_CASE(MoveAssignment_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(57);
        DVector::DVector<int> dVectorOrig;
        for (int v: testValues)
            dVectorOrig.push_back(v);

        DVector::DVector<int> dVectorDest;
        dVectorDest = std::move(dVectorOrig);

        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Size());
        BOOST_CHECK_EQUAL(true, dVectorOrig.Empty());
        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Capacity());

        BOOST_CHECK_EQUAL(testValues.size(), dVectorDest.Size());
        BOOST_CHECK_EQUAL(false, dVectorDest.Empty());
        BOOST_CHECK_EQUAL(160UL, dVectorDest.Capacity());

        Utilities::assertContent(testValues, dVectorDest);
    }

BOOST_AUTO_TEST_SUITE_END()


/**  push_back() method tests  **/
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

/**  Clear() method tests  **/
BOOST_AUTO_TEST_SUITE(ClearMethodTests)

    BOOST_AUTO_TEST_CASE(ClearVector)
    {
        constexpr size_t size {55};
        DVector::DVector<int> dVector;
        for (int i = 0; i < static_cast<int>(size); ++i)
            dVector.push_back(i);

        BOOST_CHECK_EQUAL(size, dVector.Size());
        BOOST_CHECK_EQUAL(false, dVector.Empty());

        dVector.Clear();

        BOOST_CHECK_EQUAL(0, dVector.Size());
        BOOST_CHECK_EQUAL(true, dVector.Empty());
    }

BOOST_AUTO_TEST_SUITE_END()

/**  Clear() method tests  **/
BOOST_AUTO_TEST_SUITE(SizeTests)

    BOOST_AUTO_TEST_CASE(CheckAfterPush_NoReallocation)
    {
        constexpr size_t size {5};
        DVector::DVector<int> dVector;
        for (int i = 0; i < static_cast<int>(size); ++i)
            dVector.push_back(i);

        BOOST_CHECK_EQUAL(10U, dVector.Capacity());
        BOOST_CHECK_EQUAL(size, dVector.Size());
    }

    BOOST_AUTO_TEST_CASE(CheckAfterPush_Reallocation)
    {
        constexpr size_t size {15};
        DVector::DVector<int> dVector;
        for (int i = 0; i < static_cast<int>(size); ++i)
            dVector.push_back(i);

        BOOST_CHECK_EQUAL(40U, dVector.Capacity());
        BOOST_CHECK_EQUAL(size, dVector.Size());
    }

    BOOST_AUTO_TEST_CASE(CheckAfterMove)
    {
        constexpr size_t size {20};
        DVector::DVector<int> dVectorOrig;
        for (int i = 0; i < static_cast<int>(size); ++i)
            dVectorOrig.push_back(i);

        DVector::DVector<int> dVectorDest = std::move(dVectorOrig);

        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Size());
        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Capacity());
        BOOST_CHECK_EQUAL(true, dVectorOrig.Empty());

        BOOST_CHECK_EQUAL(size, dVectorDest.Size());
        BOOST_CHECK_EQUAL(40U, dVectorDest.Capacity());
        BOOST_CHECK_EQUAL(false, dVectorDest.Empty());
    }
BOOST_AUTO_TEST_SUITE_END()

/**    **/
BOOST_AUTO_TEST_SUITE(AtMethodMethodTests)

    BOOST_AUTO_TEST_CASE(GetElement)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(7);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        for (size_t idx = 0; idx < testValues.size(); ++idx)
            BOOST_CHECK_EQUAL(dVector[idx], testValues[idx]);
    }

    BOOST_AUTO_TEST_CASE(GetElement_Reallocation)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(57);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        for (size_t idx = 0; idx < testValues.size(); ++idx)
            BOOST_CHECK_EQUAL(dVector[idx], testValues[idx]);
    }

    BOOST_AUTO_TEST_CASE(OutOufRangeError)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(7);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_REQUIRE_THROW([&]{ [[maybe_unused]] auto x = dVector.at(10);}(), std::out_of_range);
    }

BOOST_AUTO_TEST_SUITE_END()

/**  IndexOperatorTests  **/
BOOST_AUTO_TEST_SUITE(IndexOperatorTests)

    BOOST_AUTO_TEST_CASE(BasicTest)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(7);
        DVector::DVector<int> dVector;
        for (int v: testValues)
            dVector.push_back(v);

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        for (size_t idx = 0; idx < testValues.size(); ++idx)
            BOOST_CHECK_EQUAL(dVector[idx], testValues[idx]);
    }

    BOOST_AUTO_TEST_CASE(CheckValues_With_PushBack_and_PushFront)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(8);

        constexpr int N = 3;
        DVector::DVector<int> dVector;
        for (size_t idx = N; idx < testValues.size(); ++idx)
            dVector.push_back(testValues[idx]);
        for (int idx = N - 1; idx >= 0; --idx)
            dVector.push_front(testValues[idx]);

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        for (size_t idx = 0; idx < testValues.size(); ++idx)
            BOOST_CHECK_EQUAL(dVector[idx], testValues[idx]);
    }

    BOOST_AUTO_TEST_CASE(CheckValues_With_PushBack_and_PushFront_Realloc)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(100);

        constexpr int N = 40;
        DVector::DVector<int> dVector;
        for (size_t idx = N; idx < testValues.size(); ++idx)
            dVector.push_back(testValues[idx]);
        for (int idx = N - 1; idx >= 0; --idx)
            dVector.push_front(testValues[idx]);

        BOOST_CHECK_EQUAL(testValues.size(), dVector.Size());
        for (size_t idx = 0; idx < testValues.size(); ++idx)
            BOOST_CHECK_EQUAL(dVector[idx], testValues[idx]);
    }

    BOOST_AUTO_TEST_CASE(CheckValues_AfterMove)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(100);

        constexpr int N = 40;
        DVector::DVector<int> dVectorOrig;
        for (size_t idx = N; idx < testValues.size(); ++idx)
            dVectorOrig.push_back(testValues[idx]);
        for (int idx = N - 1; idx >= 0; --idx)
            dVectorOrig.push_front(testValues[idx]);

        const DVector::DVector<int> dVectorDest = std::move(dVectorOrig);

        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Size());
        BOOST_CHECK_EQUAL(0UL, dVectorOrig.Capacity());
        BOOST_CHECK_EQUAL(true, dVectorOrig.Empty());

        BOOST_CHECK_EQUAL(testValues.size(), dVectorDest.Size());
        for (size_t idx = 0; idx < testValues.size(); ++idx)
            BOOST_CHECK_EQUAL(dVectorDest[idx], testValues[idx]);
    }

    BOOST_AUTO_TEST_CASE(CheckValues_WithCopyConstructor)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerDeque(100);

        constexpr int N = 40;
        DVector::DVector<int> dVectorOrig;
        for (size_t idx = N; idx < testValues.size(); ++idx)
            dVectorOrig.push_back(testValues[idx]);
        for (int idx = N - 1; idx >= 0; --idx)
            dVectorOrig.push_front(testValues[idx]);

        const DVector::DVector<int> dVectorDest(dVectorOrig);

        for (size_t idx = 0; idx < testValues.size(); ++idx)
            BOOST_CHECK_EQUAL(dVectorDest[idx], testValues[idx]);
    }

BOOST_AUTO_TEST_SUITE_END()


/**  EmptyMethodTests  **/
BOOST_AUTO_TEST_SUITE(EmptyMethodTests)

    BOOST_AUTO_TEST_CASE(CheckAfterPush)
    {
        constexpr size_t size {55};
        DVector::DVector<int> dVector;
        for (int i = 0; i < static_cast<int>(size); ++i)
            dVector.push_back(i);

        BOOST_CHECK_EQUAL(size, dVector.Size());
        BOOST_CHECK_EQUAL(false, dVector.Empty());
    }

    BOOST_AUTO_TEST_CASE(CheckAfterClear)
    {
        constexpr size_t size {55};
        DVector::DVector<int> dVector;
        for (int i = 0; i < static_cast<int>(size); ++i)
            dVector.push_back(i);

        BOOST_CHECK_EQUAL(size, dVector.Size());
        BOOST_CHECK_EQUAL(false, dVector.Empty());

        dVector.Clear();

        BOOST_CHECK_EQUAL(0UL, dVector.Size());
        BOOST_CHECK_EQUAL(true, dVector.Empty());
    }

    BOOST_AUTO_TEST_CASE(CheckAfterMove)
    {
        constexpr size_t size {55};
        DVector::DVector<int> dVector1;
        for (int i = 0; i < static_cast<int>(size); ++i)
            dVector1.push_back(i);

        DVector::DVector<int> dVector2 = std::move(dVector1);

        BOOST_CHECK_EQUAL(0UL, dVector1.Size());
        BOOST_CHECK_EQUAL(true, dVector1.Empty());

        BOOST_CHECK_EQUAL(size, dVector2.Size());
        BOOST_CHECK_EQUAL(false, dVector2.Empty());
    }

BOOST_AUTO_TEST_SUITE_END()

/**  DataMethodTests  **/
BOOST_AUTO_TEST_SUITE(DataMethodTests)

    BOOST_AUTO_TEST_CASE(GetData_CheckValues)
    {
        DVector::DVector<int> dVector;

        const std::deque<int> testValues = Utilities::getRandomIntegerUniqueDeque(5);
        for (int v: testValues)
            dVector.push_back(v);

        const int* data = dVector.Data();
        for (size_t idx = 0; idx < dVector.Size(); ++idx)
            BOOST_CHECK_EQUAL(testValues[idx], data[idx]);
    }

    BOOST_AUTO_TEST_CASE(GetData_CheckValues2)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerUniqueDeque(20);

        constexpr int N = 5;
        DVector::DVector<int> dVector;
        for (size_t idx = N; idx < testValues.size(); ++idx)
            dVector.push_back(testValues[idx]);
        for (int idx = N - 1; idx >= 0; --idx)
            dVector.push_front(testValues[idx]);

        const int* data = dVector.Data();
        for (size_t idx = 0; idx < dVector.Size(); ++idx)
            BOOST_CHECK_EQUAL(testValues[idx], data[idx]);
    }

    BOOST_AUTO_TEST_CASE(GetData_CheckValues_Realloc)
    {
        const std::deque<int> testValues = Utilities::getRandomIntegerUniqueDeque(100);

        constexpr int N = 40;
        DVector::DVector<int> dVector;
        for (size_t idx = N; idx < testValues.size(); ++idx)
            dVector.push_back(testValues[idx]);
        for (int idx = N - 1; idx >= 0; --idx)
            dVector.push_front(testValues[idx]);

        const int* data = dVector.Data();
        for (size_t idx = 0; idx < dVector.Size(); ++idx)
            BOOST_CHECK_EQUAL(testValues[idx], data[idx]);
    }

BOOST_AUTO_TEST_SUITE_END()
