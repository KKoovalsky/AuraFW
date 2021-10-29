/**
 * @file    catch2_main.cpp
 * @brief   Defines miscellaneous for Catch2, to run it on the device.
 * @author  Kacper Kowalski - kacper.s.kowalski@gmail.com
 */

#include "catch2/catch_session.hpp"

#include <sstream>

namespace Catch
{

std::ostream& cout()
{
    static auto buf{new std::stringbuf};
    static auto output_stream{new std::ostream{buf}};
    return *output_stream;
}
std::ostream& clog()
{
    return cout();
}
std::ostream& cerr()
{
    return cout();
}

} // namespace Catch

extern "C" void app_main()
{
    auto result{Catch::Session().run()};
}
