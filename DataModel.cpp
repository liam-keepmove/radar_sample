#include "DataModel.hpp"
#include <QRandomGenerator>
#include <QTimer>

// 更新单个目标位置
void updateTargetPosition(DataModel::VehicleInfo& vehicle, int timeElapsed /*ms*/) {
    // 计算移动距离 (速度 * 时间)
    double moveDistance = vehicle.speed * (timeElapsed / 1000);

    // 将极坐标转换为直角坐标
    double x = vehicle.distance * qCos(qDegreesToRadians(vehicle.angle));
    double y = vehicle.distance * qSin(qDegreesToRadians(vehicle.angle));

    // 根据航向计算位移
    double dx = moveDistance * qCos(qDegreesToRadians(vehicle.azi));
    double dy = moveDistance * qSin(qDegreesToRadians(vehicle.azi));

    // 更新位置
    x += dx;
    y += dy;

    // 转换回极坐标
    vehicle.distance = qSqrt(x * x + y * y);
    vehicle.angle = qRadiansToDegrees(qAtan2(y, x));

    // 确保角度在0-360度范围内
    if (vehicle.angle < 0) {
        vehicle.angle += 360;
    }
}

// 生成随机目标
DataModel::VehicleInfo DataModel::generateRandomTarget() {
    static int id = 4000;
    static auto randDouble = [](double maxValue) {
        return QRandomGenerator::global()->bounded(maxValue);
    };
    return DataModel::VehicleInfo{id++, randDouble(360.0), randDouble(radiusOfDetect), 1 << (int)randDouble(5), 1 << (int)randDouble(4), randDouble(360.0), 40 + randDouble(20)};
}

DataModel::DataModel(QObject* parent) :
    QAbstractListModel(parent) {
    this->roleNumNameHashMap = {
        {IdRole, "id"},
        {AngleRole, "angle"},
        {DistanceRole, "distance"},
        {StandpointRole, "standpoint"},
        {AreaRole, "area"},
        {AziRole, "azi"},
    };

    for (auto [roleNum, name] : roleNumNameHashMap.asKeyValueRange()) {
        this->roleNameNumMap[name] = roleNum;
    }
    int randCount = QRandomGenerator::global()->bounded(15, 40);
    for (int i = 0; i < randCount; ++i) {
        vehicleInfos.push_back(generateRandomTarget());
    }
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DataModel::updataData);
    timer->start(this->updateInterval);
}

DataModel::DataModel(const DataModel& rhs) {
    this->vehicleInfos = rhs.vehicleInfos;
    this->roleNumNameHashMap = rhs.roleNumNameHashMap;
    this->roleNameNumMap = rhs.roleNameNumMap;
    this->updateInterval = rhs.updateInterval;
    this->radiusOfDetect = rhs.radiusOfDetect;
}

DataModel& DataModel::operator=(const DataModel& rhs) {
    if (this != &rhs) {
        this->vehicleInfos = rhs.vehicleInfos;
        this->roleNumNameHashMap = rhs.roleNumNameHashMap;
        this->roleNameNumMap = rhs.roleNameNumMap;
        this->updateInterval = rhs.updateInterval;
        this->radiusOfDetect = rhs.radiusOfDetect;
    }
    return *this;
}

DataModel::~DataModel() {}

int DataModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : vehicleInfos.size();
}

QVariant DataModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= vehicleInfos.size())
        return QVariant();

    auto&& entity = vehicleInfos.at(index.row());
    switch (role) {
    case IdRole:
        return entity.id;
    case AngleRole:
        return entity.angle;
    case DistanceRole:
        return entity.distance;
    case StandpointRole:
        return entity.standpoint;
    case AreaRole:
        return entity.area;
    case AziRole:
        return entity.azi;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> DataModel::roleNames() const {
    return this->roleNumNameHashMap;
}

QVariantMap DataModel::getRoleNameNumMap() const {
    return this->roleNameNumMap;
}

void DataModel::updataData() {
    decltype(vehicleInfos) waitList;
    for (auto it = vehicleInfos.begin(); it != vehicleInfos.end();) {
        // 利用航向和当前速度算极坐标新的距离和角度，如果距离大于等于探测半径，则判断为超出雷达探测范围，删除此目标，然后增加一个随机新目标
        // 如果距离小于探测半径，则更新目标
        auto&& vehicle = *it;
        updateTargetPosition(vehicle, updateInterval);

        // 检查是否超出雷达范围
        if (vehicle.distance >= radiusOfDetect) {
            it = vehicleInfos.erase(decltype(vehicleInfos)::const_iterator(it));
            waitList.push_back(generateRandomTarget());
        } else {
            ++it;
        }
    }
    vehicleInfos.append(waitList);
    emit dataChanged(QModelIndex(), QModelIndex());
}
