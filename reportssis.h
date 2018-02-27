#ifndef REPORTSSIS_H
#define REPORTSSIS_H

#include <QMap>

#include "report.h"
#include "scalenorms.h"

class ReportSsis : public Report
{
    Q_OBJECT

// Structures and Enumerations
public:
    struct RawScore
    {
        int                                 iTotalScore;
        int                                 iTotalScore68Lower;
        int                                 iTotalScore68Upper;
        int                                 iTotalScore95Lower;
        int                                 iTotalScore95Upper;

        QMap<SubDimensionType, int>         mapSubDimensionTypeToScore;
    };

    struct RptScore
    {
        int                                 iTotalScore;
        int                                 iTotalScore68Lower;
        int                                 iTotalScore68Upper;
        int                                 iTotalScore95Lower;
        int                                 iTotalScore95Upper;

        QMap<SubDimensionType, int>         mapSubDimensionTypeToScore;
    };

    struct SddScore
    {
        int                                 iTotalScore;
        int                                 iTotalScore68Lower;
        int                                 iTotalScore68Upper;
        int                                 iTotalScore95Lower;
        int                                 iTotalScore95Upper;
    };

    struct GeneData
    {
        int                                 iFIndex;
        LevelType                           eTotalLevelType;
        QString                             strSuggestion1;
        QString                             strSuggestion2;
        int                                 iTotalPercentCombined;
        int                                 iTotalPercentGender;
        QList<SubDimensionType>             lstOrderedSubDimensionType;
        QMap<SubDimensionType, LevelType>   mapSubDimensionTypeToLevelTypeCombined;
        QStringList                         lstBelowAverageSubDimensionNameCombined;
        QStringList                         lstAverageSubDimensionNameCombined;
        QStringList                         lstAboveAverageSubDimensionNameCombined;
        QMap<SubDimensionType, LevelType>   mapSubDimensionTypeToLevelTypeGender;
        QStringList                         lstBelowAverageSubDimensionNameGender;
        QStringList                         lstAverageSubDimensionNameGender;
        QStringList                         lstAboveAverageSubDimensionNameGender;
    };

// Construction and Destruction
public:
    ReportSsis(int iUserEvaluationId, const QString &strReportFileName);
    ~ReportSsis();

// Operations
protected:
    virtual bool                        BuildReportData();
    virtual QString                     GetHtmlTempalte() const;
    virtual QJsonObject                 BuildJsonResult();

private:
    SubDimensionType                    GetSubDimensionTypeFromQestionIndex(int iQuestionIndex) const;
    int                                 GetTotalSddScore();
    int                                 Get68ConInterval();
    int                                 Get95ConInterval();
    int                                 GetTotalRawScoreFromSddScore(int iTotalSddScore);

    int                                 AnswerToScore(const QString &strAnswer);
    bool                                IsFIndex(int iQuestionIndex);
    LevelType                           GetTotalLevelType();
    int                                 GetTotalPercentCombined();
    int                                 GetTotalPercentGender();
    LevelType                           GetSubDimensionLevelTypeCombined(SubDimensionType eSubDimensionType);
    LevelType                           GetSubDimensionLevelTypeGender(SubDimensionType eSubDimensionType);

    void                                DrawImageForSLevel();
    void                                DrawImageForSubDimenNum();
    void                                DrawImageForSubDimenBar();
    void                                DrawImageRScoreRange();
    void                                DrawImageSubDimenTable();
    void                                DrawPercentCombined();
    void                                DrawPercentGender();

// Properties
private:
    RawScore                            m_RawScore;
    RptScore                            m_RptScore;
    SddScore                            m_SddScore;
    GeneData                            m_GeneData;
};

#endif // REPORTSSIS_H
