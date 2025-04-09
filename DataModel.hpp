#pragma once
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>

#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>

class DataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);
    DataModel(const DataModel &);
    DataModel &operator=(const DataModel &);
    ~DataModel();
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    Q_INVOKABLE virtual QVariant data(const QModelIndex &index,
                                      int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
    Q_INVOKABLE QVariantMap getRoleNameNumMap() const;

    enum RoleNames {
        IdRole = Qt::UserRole + 1,
        AngleRole = Qt::UserRole + 2,
        DistanceRole = Qt::UserRole + 3,
        StandpointRole = Qt::UserRole + 4,
        AreaRole = Qt::UserRole + 5,
        AziRole = Qt::UserRole + 6,
    };

    struct VehicleInfo {
        int id;          // 目标载具批号
        double angle;    // 目标载具方位，单位度
        double distance; // 目标载具距离，单位米
        int standpoint;  // 目标载具立场：我方、友方、敌方、中立、不明
        int area;        // 载具所在区域：空中、水面、水下、岸上
        double azi;      // 目标载具航向，单位度
        double speed;    // 速度，用来计算位置变化，单位m/s
    };
    DataModel::VehicleInfo generateRandomTarget();

private slots:
    void updataData();

private:
    QList<VehicleInfo> vehicleInfos;
    QHash<int, QByteArray> roleNumNameHashMap;
    QVariantMap roleNameNumMap;
    int updateInterval = 1000;
    int radiusOfDetect = 24000;
};

#endif // DATAMODEL_H
