#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>

class Worker
{
public:
    /// \brief Represents the current state in which a worker is.
    enum State
    {
        Empty = 0,  ///< Not processed yet.
        Running,    ///< The processing is in progress.
        Paused,     ///< The processing is paused.
        Finished,   ///< The processing has finished.
        Failed,     ///< The processing has failed.
        Stopped     ///< The processing is stopped.
    };

    /** \brief Constructor.
     */
    Worker();

    /// \brief Destructor.
    virtual ~Worker();

    /// \brief Returns the current processing state.
    State state() const;

    /// \brief Returns the current processing state as a string.
    std::string stateAsString() const;

    /// \brief Returns the number of processing steps.
    std::size_t noSteps() const;

    /// \brief Returns the current step which the worker is processing.
    std::size_t step() const;

    /** \brief Starts this worker processing.
     *  \note It's a non-blocking function.
     */
    bool start();

    /** \brief Pause this worker processing. It could be resumed later.
     *  \note It's a non-blocking function.
     */
    bool pause();

    /** \brief Stops this worker processing. It could be restarted later (not resumed).
     *  \note It's a non-blocking function.
     */
    bool stop();

    /// \brief same than stop() but being a blockin function.
    void stopAndWait();

    /** \brief Wait until processing of this worker finish.
     *  \note It's a blocking function.
     */
    void wait();

protected:
    /** \brief Initialize the worker.
     *  \param [out] steps The number of processing steps necessary for a particular worker.
     *  \return true if the worker has been correctly initialized, false otherwise.
     *  \todo return the number of steps and do not use any argument. All the workers return true here.
     */
    virtual bool do_init(std::size_t& steps) = 0;

    /// \brief Run a specific processing \b step
    virtual bool do_step(std::size_t step) = 0;

    /// \brief Reset the state.
    /// \todo Probably we can remove this method. It isn't being used.
    void reset();

private:
    /// \brief Run the thread (blocking)
    void run();

    /// \brief Run a processing step (blocking)
    void run_step(std::size_t step);

    /// \brief Represents a request to change the State of the Worker.
    enum Request
    {
        NoRequest,  ///< Let the thread running
        Pause,      ///< Pause the thread
        Stop        ///< Stop the thread
    };

    /// \brief Change request (locks)
    void setRequest(Request req);

    /// \brief Handle pause (if requested)
    void tryPause();

    /// \brief Safely change the current step
    void setStep(std::size_t step);

    /// \brief Safely change the number of steps
    void setSteps(std::size_t steps);

    /// \brief Safely change the current state
    void setState(State state);

private:
    // Attributes
    std::unique_ptr<std::thread> m_thread;    ///< Actual thread
    mutable std::mutex           m_mutex;     ///< Synchronization

    std::mutex                   m_cmutex;    ///< Synchronization (condition variable)
    std::condition_variable      m_condition; ///< Synchronization (condition variable)

    State                        m_state;     ///< Thread state
    Request                      m_request;   ///< User request

    std::size_t                  m_steps;     ///< Number of processing steps
    std::size_t                  m_step;      ///< Current processing step
};
