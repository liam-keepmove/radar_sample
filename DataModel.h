
#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>

#include <QObject>
#include <QQuickItem>
#include <QWidget>


class DataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    DataModel();
    DataModel(const DataModel &);
    DataModel &operator=(const DataModel &);
    ~DataModel();
};

#endif // DATAMODEL_H
