#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <DataModel.hpp>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url("qrc:/radar/Main.qml");
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject* obj, const QUrl& objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    //  QScopedPointer<DataModel> dataModel(new DataModel());
    //	engine.rootContext()->setContextProperty("dataModel", dataModel.data());
    qmlRegisterType<DataModel>("org.data_model", 1, 0, "DataModel");
    engine.load(url);
    return app.exec();
}
