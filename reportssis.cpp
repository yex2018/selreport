#include <QGuiApplication>
#include <QPainter>
#include <QPaintEngine>
#include <QSettings>
#include <QJsonObject>
#include <QJsonArray>

#include "mysqlwrapper/mysqldatabase.h"
#include "mysqlwrapper/mysqlquery.h"

#include "reportssis.h"
#include "variabledefine.h"
#include "reportpdfwriter.h"

ReportSsis::ReportSsis(int iUserEvaluationId, const QString &strReportFileName) : Report(iUserEvaluationId, strReportFileName)
{
    m_RawScore.iTotalScore = 0;
    m_RawScore.iTotalScore68Lower = 0;
    m_RawScore.iTotalScore68Upper = 0;
    m_RawScore.iTotalScore95Lower = 0;
    m_RawScore.iTotalScore95Upper = 0;

    m_RptScore.iTotalScore = 0;
    m_RptScore.iTotalScore68Lower = 0;
    m_RptScore.iTotalScore68Upper = 0;
    m_RptScore.iTotalScore95Lower = 0;
    m_RptScore.iTotalScore95Upper = 0;

    m_SddScore.iTotalScore = 0;
    m_SddScore.iTotalScore68Lower = 0;
    m_SddScore.iTotalScore68Upper = 0;
    m_SddScore.iTotalScore95Lower = 0;
    m_SddScore.iTotalScore95Upper = 0;

    m_GeneData.iFIndex = 0;

    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        SubDimensionType eSubDimensionType = SubDimensionType(i);

        m_RawScore.mapSubDimensionTypeToScore[eSubDimensionType] = 0;
        m_RptScore.mapSubDimensionTypeToScore[eSubDimensionType] = 0;
    }
}

ReportSsis::~ReportSsis()
{
}

QString ReportSsis::GetHtmlTempalte() const
{
    return QLatin1String(":/selreport/html/rpt_ssis.html");
}

bool ReportSsis::BuildReportData()
{
    MySqlWrapper::MySqlQuery query(m_pDatabase);

    query.Clear();
    if(!query.Exec(QString("SELECT * FROM `question` WHERE `evaluation_id`='%1'").arg(m_ReportInfo.iEvaluationId)))
    {
        printf("Failed to query question table");
        return false;
    }

    QMap<int, int> mapQuestionIdToIndex;
    foreach(MySqlWrapper::MySqlRecord record, query.GetResults())
        mapQuestionIdToIndex.insert(record.value("question_id").toInt(), record.value("question_index").toInt());

    query.Clear();
    if(!query.Exec(QString("SELECT * FROM `user_question` WHERE `user_evaluation_id`='%1'").arg(m_iUserEvaluationId)))
    {
        printf("Failed to query user_question table");
        return false;
    }

    QMap<int, int> mapQuestionIndexToScore;
    foreach(MySqlWrapper::MySqlRecord record, query.GetResults())
    {
        QString strAnswer = record.value("answer").toString();
        int iQuestionId = record.value("question_id").toInt();
        if(!strAnswer.isEmpty() && mapQuestionIdToIndex.contains(iQuestionId))
            mapQuestionIndexToScore.insert(mapQuestionIdToIndex.value(iQuestionId), AnswerToScore(strAnswer));
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // Calculate data
    ///////////////////////////////////////////////////////////////////////////////////////

    // Calculate raw score
    foreach(int iQuestionIndex, mapQuestionIndexToScore.keys())
    {
        const int iQuestionScore = mapQuestionIndexToScore.value(iQuestionIndex);

        SubDimensionType eSubDimensionType = GetSubDimensionTypeFromQestionIndex(iQuestionIndex);
        if(eSubDimensionType != sdtUnknown)
        {
            m_RawScore.mapSubDimensionTypeToScore[eSubDimensionType] += iQuestionScore;
            m_RawScore.iTotalScore += iQuestionScore;
        }

        if(IsFIndex(iQuestionIndex))
        {
            if(iQuestionScore == 0)
                m_GeneData.iFIndex++;
        }
    }

    // Calculate report score
    m_RptScore.mapSubDimensionTypeToScore[sdtCommunication] = m_RawScore.mapSubDimensionTypeToScore.value(sdtCommunication) * 100 / 21;
    m_RptScore.mapSubDimensionTypeToScore[sdtCooperation] = m_RawScore.mapSubDimensionTypeToScore.value(sdtCooperation) * 100 / 18;
    m_RptScore.mapSubDimensionTypeToScore[sdtAssertion] = m_RawScore.mapSubDimensionTypeToScore.value(sdtAssertion) * 100 / 21;
    m_RptScore.mapSubDimensionTypeToScore[sdtResponsibility] = m_RawScore.mapSubDimensionTypeToScore.value(sdtResponsibility) * 100 / 18;
    m_RptScore.mapSubDimensionTypeToScore[sdtEmpathy] = m_RawScore.mapSubDimensionTypeToScore.value(sdtEmpathy) * 100 / 18;
    m_RptScore.mapSubDimensionTypeToScore[sdtEngagement] = m_RawScore.mapSubDimensionTypeToScore.value(sdtEngagement) * 100 / 21;
    m_RptScore.mapSubDimensionTypeToScore[sdtSelfControl] = m_RawScore.mapSubDimensionTypeToScore.value(sdtSelfControl) * 100 / 21;
    m_RptScore.iTotalScore = m_RawScore.iTotalScore * 100 / 138;

    // Calculate standard score
    m_SddScore.iTotalScore = GetTotalSddScore();

    int i68ConInterval = Get68ConInterval();
    int i95ConInterval = Get95ConInterval();
    m_SddScore.iTotalScore68Lower = m_SddScore.iTotalScore - i68ConInterval;
    m_SddScore.iTotalScore68Upper = m_SddScore.iTotalScore + i68ConInterval;
    m_SddScore.iTotalScore95Lower = m_SddScore.iTotalScore - i95ConInterval;
    m_SddScore.iTotalScore95Upper = m_SddScore.iTotalScore + i95ConInterval;

    m_RawScore.iTotalScore68Lower = GetTotalRawScoreFromSddScore(m_SddScore.iTotalScore68Lower);
    m_RawScore.iTotalScore68Upper = GetTotalRawScoreFromSddScore(m_SddScore.iTotalScore68Upper);
    m_RawScore.iTotalScore95Lower = GetTotalRawScoreFromSddScore(m_SddScore.iTotalScore95Lower);
    m_RawScore.iTotalScore95Upper = GetTotalRawScoreFromSddScore(m_SddScore.iTotalScore95Upper);

    m_RptScore.iTotalScore68Lower = m_RawScore.iTotalScore68Lower * 100 / 138;
    m_RptScore.iTotalScore68Upper = m_RawScore.iTotalScore68Upper * 100 / 138;
    m_RptScore.iTotalScore95Lower = m_RawScore.iTotalScore95Lower * 100 / 138;
    m_RptScore.iTotalScore95Upper = m_RawScore.iTotalScore95Upper * 100 / 138;

    // Calculate general data
    m_GeneData.eTotalLevelType = GetTotalLevelType();
    m_GeneData.iTotalPercentCombined = GetTotalPercentCombined();
    m_GeneData.iTotalPercentGender = GetTotalPercentGender();

    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        SubDimensionType eSubDimensionType = SubDimensionType(i);

        LevelType eLevelTypeCombined = GetSubDimensionLevelTypeCombined(eSubDimensionType);
        m_GeneData.mapSubDimensionTypeToLevelTypeCombined[eSubDimensionType] = eLevelTypeCombined;
        if(eLevelTypeCombined == ltAboveAverage)
            m_GeneData.lstAboveAverageSubDimensionNameCombined.append(SubDimensionTypeToName(eSubDimensionType));
        else if(eLevelTypeCombined == ltAverage)
            m_GeneData.lstAverageSubDimensionNameCombined.append(SubDimensionTypeToName(eSubDimensionType));
        else if(eLevelTypeCombined == ltBelowAverage)
            m_GeneData.lstBelowAverageSubDimensionNameCombined.append(SubDimensionTypeToName(eSubDimensionType));

        LevelType eLevelTypeGender = GetSubDimensionLevelTypeGender(eSubDimensionType);
        m_GeneData.mapSubDimensionTypeToLevelTypeGender[eSubDimensionType] = eLevelTypeGender;
        if(eLevelTypeGender == ltAboveAverage)
            m_GeneData.lstAboveAverageSubDimensionNameGender.append(SubDimensionTypeToName(eSubDimensionType));
        else if(eLevelTypeGender == ltAverage)
            m_GeneData.lstAverageSubDimensionNameGender.append(SubDimensionTypeToName(eSubDimensionType));
        else if(eLevelTypeGender == ltBelowAverage)
            m_GeneData.lstBelowAverageSubDimensionNameGender.append(SubDimensionTypeToName(eSubDimensionType));
    }

    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        SubDimensionType eSubDimensionType = SubDimensionType(i);

        int iIndex = m_GeneData.lstOrderedSubDimensionType.size();
        for(int j = 0; j < m_GeneData.lstOrderedSubDimensionType.size(); j++)
        {
            SubDimensionType e = SubDimensionType(m_GeneData.lstOrderedSubDimensionType.at(j));
            if(m_RptScore.mapSubDimensionTypeToScore.value(eSubDimensionType) > m_RptScore.mapSubDimensionTypeToScore.value(e))
            {
                iIndex = j;
                break;
            }
        }
        m_GeneData.lstOrderedSubDimensionType.insert(iIndex, eSubDimensionType);
    }

    // Get TotalLevelConclusion1
    QString strConclustion1WellAboveAverage = m_pSettings->value("ssis_total_level_conclusion1/ltWellAboveAverage").toString();
    QString strConclustion1AboveAverage = m_pSettings->value("ssis_total_level_conclusion1/ltAboveAverage").toString();
    QString strConclustion1Average = m_pSettings->value("ssis_total_level_conclusion1/ltAverage").toString();
    QString strConclustion1BelowAverage = m_pSettings->value("ssis_total_level_conclusion1/ltBelowAverage").toString();
    QString strConclustion1WellBelowAverage = m_pSettings->value("ssis_total_level_conclusion1/ltWellBelowAverage").toString();

    QMap<LevelType, QString> mapLevelTypeToConclusion1;
    mapLevelTypeToConclusion1.insert(ltWellAboveAverage, strConclustion1WellAboveAverage);
    mapLevelTypeToConclusion1.insert(ltAboveAverage, strConclustion1AboveAverage);
    mapLevelTypeToConclusion1.insert(ltAverage, strConclustion1Average);
    mapLevelTypeToConclusion1.insert(ltBelowAverage, strConclustion1BelowAverage);
    mapLevelTypeToConclusion1.insert(ltWellBelowAverage, strConclustion1WellBelowAverage);

    m_GeneData.strSuggestion1 = mapLevelTypeToConclusion1.value(m_GeneData.eTotalLevelType);

    // Get Subdimension Suggestion
    QMap<SubDimensionType, QString> mapSubDimensionTypeToSuggestion;
    mapSubDimensionTypeToSuggestion.insert(sdtCommunication, m_pSettings->value("ssis_report_suggestion/sdtCommunication").toString());
    mapSubDimensionTypeToSuggestion.insert(sdtCooperation, m_pSettings->value("ssis_report_suggestion/sdtCooperation").toString());
    mapSubDimensionTypeToSuggestion.insert(sdtAssertion, m_pSettings->value("ssis_report_suggestion/sdtAssertion").toString());
    mapSubDimensionTypeToSuggestion.insert(sdtResponsibility, m_pSettings->value("ssis_report_suggestion/sdtResponsibility").toString());
    mapSubDimensionTypeToSuggestion.insert(sdtEmpathy, m_pSettings->value("ssis_report_suggestion/sdtEmpathy").toString());
    mapSubDimensionTypeToSuggestion.insert(sdtEngagement, m_pSettings->value("ssis_report_suggestion/sdtEngagement").toString());
    mapSubDimensionTypeToSuggestion.insert(sdtSelfControl, m_pSettings->value("ssis_report_suggestion/sdtSelfControl").toString());

    QString strHighestSuggestion = mapSubDimensionTypeToSuggestion.value(m_GeneData.lstOrderedSubDimensionType.first());
    QString strAboveSuggestion, strBelowSuggestion;
    for(int i = m_GeneData.lstOrderedSubDimensionType.size() - 1; i >= 0; i--)
    {
        const QString strSubDimensionTypeNane = SubDimensionTypeToName(m_GeneData.lstOrderedSubDimensionType.at(i));
        if(m_GeneData.lstBelowAverageSubDimensionNameGender.contains(strSubDimensionTypeNane))
            strBelowSuggestion = QString("%1%2").arg(strBelowSuggestion).arg(mapSubDimensionTypeToSuggestion.value(m_GeneData.lstOrderedSubDimensionType.at(i)));
        if(m_GeneData.lstAboveAverageSubDimensionNameGender.contains(strSubDimensionTypeNane))
            strAboveSuggestion = QString("%1%2").arg(strAboveSuggestion).arg(mapSubDimensionTypeToSuggestion.value(m_GeneData.lstOrderedSubDimensionType.at(i)));
    }

    // Get TotalLevelConclusion2
    QString strConclustion2OnlyA = m_pSettings->value("ssis_total_level_conclusion2/OnlyA").toString();
    QString strConclustion2OnlyB = m_pSettings->value("ssis_total_level_conclusion2/OnlyB").toString();
    QString strConclustion2BothAB = m_pSettings->value("ssis_total_level_conclusion2/BothAB").toString();
    QString strConclustion2NoneAB = m_pSettings->value("ssis_total_level_conclusion2/NoneAB").toString();

    QString strTotalLevelConclusion2;
    if(!strAboveSuggestion.isEmpty() && strBelowSuggestion.isEmpty())
    {
        strTotalLevelConclusion2 = strConclustion2OnlyA.replace(g_strAboveSuggestion, strAboveSuggestion);
    }
    else if(strAboveSuggestion.isEmpty() && !strBelowSuggestion.isEmpty())
    {
        strTotalLevelConclusion2 = strConclustion2OnlyB.replace(g_strBelowSuggestion, strBelowSuggestion);
    }
    else if(!strAboveSuggestion.isEmpty() && !strBelowSuggestion.isEmpty())
    {
        strTotalLevelConclusion2 = strConclustion2BothAB.replace(g_strAboveSuggestion, strAboveSuggestion);
        strTotalLevelConclusion2 = strTotalLevelConclusion2.replace(g_strBelowSuggestion, strBelowSuggestion);
    }
    else
    {
        strTotalLevelConclusion2 = strConclustion2NoneAB.replace(g_strHighestSubDimen, SubDimensionTypeToName(m_GeneData.lstOrderedSubDimensionType.first()));
        strTotalLevelConclusion2 = strTotalLevelConclusion2.replace(g_strHighestSuggestion, strHighestSuggestion);
    }
    m_GeneData.strSuggestion2 = strTotalLevelConclusion2;

    m_mapVariableData.insert(g_strTotalRScore, QString::number(m_RptScore.iTotalScore));
    m_mapVariableData.insert(g_strTotalRScore68Lower, QString::number(m_RptScore.iTotalScore68Lower));
    m_mapVariableData.insert(g_strTotalRScore68Upper, QString::number(m_RptScore.iTotalScore68Upper));
    m_mapVariableData.insert(g_strTotalRScore95Lower, QString::number(m_RptScore.iTotalScore95Lower));
    m_mapVariableData.insert(g_strTotalRScore95Upper, QString::number(m_RptScore.iTotalScore95Upper));
    m_mapVariableData.insert(g_strTotalLevel, LevelTypeToName(m_GeneData.eTotalLevelType));
    m_mapVariableData.insert(g_strAboveAveSubDimenNum, QString::number(m_GeneData.lstAboveAverageSubDimensionNameCombined.size()));
    m_mapVariableData.insert(g_strAboveAveSubDimenList, m_GeneData.lstAboveAverageSubDimensionNameCombined.isEmpty() ?  "" : QLatin1String(" : ") + m_GeneData.lstAboveAverageSubDimensionNameCombined.join(QLatin1String(", ")));
    m_mapVariableData.insert(g_strAveSubDimenNum, QString::number(m_GeneData.lstAverageSubDimensionNameCombined.size()));
    m_mapVariableData.insert(g_strAveSubDimenList, m_GeneData.lstAverageSubDimensionNameCombined.isEmpty() ? "" : QLatin1String(" : ") + m_GeneData.lstAverageSubDimensionNameCombined.join(QLatin1String(", ")));
    m_mapVariableData.insert(g_strBelowAveSubDimenNum, QString::number(m_GeneData.lstBelowAverageSubDimensionNameCombined.size()));
    m_mapVariableData.insert(g_strBelowAveSubDimenList, m_GeneData.lstBelowAverageSubDimensionNameCombined.isEmpty() ? "" : QLatin1String(" : ") + m_GeneData.lstBelowAverageSubDimensionNameCombined.join(QLatin1String(", ")));
    m_mapVariableData.insert(g_strHighestSubDimen, SubDimensionTypeToName(m_GeneData.lstOrderedSubDimensionType.first()));
    m_mapVariableData.insert(g_strLowestSubDimen, SubDimensionTypeToName(m_GeneData.lstOrderedSubDimensionType.last()));
    m_mapVariableData.insert(g_strTotalPercentCombined, QString::number(m_GeneData.iTotalPercentCombined));
    m_mapVariableData.insert(g_strTotalPercentGender, QString::number(m_GeneData.iTotalPercentGender));
    m_mapVariableData.insert(g_strFIndex, QString::number(m_GeneData.iFIndex));
    //m_mapVariableData.insert(g_strSubdimenSuggestion, strSubdimenSuggestion);
    m_mapVariableData.insert(g_strTotalLevelConclusion1, m_GeneData.strSuggestion1);
    m_mapVariableData.insert(g_strTotalLevelConclusion2, m_GeneData.strSuggestion2);

    DrawImageForBasicInfo();
    DrawImageForSLevel();
    DrawImageForSubDimenNum();
    DrawImageForSubDimenBar();
    DrawImageRScoreRange();
    DrawImageSubDimenTable();
    DrawPercentCombined();
    DrawPercentGender();

    return true;
}

QJsonObject ReportSsis::BuildJsonResult()
{
    static QMap<SubDimensionType, QString> s_mapTypeToJsonkey;
    if(s_mapTypeToJsonkey.isEmpty())
    {
        s_mapTypeToJsonkey.insert(sdtCommunication, "communication");
        s_mapTypeToJsonkey.insert(sdtCooperation, "cooperation");
        s_mapTypeToJsonkey.insert(sdtAssertion, "assertion");
        s_mapTypeToJsonkey.insert(sdtResponsibility, "responsibility");
        s_mapTypeToJsonkey.insert(sdtEmpathy, "empathy");
        s_mapTypeToJsonkey.insert(sdtEngagement, "engagement");
        s_mapTypeToJsonkey.insert(sdtSelfControl, "selfcontrol");
    }

    QJsonObject joResult;
    joResult.insert("f_index", QString::number(m_GeneData.iFIndex));
    joResult.insert("suggestion1", m_GeneData.strSuggestion1);
    joResult.insert("suggestion2", m_GeneData.strSuggestion2);

    // level
    QJsonObject joLevel;
    joLevel.insert("total", LevelTypeToName(m_GeneData.eTotalLevelType));
    joLevel.insert("percent_combined", QString::number(m_GeneData.iTotalPercentCombined));
    joLevel.insert("percent_gender", QString::number(m_GeneData.iTotalPercentGender));

    QJsonArray jaDimensionForLevel;
    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        SubDimensionType eSubDimensionType = SubDimensionType(i);

        QJsonObject jo;
        jo.insert("name", s_mapTypeToJsonkey.value(eSubDimensionType));
        jo.insert("level", LevelTypeToName(m_GeneData.mapSubDimensionTypeToLevelTypeCombined.value(eSubDimensionType)));
        jaDimensionForLevel.append(jo);
    }
    joLevel.insert("dimension", jaDimensionForLevel);
    joResult.insert("level", joLevel);

    // raw_score
    QJsonObject joRawScore;
    joRawScore.insert("total", QString::number(m_RawScore.iTotalScore));
    joRawScore.insert("68lower", QString::number(m_RawScore.iTotalScore68Lower));
    joRawScore.insert("68upper", QString::number(m_RawScore.iTotalScore68Upper));
    joRawScore.insert("95lower", QString::number(m_RawScore.iTotalScore95Lower));
    joRawScore.insert("95upper", QString::number(m_RawScore.iTotalScore95Upper));

    QJsonArray jaDimensionForRawScore;
    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        SubDimensionType eSubDimensionType = SubDimensionType(i);

        QJsonObject jo;
        jo.insert("name", s_mapTypeToJsonkey.value(eSubDimensionType));
        jo.insert("score", QString::number(m_RawScore.mapSubDimensionTypeToScore.value(eSubDimensionType)));
        jaDimensionForRawScore.append(jo);
    }
    joRawScore.insert("dimension", jaDimensionForRawScore);
    joResult.insert("raw_score", joRawScore);

    // rpt_score
    QJsonObject joRptScore;
    joRptScore.insert("total", QString::number(m_RptScore.iTotalScore));
    joRptScore.insert("68lower", QString::number(m_RptScore.iTotalScore68Lower));
    joRptScore.insert("68upper", QString::number(m_RptScore.iTotalScore68Upper));
    joRptScore.insert("95lower", QString::number(m_RptScore.iTotalScore95Lower));
    joRptScore.insert("95upper", QString::number(m_RptScore.iTotalScore95Upper));

    QJsonArray jaDimensionForRptScore;
    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        SubDimensionType eSubDimensionType = SubDimensionType(i);

        QJsonObject jo;
        jo.insert("name", s_mapTypeToJsonkey.value(eSubDimensionType));
        jo.insert("score", QString::number(m_RptScore.mapSubDimensionTypeToScore.value(eSubDimensionType)));
        jaDimensionForRptScore.append(jo);
    }
    joRptScore.insert("dimension", jaDimensionForRptScore);
    joResult.insert("rpt_score", joRptScore);

    // sdd_score
    QJsonObject joSddScore;
    joSddScore.insert("total", QString::number(m_SddScore.iTotalScore));
    joSddScore.insert("68lower", QString::number(m_SddScore.iTotalScore68Lower));
    joSddScore.insert("68upper", QString::number(m_SddScore.iTotalScore68Upper));
    joSddScore.insert("95lower", QString::number(m_SddScore.iTotalScore95Lower));
    joSddScore.insert("95upper", QString::number(m_SddScore.iTotalScore95Upper));
    joResult.insert("sdd_score", joSddScore);

    return joResult;
}

SubDimensionType ReportSsis::GetSubDimensionTypeFromQestionIndex(int iQuestionIndex) const
{
    static QMap<SubDimensionType, QList<int> > s_mapSubDimensionTypeToQuestionIndex;
    if(s_mapSubDimensionTypeToQuestionIndex.isEmpty())
    {
        s_mapSubDimensionTypeToQuestionIndex.insert(sdtCommunication, QList<int>() << 4 << 10 << 14 << 20 << 24 << 30 << 40);
        s_mapSubDimensionTypeToQuestionIndex.insert(sdtCooperation, QList<int>() << 2 << 7 << 12 << 17 << 27 << 37);
        s_mapSubDimensionTypeToQuestionIndex.insert(sdtAssertion, QList<int>() << 1 << 5 << 11 << 15 << 25 << 35 << 45);
        s_mapSubDimensionTypeToQuestionIndex.insert(sdtResponsibility, QList<int>() << 6 << 16 << 22 << 26 << 32 << 42);
        s_mapSubDimensionTypeToQuestionIndex.insert(sdtEmpathy, QList<int>() << 3 << 8 << 13 << 18 << 28 << 38);
        s_mapSubDimensionTypeToQuestionIndex.insert(sdtEngagement, QList<int>() << 9 << 19 << 23 << 29 << 33 << 39 << 43);
        s_mapSubDimensionTypeToQuestionIndex.insert(sdtSelfControl, QList<int>() << 21 << 31 << 34 << 36 << 41 << 44 << 46);
    }

    foreach(SubDimensionType eSubDimensionType, s_mapSubDimensionTypeToQuestionIndex.keys())
    {
        if(s_mapSubDimensionTypeToQuestionIndex.value(eSubDimensionType).contains(iQuestionIndex))
            return eSubDimensionType;
    }

    return sdtUnknown;
}

int ReportSsis::GetTotalSddScore()
{
    Raw_Standard_Percent result;
    bool bResult;

    if(m_ChildInfo.iAge <= 5)
        bResult = C4_P_3_5C_FromRawScore(m_RawScore.iTotalScore, result);
    else
        bResult = C5_P_5_12C_FromRawScore(m_RawScore.iTotalScore, result);

    if(bResult)
        return result.uchSSStandardScore;

    return 0;
}

int ReportSsis::Get68ConInterval()
{
    Con_Interval result;
    bool bResult;

    if(m_ChildInfo.iAge <= 5)
        bResult = D1_P_3_5C_From(result);
    else
        bResult = D1_P_5_12C_From(result);

    if(bResult)
        return result.uchSS68;

    return 0;
}

int ReportSsis::Get95ConInterval()
{
    Con_Interval result;
    bool bResult;

    if(m_ChildInfo.iAge <= 5)
        bResult = D1_P_3_5C_From(result);
    else
        bResult = D1_P_5_12C_From(result);

    if(bResult)
        return result.uchSS95;

    return 0;
}

int ReportSsis::GetTotalRawScoreFromSddScore(int iTotalSddScore)
{
    QList<Raw_Standard_Percent> lstAllResults;
    if(m_ChildInfo.iAge <= 5)
        C4_P_3_5C_AllData(lstAllResults);
    else
        C5_P_5_12C_AllData(lstAllResults);

    if(!lstAllResults.isEmpty())
    {
        if(iTotalSddScore >= lstAllResults.first().uchSSStandardScore)
            return lstAllResults.first().uchRawScore;
        else if(iTotalSddScore <= lstAllResults.last().uchSSStandardScore)
            return lstAllResults.last().uchRawScore;
    }

    int iTotalRawScore = 0;
    QList<Raw_Standard_Percent> lstResults;
    bool bResult;

    if(m_ChildInfo.iAge <= 5)
        bResult = C4_P_3_5C_FromSSStandardScore(iTotalSddScore, lstResults);
    else
        bResult = C5_P_5_12C_FromSSStandardScore(iTotalSddScore, lstResults);

    if(bResult && !lstResults.isEmpty())
    {
        foreach(const Raw_Standard_Percent &result, lstResults)
            iTotalRawScore += result.uchRawScore;

        iTotalRawScore = iTotalRawScore / lstResults.size();
    }

    return iTotalRawScore;
}

int ReportSsis::AnswerToScore(const QString &strAnswer)
{
    static QMap<QString, int> mapAnswerToScore;
    if(mapAnswerToScore.isEmpty())
    {
        mapAnswerToScore.insert("A", 0);
        mapAnswerToScore.insert("B", 1);
        mapAnswerToScore.insert("C", 2);
        mapAnswerToScore.insert("D", 3);
    }

    return mapAnswerToScore.value(strAnswer, 0);
}

bool ReportSsis::IsFIndex(int iQuestionIndex)
{
    static QList<int> lstF = QList<int>() << 2 << 12 << 17 << 24 << 29;
    return lstF.contains(iQuestionIndex);
}

LevelType ReportSsis::GetTotalLevelType()
{
    LevelType eLevelType = ltUnknown;

    if(m_SddScore.iTotalScore < 70)
        eLevelType = ltWellBelowAverage;
    else if(m_SddScore.iTotalScore >= 70 && m_SddScore.iTotalScore < 85)
        eLevelType = ltBelowAverage;
    else if(m_SddScore.iTotalScore >= 85 && m_SddScore.iTotalScore < 115)
        eLevelType = ltAverage;
    else if(m_SddScore.iTotalScore >= 115 && m_SddScore.iTotalScore < 130)
        eLevelType = ltAboveAverage;
    else
        eLevelType = ltWellAboveAverage;

    return eLevelType;
}

int ReportSsis::GetTotalPercentCombined()
{
    Raw_Standard_Percent result;
    bool bResult;

    if(m_ChildInfo.iAge <= 5)
        bResult = C4_P_3_5C_FromRawScore(m_RawScore.iTotalScore, result);
    else
        bResult = C5_P_5_12C_FromRawScore(m_RawScore.iTotalScore, result);

    if(bResult)
        return result.uchSSPercent;

    return 0;
}

int ReportSsis::GetTotalPercentGender()
{
    if(m_ChildInfo.eGenderType == gtMale) // Male
    {
        Raw_Standard_Percent result;
        bool bResult;

        if(m_ChildInfo.iAge <= 5)
            bResult = C4_P_3_5M_FromRawScore(m_RawScore.iTotalScore, result);
        else
            bResult = C5_P_5_12M_FromRawScore(m_RawScore.iTotalScore, result);

        if(bResult)
            return result.uchSSPercent;
    }
    else // Female
    {
        Raw_Standard_Percent result;
        bool bResult;

        if(m_ChildInfo.iAge <= 5)
            bResult = C4_P_3_5F_FromRawScore(m_RawScore.iTotalScore, result);
        else
            bResult = C5_P_5_12F_FromRawScore(m_RawScore.iTotalScore, result);

        if(bResult)
            return result.uchSSPercent;
    }

    return 0;
}

LevelType ReportSsis::GetSubDimensionLevelTypeCombined(SubDimensionType eSubDimensionType)
{
    LevelType eLevelType = ltUnknown;

    Raw_Level result;
    if(D2_P_3_5C_FromSubDimensionType(eSubDimensionType, result))
    {
        int iSubDimensionScore = m_RawScore.mapSubDimensionTypeToScore.value(eSubDimensionType, 0);

        if(iSubDimensionScore >= result.uchBelowAverageLower && iSubDimensionScore <= result.uchBelowAverageUpper)
            eLevelType = ltBelowAverage;
        else if(iSubDimensionScore >= result.uchAverageLower && iSubDimensionScore <= result.uchAverageUpper)
            eLevelType = ltAverage;
        else if(iSubDimensionScore >= result.uchAboveAverageLower && iSubDimensionScore <= result.uchAboveAverageUpper)
            eLevelType = ltAboveAverage;
    }

    return eLevelType;
}

LevelType ReportSsis::GetSubDimensionLevelTypeGender(SubDimensionType eSubDimensionType)
{
    LevelType eLevelType = ltUnknown;

    Raw_Level result;
    bool bResult;

    if(m_ChildInfo.eGenderType == gtMale) // Male
    {
        if(m_ChildInfo.iAge <= 5)
            bResult = D2_P_3_5M_FromSubDimensionType(eSubDimensionType, result);
        else
            bResult = D3_P_5_12M_FromSubDimensionType(eSubDimensionType, result);
    }
    else // Female
    {
        if(m_ChildInfo.iAge <= 5)
            bResult = D2_P_3_5F_FromSubDimensionType(eSubDimensionType, result);
        else
            bResult = D3_P_5_12F_FromSubDimensionType(eSubDimensionType, result);
    }

    if(bResult)
    {
        int iSubDimensionScore = m_RawScore.mapSubDimensionTypeToScore.value(eSubDimensionType, 0);

        if(iSubDimensionScore >= result.uchBelowAverageLower && iSubDimensionScore <= result.uchBelowAverageUpper)
            eLevelType = ltBelowAverage;
        else if(iSubDimensionScore >= result.uchAverageLower && iSubDimensionScore <= result.uchAverageUpper)
            eLevelType = ltAverage;
        else if(iSubDimensionScore >= result.uchAboveAverageLower && iSubDimensionScore <= result.uchAboveAverageUpper)
            eLevelType = ltAboveAverage;
    }

    return eLevelType;
}

void ReportSsis::DrawImageForSLevel()
{
    const QRect rcImage(0, 0, 1400, 1400);
    const QRect rcPainter(100, 100, 1200, 1200);
    const int iCaptionWidth = 500;

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(Qt::darkGray, 0));
    painter.setBrush(Qt::NoBrush);

    int iXContentCenter = rcPainter.left() + (iCaptionWidth + rcPainter.width()) / 2;
    painter.drawLine(iXContentCenter, rcPainter.top(), iXContentCenter, rcPainter.bottom());
    for(int i = rcPainter.top(); i < rcPainter.bottom(); i += 50)
        painter.drawLine(iXContentCenter - 30, i, iXContentCenter + 30, i);

    painter.setPen(QPen(Qt::black, 1));

    painter.drawRect(rcPainter);

    painter.setPen(QPen(Qt::black, 1));

    painter.drawLine(rcPainter.left(), rcPainter.top() + 300, rcPainter.right(), rcPainter.top() + 300);
    painter.drawLine(rcPainter.left(), rcPainter.top() + 450, rcPainter.right(), rcPainter.top() + 450);
    painter.drawLine(rcPainter.left(), rcPainter.top() + 750, rcPainter.right(), rcPainter.top() + 750);
    painter.drawLine(rcPainter.left(), rcPainter.top() + 900, rcPainter.right(), rcPainter.top() + 900);
    painter.drawLine(rcPainter.left() + iCaptionWidth, rcPainter.top(), rcPainter.left() + iCaptionWidth, rcPainter.bottom());
    painter.drawLine(rcPainter.left() + iCaptionWidth, rcPainter.top(), rcPainter.left() + iCaptionWidth, rcPainter.bottom());

    QFont font = qApp->font();
    font.setPointSize(45);
    painter.setFont(font);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignCenter);
    TextOption.setWrapMode(QTextOption::WordWrap);
    painter.drawText(QRect(rcPainter.left(), rcPainter.top(), iCaptionWidth, 300), tr("Well-above Average"), TextOption);
    painter.drawText(QRect(rcPainter.left(), rcPainter.top() + 300, iCaptionWidth, 150), tr("Above Average"), TextOption);
    painter.drawText(QRect(rcPainter.left(), rcPainter.top() + 450, iCaptionWidth, 300), tr("Average"), TextOption);
    painter.drawText(QRect(rcPainter.left(), rcPainter.top() + 750, iCaptionWidth, 150), tr("Below Average"), TextOption);
    painter.drawText(QRect(rcPainter.left(), rcPainter.top() + 900, iCaptionWidth, 300), tr("Well-below Average"), TextOption);

    QRect rc95 = QRect(iXContentCenter - 30, rcPainter.bottom() - (m_SddScore.iTotalScore95Upper - 40) * 10, 60, (m_SddScore.iTotalScore95Upper - m_SddScore.iTotalScore95Lower) * 10);
    painter.setPen(QPen(QColor(112, 173, 71), 0));
    painter.setBrush(QColor(112, 173, 71));
    painter.drawRect(rc95);

    QRect rc68 = QRect(iXContentCenter - 30, rcPainter.bottom() - (m_SddScore.iTotalScore68Upper - 40) * 10, 60, (m_SddScore.iTotalScore68Upper - m_SddScore.iTotalScore68Lower) * 10);
    painter.setPen(QPen(QColor(180, 207, 168), 0));
    painter.setBrush(QColor(180, 207, 168));
    painter.drawRect(rc68);

    painter.setBrush(Qt::black);
    QRect rcPoint(0, 0, 30, 30);
    rcPoint.moveCenter(QPoint(iXContentCenter, rcPainter.bottom() - (m_SddScore.iTotalScore - 40) * 10));
    painter.drawEllipse(rcPoint);

    m_mapImageData.insert(QLatin1String("userdata://slevel.png"), image);
}

void ReportSsis::DrawImageForSubDimenNum()
{
    const QRect rcImage(0, 0, 2200, 1200);
    const QRect rcPainter(100, 100, 2000, 1000);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::NoBrush);

    //painter.drawRect(rcPainter);

    int iAboveAverageAngle = int(double(m_GeneData.lstAboveAverageSubDimensionNameCombined.size() * 360 * 16) / 7.0 + 0.5);
    int iAverageAngle = int(double(m_GeneData.lstAverageSubDimensionNameCombined.size() * 360 * 16) / 7.0 + 0.5);
    int iBelowAverageAngle = int(double(m_GeneData.lstBelowAverageSubDimensionNameCombined.size() * 360 * 16) / 7.0 + 0.5);

    QRect rcPie = QRect(rcPainter.left() + 150, rcPainter.top() + 50, 900, 900);
    QRect rcDes = QRect(0, 0, 40, 40);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    TextOption.setWrapMode(QTextOption::NoWrap);

    QFont font = qApp->font();
    font.setPointSize(40);
    painter.setFont(font);

    painter.setPen(QPen(QColor(91, 142, 57), 0));
    painter.setBrush(QBrush(QColor(91, 142, 57)));
    painter.drawPie(rcPie, 0, iAboveAverageAngle);
    rcDes.moveCenter(QPoint(rcPainter.left() + 1400, rcPainter.center().y() - 100));
    painter.drawRect(rcDes);

    painter.setPen(QPen(Qt::black, 0));
    painter.drawText(QRect(rcDes.right() + 50, rcDes.center().y() - 100, 500, 200) , LevelTypeToName(ltAboveAverage), TextOption);

    painter.setPen(QPen(QColor(112, 173, 71), 0));
    painter.setBrush(QBrush(QColor(112, 173, 71)));
    painter.drawPie(rcPie, iAboveAverageAngle, iAverageAngle);
    rcDes.moveCenter(QPoint(rcPainter.left() + 1400, rcPainter.center().y()));
    painter.drawRect(rcDes);

    painter.setPen(QPen(Qt::black, 0));
    painter.drawText(QRect(rcDes.right() + 50, rcDes.center().y() - 100, 500, 200) , LevelTypeToName(ltAverage), TextOption);

    painter.setPen(QPen(QColor(180, 207, 168), 0));
    painter.setBrush(QBrush(QColor(180, 207, 168)));
    painter.drawPie(rcPie, iAboveAverageAngle + iAverageAngle, iBelowAverageAngle);
    rcDes.moveCenter(QPoint(rcPainter.left() + 1400, rcPainter.center().y() + 100));
    painter.drawRect(rcDes);

    painter.setPen(QPen(Qt::black, 0));
    painter.drawText(QRect(rcDes.right() + 50, rcDes.center().y() - 100, 500, 200) , LevelTypeToName(ltBelowAverage), TextOption);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::white));
    QRect rcPieInteral = QRect(rcPie.center().x() - 250, rcPie.center().y() - 250, 500, 500);
    painter.drawEllipse(rcPieInteral);

    m_mapImageData.insert(QLatin1String("userdata://subdimen_num.png"), image);
}

void ReportSsis::DrawImageForSubDimenBar()
{
    const QRect rcImage(0, 0, 2020, 1400);
    const QRect rcPainter(10, 100, 2000, 1200);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::NoBrush);

    //painter.drawRect(rcPainter);

    painter.setPen(QPen(Qt::black, 0));

    painter.drawLine(rcPainter.left() + 200, rcPainter.top() + 50, rcPainter.left() + 200, rcPainter.bottom() - 150);

    QFont font = qApp->font();
    font.setPointSize(35);
    painter.setFont(font);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignCenter);
    TextOption.setWrapMode(QTextOption::WordWrap);

    for(int i = 0; i <= 5; i++)
    {
        QRect rcXText(rcPainter.left(), rcPainter.bottom() - 150 - 100 - i * 200, 200, 200);
        painter.drawText(rcXText, QString::number(i * 20), TextOption);
        painter.drawLine(rcPainter.left() + 200 - 20, rcPainter.bottom() - 150 - i * 200, rcPainter.right() - 50, rcPainter.bottom() - 150 - i * 200);
    }

    for(int i = 0; i < m_GeneData.lstOrderedSubDimensionType.size(); i++)
    {
        QRect rcXText(rcPainter.left() + 200 + i * 250, rcPainter.bottom() - 150, 250, 150);
        painter.drawText(rcXText, SubDimensionTypeToName(m_GeneData.lstOrderedSubDimensionType.at(i)), TextOption);
        painter.drawLine(rcPainter.left() + 200 + i * 250, rcPainter.bottom() - 150, rcPainter.left() + 200 + i * 250, rcPainter.bottom() - 150 + 20);

        int iRScore = m_RptScore.mapSubDimensionTypeToScore.value(m_GeneData.lstOrderedSubDimensionType.at(i));
        int iYScore = iRScore * 10;

        painter.save();
        painter.setPen(QPen(QColor(112, 173, 71), 0));
        painter.setBrush(QColor(112, 173, 71));
        painter.drawRect(rcPainter.left() + 200 + i * 250 + 125 - 50, rcPainter.bottom() - 150 - iYScore, 100, iYScore);
        painter.restore();

    }
    painter.drawLine(rcPainter.left() + 200 + m_GeneData.lstOrderedSubDimensionType.size() * 250, rcPainter.bottom() - 150, rcPainter.left() + 200 + m_GeneData.lstOrderedSubDimensionType.size() * 250, rcPainter.bottom() - 150 + 20);

    m_mapImageData.insert(QLatin1String("userdata://subdimen_bar.png"), image);
}

void ReportSsis::DrawImageRScoreRange()
{
    const QRect rcImage(0, 0, 2100, 300);
    const QRect rcPainter(50, 100, 2000, 200);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(Qt::black, 8));

    painter.drawLine(rcPainter.left(), rcPainter.top() + 100, rcPainter.right(), rcPainter.top() + 100);

    QFont font = qApp->font();
    font.setPointSize(35);
    painter.setFont(font);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignCenter);
    TextOption.setWrapMode(QTextOption::WordWrap);

    QRect rcText = QRect(0, 0, 200, 100);
    for(int i = 0; i <= 10; i++)
    {
        rcText.moveCenter(QPoint(rcPainter.left() + i * 200, rcPainter.top() + 100 + rcText.height() / 2));
        painter.drawText(rcText, QString::number(i * 10), TextOption);

        painter.save();
        painter.setPen(QPen(Qt::darkGray, 4));
        painter.drawLine(rcText.center().x(), rcPainter.top() + 100, rcText.center().x(), rcPainter.top() + 100 + 20);
        painter.restore();
    }

    int iXRScore = rcPainter.left() + m_RptScore.iTotalScore * 20;
    int iXRScore68Lower = rcPainter.left() + m_RptScore.iTotalScore68Lower * 20;
    int iXRScore68Upper = rcPainter.left() + m_RptScore.iTotalScore68Upper * 20;
    int iXRScore95Lower = rcPainter.left() + m_RptScore.iTotalScore95Lower * 20;
    int iXRScore95Upper = rcPainter.left() + m_RptScore.iTotalScore95Upper * 20;

    QRect rc95 = QRect(iXRScore95Lower, rcPainter.top() + 10, iXRScore95Upper - iXRScore95Lower, 80);
    painter.setPen(QPen(QColor(112, 173, 71), 0));
    painter.setBrush(QColor(112, 173, 71));
    painter.drawRect(rc95);

    QRect rc68 = QRect(iXRScore68Lower, rcPainter.top() + 10, iXRScore68Upper - iXRScore68Lower, 80);
    painter.setPen(QPen(QColor(180, 207, 168), 0));
    painter.setBrush(QColor(180, 207, 168));
    painter.drawRect(rc68);

    painter.setBrush(Qt::black);
    QRect rcPoint(0, 0, 30, 30);
    rcPoint.moveCenter(QPoint(iXRScore, rcPainter.top() + 50));
    painter.drawEllipse(rcPoint);

    m_mapImageData.insert(QLatin1String("userdata://rscore_range.png"), image);
}

void ReportSsis::DrawImageSubDimenTable()
{
    int iTableWidth = m_pSettings->value("ssis_report_table/table_width").toString().toInt();
    int iTableHeight = m_pSettings->value("ssis_report_table/table_height").toString().toInt();

    QList<int> lstRowHeight;
    foreach(const QString &strRowHeight, m_pSettings->value("ssis_report_table/row_height").toStringList())
        lstRowHeight.append(strRowHeight.toInt());

    QList<int> lstColWidth;
    foreach(const QString &strColWidth, m_pSettings->value("ssis_report_table/col_width").toStringList())
        lstColWidth.append(strColWidth.toInt());

    QStringList lstHeader = m_pSettings->value("ssis_report_table/header").toStringList();
    QStringList lstCol1 = m_pSettings->value("ssis_report_table/col1").toStringList();
    QStringList lstCol2 = m_pSettings->value("ssis_report_table/col2").toStringList();

    const QRect rcImage(0, 0, iTableWidth + 10, iTableHeight + 10);
    const QRect rcPainter(5, 5, iTableWidth, iTableHeight);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));
    painter.fillRect(QRect(rcPainter.topLeft(), QSize(rcPainter.width(), lstRowHeight.at(0))), QBrush(QColor(242, 242, 242)));

    QFont font = qApp->font();
    font.setPointSize(33);
    font.setWeight(QFont::Bold);
    painter.setFont(font);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignCenter);
    TextOption.setWrapMode(QTextOption::WordWrap);

    int iX = rcPainter.left();
    for(int i = 0; i < lstHeader.size(); i++)
    {
        QRect rcHeader = QRect(iX + 10, rcPainter.top(), lstColWidth.at(i) - 20, lstRowHeight.at(0));
        painter.drawText(rcHeader, lstHeader.at(i), TextOption);
        iX += lstColWidth.at(i);
    }

    font.setWeight(QFont::Normal);
    painter.setFont(font);

    int iY = rcPainter.top() + lstRowHeight.at(0);
    for(int i = 0; i < lstCol1.size(); i++)
    {
        QRect rcCol1 = QRect(rcPainter.left(), iY, lstColWidth.at(0), lstRowHeight.at(i + 1));
        painter.drawText(rcCol1, lstCol1.at(i), TextOption);
        iY += lstRowHeight.at(i + 1);
    }

    TextOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    iY = rcPainter.top() + lstRowHeight.at(0);
    for(int i = 0; i < lstCol2.size(); i++)
    {
        QRect rcCol2 = QRect(rcPainter.left() + lstColWidth.at(0) + 20, iY, lstColWidth.at(1) - 20, lstRowHeight.at(i + 1));
        painter.drawText(rcCol2, lstCol2.at(i), TextOption);
        iY += lstRowHeight.at(i + 1);
    }

    TextOption.setAlignment(Qt::AlignCenter);

    iY = rcPainter.top() + lstRowHeight.at(0);
    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        QRect rcCol3 = QRect(rcPainter.left() + lstColWidth.at(0) + lstColWidth.at(1), iY, lstColWidth.at(2), lstRowHeight.at(i + 1));
        painter.drawText(rcCol3, QString::number(m_RptScore.mapSubDimensionTypeToScore.value(SubDimensionType(i))), TextOption);
        iY += lstRowHeight.at(i + 1);
    }

    iY = rcPainter.top() + lstRowHeight.at(0);
    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        QString strText;
        QColor crBack;
        if(m_GeneData.lstAboveAverageSubDimensionNameCombined.contains(SubDimensionTypeToName(SubDimensionType(i))))
        {
            strText = LevelTypeToName(ltAboveAverage);
            crBack = QColor(146, 208, 80);
        }
        else if(m_GeneData.lstAverageSubDimensionNameCombined.contains(SubDimensionTypeToName(SubDimensionType(i))))
        {
            strText = LevelTypeToName(ltAverage);
            crBack = QColor(255, 242, 204);
        }
        else if(m_GeneData.lstBelowAverageSubDimensionNameCombined.contains(SubDimensionTypeToName(SubDimensionType(i))))
        {
            strText = LevelTypeToName(ltBelowAverage);
            crBack = QColor(255, 192, 0);
        }

        QRect rcCol4 = QRect(rcPainter.left() + lstColWidth.at(0) + lstColWidth.at(1) + lstColWidth.at(2), iY, lstColWidth.at(3), lstRowHeight.at(i + 1));
        painter.fillRect(rcCol4, QBrush(crBack));
        painter.drawText(rcCol4, strText, TextOption);
        iY += lstRowHeight.at(i + 1);
    }

    iY = rcPainter.top() + lstRowHeight.at(0);
    for(int i = sdtCommunication; i <= sdtSelfControl; i++)
    {
        QString strText;
        QColor crBack;
        if(m_GeneData.lstAboveAverageSubDimensionNameGender.contains(SubDimensionTypeToName(SubDimensionType(i))))
        {
            strText = LevelTypeToName(ltAboveAverage);
            crBack = QColor(146, 208, 80);
        }
        else if(m_GeneData.lstAverageSubDimensionNameGender.contains(SubDimensionTypeToName(SubDimensionType(i))))
        {
            strText = LevelTypeToName(ltAverage);
            crBack = QColor(255, 242, 204);
        }
        else if(m_GeneData.lstBelowAverageSubDimensionNameGender.contains(SubDimensionTypeToName(SubDimensionType(i))))
        {
            strText = LevelTypeToName(ltBelowAverage);
            crBack = QColor(255, 192, 0);
        }

        QRect rcCol5 = QRect(rcPainter.left() + lstColWidth.at(0) + lstColWidth.at(1) + lstColWidth.at(2) + lstColWidth.at(3), iY, lstColWidth.at(4), lstRowHeight.at(i + 1));
        painter.fillRect(rcCol5, QBrush(crBack));
        painter.drawText(rcCol5, strText, TextOption);
        iY += lstRowHeight.at(i + 1);
    }

    painter.setPen(QPen(Qt::black, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(rcPainter);

    painter.setPen(QPen(Qt::black, 2));
    iY = rcPainter.top();
    for(int i = 1; i < lstRowHeight.size(); i++)
    {
        iY += lstRowHeight.at(i - 1);
        painter.drawLine(rcPainter.left(), iY, rcPainter.right(), iY);
    }
    iX = rcPainter.left();
    for(int i = 1; i < lstColWidth.size(); i++)
    {
        iX += lstColWidth.at(i - 1);
        painter.drawLine(iX, rcPainter.top(), iX, rcPainter.bottom());
    }

    m_mapImageData.insert(QLatin1String("userdata://subdimen_table.png"), image);
}

void ReportSsis::DrawPercentCombined()
{
    const QRect rcImage(0, 0, 1010, 1010);
    const QRect rcPainter(5, 5, 1000, 1000);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    QImage imageGirl(":/selreport/images/girl.png");
    QImage imageBoy(":/selreport/images/boy.png");

    for(int i = 0; i < m_GeneData.iTotalPercentCombined; i++)
    {
        int iRow = i / 10;
        int iCol = i % 10;

        QRect rcFill = QRect(rcPainter.left() + iCol * 100, rcPainter.bottom() - iRow * 100 - 100, 100, 100);
        if(i % 2 == 0)
            painter.drawImage(rcFill, imageGirl);
        else
            painter.drawImage(rcFill, imageBoy);

        //painter.fillRect(rcFill, QBrush(i % 2 == 0 ? QColor(249, 202, 201) : QColor(146, 166, 208)));
    }

    painter.setPen(QPen(QColor(191, 191, 191), 1));

    painter.drawRect(rcPainter);
    for(int i = 1; i < 10; i++)
    {
        painter.drawLine(rcPainter.left(), rcPainter.top() + i * 100, rcPainter.right(), rcPainter.top() + i * 100);
        painter.drawLine(rcPainter.left() + i * 100, rcPainter.top(), rcPainter.left() + i * 100, rcPainter.bottom());
    }

    m_mapImageData.insert(QLatin1String("userdata://percent_combined.png"), image);
}

void ReportSsis::DrawPercentGender()
{
    const QRect rcImage(0, 0, 1010, 1010);
    const QRect rcPainter(5, 5, 1000, 1000);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    //QBrush brush(m_ChildInfo.bChildGender ? QColor(146, 166, 208) : QColor(249, 202, 201));

    QImage imageChild(m_ChildInfo.eGenderType == gtMale ? ":/selreport/images/boy.png" : ":/selreport/images/girl.png");
    for(int i = 0; i < m_GeneData.iTotalPercentGender; i++)
    {
        int iRow = i / 10;
        int iCol = i % 10;

        QRect rcFill = QRect(rcPainter.left() + iCol * 100, rcPainter.bottom() - iRow * 100 - 100, 100, 100);
        painter.drawImage(rcFill, imageChild);
        //painter.fillRect(rcFill, brush);
    }

    painter.setPen(QPen(QColor(191, 191, 191), 1));

    painter.drawRect(rcPainter);
    for(int i = 1; i < 10; i++)
    {
        painter.drawLine(rcPainter.left(), rcPainter.top() + i * 100, rcPainter.right(), rcPainter.top() + i * 100);
        painter.drawLine(rcPainter.left() + i * 100, rcPainter.top(), rcPainter.left() + i * 100, rcPainter.bottom());
    }

    m_mapImageData.insert(QLatin1String("userdata://percent_gender.png"), image);
}
