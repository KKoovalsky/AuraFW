#include "catch2/catch_test_macros.hpp"

TEST_CASE("Dummy test", "[dummy_test]")
{
    SECTION("Dummy section")
    {
        SUCCEED();
    }
}