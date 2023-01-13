/**
 * @file        active.hpp
 * @brief       Active Object Pattern implementation.
 */
#ifndef ACTIVE_HPP
#define ACTIVE_HPP

#include <functional>

template<typename Thread, template<typename> typename MessagePumpTemplate>
class Active
{
  public:
    using Message = std::function<void()>;
    using MessagePump = MessagePumpTemplate<Message>;

    Active(const Active&) = delete;
    Active& operator=(const Active&) = delete;
    Active(Active&&) noexcept = default;
    Active& operator=(Active&&) noexcept = default;

    ~Active()
    {
        send([&] { done = true; });
        thread->join();
    }

    void send(Message m)
    {
        message_pump.send(std::move(m));
    }

  private:
    void run()
    {
        while (!done)
        {
            auto msg{message_pump.receive()};
            msg(); // note: last message sets done to true
        }
    }

  public:
    bool done{false};
    MessagePump message_pump;
    Thread thread;
};

#endif /* ACTIVE_HPP */
