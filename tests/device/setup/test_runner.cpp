/**
 * @file	test_runner.cpp
 * @brief	Defines the test runner.
 * @author	Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include <exception>
#include <string>

#include "jungles_os_helpers/freertos/thread.hpp"

extern "C" void app_main();

extern int test_main();

extern "C" void app_main()
{
    jungles::freertos::thread t{std::string("test_runner"), 2048, 5};
    t.start([]() {
        try
        {
            test_main();
        } catch (const std::exception&)
        {
            // TODO: Here log it!
        }
    });
    t.join();
}
