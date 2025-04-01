
#include "DataModel.hpp"

DataModel::DataModel(QObject* parent): QAbstractListModel(parent){
    QHash<int, QByteArray> roleNamesMap = {
        {IdRole, "id"},
        {AngleRole, "angle"},
        {DistanceRole, "distance"},
        {StandpointRole, "standpoint"},
        {AreaRole, "area"},
        {AziRole, "azi"},
    };
}

DataModel::DataModel(const DataModel &rhs): vehicleInfo{rhs.vehicleInfo}, roleNamesMap(rhs.roleNames()) { }

DataModel &DataModel::operator=(const DataModel &rhs) {
    if (this != &rhs) {
        vehicleInfo = rhs.vehicleInfo;
        roleNamesMap = rhs.roleNames();
    }
    return *this;
}

DataModel::~DataModel() { }

int DataModel::rowCount(const QModelIndex& parent) const{
    return parent.isValid() ? 0 : vehicleInfo.size();
}

QVariant DataModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= vehicleInfo.size())
        return QVariant();

    auto&& entity = vehicleInfo.at(index.row());
    switch (role) {
        case IdRole: return entity.id;
        case AngleRole: return entity.angle;
        case DistanceRole: return entity.distance;
        case StandpointRole: return entity.standpoint;
        case AreaRole: return entity.area;
        case AziRole: return entity.azi;
        default: return QVariant();
    }
}

QHash<int, QByteArray> DataModel::roleNames() const {
    return this->roleNamesMap;
}


