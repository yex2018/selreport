#ifndef REPORTPRQ_H
#define REPORTPRQ_H

#include <QMap>

#include "report.h"

class ScaleNormsPrq : public QObject
{
    Q_OBJECT

// Construction and Destruction
public:
    enum SubDimensionType { sdtAttachment = 0, sdtCommunication, sdtDisciplinePractice, sdtInvolvement,
                            sdtParentingConfidence, sdtSatisfactionWithSchool, sdtRelationalFrustration, sdtUnknown };

    enum LevelType { ltAcceptable = 0, ltCaution, ltExtremeCaution, ltUnknown };

    struct T_Percent
    {
        unsigned char       uchTScore;
        unsigned char       uchPercent;
    };

    struct Raw_T_Percent
    {
        unsigned char       uchRawScore;
        T_Percent           tp[7];
    };

    struct Conf_Interval
    {
        SubDimensionType    eSubDimensionType;
        unsigned char       uch90_2_5F;
        unsigned char       uch90_2_5M;
        unsigned char       uch90_6_9F;
        unsigned char       uch90_6_9M;
        unsigned char       uch90_10_12F;
        unsigned char       uch90_10_12M;
        unsigned char       uch90_13_15F;
        unsigned char       uch90_13_15M;
        unsigned char       uch90_16_18F;
        unsigned char       uch90_16_18M;
    };

// Operations
public:
    static QString  SubDimensionTypeToName(SubDimensionType eSubDimensionType);
    static QString  LevelTypeToName(LevelType eLevelType);

    static bool     A1_2_5F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A2_2_5M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A3_6_9F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A4_6_9M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A5_10_12F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A6_10_12M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A7_13_15F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A8_13_15M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A9_16_18F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);
    static bool     A10_16_18M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result);

    static bool     Conf_Interval_FromType(SubDimensionType eSubDimensionType, Conf_Interval &result);

// Properties
private:
    static const Raw_T_Percent  A1_2_5F[];
    static const Raw_T_Percent  A2_2_5M[];
    static const Raw_T_Percent  A3_6_9F[];
    static const Raw_T_Percent  A4_6_9M[];
    static const Raw_T_Percent  A5_10_12F[];
    static const Raw_T_Percent  A6_10_12M[];
    static const Raw_T_Percent  A7_13_15F[];
    static const Raw_T_Percent  A8_13_15M[];
    static const Raw_T_Percent  A9_16_18F[];
    static const Raw_T_Percent  A10_16_18M[];
    static const Conf_Interval  conf_interval[];
};

class ReportPrq : public Report
{
    Q_OBJECT

// Construction and Destruction
public:
    ReportPrq(int iUserEvaluationId, const QString &strReportFileName, bool bPP);
    ~ReportPrq();

// Operations
protected:
    virtual bool                    BuildReportData();
    virtual QString                 GetHtmlTempalte() const;
    virtual QJsonObject             BuildJsonResult();

private:
    int                             AnswerToScore(int iQuestionIndex, const QString &strAnswer);
    bool                            IsFIndex(int iQuestionIndex, int iQuestionScore);
    bool                            IsSuggestionQuestion(int iQuestionIndex, int iQuestionScore);
    ScaleNormsPrq::SubDimensionType GetSubDimensionTypeFromQestionIndex(int iQuestionIndex) const;
    ScaleNormsPrq::LevelType        GetSubDimensionLevelType(ScaleNormsPrq::SubDimensionType eSubDimensionType);

    void                            DrawImageForSLevel();
    void                            DrawImageSubDimenTable();
    void                            DrawPercentGender();

// Properties
private:
    bool                                                            m_bPP;
    int                                                             m_iFIndex;

    QMap<ScaleNormsPrq::SubDimensionType, int>                      m_mapSubDimensionTypeToRawScore;
    QMap<ScaleNormsPrq::SubDimensionType, int>                      m_mapSubDimensionTypeToTScore;
    QMap<ScaleNormsPrq::SubDimensionType, int>                      m_mapSubDimensionTypeToPercent;
    QMap<ScaleNormsPrq::SubDimensionType, int>                      m_mapSubDimensionTypeTo90Lower;
    QMap<ScaleNormsPrq::SubDimensionType, int>                      m_mapSubDimensionTypeTo90Upper;
    QMap<ScaleNormsPrq::SubDimensionType, ScaleNormsPrq::LevelType> m_mapSubDimensionTypeToLevelType;
};

#endif // REPORTPRQ_H
