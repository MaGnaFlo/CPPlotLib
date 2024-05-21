#include <QtWidgets/QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "image_provider.hpp"


int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/qt/qml/cpplotlib_ui/qml/main.qml"_qs);
    engine.addImageProvider(QLatin1String("colors"), new ImageProvider);
    engine.load(url);

    return app.exec();
}