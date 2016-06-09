#include "workermanagerwrapper.h"

#include "WorkerManager.h"

WorkerManagerWrapper::WorkerManagerWrapper(QObject *parent)
    : QObject(parent)
    , m_pWm(new WorkerManager)
    , m_workersModel(*m_pWm)
{
}

void WorkerManagerWrapper::startWorker(const QString &name, size_t numSteps, size_t stepTime)
{
    m_pWm->startWorker(name.toStdString(), numSteps, stepTime);
}

void WorkerManagerWrapper::pause(size_t workerId)
{
    m_pWm->pause(workerId);
}

void WorkerManagerWrapper::restart(size_t workerId)
{
    m_pWm->restart(workerId);
}

void WorkerManagerWrapper::stop(size_t workerId)
{
    m_pWm->stop(workerId);
}

QString WorkerManagerWrapper::status(size_t workerId)
{
    return QString::fromStdString(m_pWm->status(workerId));
}

QAbstractItemModel *WorkerManagerWrapper::workersModel()
{
    return &m_workersModel;
}
