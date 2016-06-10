#include "workerlistmodel.h"

#include "WorkerManager.h"

#include <iostream>

WorkerListModel::WorkerListModel(const WorkerManager & wm, QAbstractListModel *parent)
    : m_wm(wm)
    , QAbstractListModel(parent)
{

}

int WorkerListModel::rowCount(const QModelIndex &parent) const
{
    return m_wm.workersSize();
}

QVariant WorkerListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        std::cerr << "Index is invalid" << std::endl;
        return QVariant();
    }

    if (index.row() >= m_wm.workersSize()) {
        std::cerr << "index is too big" << std::endl;
        return QVariant();
    }

    if (role == WorkerListRoles::NameRole)
        return m_wm.name(index.row());
    else if (role == WorkerListRoles::StatusRole)
        return m_wm.status(index.row());
    else if (role == WorkerListRoles::NoStepsRole)
        return m_wm.noSteps(index.row());
    else if (role == WorkerListRoles::StepRole)
        return m_wm.step(index.row());
}

QVariant WorkerListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (section) {
    case 1:
        return QString("Name");
    case 2:
        return QString("Status");
    default:
        return QString();
    }
}

QHash<int, QByteArray> WorkerListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[StatusRole] = "status";
    roles[NoStepsRole] = "noSteps";
    roles[StepRole] = "step";
    return roles;
}

void WorkerListModel::beginInsertRow(int index)
{
    beginInsertRows(QModelIndex(), index, index);
}

void WorkerListModel::endInsertRow()
{
    endInsertRows();
}

