#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "frmlogin.h"
#include "dtm.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "mnproject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    Dtm _dtm;
    dtm = &_dtm;
    FrmLogin frmLogin;
    if(frmLogin.exec()){
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;

}
