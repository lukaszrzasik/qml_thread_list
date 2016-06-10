#pragma once

#include "Worker.h"
#include "workerlistmodel.h"
#include "observer.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include <QObject>
#include <QScopedPointer>

class QAbstractItemModel;

class WorkerManager : public QObject, public Observer
{
    Q_OBJECT
public:
    /// Inherited from Observer

    void notify(const Observable *);

public:
    enum class WorkerType
    {
        Sleep
    };

    explicit WorkerManager(QObject *parent = 0);
    ~WorkerManager();

    Q_PROPERTY(QAbstractItemModel * workersModel READ workersModel NOTIFY workersModelChanged)

    Q_INVOKABLE void startWorker(const QString& name, int numSteps, int stepTime);
    Q_INVOKABLE size_t workersSize() const;
    Q_INVOKABLE void pause(int workerId);
    Q_INVOKABLE void restart(int workerId);
    Q_INVOKABLE void stop(int workerId);
    Q_INVOKABLE QString status(int workerId) const;
    Q_INVOKABLE QString name(int workerId) const;
    Q_INVOKABLE int noSteps(int workerId) const;
    Q_INVOKABLE int step(int workerId) const;

    std::vector<WorkerType> listWorkers() const;

    QAbstractItemModel * workersModel();

signals:
    void workersModelChanged();
    void dataInModelChanged(int);

public slots:
    void emitDataModelChange(int);

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
    std::unordered_map<const Observable *, size_t> m_mappedWorkers;

    WorkerListModel m_workersModel;
};
