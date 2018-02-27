#include <QGuiApplication>
#include <QFontDatabase>
#include <QTranslator>
#include <QFileInfo>
#include <QDir>

#include "report.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QFileInfo FileInfo(QCoreApplication::applicationDirPath() + QLatin1String("/selreport_zh_CN.qm"));
    if(FileInfo.exists())
    {
        QTranslator *pTranslator = new QTranslator;
        if(pTranslator->load(FileInfo.baseName(), FileInfo.dir().absolutePath()))
            app.installTranslator(pTranslator);
    }

    if(app.arguments().size() != 2)
    {
        printf("Invalid arguments number\n");
        return -1;
    }

    QStringList lstArguments = app.arguments().at(1).split(QLatin1Char(','));
    if(lstArguments.size() != 2)
    {
        printf("Invalid arguments format\n");
        return -1;
    }

    bool bOk;
    int iUserEvaluationId = lstArguments.first().toInt(&bOk);
    if(!bOk || iUserEvaluationId < 0)
    {
        printf("Invalid 'user_evaluation_id' in arguments\n");
        return -1;
    }

    QString strReportFileName = lstArguments.last().trimmed();
    if(strReportFileName.isEmpty())
    {
        printf("Invalid 'report file name' in arguments\n");
        return -1;
    }

    int iId = QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + QLatin1String("/msyh.ttc"));
    if(iId != -1)
    {
        QStringList lstFamilies = QFontDatabase::applicationFontFamilies(iId);
        if(!lstFamilies.empty())
            qApp->setFont(QFont(lstFamilies.first()));
    }

    if(!Report::CreateReport(iUserEvaluationId, strReportFileName))
        return -1;

    return 0;
}
