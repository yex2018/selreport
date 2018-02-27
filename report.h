#ifndef REPORT_H
#define REPORT_H

#include <QObject>
#include <QMap>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace MySqlWrapper
{
    class MySqlDatabase;
}

class Report : public QObject
{
    Q_OBJECT

// Structures and Enumerations
public:
    enum GenderType {gtUnknown = 0, gtMale = 1, gtFemale};

    struct ReportInfo
    {
        int                                 iEvaluationId;
        QString                             strKeyName;
        QString                             strEvaluationTime;
        QString                             strReportTime;
    };

    struct ChildInfo
    {
        int                                 iChildId;
        QString                             strName;
        GenderType                          eGenderType;
        QString                             strBirth;
        int                                 iAge;
    };

    struct UserInfo
    {
        QString                             strName;
        GenderType                          eGenderType;
        QString                             strRelation;
    };

// Construction and Destruction
public:
    Report(int iUserEvaluationId, const QString &strReportFileName);
    ~Report();


// Operations
public:
    static bool                         CreateReport(int iUserEvaluationId, const QString &strReportFileName);
    static QString                      GenderNameFromType(GenderType eGenderType);

protected:
    virtual bool                        GenerateReport();
    virtual QString                     GetHtmlTempalte() const = 0;
    virtual bool                        BuildReportData() = 0;
    virtual QByteArray                  GenerateJson();
    virtual QJsonObject                 BuildJsonResult() = 0;

    void                                DrawImageForBasicInfo();

// Properties
protected:
    static MySqlWrapper::MySqlDatabase  *m_pDatabase;
    static ReportInfo                   m_ReportInfo;
    static ChildInfo                    m_ChildInfo;
    static UserInfo                     m_UserInfo;

    int                                 m_iUserEvaluationId;
    QString                             m_strReportFileName;

    QMap<QString, QString>              m_mapVariableData;
    QMap<QString, QImage>               m_mapImageData;

    QSettings                           *m_pSettings;
};

#endif // REPORT_H
