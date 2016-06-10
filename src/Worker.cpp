#include "Worker.h"
#include "observer.h"

#include <iostream>

void Worker::registerObserver(Observer * observer)
{
    if (observer != nullptr)
    {
        std::lock_guard<std::mutex> lock(m_observersMutex);
        m_observers.insert(observer);
    }
}

void Worker::unregisterObserver(Observer * observer)
{
    if (observer != nullptr)
    {
        std::lock_guard<std::mutex> lock(m_observersMutex);
        m_observers.erase(observer);
    }
}

void Worker::notifyAll() const
{
    std::lock_guard<std::mutex> lock(m_observersMutex);
    for (Observer * observer : m_observers)
    {
        observer->notify(this);
    }
}

Worker::Worker()
      : m_state(Empty)
      , m_request(NoRequest)
      , m_steps(0)
      , m_step(0)
{
    std::cout << "Worker constructed!\n";
}

Worker::~Worker()
{
    stopAndWait();
    std::cout << "Worker destroyed!\n";
}

void Worker::reset()
{
    m_state = Empty;
    m_request = NoRequest;
    m_steps = 0;
    m_step = 0;
}

bool Worker::start()
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    switch (m_state)
    {
    // Create thread and run it
    case Empty:
    case Finished:
    case Failed:
    case Stopped:
        setRequest(Stop);
        wait();
        setRequest(NoRequest);
        m_thread.reset(new std::thread([this] () { this->run(); }));
        return true;

    // Restart paused thread
    case Paused:
        setRequest(NoRequest);
        return true;

    // Error
    default:
        return false;
    }
}

bool Worker::pause()
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    switch (m_state)
    {
    // Pause running thread (cannot override stopping)
    case Running:
        if (m_request != Stop)
        {
            setRequest(Pause);
            return true;
        }

    // Error
    default:
        return false;
    }
}

bool Worker::stop()
{
    const std::lock_guard<std::mutex> lock(m_mutex);

    setRequest(Stop);
    return true;
}

void Worker::setRequest(Request req)
{
    const std::lock_guard<std::mutex> lock(m_cmutex);
    m_request = req;
    m_condition.notify_one();
}

void Worker::stopAndWait()
{
    setRequest(Stop);
    if (m_thread && m_thread->joinable())
    {
        m_thread->join();
    }
}

void Worker::wait()
{
    if (m_thread && m_thread->joinable())
    {
        m_thread->join();
    }
}

void Worker::tryPause()
{
    std::unique_lock<std::mutex> lock(m_cmutex);

    while (m_request == Pause)
    {
        std::cout << "Pausing!\n";
        setState(Paused);
        m_condition.wait(lock);
    }
}

void Worker::run_step(std::size_t step)
{
    if (!do_step(step))
    {
        throw std::runtime_error("failed to run a step!");
    }
}

void Worker::run()
{
    try
    {
        setStep(0);
        setSteps(0);
        setState(Running);

        // Initialize processing
        size_t steps;
        if (!do_init(steps))
        {
            setState(Failed);
            return;
        }
        else
        {
            setSteps(steps);
        }

        // Handle use requests (pausing and stopping)
        tryPause();
        switch (m_request)
        {
        case Stop:
            setState(Stopped);
            return;
        default:
            break;
        }

        // Run the processing step
        for (std::size_t step = 0; step < m_steps; step++)
        {
            setState(Running);

            // Handle user requests (pausing and stopping)
            tryPause();
            if (m_request == Stop)
            {
                setState(Stopped);
                return;
            }

            setStep(step);
            run_step(step);
        }
    }

    catch (std::exception& e)
    {
        setState(Failed);
        return;
    }

    // OK
    setState(Finished);
}

std::string Worker::stateAsString() const
{
    const auto state = this->state();

    switch (state)
    {
    case Empty:
        return "empty";

    case Running:
        return "running";

    case Paused:
        return "paused";

    case Finished:
        return "finished";

    case Failed:
        return "failed";

    case Stopped:
        return "stopped";

    default:
        return "unknown";
    }
}

Worker::State Worker::state() const
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_state;
}

std::size_t Worker::noSteps() const
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_steps;
}

std::size_t Worker::step() const
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    return m_step;
}

void Worker::setStep(std::size_t step)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_step != step)
    {
        m_step = step;
        notifyAll();
    }
}

void Worker::setSteps(std::size_t steps)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_steps != steps)
    {
        m_steps = steps;
        notifyAll();
    }
}

void Worker::setState(State state)
{
    const std::lock_guard<std::mutex> lock(m_mutex);
    if (m_state != state)
    {
        m_state = state;
        notifyAll();
    }
}
