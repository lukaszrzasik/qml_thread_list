#include "WorkerManager.h"

#include "SleepWorker.h"

#include <algorithm>
#include <limits>
#include <iostream>

WorkerManager::WorkerManager()
      : m_workers()
{
}

WorkerManager::~WorkerManager()
{
}

size_t WorkerManager::startWorker(const std::string& name, size_t numSteps, size_t stepTime)
{
    m_workers.emplace_back(WorkerType::Sleep, new SleepWorker(name, numSteps, stepTime));
    m_workers.back().m_worker->start();

    return m_workers.size() - 1;
}

std::vector<WorkerManager::WorkerType> WorkerManager::listWorkers() const
{
    std::vector<WorkerType> l;
    std::for_each(m_workers.begin(),
                  m_workers.end(),
                  [&l] (const WorkerRecord& r) { l.push_back(r.m_type); });
    return l;
}

size_t WorkerManager::workersSize() const
{
    return m_workers.size();
}

void WorkerManager::pause(size_t workerId)
{
    if (workerId < m_workers.size())
    {
        m_workers.at(workerId).m_worker->pause();
    }
    else
    {
        std::cerr << "WorkerManager::pause - error : worker " << workerId << " does not exist" << std::endl;
    }
}

void WorkerManager::restart(size_t workerId)
{
    if (workerId < m_workers.size())
    {
        m_workers.at(workerId).m_worker->start();
    }
    else
    {
        std::cerr << "WorkerManager::restart - error : worker " << workerId << " does not exist" << std::endl;
    }
}

void WorkerManager::stop(size_t workerId)
{
    if (workerId < m_workers.size())
    {
        m_workers.at(workerId).m_worker->stop();
    }
    else
    {
        std::cerr << "WorkerManager::stop - error : worker " << workerId << " does not exist" << std::endl;
    }
}

std::string WorkerManager::status(size_t workerId) const
{
    return m_workers.at(workerId).m_worker->stateAsString();
}

std::string WorkerManager::name(size_t workerId) const
{
    SleepWorker * sw;
    if (m_workers.at(workerId).m_type == WorkerType::Sleep) {
        sw = dynamic_cast<SleepWorker*>(m_workers.at(workerId).m_worker.get());
    }
    return sw ? sw->name() : "";
}
