#include "workerlistmodel.h"


#include <iostream>

workerlistmodel::workerlistmodel(const WorkerManager & wm, QAbstractListModel *parent)
    : m_wm(wm)
    , QAbstractListModel(parent)
{

}

int workerlistmodel::rowCount(const QModelIndex &parent) const
{
    return m_wm.workersSize();
}

QVariant workerlistmodel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        std::cout << "Index is invalid" << std::endl;
        return QVariant();
    }

    if (index.row() >= m_wm.workersSize()) {
        std::cout << "index is too big" << std::endl;
        return QVariant();
    }

    if (role == WorkerListRoles::NameRole)
        return QString::fromStdString(m_wm.name(index.row()));

    if (role == WorkerListRoles::StatusRole)
        return QString::fromStdString(m_wm.status(index.row()));
}

QVariant workerlistmodel::headerData(int section, Qt::Orientation orientation, int role) const
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

QHash<int, QByteArray> workerlistmodel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[StatusRole] = "status";
    return roles;
}

