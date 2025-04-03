#include "DataModel.hpp"
#include <QRandomGenerator>
#include <QTimer>

// 更新单个目标位置
void updateTargetPosition(DataModel::VehicleInfo& vehicle, int timeElapsed) {
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
static DataModel::VehicleInfo generateRandomTarget() {
    static int id = 4000;
    static auto randDouble = [](double maxValue) {
        return QRandomGenerator::global()->bounded(maxValue);
    };
    return DataModel::VehicleInfo{.id = id++, .angle = randDouble(360.0), .distance = randDouble(24000), .standpoint = (int)randDouble(5), .area = (int)randDouble(4), .azi = randDouble(360.0), .speed = 5 + randDouble(20)};
}

DataModel::DataModel(QObject* parent) :
    QAbstractListModel(parent) {
    this->roleNamesMap = {
        {IdRole, "id"},
        {AngleRole, "angle"},
        {DistanceRole, "distance"},
        {StandpointRole, "standpoint"},
        {AreaRole, "area"},
        {AziRole, "azi"},
    };
    auto randDouble = [](double maxValue) {
        return QRandomGenerator::global()->bounded(maxValue);
    };
    for (int i = 0; i < 7; ++i) {
        vehicleInfos.push_back(generateRandomTarget());
    }
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DataModel::updataData);
    timer->start(1000);
}

DataModel::DataModel(const DataModel& rhs) :
    vehicleInfos{rhs.vehicleInfos}, roleNamesMap(rhs.roleNames()) {
}

DataModel& DataModel::operator=(const DataModel& rhs) {
    if (this != &rhs) {
        vehicleInfos = rhs.vehicleInfos;
        roleNamesMap = rhs.roleNames();
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
    return this->roleNamesMap;
}

void DataModel::updataData() {
    QList<VehicleInfo> waitList;
    for (auto it = vehicleInfos.begin(); it != vehicleInfos.end();) {
        // 利用航向和当前速度算极坐标新的距离和角度，如果距离大于等于探测半径，则判断为超出雷达探测范围，删除此目标，然后增加一个随机新目标
        // 如果距离小于探测半径，则更新目标
        auto&& vehicle = *it;
        updateTargetPosition(vehicle, updateInterval);

        // 检查是否超出雷达范围
        if (vehicle.distance >= radiusOfInvestigation) {
            it = vehicleInfos.erase(it);
            waitList.push_back(generateRandomTarget());
        } else {
            ++it;
        }
    }
    vehicleInfos.append(waitList);
}
