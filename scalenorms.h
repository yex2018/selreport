#ifndef SCALENORMS_H
#define SCALENORMS_H

#include <QList>

// Enumerations
enum SubDimensionType { sdtCommunication, sdtCooperation, sdtAssertion, sdtResponsibility, sdtEmpathy, sdtEngagement, sdtSelfControl,
                        sdtExternalizing, sdtBullying, sdtHyperactivityInattention, sdtInternalizing, sdtAutismSpectrum, sdtERI, sdtIRI,
                        sdtARI, sdtAttachment, sdtDisciplinePractice, sdtInvolvement, sdtParentingConfidence, sdtRelationalFrustration,
                        sdtComm, sdtSatisfactionWithSchool, sdtUnknown };

enum LevelType { ltWellBelowAverage = 0, ltBelowAverage, ltAverage, ltAboveAverage, ltWellAboveAverage, ltNormal, ltElevated, ltExtremely, ltUnknown };

// Structures
struct Raw_Standard_Percent
{
    unsigned char       uchRawScore;
    unsigned char       uchSSStandardScore;
    unsigned char       uchSSPercent;
};

struct Raw_Level
{
    SubDimensionType    eSubDimensionType;
    unsigned char       uchBelowAverageLower;
    unsigned char       uchBelowAverageUpper;
    unsigned char       uchAverageLower;
    unsigned char       uchAverageUpper;
    unsigned char       uchAboveAverageLower;
    unsigned char       uchAboveAverageUpper;
};

struct Con_Interval
{
    unsigned char       uchSS68;
    unsigned char       uchSS95;
};

struct Raw_T_Percent
{
    unsigned char       uchRawScore;
    unsigned char       uchTScore_3;
    unsigned char       uchPercent_3;
    unsigned char       uchTScore_4_5;
    unsigned char       uchPercent_4_5;
    unsigned char       uchTScore_6_7;
    unsigned char       uchPercent_6_7;
    unsigned char       uchTScore_8_11;
    unsigned char       uchPercent_8_11;
    unsigned char       uchTScore_12_14;
    unsigned char       uchPercent_12_14;
    unsigned char       uchTScore_15_18;
    unsigned char       uchPercent_15_18;
};

struct Raw_Risk_Level
{
    SubDimensionType    eSubDimensionType;
    unsigned char       uchNormalLower_3;
    unsigned char       uchNormalUpper_3;
    unsigned char       uchElevatedLower_3;
    unsigned char       uchElevatedUpper_3;
    unsigned char       uchExtremelyLower_3;
    unsigned char       uchExtremelyUpper_3;
    unsigned char       uchNormalLower_4_5;
    unsigned char       uchNormalUpper_4_5;
    unsigned char       uchElevatedLower_4_5;
    unsigned char       uchElevatedUpper_4_5;
    unsigned char       uchExtremelyLower_4_5;
    unsigned char       uchExtremelyUpper_4_5;
    unsigned char       uchNormalLower_6_7;
    unsigned char       uchNormalUpper_6_7;
    unsigned char       uchElevatedLower_6_7;
    unsigned char       uchElevatedUpper_6_7;
    unsigned char       uchExtremelyLower_6_7;
    unsigned char       uchExtremelyUpper_6_7;
    unsigned char       uchNormalLower_8_11;
    unsigned char       uchNormalUpper_8_11;
    unsigned char       uchElevatedLower_8_11;
    unsigned char       uchElevatedUpper_8_11;
    unsigned char       uchExtremelyLower_8_11;
    unsigned char       uchExtremelyUpper_8_11;
    unsigned char       uchNormalLower_12_14;
    unsigned char       uchNormalUpper_12_14;
    unsigned char       uchElevatedLower_12_14;
    unsigned char       uchElevatedUpper_12_14;
    unsigned char       uchExtremelyLower_12_14;
    unsigned char       uchExtremelyUpper_12_14;
    unsigned char       uchNormalLower_15_18;
    unsigned char       uchNormalUpper_15_18;
    unsigned char       uchElevatedLower_15_18;
    unsigned char       uchElevatedUpper_15_18;
    unsigned char       uchExtremelyLower_15_18;
    unsigned char       uchExtremelyUpper_15_18;
};

struct T_Con_Interval
{
    unsigned char       uch90_3;
    unsigned char       uch90_4_5;
    unsigned char       uch90_6_7;
    unsigned char       uch90_8_11;
    unsigned char       uch90_12_14;
    unsigned char       uch90_15_18;
};

// Operations
QString SubDimensionTypeToName(SubDimensionType eSubDimensionType);
QString LevelTypeToName(LevelType eLevelType);

void    C4_P_3_5C_AllData(QList<Raw_Standard_Percent> &lstAllResults);
bool    C4_P_3_5C_FromRawScore(unsigned char uchRawScore, Raw_Standard_Percent &result);
bool    C4_P_3_5C_FromSSStandardScore(unsigned char uchSSStandardScore, QList<Raw_Standard_Percent> &lstResults);
bool    C4_P_3_5F_FromRawScore(unsigned char uchRawScore, Raw_Standard_Percent &result);
bool    C4_P_3_5F_FromSSStandardScore(unsigned char uchSSStandardScore, QList<Raw_Standard_Percent> &lstResults);
bool    C4_P_3_5M_FromRawScore(unsigned char uchRawScore, Raw_Standard_Percent &result);
bool    C4_P_3_5M_FromSSStandardScore(unsigned char uchSSStandardScore, QList<Raw_Standard_Percent> &lstResults);

void    C5_P_5_12C_AllData(QList<Raw_Standard_Percent> &lstAllResults);
bool    C5_P_5_12C_FromRawScore(unsigned char uchRawScore, Raw_Standard_Percent &result);
bool    C5_P_5_12C_FromSSStandardScore(unsigned char uchSSStandardScore, QList<Raw_Standard_Percent> &lstResults);
bool    C5_P_5_12F_FromRawScore(unsigned char uchRawScore, Raw_Standard_Percent &result);
bool    C5_P_5_12F_FromSSStandardScore(unsigned char uchSSStandardScore, QList<Raw_Standard_Percent> &lstResults);
bool    C5_P_5_12M_FromRawScore(unsigned char uchRawScore, Raw_Standard_Percent &result);
bool    C5_P_5_12M_FromSSStandardScore(unsigned char uchSSStandardScore, QList<Raw_Standard_Percent> &lstResults);

bool    D2_P_3_5F_FromSubDimensionType(SubDimensionType eSubDimensionType, Raw_Level &result);
bool    D2_P_3_5M_FromSubDimensionType(SubDimensionType eSubDimensionType, Raw_Level &result);
bool    D2_P_3_5C_FromSubDimensionType(SubDimensionType eSubDimensionType, Raw_Level &result);

bool    D3_P_5_12F_FromSubDimensionType(SubDimensionType eSubDimensionType, Raw_Level &result);
bool    D3_P_5_12M_FromSubDimensionType(SubDimensionType eSubDimensionType, Raw_Level &result);
bool    D3_P_5_12C_FromSubDimensionType(SubDimensionType eSubDimensionType, Raw_Level &result);

bool    D1_P_3_5F_From(Con_Interval &result);
bool    D1_P_3_5M_From(Con_Interval &result);
bool    D1_P_3_5C_From(Con_Interval &result);

bool    D1_P_5_12F_From(Con_Interval &result);
bool    D1_P_5_12M_From(Con_Interval &result);
bool    D1_P_5_12C_From(Con_Interval &result);

void    A4_P_C_AllData(QList<Raw_T_Percent> &lstAllResults);
bool    A4_P_C_FromRawScore(unsigned char uchRawScore, Raw_T_Percent &result);
bool    A4_P_C_TConInterval(T_Con_Interval &result);

void    A5_P_F_AllData(QList<Raw_T_Percent> &lstAllResults);
bool    A5_P_F_FromRawScore(unsigned char uchRawScore, Raw_T_Percent &result);
bool    A5_P_F_TConInterval(T_Con_Interval &result);

void    A6_P_M_AllData(QList<Raw_T_Percent> &lstAllResults);
bool    A6_P_M_FromRawScore(unsigned char uchRawScore, Raw_T_Percent &result);
bool    A6_P_M_TConInterval(T_Con_Interval &result);

bool    Table_2_4_P_FromSubDimensionType(SubDimensionType eSubDimensionType, Raw_Risk_Level &result);

#endif // SCALENORMS_H
