#pragma once

#include <QObject>
#include <QAbstractListModel>

class WorkerManager;

class WorkerListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum WorkerListRoles {
        NameRole = Qt::UserRole + 1,
        StatusRole,
        NoStepsRole,
        StepRole
    };

    explicit WorkerListModel(const WorkerManager & wm, QAbstractListModel *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;
    void beginInsertRow(int index);
    void endInsertRow();

private:
    const WorkerManager & m_wm;
};

