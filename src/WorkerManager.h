#pragma once

#include "Worker.h"

#include <memory>
#include <string>
#include <vector>

class WorkerManager
{
public:
    enum class WorkerType
    {
        Sleep
    };

    WorkerManager();
    ~WorkerManager();

    size_t startWorker(const std::string& name, size_t numSteps, size_t stepTime);
    std::vector<WorkerType> listWorkers() const;
    size_t workersSize() const;

    void pause(size_t workerId);
    void restart(size_t workerId);
    void stop(size_t workerId);
    std::string status(size_t workerId) const;
    std::string name(size_t workerId) const;

private:
    struct WorkerRecord
    {
        WorkerRecord(WorkerType t, Worker* w)
              : m_type(t)
              , m_worker(w)
        {
        }

        WorkerType              m_type;
        std::unique_ptr<Worker> m_worker;
    };

    std::vector<WorkerRecord>   m_workers;
};
