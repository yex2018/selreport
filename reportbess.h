#ifndef REPORTBESS_H
#define REPORTBESS_H

#include <QMap>

#include "report.h"
#include "scalenorms.h"

class ReportBess : public Report
{
    Q_OBJECT

// Construction and Destruction
public:
    ReportBess(int iUserEvaluationId, const QString &strReportFileName, bool bPP);
    ~ReportBess();

// Operations
protected:
    virtual bool                        BuildReportData();
    virtual QString                     GetHtmlTempalte() const;
    virtual QJsonObject                 BuildJsonResult();

private:
    int                                 AnswerToScore(int iQuestionIndex, const QString &strAnswer);
    bool                                IsFIndex(int iQuestionIndex);
    SubDimensionType                    GetSubDimensionTypeFromQestionIndex(int iQuestionIndex) const;

    LevelType                           GetTotalLevelType();
    LevelType                           GetSubDimensionLevelType(SubDimensionType eSubDimensionType);

    void                                DrawImageTScoreRange(bool bCombined);
    void                                DrawPercentCombined();
    void                                DrawPercentGender();
    void                                DrawImageSubDimenTable();

// Properties
private:
    bool                                m_bPP;
    int                                 m_iRawScore;
    QMap<SubDimensionType, int>         m_mapSubDimensionTypeToRawScore;

    int                                 m_iTScoreCombined;
    int                                 m_iTScore90LowerCombined;
    int                                 m_iTScore90UpperCombined;
    int                                 m_iPencentCombined;

    LevelType                           m_eLevelTypeCombined;
    QMap<SubDimensionType, LevelType>   m_mapSubDimensionTypeToLevelType;

    int                                 m_iTScoreGender;
    int                                 m_iTScore90LowerGender;
    int                                 m_iTScore90UpperGender;
    int                                 m_iPencentGender;

    int                                 m_iFIndex;
};

#endif // REPORTBESS_H
