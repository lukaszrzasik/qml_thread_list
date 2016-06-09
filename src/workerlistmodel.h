#pragma once

#include "WorkerManager.h"

#include <QObject>
#include <QAbstractListModel>


class workerlistmodel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum WorkerListRoles {
        NameRole = Qt::UserRole + 1,
        StatusRole
    };

    explicit workerlistmodel(const WorkerManager & wm, QAbstractListModel *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:

private:
    const WorkerManager & m_wm;
};

