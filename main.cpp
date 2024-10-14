#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[])
{
 #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#else
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Round);
#endif

    QApplication a(argc, argv);

    QTranslator q;
    if (q.load(":/qm/qtbase_zh_CN.qm"))
        a.installTranslator(&q);

    QTranslator t;
    if (t.load(":/qm/zh_CN.qm"))
        a.installTranslator(&t);

    QFile file(":/qss/onekeyword.qss");
    file.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(file.readAll());
    file.close();

    MainWindow w;
    w.show();
    return a.exec();
}
