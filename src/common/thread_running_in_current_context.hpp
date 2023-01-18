/**
 * @file        thread_running_in_current_context.hpp
 * @brief       A thread which immediately runs in the current context, so it doesn't spawn any background thread, but
 *              runs in the current active thread.
 */
#ifndef THREAD_RUNNING_IN_CURRENT_CONTEXT_HPP
#define THREAD_RUNNING_IN_CURRENT_CONTEXT_HPP

namespace experimental::native
{

namespace detail
{

template<typename T>
struct ClassOf
{
};

template<typename ReturnType, typename Class>
struct ClassOf<ReturnType(Class::*)>
{
    using type = Class;
};

template<typename T>
concept Method = requires(T)
{
    typename ClassOf<T>::type;
};

} // namespace detail

template<auto MethodPointer>
requires detail::Method<decltype(MethodPointer)>
class thread_running_in_current_context
{
  public:
    using Owner = typename detail::ClassOf<decltype(MethodPointer)>::type;

    void start(Owner* o)
    {
        (o->*MethodPointer)();
    }

    void join()
    {
    }

  private:
};

} // namespace experimental::native

#endif /* THREAD_RUNNING_IN_CURRENT_CONTEXT_HPP */
