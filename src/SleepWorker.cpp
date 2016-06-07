#include "SleepWorker.h"

#include <iostream>

SleepWorker::SleepWorker(const std::string& name, size_t numSteps, size_t stepTime)
      : m_name(name)
      , m_numSteps(numSteps)
      , m_stepTime(stepTime)
{
}

SleepWorker::~SleepWorker()
{
}

bool SleepWorker::do_init(std::size_t& steps)
{
    steps = m_numSteps;
    std::cout << m_name << " initialized." << std::endl;
    return true;
}

bool SleepWorker::do_step(std::size_t step)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(m_stepTime));
    std::cout << m_name << ": step " << step << " complete." << std::endl;
    return true;
}
