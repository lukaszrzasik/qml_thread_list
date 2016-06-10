#include "WorkerManager.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
        QGuiApplication app(argc, argv);

        WorkerManager wm;

        QQmlApplicationEngine engine;
        QQmlContext * context = engine.rootContext();
        context->setContextProperty("workerManager", &wm);
        engine.load(QUrl("qrc:/qml/MainView.qml"));

        return app.exec();
}
