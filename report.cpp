#include <QGuiApplication>
#include <QFile>
#include <QTextDocument>
#include <QTextCodec>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QImage>
#include <QSettings>
#include <QPainter>

#include "mysqlwrapper/mysqldatabase.h"
#include "mysqlwrapper/mysqlquery.h"
#include "mysqlwrapper/mysqltransaction.h"

#include "report.h"
#include "variabledefine.h"
#include "reportpdfwriter.h"
#include "reportssis.h"
#include "reportbess.h"
#include "reportprq.h"

MySqlWrapper::MySqlDatabase* Report::m_pDatabase = 0;
Report::ReportInfo Report::m_ReportInfo = {0, "", "", ""};
Report::ChildInfo Report::m_ChildInfo = {0, "", Report::gtUnknown, "", 0};
Report::UserInfo Report::m_UserInfo = {"", Report::gtUnknown, ""};

Report::Report(int iUserEvaluationId, const QString &strReportFileName) :
    QObject(0), m_iUserEvaluationId(iUserEvaluationId), m_strReportFileName(strReportFileName)
{
    m_pSettings = new QSettings(QLatin1String(":/selreport/data/report_data.ini"), QSettings::IniFormat);
    m_pSettings->setIniCodec("Utf-8");
}

Report::~Report()
{
}

bool Report::CreateReport(int iUserEvaluationId, const QString &strReportFileName)
{
    // Open Database
    if(!m_pDatabase)
    {
        m_pDatabase = MySqlWrapper::MySqlDatabase::AddDatabase(QLatin1String("106.14.195.198"),
                                                               QLatin1String("root"),
                                                               QLatin1String("Sel22/Bhe94+Online-5"),
                                                               QLatin1String("sel"));
        if(!m_pDatabase->Open())
        {
            printf("Failed to connect to database");
            return false;
        }
    }

    MySqlWrapper::MySqlQuery query(m_pDatabase);

    // Query user_evaluation
    query.Clear();
    if(!query.Exec(QString("SELECT * FROM `user_evaluation` WHERE `user_evaluation_id`='%1'").arg(iUserEvaluationId)))
    {
        printf("Failed to query user_evaluation table");
        return false;
    }

    if(query.GetResults().isEmpty())
    {
        printf("Failed to query user_evaluation table");
        return false;
    }

    MySqlWrapper::MySqlRecord record = query.GetResults().first();
    m_ReportInfo.iEvaluationId = record.value("evaluation_id").toInt();
    int iUserId = record.value("user_id").toInt();
    m_ChildInfo.iChildId = record.value("child_id").toInt();
    m_ReportInfo.strEvaluationTime = record.value("evaluation_time").toDateTime().toString("yyyy-MM-dd hh:mm:ss");
    m_ReportInfo.strReportTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    // Query evaluation
    query.Clear();
    if(!query.Exec(QString("SELECT * FROM `evaluation` WHERE `evaluation_id`='%1'").arg(m_ReportInfo.iEvaluationId)))
    {
        printf("Failed to query evaluation table");
        return false;
    }

    if(query.GetResults().isEmpty())
    {
        printf("Failed to query evaluation table");
        return false;
    }

    record = query.GetResults().first();
    m_ReportInfo.strKeyName = record.value("key_name").toString();

    // Query child
    query.Clear();
    if(!query.Exec(QString("SELECT * FROM `child` WHERE `child_id`='%1'").arg(m_ChildInfo.iChildId)))
    {
        printf("Failed to query child table");
        return false;
    }

    if(query.GetResults().isEmpty())
    {
        printf("Failed to query child table");
        return false;
    }

    record = query.GetResults().first();
    m_ChildInfo.strName = record.value("name").toString();
    QDateTime dtBirth = record.value("birth_date").toDateTime();
    m_ChildInfo.strBirth = dtBirth.toString("yyyy-MM-dd");
    m_ChildInfo.iAge = QDateTime::currentDateTime().date().year() - dtBirth.date().year();

    bool bOk;
    int iGender = record.value("gender").toInt(&bOk);
    if(bOk)
    {
        if(iGender == 1)
            m_ChildInfo.eGenderType = gtMale;
        else if(iGender == 2)
            m_ChildInfo.eGenderType = gtFemale;
    }

    // Query user
    query.Clear();
    if(!query.Exec(QString("SELECT * FROM `user` WHERE `user_id`='%1'").arg(iUserId)))
    {
        printf("Failed to query user table");
        return false;
    }

    if(query.GetResults().isEmpty())
    {
        printf("Failed to query user table");
        return false;
    }

    record = query.GetResults().first();
    m_UserInfo.strName = record.value("name").toString().trimmed();
    if(m_UserInfo.strName.isEmpty())
        m_UserInfo.strName = record.value("nick_name").toString().trimmed();

    iGender = record.value("gender").toInt(&bOk);
    if(bOk)
    {
        if(iGender == 1)
            m_UserInfo.eGenderType = gtMale;
        else if(iGender == 2)
            m_UserInfo.eGenderType = gtFemale;
    }

    // Query uc_relation
    int iRelation = 0;

    query.Clear();
    if(query.Exec(QString("SELECT * FROM `uc_relation` WHERE `user_id`='%1' AND `child_id`='%2'").arg(iUserId).arg(m_ChildInfo.iChildId)))
    {
        if(!query.GetResults().isEmpty())
        {
            bool bok;
            iRelation = query.GetResults().first().value("relation").toInt(&bok);
        }
    }

    QMap<int, QString> s_mapRelation;
    if(s_mapRelation.isEmpty())
    {
        s_mapRelation.insert(0, tr("Parent"));
        s_mapRelation.insert(1, tr("Father"));
        s_mapRelation.insert(2, tr("Mother"));
        s_mapRelation.insert(3, tr("Father's father"));
        s_mapRelation.insert(4, tr("Father's mother"));
        s_mapRelation.insert(5, tr("Mother's father"));
        s_mapRelation.insert(6, tr("Mother's mother"));
    }
    m_UserInfo.strRelation = s_mapRelation.value(iRelation, tr("Parent"));

    Report *pReport = 0;
    if(m_ReportInfo.strKeyName == QLatin1String("skill"))
        pReport = new ReportSsis(iUserEvaluationId, strReportFileName);
    else if(m_ReportInfo.strKeyName == QLatin1String("riskpp"))
        pReport = new ReportBess(iUserEvaluationId, strReportFileName, true);
    else if(m_ReportInfo.strKeyName == QLatin1String("riskpca"))
        pReport = new ReportBess(iUserEvaluationId, strReportFileName, false);
    else if(m_ReportInfo.strKeyName == QLatin1String("qzgxpp"))
        pReport = new ReportPrq(iUserEvaluationId, strReportFileName, true);
    else if(m_ReportInfo.strKeyName == QLatin1String("qzgxpca"))
        pReport = new ReportPrq(iUserEvaluationId, strReportFileName, false);

    if(!pReport)
    {
        printf("invalid evaluation key name");
        return false;
    }

    if(!pReport->GenerateReport())
    {
        printf("failed to generate report");
        return false;
    }

    MySqlWrapper::MySqlTransaction Transaction(m_pDatabase, true);

    if(!m_pDatabase->ExecSql(QString("UPDATE `user_evaluation` SET `report_result`='/front/report/%1' WHERE `user_evaluation_id`='%2'").arg(strReportFileName).arg(iUserEvaluationId)))
    {
        printf("Failed to update user_evaluation 'report_result'");

        Transaction.Rollback();
        return false;
    }

    QString strSql = QString("UPDATE `user_evaluation` SET `data_result`=? WHERE `user_evaluation_id`='%1'").arg(iUserEvaluationId);
    if(!m_pDatabase->WriteBlog(strSql, pReport->GenerateJson()))
    {
        printf("Failed to update user_evaluation 'data_result'");
        printf("\n%d:%s", m_pDatabase->GetLastErrorCode(), m_pDatabase->GetLastErrorString().toLatin1().constData());

        Transaction.Rollback();
        return false;
    }

    Transaction.Commit();

    return true;
}

QString Report::GenderNameFromType(GenderType eGenderType)
{
    static QMap<GenderType, QString> s_mapGenderTypeToName;
    if(s_mapGenderTypeToName.isEmpty())
    {
        s_mapGenderTypeToName.insert(gtUnknown, tr("Unknown"));
        s_mapGenderTypeToName.insert(gtMale, tr("Male"));
        s_mapGenderTypeToName.insert(gtFemale, tr("Female"));
    }

    return s_mapGenderTypeToName.value(eGenderType, tr("Unknown"));
}

bool Report::GenerateReport()
{
    QFile file(GetHtmlTempalte());
    if(!file.open(QFile::ReadOnly))
        return false;

    m_mapVariableData.insert(g_strReportTime, m_ReportInfo.strReportTime);
    m_mapVariableData.insert(g_strEvaluationTime, m_ReportInfo.strEvaluationTime);
    m_mapVariableData.insert(g_strReportId, QString::number(m_iUserEvaluationId));
    m_mapVariableData.insert(g_strChildId, QString::number(m_ChildInfo.iChildId));
    m_mapVariableData.insert(g_strChildName, m_ChildInfo.strName);
    m_mapVariableData.insert(g_strChildGender, GenderNameFromType(m_ChildInfo.eGenderType));
    m_mapVariableData.insert(g_strChildHimHer, m_ChildInfo.eGenderType == gtMale ? tr("him") : tr("her"));
    m_mapVariableData.insert(g_strChildBirth, m_ChildInfo.strBirth);
    m_mapVariableData.insert(g_strChildAge, QString::number(m_ChildInfo.iAge));
    m_mapVariableData.insert(g_strUserName, m_UserInfo.strName);
    m_mapVariableData.insert(g_strUserGender, GenderNameFromType(m_UserInfo.eGenderType));
    m_mapVariableData.insert(g_strUserRelation, m_UserInfo.strRelation);

    if(!BuildReportData())
        return false;

    QByteArray baData = file.readAll();
    QString strHtml = Qt::codecForHtml(baData)->toUnicode(baData);
    foreach(const QString &strKey, m_mapVariableData.keys())
        strHtml.replace(strKey, m_mapVariableData.value(strKey));

    QTextDocument TextDocument;
    foreach(const QString &strKey, m_mapImageData.keys())
        TextDocument.addResource(QTextDocument::ImageResource, QUrl(strKey), m_mapImageData.value(strKey));

    ReportPdfWriter PdfWriter(QCoreApplication::applicationDirPath() + "/../front/report/" + m_strReportFileName);
    PdfWriter.setResolution(72);
    PdfWriter.setPageSize(QPagedPaintDevice::A4);
    PdfWriter.setPageMargins(QMarginsF(25, 20, 25, 20));

    TextDocument.setHtml(strHtml);
    TextDocument.print(&PdfWriter);

    return true;
}

QByteArray Report::GenerateJson()
{
    QJsonObject joReport;
    joReport.insert("key_name", m_ReportInfo.strKeyName);
    joReport.insert("report_time", m_ReportInfo.strReportTime);
    joReport.insert("evaluation_time", m_ReportInfo.strEvaluationTime);

    QJsonObject joChild;
    joChild.insert("id", QString::number(m_ChildInfo.iChildId));
    joChild.insert("name", m_ChildInfo.strName);
    joChild.insert("gender", GenderNameFromType(m_ChildInfo.eGenderType));
    joChild.insert("birth", m_ChildInfo.strBirth);
    joReport.insert("child", joChild);

    QJsonObject joUser;
    joUser.insert("name", m_UserInfo.strName);
    joUser.insert("gender", GenderNameFromType(m_UserInfo.eGenderType));
    joUser.insert("role", m_UserInfo.strRelation);
    joReport.insert("user", joUser);

    QJsonObject joResult = BuildJsonResult();
    if(!joResult.isEmpty())
        joReport.insert("result", joResult);

    QJsonDocument document;
    document.setObject(joReport);

    return document.toJson(QJsonDocument::Compact);
}

void Report::DrawImageForBasicInfo()
{
    const QString strChildInfoHeader = m_pSettings->value("basic_info/child_info_header").toString();
    const QString strReportInfoHeader = m_pSettings->value("basic_info/report_info_header").toString();
    QString strChildInfo = m_pSettings->value("basic_info/child_info_content").toString();
    QString strReportInfo = m_pSettings->value("basic_info/report_info_content").toString();
    QString strFIndex = m_pSettings->value("basic_info/f_index").toString();

    foreach(const QString &strKey, m_mapVariableData.keys())
    {
        strChildInfo.replace(strKey, m_mapVariableData.value(strKey));
        strReportInfo.replace(strKey, m_mapVariableData.value(strKey));
        strFIndex.replace(strKey, m_mapVariableData.value(strKey));
    }

    QStringList lstChildInfo = strChildInfo.split(QLatin1String("/"));
    QStringList lstReportInfo = strReportInfo.split(QLatin1String("/"));

    const int iCaptionHeight = 150;
    const int iContentLineHeight = 150;

    const QRect rcImage(0, 0, 2020, 1200);
    const QRect rcPainter(10, 100, 2000, 1000);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(QColor(91, 142, 57), 2));
    painter.setBrush(Qt::NoBrush);

    const QRect rcChildInfo = QRect(rcPainter.left(), rcPainter.top(), 950, rcPainter.height());
    const QRect rcReportInfo = QRect(rcChildInfo.right() + 100, rcPainter.top(), 950, rcPainter.height());
    painter.drawRoundedRect(rcChildInfo, 30, 30);
    painter.drawRoundedRect(rcReportInfo, 30, 30);

    painter.setBrush(QColor(180, 207, 168));

    const QRect rcCaptionChildInfo = QRect(rcPainter.left(), rcPainter.top(), 950, iCaptionHeight);
    const QRect rcCaptionReportInfo = QRect(rcChildInfo.right() + 100, rcPainter.top(), 950, iCaptionHeight);
    painter.drawRoundedRect(rcCaptionChildInfo, 30, 30);
    painter.drawRoundedRect(rcCaptionReportInfo, 30, 30);

    QFont font = qApp->font();
    font.setPointSize(50);
    font.setBold(true);
    painter.setFont(font);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignCenter);
    TextOption.setWrapMode(QTextOption::WordWrap);

    painter.setPen(QPen(Qt::black, 2));

    painter.drawText(rcCaptionChildInfo, strChildInfoHeader, TextOption);
    painter.drawText(rcCaptionReportInfo, strReportInfoHeader, TextOption);

    font.setPointSize(40);
    font.setBold(true);
    QFontMetrics fm(font);

    int iMaxWidth = 0;
    foreach(const QString str, lstChildInfo + lstReportInfo)
    {
        QStringList lstContent = str.split(QLatin1String("="));
        if(lstContent.size() != 2)
            continue;

        iMaxWidth = qMax(iMaxWidth, fm.width(lstContent.at(0)));
    }

    TextOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    for(int i = 0; i < lstChildInfo.size(); i++)
    {
        QStringList lstContent = lstChildInfo.at(i).split(QLatin1String("="));
        if(lstContent.size() != 2)
            continue;

        font.setBold(true);
        painter.setFont(font);

        QRect rcText = QRect(rcChildInfo.left() + 40, rcChildInfo.top() + 50 + iCaptionHeight + i * iContentLineHeight, iMaxWidth, iContentLineHeight);
        painter.drawText(rcText, lstContent.at(0), TextOption);

        font.setBold(false);
        painter.setFont(font);
        rcText.setLeft(rcChildInfo.left() + 40 + iMaxWidth);
        rcText.setRight(rcChildInfo.right());
        painter.drawText(rcText, lstContent.at(1), TextOption);
    }

    for(int i = 0; i < lstReportInfo.size(); i++)
    {
        QStringList lstContent = lstReportInfo.at(i).split(QLatin1String("="));
        if(lstContent.size() != 2)
            continue;

        font.setBold(true);
        painter.setFont(font);

        QRect rcText = QRect(rcReportInfo.left() + 40, rcReportInfo.top() + 50 + iCaptionHeight + i * iContentLineHeight, iMaxWidth, iContentLineHeight);
        painter.drawText(rcText, lstContent.at(0), TextOption);

        font.setBold(false);
        painter.setFont(font);
        rcText.setLeft(rcReportInfo.left() + 40 + iMaxWidth);
        rcText.setRight(rcReportInfo.right());
        painter.drawText(rcText, lstContent.at(1), TextOption);
    }

    font.setPointSize(30);
    font.setBold(false);
    painter.setFont(font);

    TextOption.setAlignment(Qt::AlignRight | Qt::AlignBottom);
    painter.drawText(QRect(rcReportInfo.left(), rcReportInfo.top(), rcReportInfo.width() - 50, rcReportInfo.height() - 50), strFIndex, TextOption);

    m_mapImageData.insert(QLatin1String("userdata://basic_info.png"), image);
}
