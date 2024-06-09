#include <gtest/gtest.h>

TEST(MyTestSuite, MyFirstTest) {
    // Your test code goes here
    ASSERT_EQ(2 + 2, 5);  // Example assertion
}

TEST(MyTestSuite, MySecondTest) {
    // Your test code goes here
    ASSERT_EQ(2 + 1, 4);  // Example assertion
}