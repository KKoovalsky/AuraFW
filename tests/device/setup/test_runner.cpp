/**
 * @file	test_runner.cpp
 * @brief	Defines the test runner.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <exception>

extern "C" void app_main();

extern int test_main();

extern "C" void app_main()
{
    try
    {
        test_main();
    } catch (const std::exception&)
    {
        // TODO: Here log it!
    }
}
