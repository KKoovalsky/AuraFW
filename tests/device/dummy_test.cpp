#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

extern "C" void app_main()
{
    int argc{1};
    char argv1[]{"aura"};
    char* argv[] = {argv1};
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
}

extern "C" int mkdir(const char*, mode_t)
{
}