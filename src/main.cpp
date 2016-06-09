#include "workermanagerwrapper.h"
#include "workerlistmodel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>




int main(int argc, char *argv[])
{
        QGuiApplication app(argc, argv);

        WorkerManagerWrapper wm;
        wm.startWorker("first", 1, 10000);
        wm.startWorker("second", 2, 10000);
        wm.startWorker("third", 3, 10000);
        wm.startWorker("fourth", 4, 10000);
        wm.startWorker("fifth", 5, 10000);

        QQmlApplicationEngine engine;
        QQmlContext * context = engine.rootContext();
        context->setContextProperty("workerManager", &wm);
        engine.load(QUrl("qrc:/qml/MainView.qml"));

        return app.exec();
}
