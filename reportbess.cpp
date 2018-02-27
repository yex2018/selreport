#include <QGuiApplication>
#include <QPainter>
#include <QPaintEngine>
#include <QSettings>
#include <QJsonObject>
#include <QJsonArray>

#include "mysqlwrapper/mysqldatabase.h"
#include "mysqlwrapper/mysqlquery.h"

#include "reportbess.h"
#include "variabledefine.h"
#include "reportpdfwriter.h"

ReportBess::ReportBess(int iUserEvaluationId, const QString &strReportFileName, bool bPP) :
    Report(iUserEvaluationId, strReportFileName), m_bPP(bPP)
{
    m_iRawScore = 0;
    m_iTScoreCombined = 0;
    m_iTScore90LowerCombined = 0;
    m_iTScore90UpperCombined = 0;
    m_iPencentCombined = 0;

    m_iTScoreGender = 0;
    m_iTScore90LowerGender = 0;
    m_iTScore90UpperGender = 0;
    m_iPencentGender = 0;

    m_iFIndex = 0;

    for(int i = sdtERI; i <= sdtARI; i++)
    {
        SubDimensionType eSubDimensionType = SubDimensionType(i);
        m_mapSubDimensionTypeToRawScore[eSubDimensionType] = 0;
        m_mapSubDimensionTypeToLevelType[eSubDimensionType] = ltUnknown;
    }
}

ReportBess::~ReportBess()
{
}

QString ReportBess::GetHtmlTempalte() const
{
    return QLatin1String(":/selreport/html/rpt_bess.html");
}

bool ReportBess::BuildReportData()
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
        {
            int iQuestionIndex = mapQuestionIdToIndex.value(iQuestionId);
            mapQuestionIndexToScore.insert(iQuestionIndex, AnswerToScore(iQuestionIndex, strAnswer));
        }
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
            m_mapSubDimensionTypeToRawScore[eSubDimensionType] += iQuestionScore;

        m_iRawScore += iQuestionScore;

        if(IsFIndex(iQuestionIndex))
        {
            if(iQuestionScore == 3)
                m_iFIndex++;
        }
    }
    m_mapSubDimensionTypeToRawScore[sdtARI] = 27 - m_mapSubDimensionTypeToRawScore[sdtARI];

    // Calculate T score

    Raw_T_Percent result;
    if(A4_P_C_FromRawScore(m_iRawScore, result))
    {
        T_Con_Interval tci;
        A4_P_C_TConInterval(tci);

        if(m_ChildInfo.iAge <= 3)
        {
            m_iTScoreCombined = result.uchTScore_3;
            m_iPencentCombined = result.uchPercent_3;

            m_iTScore90LowerCombined = m_iTScoreCombined - tci.uch90_3;
            m_iTScore90UpperCombined = m_iTScoreCombined + tci.uch90_3;
        }
        else if(m_ChildInfo.iAge >= 4 && m_ChildInfo.iAge <= 5)
        {
            m_iTScoreCombined = result.uchTScore_4_5;
            m_iPencentCombined = result.uchPercent_4_5;

            m_iTScore90LowerCombined = m_iTScoreCombined - tci.uch90_4_5;
            m_iTScore90UpperCombined = m_iTScoreCombined + tci.uch90_4_5;
        }
        else if(m_ChildInfo.iAge >= 6 && m_ChildInfo.iAge <= 7)
        {
            m_iTScoreCombined = result.uchTScore_6_7;
            m_iPencentCombined = result.uchPercent_6_7;

            m_iTScore90LowerCombined = m_iTScoreCombined - tci.uch90_6_7;
            m_iTScore90UpperCombined = m_iTScoreCombined + tci.uch90_6_7;
        }
        else if(m_ChildInfo.iAge >= 8 && m_ChildInfo.iAge <= 11)
        {
            m_iTScoreCombined = result.uchTScore_8_11;
            m_iPencentCombined = result.uchPercent_8_11;

            m_iTScore90LowerCombined = m_iTScoreCombined - tci.uch90_8_11;
            m_iTScore90UpperCombined = m_iTScoreCombined + tci.uch90_8_11;
        }
        else if(m_ChildInfo.iAge >= 12 && m_ChildInfo.iAge <= 14)
        {
            m_iTScoreCombined = result.uchTScore_12_14;
            m_iPencentCombined = result.uchPercent_12_14;

            m_iTScore90LowerCombined = m_iTScoreCombined - tci.uch90_12_14;
            m_iTScore90UpperCombined = m_iTScoreCombined + tci.uch90_12_14;
        }
        else if(m_ChildInfo.iAge >= 15)
        {
            m_iTScoreCombined = result.uchTScore_15_18;
            m_iPencentCombined = result.uchPercent_15_18;

            m_iTScore90LowerCombined = m_iTScoreCombined - tci.uch90_15_18;
            m_iTScore90UpperCombined = m_iTScoreCombined + tci.uch90_15_18;
        }
    }

    if(m_ChildInfo.eGenderType == gtMale ? A6_P_M_FromRawScore(m_iRawScore, result) : A5_P_F_FromRawScore(m_iRawScore, result))
    {
        T_Con_Interval tci;
        m_ChildInfo.eGenderType == gtMale ? A6_P_M_TConInterval(tci) : A5_P_F_TConInterval(tci);

        if(m_ChildInfo.iAge <= 3)
        {
            m_iTScoreGender = result.uchTScore_3;
            m_iPencentGender = result.uchPercent_3;

            m_iTScore90LowerGender = m_iTScoreGender - tci.uch90_3;
            m_iTScore90UpperGender = m_iTScoreGender + tci.uch90_3;
        }
        else if(m_ChildInfo.iAge >= 4 && m_ChildInfo.iAge <= 5)
        {
            m_iTScoreGender = result.uchTScore_4_5;
            m_iPencentGender = result.uchPercent_4_5;

            m_iTScore90LowerGender = m_iTScoreGender - tci.uch90_4_5;
            m_iTScore90UpperGender = m_iTScoreGender + tci.uch90_4_5;
        }
        else if(m_ChildInfo.iAge >= 6 && m_ChildInfo.iAge <= 7)
        {
            m_iTScoreGender = result.uchTScore_6_7;
            m_iPencentGender = result.uchPercent_6_7;

            m_iTScore90LowerGender = m_iTScoreGender - tci.uch90_6_7;
            m_iTScore90UpperGender = m_iTScoreGender + tci.uch90_6_7;
        }
        else if(m_ChildInfo.iAge >= 8 && m_ChildInfo.iAge <= 11)
        {
            m_iTScoreGender = result.uchTScore_8_11;
            m_iPencentGender = result.uchPercent_8_11;

            m_iTScore90LowerGender = m_iTScoreGender - tci.uch90_8_11;
            m_iTScore90UpperGender = m_iTScoreGender + tci.uch90_8_11;
        }
        else if(m_ChildInfo.iAge >= 12 && m_ChildInfo.iAge <= 14)
        {
            m_iTScoreGender = result.uchTScore_12_14;
            m_iPencentGender = result.uchPercent_12_14;

            m_iTScore90LowerGender = m_iTScoreGender - tci.uch90_12_14;
            m_iTScore90UpperGender = m_iTScoreGender + tci.uch90_12_14;
        }
        else if(m_ChildInfo.iAge >= 15)
        {
            m_iTScoreGender = result.uchTScore_15_18;
            m_iPencentGender = result.uchPercent_15_18;

            m_iTScore90LowerGender = m_iTScoreGender - tci.uch90_15_18;
            m_iTScore90UpperGender = m_iTScoreGender + tci.uch90_15_18;
        }
    }

    m_eLevelTypeCombined = GetTotalLevelType();
    m_mapSubDimensionTypeToLevelType[sdtERI] = GetSubDimensionLevelType(sdtERI);
    m_mapSubDimensionTypeToLevelType[sdtIRI] = GetSubDimensionLevelType(sdtIRI);
    m_mapSubDimensionTypeToLevelType[sdtARI] = GetSubDimensionLevelType(sdtARI);

    QString strConclustionNormal = m_pSettings->value("bess_total_level_conclusion/ltNormal").toString();
    QString strConclustionElevated = m_pSettings->value("bess_total_level_conclusion/ltElevated").toString();
    QString strConclustionExtremely = m_pSettings->value("bess_total_level_conclusion/ltExtremely").toString();
    foreach(const QString &strKey, m_mapVariableData.keys())
    {
        strConclustionNormal.replace(strKey, m_mapVariableData.value(strKey));
        strConclustionElevated.replace(strKey, m_mapVariableData.value(strKey));
        strConclustionExtremely.replace(strKey, m_mapVariableData.value(strKey));
    }
    QMap<LevelType, QString> mapLevelTypeToConclusion;
    mapLevelTypeToConclusion.insert(ltNormal, strConclustionNormal);
    mapLevelTypeToConclusion.insert(ltElevated, strConclustionElevated);
    mapLevelTypeToConclusion.insert(ltExtremely, strConclustionExtremely);

    QMap<LevelType, QString> mapLevelTypeToConclusion1;
    mapLevelTypeToConclusion1.insert(ltNormal, m_pSettings->value("bess_total_level_conclusion1/ltNormal").toString());
    mapLevelTypeToConclusion1.insert(ltElevated, m_pSettings->value("bess_total_level_conclusion1/ltElevated").toString());
    mapLevelTypeToConclusion1.insert(ltExtremely, m_pSettings->value("bess_total_level_conclusion1/ltExtremely").toString());

    QList<SubDimensionType> lstSubDimensionTypeConclusion2 = m_mapSubDimensionTypeToLevelType.keys(ltElevated) + m_mapSubDimensionTypeToLevelType.keys(ltExtremely);
    QString strConclusion2;
    if(!lstSubDimensionTypeConclusion2.isEmpty())
    {
        strConclusion2 = m_pSettings->value("bess_total_level_conclusion2/conclusion2").toString();
        foreach(SubDimensionType eSubDimensionType, lstSubDimensionTypeConclusion2)
        {
            if(eSubDimensionType == sdtERI)
                strConclusion2 += m_pSettings->value("bess_total_level_conclusion2/sdtERI").toString();
            else if(eSubDimensionType == sdtIRI)
                strConclusion2 += m_pSettings->value("bess_total_level_conclusion2/sdtIRI").toString();
            else if(eSubDimensionType == sdtARI)
                strConclusion2 += m_pSettings->value("bess_total_level_conclusion2/sdtARI").toString();
        }
    }

    QString strTotalLevelColor = "#000000";
    if(m_eLevelTypeCombined == ltNormal)
        strTotalLevelColor = "#92d050";
    else if(m_eLevelTypeCombined == ltElevated)
        strTotalLevelColor = "#ffc000";
    else if(m_eLevelTypeCombined == ltExtremely)
        strTotalLevelColor = "#ff0000";

    m_mapVariableData.insert(g_strTotalLevelColor, strTotalLevelColor);
    m_mapVariableData.insert(g_strTotalLevel, LevelTypeToName(m_eLevelTypeCombined));
    m_mapVariableData.insert(g_strTotalLevelConclusion, mapLevelTypeToConclusion.value(m_eLevelTypeCombined));
    m_mapVariableData.insert(g_strTotalLevelConclusion1, mapLevelTypeToConclusion1.value(m_eLevelTypeCombined));
    m_mapVariableData.insert(g_strTotalLevelConclusion2, strConclusion2);

    m_mapVariableData.insert(g_strTotalTScoreCombined, QString::number(m_iTScoreCombined));
    m_mapVariableData.insert(g_strTotalTScore90LowerCombined, QString::number(m_iTScore90LowerCombined));
    m_mapVariableData.insert(g_strTotalTScore90UpperCombined, QString::number(m_iTScore90UpperCombined));
    m_mapVariableData.insert(g_strTotalPercentCombined, QString::number(m_iPencentCombined));

    m_mapVariableData.insert(g_strTotalTScoreGender, QString::number(m_iTScoreGender));
    m_mapVariableData.insert(g_strTotalTScore90LowerGender, QString::number(m_iTScore90LowerGender));
    m_mapVariableData.insert(g_strTotalTScore90UpperGender, QString::number(m_iTScore90UpperGender));
    m_mapVariableData.insert(g_strTotalPercentGender, QString::number(m_iPencentGender));

    m_mapVariableData.insert(g_strFIndex, QString::number(m_iFIndex));
    m_mapVariableData.insert(g_strVersionAge,(m_bPP ? tr("Preschool Verison, 3-6 Yeas old") : tr("School Verison, 6-18 Yeas old")));

    DrawImageForBasicInfo();
    DrawImageTScoreRange(true);
    DrawImageTScoreRange(false);
    DrawPercentCombined();
    DrawPercentGender();
    DrawImageSubDimenTable();

    return true;
}

QJsonObject ReportBess::BuildJsonResult()
{
    return QJsonObject();
}

int ReportBess::AnswerToScore(int iQuestionIndex, const QString &strAnswer)
{
    static QList<int> lstNegativeQuestionIndex_PP = QList<int>() << 1 << 4 << 5 << 10 << 13 << 18 << 22 << 28 << 29;
    static QList<int> lstNegativeQuestionIndex_PCA = QList<int>() << 1 << 5 << 9 << 11 << 17 << 20 << 22 << 25 << 28;

    static QMap<QString, int> mapAnswerToScore;
    if(mapAnswerToScore.isEmpty())
    {
        mapAnswerToScore.insert("A", 0);
        mapAnswerToScore.insert("B", 1);
        mapAnswerToScore.insert("C", 2);
        mapAnswerToScore.insert("D", 3);
    }

    static QMap<QString, int> mapNegativeAnswerToScore;
    if(mapNegativeAnswerToScore.isEmpty())
    {
        mapNegativeAnswerToScore.insert("A", 3);
        mapNegativeAnswerToScore.insert("B", 2);
        mapNegativeAnswerToScore.insert("C", 1);
        mapNegativeAnswerToScore.insert("D", 0);
    }

    QList<int> &lstNegativeQuestionIndex = (m_bPP ? lstNegativeQuestionIndex_PP : lstNegativeQuestionIndex_PCA);
    return (lstNegativeQuestionIndex.contains(iQuestionIndex) ? mapNegativeAnswerToScore.value(strAnswer, 0) : mapAnswerToScore.value(strAnswer, 0));
}

bool ReportBess::IsFIndex(int iQuestionIndex)
{
    static QList<int> lstF_PP = QList<int>() << 3 << 7 << 8 << 11 << 14 << 16 << 19 << 23 << 25 << 28;
    static QList<int> lstF_PCA = QList<int>() << 1 << 8 << 12 << 15 << 16 << 18 << 24 << 26 << 27 << 28;

    return (m_bPP ? lstF_PP.contains(iQuestionIndex) : lstF_PCA.contains(iQuestionIndex));
}

SubDimensionType ReportBess::GetSubDimensionTypeFromQestionIndex(int iQuestionIndex) const
{
    static QMap<SubDimensionType, QList<int> > s_mapSubDimensionTypeToQuestionIndex_PP;
    if(s_mapSubDimensionTypeToQuestionIndex_PP.isEmpty())
    {
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(sdtERI, QList<int>() << 7 << 9 << 11 << 12 << 15 << 17 << 21 << 24 << 26);
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(sdtIRI, QList<int>() << 3 << 6 << 8 << 14 << 16 << 19 << 23 << 25 << 27);
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(sdtARI, QList<int>() << 1 << 4 << 5 << 10 << 13 << 18 << 22 << 28 << 29);
    }

    static QMap<SubDimensionType, QList<int> > s_mapSubDimensionTypeToQuestionIndex_PCA;
    if(s_mapSubDimensionTypeToQuestionIndex_PCA.isEmpty())
    {
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(sdtERI, QList<int>() << 4 << 7 << 12 << 14 << 16 << 18 << 24 << 26 << 29);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(sdtIRI, QList<int>() << 2 << 6 << 8 << 10 << 13 << 15 << 17 << 19 << 23 << 27);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(sdtARI, QList<int>() << 1 << 5 << 9 << 11 << 20 << 22 << 25 << 28);
    }

    QMap<SubDimensionType, QList<int> > &mapSubDimensionTypeToQuestionIndex = (m_bPP ? s_mapSubDimensionTypeToQuestionIndex_PP : s_mapSubDimensionTypeToQuestionIndex_PCA);
    foreach(SubDimensionType eSubDimensionType, mapSubDimensionTypeToQuestionIndex.keys())
    {
        if(mapSubDimensionTypeToQuestionIndex.value(eSubDimensionType).contains(iQuestionIndex))
            return eSubDimensionType;
    }

    return sdtUnknown;
}

LevelType ReportBess::GetTotalLevelType()
{
    LevelType eLevelType = ltUnknown;

    if(m_iTScoreCombined <= 60)
        eLevelType = ltNormal;
    else if(m_iTScoreCombined >= 61 && m_iTScoreCombined <= 70)
        eLevelType = ltElevated;
    else
        eLevelType = ltExtremely;

    return eLevelType;
}

LevelType ReportBess::GetSubDimensionLevelType(SubDimensionType eSubDimensionType)
{
    LevelType eLevelType = ltUnknown;

    Raw_Risk_Level result;
    if(Table_2_4_P_FromSubDimensionType(eSubDimensionType, result))
    {
        int iSubDimensionScore = m_mapSubDimensionTypeToRawScore.value(eSubDimensionType, 0);

        if(m_ChildInfo.iAge <= 3)
        {
            if(iSubDimensionScore >= result.uchNormalLower_3 && iSubDimensionScore <= result.uchNormalUpper_3)
                eLevelType = ltNormal;
            else if(iSubDimensionScore >= result.uchElevatedLower_3 && iSubDimensionScore <= result.uchElevatedUpper_3)
                eLevelType = ltElevated;
            else if(iSubDimensionScore >= result.uchExtremelyLower_3 && iSubDimensionScore <= result.uchExtremelyUpper_3)
                eLevelType = ltExtremely;
        }
        else if(m_ChildInfo.iAge >= 4 && m_ChildInfo.iAge <= 5)
        {
            if(iSubDimensionScore >= result.uchNormalLower_4_5 && iSubDimensionScore <= result.uchNormalUpper_4_5)
                eLevelType = ltNormal;
            else if(iSubDimensionScore >= result.uchElevatedLower_4_5 && iSubDimensionScore <= result.uchElevatedUpper_4_5)
                eLevelType = ltElevated;
            else if(iSubDimensionScore >= result.uchExtremelyLower_4_5 && iSubDimensionScore <= result.uchExtremelyUpper_4_5)
                eLevelType = ltExtremely;
        }
        else if(m_ChildInfo.iAge >= 6 && m_ChildInfo.iAge <= 7)
        {
            if(iSubDimensionScore >= result.uchNormalLower_6_7 && iSubDimensionScore <= result.uchNormalUpper_6_7)
                eLevelType = ltNormal;
            else if(iSubDimensionScore >= result.uchElevatedLower_6_7 && iSubDimensionScore <= result.uchElevatedUpper_6_7)
                eLevelType = ltElevated;
            else if(iSubDimensionScore >= result.uchExtremelyLower_6_7 && iSubDimensionScore <= result.uchExtremelyUpper_6_7)
                eLevelType = ltExtremely;
        }
        else if(m_ChildInfo.iAge >= 8 && m_ChildInfo.iAge <= 11)
        {
            if(iSubDimensionScore >= result.uchNormalLower_8_11 && iSubDimensionScore <= result.uchNormalUpper_8_11)
                eLevelType = ltNormal;
            else if(iSubDimensionScore >= result.uchElevatedLower_8_11 && iSubDimensionScore <= result.uchElevatedUpper_8_11)
                eLevelType = ltElevated;
            else if(iSubDimensionScore >= result.uchExtremelyLower_8_11 && iSubDimensionScore <= result.uchExtremelyUpper_8_11)
                eLevelType = ltExtremely;
        }
        else if(m_ChildInfo.iAge >= 12 && m_ChildInfo.iAge <= 14)
        {
            if(iSubDimensionScore >= result.uchNormalLower_12_14 && iSubDimensionScore <= result.uchNormalUpper_12_14)
                eLevelType = ltNormal;
            else if(iSubDimensionScore >= result.uchElevatedLower_12_14 && iSubDimensionScore <= result.uchElevatedUpper_12_14)
                eLevelType = ltElevated;
            else if(iSubDimensionScore >= result.uchExtremelyLower_12_14 && iSubDimensionScore <= result.uchExtremelyUpper_12_14)
                eLevelType = ltExtremely;
        }
        else if(m_ChildInfo.iAge >= 15)
        {
            if(iSubDimensionScore >= result.uchNormalLower_15_18 && iSubDimensionScore <= result.uchNormalUpper_15_18)
                eLevelType = ltNormal;
            else if(iSubDimensionScore >= result.uchElevatedLower_15_18 && iSubDimensionScore <= result.uchElevatedUpper_15_18)
                eLevelType = ltElevated;
            else if(iSubDimensionScore >= result.uchExtremelyLower_15_18 && iSubDimensionScore <= result.uchExtremelyUpper_15_18)
                eLevelType = ltExtremely;
        }
    }

    return eLevelType;
}

void ReportBess::DrawImageTScoreRange(bool bCombined)
{
    const QRect rcImage(0, 0, 2100, 400);
    const QRect rcPainter(50, 50, 2000, 300);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    QRect rcNormal = QRect(rcPainter.left(), rcPainter.bottom() - 190, 1200, 190);
    QRect rcElevated = QRect(rcPainter.left() + 1200, rcPainter.bottom() - 190, 200, 190);
    QRect rcExtremely = QRect(rcPainter.left() + 1400, rcPainter.bottom() - 190, 600, 190);

    painter.setPen(QPen(Qt::black, 6));

    painter.setBrush(QColor(146, 208, 80));
    painter.drawRect(rcNormal);

    painter.setBrush(QColor(255, 192, 0));
    painter.drawRect(rcElevated);

    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(rcExtremely);

    QFont font = qApp->font();
    font.setPointSize(35);
    painter.setFont(font);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignCenter);
    TextOption.setWrapMode(QTextOption::WordWrap);

    painter.drawText(rcNormal, LevelTypeToName(ltNormal), TextOption);
    painter.drawText(rcElevated, LevelTypeToName(ltElevated), TextOption);
    painter.drawText(rcExtremely, LevelTypeToName(ltExtremely), TextOption);

    int iXTScore = rcPainter.left() + (bCombined ? m_iTScoreCombined : m_iTScoreGender) * 20;
    int iXTScore68Lower = rcPainter.left() + (bCombined ? m_iTScore90LowerCombined : m_iTScore90LowerGender) * 20;
    int iXTScore68Upper = rcPainter.left() + (bCombined ? m_iTScore90UpperCombined : m_iTScore90UpperGender) * 20;

    QRect r90 = QRect(iXTScore68Lower, rcPainter.top(), iXTScore68Upper - iXTScore68Lower, 100);
    painter.setPen(QPen(Qt::black));
    if(bCombined)
        painter.setBrush(QColor(165, 165, 165));
    else
        painter.setBrush(m_ChildInfo.eGenderType == gtMale ? QColor(133, 204, 232) : QColor(241, 170, 202));
    painter.drawRect(r90);

    painter.setBrush(Qt::black);
    QRect rcPoint(0, 0, 30, 30);
    rcPoint.moveCenter(QPoint(iXTScore, rcPainter.top() + 50));
    painter.drawEllipse(rcPoint);

    m_mapImageData.insert(bCombined ? QLatin1String("userdata://tscore_range_combined.png") : QLatin1String("userdata://tscore_range_gender.png"), image);
}

void ReportBess::DrawPercentCombined()
{
    const QRect rcImage(0, 0, 1010, 1010);
    const QRect rcPainter(5, 5, 1000, 1000);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    QImage imageGirl(":/selreport/images/girl.png");
    QImage imageBoy(":/selreport/images/boy.png");

    for(int i = 0; i < m_iPencentCombined; i++)
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

void ReportBess::DrawPercentGender()
{
    const QRect rcImage(0, 0, 1010, 1010);
    const QRect rcPainter(5, 5, 1000, 1000);

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    //QBrush brush(m_ChildInfo.bChildGender ? QColor(146, 166, 208) : QColor(249, 202, 201));

    QImage imageChild(m_ChildInfo.eGenderType == gtMale ? ":/selreport/images/boy.png" : ":/selreport/images/girl.png");
    for(int i = 0; i < m_iPencentGender; i++)
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

void ReportBess::DrawImageSubDimenTable()
{
    int iTableWidth = m_pSettings->value("bess_report_table/table_width").toString().toInt();
    int iTableHeight = m_pSettings->value("bess_report_table/table_height").toString().toInt();

    QList<int> lstRowHeight;
    foreach(const QString &strRowHeight, m_pSettings->value("bess_report_table/row_height").toStringList())
        lstRowHeight.append(strRowHeight.toInt());

    QList<int> lstColWidth;
    foreach(const QString &strColWidth, m_pSettings->value("bess_report_table/col_width").toStringList())
        lstColWidth.append(strColWidth.toInt());

    QStringList lstHeader = m_pSettings->value("bess_report_table/header").toStringList();
    QStringList lstCol1 = m_pSettings->value("bess_report_table/col1").toStringList();
    QStringList lstCol2 = m_pSettings->value("bess_report_table/col2").toStringList();
    QStringList lstCol3 = m_pSettings->value("bess_report_table/col3").toStringList();

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

    iY = rcPainter.top() + lstRowHeight.at(0);
    for(int i = 0; i < lstCol3.size(); i++)
    {
        QRect rcCol3 = QRect(rcPainter.left() + lstColWidth.at(0) + lstColWidth.at(1) + 20, iY, lstColWidth.at(2) - 20, lstRowHeight.at(i + 1));
        painter.drawText(rcCol3, lstCol3.at(i), TextOption);
        iY += lstRowHeight.at(i + 1);
    }

    TextOption.setAlignment(Qt::AlignCenter);

    iY = rcPainter.top() + lstRowHeight.at(0);
    for(int i = sdtERI; i <= sdtARI; i++)
    {
        QString strText;
        QColor crBack;

        SubDimensionType eSubDimensionType = SubDimensionType(i);
        LevelType eLevelType = m_mapSubDimensionTypeToLevelType.value(eSubDimensionType);

        if(eLevelType == ltNormal)
        {
            strText = LevelTypeToName(ltNormal);
            crBack = QColor(146, 208, 80);
        }
        else if(eLevelType == ltElevated)
        {
            strText = LevelTypeToName(ltElevated);
            crBack = QColor(255, 192, 0);
        }
        else if(eLevelType == ltExtremely)
        {
            strText = LevelTypeToName(ltExtremely);
            crBack = QColor(255, 0, 0);
        }

        QRect rcCol4 = QRect(rcPainter.left() + lstColWidth.at(0) + lstColWidth.at(1) + lstColWidth.at(2), iY, lstColWidth.at(3), lstRowHeight.at(i - int(sdtERI) + 1));
        painter.fillRect(rcCol4, QBrush(crBack));
        painter.drawText(rcCol4, strText, TextOption);
        iY += lstRowHeight.at(i - int(sdtERI) + 1);
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
