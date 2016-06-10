#include "WorkerManager.h"

#include "SleepWorker.h"

#include <algorithm>
#include <limits>
#include <iostream>

void WorkerManager::notify(const Observable * worker)
{
    auto it = m_mappedWorkers.find(worker);
    if (it == m_mappedWorkers.cend())
    {
        std::cerr << "No mapped worker" << std::endl;
        return;
    }

    size_t index = it->second;
    std::cout << "notify: index = " << index << std::endl;
    emit dataInModelChanged(index);
}

WorkerManager::WorkerManager(QObject *parent)
    : QObject(parent)
    , m_workers()
    , m_workersModel(*this)
{
    connect(this, &WorkerManager::dataInModelChanged, this, &WorkerManager::emitDataModelChange, Qt::QueuedConnection);
}

WorkerManager::~WorkerManager()
{
    disconnect(this, &WorkerManager::dataInModelChanged, this, &WorkerManager::emitDataModelChange);
    for (WorkerRecord & worker : m_workers)
    {
        worker.m_worker->unregisterObserver(this);
    }
}

void WorkerManager::startWorker(const QString& name, int numSteps, int  stepTime)
{
    m_workersModel.beginInsertRow(m_workers.size());

    m_workers.emplace_back(WorkerType::Sleep, new SleepWorker(name.toStdString(), numSteps, stepTime));
    m_mappedWorkers.emplace(m_workers.back().m_worker.get(), m_workers.size() - 1);
    m_workers.back().m_worker->registerObserver(this);
    m_workers.back().m_worker->start();

    m_workersModel.endInsertRow();
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

void WorkerManager::pause(int workerId)
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

void WorkerManager::restart(int workerId)
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

void WorkerManager::stop(int workerId)
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

QString WorkerManager::status(int workerId) const
{
    return QString::fromStdString(m_workers.at(workerId).m_worker->stateAsString());
}

QString WorkerManager::name(int workerId) const
{
    SleepWorker * sw;
    if (m_workers.at(workerId).m_type == WorkerType::Sleep) {
        sw = dynamic_cast<SleepWorker*>(m_workers.at(workerId).m_worker.get());
    }
    return sw ? QString::fromStdString(sw->name()) : "";
}

int WorkerManager::noSteps(int workerId) const
{
    return m_workers.at(workerId).m_worker->noSteps();
}

int WorkerManager::step(int workerId) const
{
    return m_workers.at(workerId).m_worker->step();
}

QAbstractItemModel *WorkerManager::workersModel()
{
    return &m_workersModel;
}

void WorkerManager::emitDataModelChange(int index)
{
    QModelIndex modelIndex = m_workersModel.index(index);
    emit m_workersModel.dataChanged(modelIndex, modelIndex);
}
