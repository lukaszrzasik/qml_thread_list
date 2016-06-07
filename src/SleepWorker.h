#pragma once

#include "Worker.h"

#include <string>

class SleepWorker : public Worker
{
public:
    SleepWorker(const std::string& name, size_t numSteps, size_t stepTime);
    virtual ~SleepWorker();

    virtual bool do_init(std::size_t& steps) override;
    virtual bool do_step(std::size_t step) override;

private:
    std::string m_name;
    size_t      m_numSteps;
    size_t      m_stepTime;
};
