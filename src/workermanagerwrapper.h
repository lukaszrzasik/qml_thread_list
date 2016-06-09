#pragma once

#include "workerlistmodel.h"

#include <QObject>
#include <QScopedPointer>

class WorkerManager;
class QAbstractItemModel;

class WorkerManagerWrapper : public QObject
{
    Q_OBJECT
public:
    explicit WorkerManagerWrapper(QObject *parent = 0);

    Q_PROPERTY(QAbstractItemModel * workersModel READ workersModel NOTIFY workersModelChanged)

    Q_INVOKABLE void startWorker(const QString & name, size_t numSteps, size_t stepTime);
    Q_INVOKABLE void pause(size_t workerId);
    Q_INVOKABLE void restart(size_t workerId);
    Q_INVOKABLE void stop(size_t workerId);
    Q_INVOKABLE QString status(size_t workerId);

    QAbstractItemModel * workersModel();

signals:
    void workersModelChanged();

public slots:


private:
    QScopedPointer<WorkerManager> m_pWm;
    workerlistmodel m_workersModel;
};
