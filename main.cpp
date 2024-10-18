#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "frmlogin.h"
#include "dtm.h"
#include "css.h"
// #include "qtrpt.h"
// #include <QDomDocument>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    app = &a;
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "mnproject_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    // QtRPT rpt;
    // auto doc = QDomDocument();
    // doc.setContent("/Users/mac/dev/qt/mnproject/rpt.xml");
    // rpt.loadReport("/Users/mac/dev/qt/mnproject/rpt.xml");
    // //rpt.printExec();
    // rpt.printPDF("/Users/mac/dev/qt/mnproject/rpt.pdf");
 a.setStyleSheet(cssGlobal);
    Dtm _dtm;
    dtm = &_dtm;
    FrmLogin frmLogin;
    if(frmLogin.exec()){
        MainWindow w;
        mainWin = &w;
        w.show();
        return a.exec();
    }
    return 0;

}


