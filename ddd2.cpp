#define BOOST_TEST_MODULE MultiplicationFunctionTests
#include <boost/test/unit_test.hpp>

int multiplication(int multiplicand, int multiplier)
{
    return multiplicand*multiplier;
}

BOOST_AUTO_TEST_SUITE(MultiplicationTestSuite)

BOOST_AUTO_TEST_CASE (testPositiveNumbers) {
    BOOST_CHECK_EQUAL (multiplication(5, 5), 25);
}

BOOST_AUTO_TEST_CASE (testNegativeNumbers) {
    BOOST_CHECK_EQUAL(multiplication(-5, -5), 25);
    BOOST_CHECK_EQUAL(multiplication(-5, 5), -25);
    BOOST_CHECK_EQUAL(multiplication(5, -5), -25);
}

BOOST_AUTO_TEST_CASE (testZero) {
    BOOST_CHECK_EQUAL(multiplication(0, 5), 0);
    BOOST_CHECK_EQUAL(multiplication(5, 0), 0);
}

BOOST_AUTO_TEST_CASE (testBoundaryValues) {
    BOOST_CHECK_EQUAL(multiplication(std::numeric_limits<int>::max(), 1), std::numeric_limits<int>::max());
    BOOST_CHECK_EQUAL(multiplication(std::numeric_limits<int>::min(), 1), std::numeric_limits<int>::min());
}

BOOST_AUTO_TEST_CASE(testOverlimitValues) {
    int max = std::numeric_limits<int>::max();
    int min = std::numeric_limits<int>::min();
    BOOST_CHECK_EQUAL(multiplication(max, max), 1);
    BOOST_CHECK_EQUAL(multiplication(min, min), 0);
}

BOOST_AUTO_TEST_SUITE_END();