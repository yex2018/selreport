#include <QGuiApplication>
#include <QPainter>
#include <QPaintEngine>
#include <QSettings>
#include <QJsonObject>
#include <QJsonArray>

#include "mysqlwrapper/mysqldatabase.h"
#include "mysqlwrapper/mysqlquery.h"

#include "reportprq.h"
#include "variabledefine.h"
#include "reportpdfwriter.h"

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A1_2_5F[] =
{
    {45,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {43,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {42,65,99,0,0,0,0,0,0,0,0,0,0,100,99},
    {41,63,95,0,0,0,0,0,0,0,0,0,0,100,99},
    {40,61,89,0,0,0,0,0,0,0,0,0,0,99,99},
    {39,59,82,0,0,0,0,0,0,0,0,0,0,98,99},
    {38,58,74,0,0,0,0,0,0,0,0,0,0,96,99},
    {37,56,66,0,0,0,0,0,0,0,0,0,0,94,99},
    {36,54,58,0,0,0,0,0,0,67,98,0,0,92,99},
    {35,52,51,0,0,0,0,0,0,65,95,0,0,90,99},
    {34,50,44,0,0,0,0,0,0,63,91,0,0,89,99},
    {33,48,38,0,0,0,0,0,0,61,85,0,0,87,99},
    {32,46,32,0,0,0,0,0,0,58,78,0,0,85,99},
    {31,45,27,0,0,0,0,0,0,56,70,0,0,83,99},
    {30,43,22,0,0,0,0,0,0,54,62,0,0,81,99},
    {29,41,18,0,0,0,0,0,0,52,54,0,0,80,99},
    {28,39,15,0,0,0,0,0,0,50,46,0,0,78,99},
    {27,37,12,0,0,0,0,65,95,48,39,0,0,76,99},
    {26,35,9,0,0,0,0,63,91,46,32,0,0,74,99},
    {25,33,7,0,0,0,0,61,86,44,25,0,0,72,98},
    {24,31,6,0,0,64,95,59,80,41,20,0,0,71,97},
    {23,30,4,0,0,62,90,58,74,39,15,0,0,69,96},
    {22,28,3,0,0,60,83,56,68,37,12,0,0,67,95},
    {21,26,2,0,0,57,74,54,61,35,8,0,0,65,93},
    {20,24,1,0,0,55,65,52,54,33,6,0,0,63,90},
    {19,22,1,0,0,53,56,50,48,31,4,0,0,62,87},
    {18,20,1,0,0,50,47,48,41,29,3,0,0,60,84},
    {17,18,1,0,0,48,38,46,35,27,2,0,0,58,79},
    {16,17,1,0,0,46,31,44,29,25,1,0,0,56,74},
    {15,15,1,0,0,43,24,42,24,22,1,0,0,54,68},
    {14,13,1,0,0,41,19,40,19,20,1,0,0,53,62},
    {13,11,1,0,0,39,14,38,14,18,1,0,0,51,54},
    {12,10,1,0,0,36,10,36,10,16,1,0,0,49,47},
    {11,10,1,0,0,34,7,34,7,14,1,0,0,47,40},
    {10,10,1,0,0,31,5,33,4,12,1,0,0,45,33},
    {9,10,1,0,0,29,3,31,2,10,1,0,0,44,26},
    {8,10,1,0,0,27,2,29,1,10,1,0,0,42,20},
    {7,10,1,0,0,24,1,27,1,10,1,0,0,40,15},
    {6,10,1,0,0,22,1,25,1,10,1,0,0,38,11},
    {5,10,1,0,0,20,1,23,1,10,1,0,0,36,8},
    {4,10,1,0,0,17,1,21,1,10,1,0,0,34,5},
    {3,10,1,0,0,15,1,19,1,10,1,0,0,33,3},
    {2,10,1,0,0,13,1,17,1,10,1,0,0,31,2},
    {1,10,1,0,0,10,1,15,1,10,1,0,0,29,1},
    {0,10,1,0,0,10,1,13,1,10,1,0,0,27,1},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A2_2_5M[] =
{
    {45,0,0,0,0,0,0,0,0,0,0,0,0,99,99},
    {44,0,0,0,0,0,0,0,0,0,0,0,0,97,99},
    {43,0,0,0,0,0,0,0,0,0,0,0,0,96,99},
    {42,69,99,0,0,0,0,0,0,0,0,0,0,94,99},
    {41,67,98,0,0,0,0,0,0,0,0,0,0,93,99},
    {40,65,96,0,0,0,0,0,0,0,0,0,0,91,99},
    {39,64,93,0,0,0,0,0,0,0,0,0,0,89,99},
    {38,62,90,0,0,0,0,0,0,0,0,0,0,88,99},
    {37,60,85,0,0,0,0,0,0,0,0,0,0,86,99},
    {36,59,80,0,0,0,0,0,0,66,98,0,0,85,99},
    {35,57,74,0,0,0,0,0,0,64,95,0,0,83,99},
    {34,55,67,0,0,0,0,0,0,62,91,0,0,82,99},
    {33,54,60,0,0,0,0,0,0,60,85,0,0,80,99},
    {32,52,54,0,0,0,0,0,0,58,78,0,0,79,98},
    {31,50,47,0,0,0,0,0,0,56,71,0,0,77,98},
    {30,49,41,0,0,0,0,0,0,54,63,0,0,76,98},
    {29,47,35,0,0,0,0,0,0,52,55,0,0,74,97},
    {28,45,29,0,0,0,0,0,0,50,47,0,0,73,97},
    {27,44,25,0,0,0,0,72,99,48,40,0,0,71,96},
    {26,42,20,0,0,0,0,70,98,46,33,0,0,70,95},
    {25,40,17,0,0,0,0,68,97,45,27,0,0,68,95},
    {24,39,13,0,0,64,94,66,95,43,22,0,0,67,94},
    {23,37,11,0,0,62,89,64,93,41,17,0,0,65,92},
    {22,35,8,0,0,59,82,62,90,39,14,0,0,64,91},
    {21,34,7,0,0,57,73,60,85,37,11,0,0,62,89},
    {20,32,5,0,0,55,64,58,80,35,8,0,0,61,87},
    {19,30,4,0,0,52,55,56,74,33,6,0,0,59,85},
    {18,29,3,0,0,50,46,55,67,31,4,0,0,57,82},
    {17,27,2,0,0,48,38,53,59,29,3,0,0,56,78},
    {16,25,2,0,0,45,31,51,51,27,2,0,0,54,74},
    {15,24,1,0,0,43,24,49,43,25,1,0,0,53,69},
    {14,22,1,0,0,41,18,47,36,23,1,0,0,51,64},
    {13,21,1,0,0,38,14,45,29,21,1,0,0,50,58},
    {12,19,1,0,0,36,10,43,23,19,1,0,0,48,51},
    {11,17,1,0,0,34,7,41,18,17,1,0,0,47,43},
    {10,16,1,0,0,32,5,39,14,15,1,0,0,45,36},
    {9,14,1,0,0,29,3,37,10,13,1,0,0,44,28},
    {8,12,1,0,0,27,2,35,7,11,1,0,0,42,20},
    {7,11,1,0,0,25,1,33,5,10,1,0,0,41,14},
    {6,10,1,0,0,22,1,31,4,10,1,0,0,39,8},
    {5,10,1,0,0,20,1,29,2,10,1,0,0,38,5},
    {4,10,1,0,0,18,1,27,2,10,1,0,0,36,2},
    {3,10,1,0,0,15,1,25,1,10,1,0,0,35,1},
    {2,10,1,0,0,13,1,23,1,10,1,0,0,33,1},
    {1,10,1,0,0,11,1,22,1,10,1,0,0,32,1},
    {0,10,1,0,0,10,1,20,1,10,1,0,0,30,1},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A3_6_9F[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {45,64,95,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,63,90,0,0,0,0,0,0,0,0,0,0,100,99},
    {43,61,84,0,0,0,0,0,0,0,0,0,0,100,99},
    {42,59,78,0,0,0,0,0,0,0,0,0,0,100,99},
    {41,57,71,0,0,0,0,0,0,0,0,0,0,100,99},
    {40,55,65,0,0,0,0,0,0,0,0,0,0,98,99},
    {39,53,58,60,90,0,0,0,0,0,0,0,0,97,99},
    {38,52,52,59,79,0,0,0,0,0,0,0,0,95,99},
    {37,50,46,57,69,0,0,0,0,0,0,0,0,93,99},
    {36,48,40,55,59,0,0,0,0,66,98,0,0,92,99},
    {35,46,35,53,51,0,0,0,0,64,93,0,0,90,99},
    {34,44,30,51,44,0,0,0,0,62,87,0,0,88,99},
    {33,43,25,49,38,0,0,0,0,60,81,0,0,87,99},
    {32,41,20,47,32,0,0,0,0,58,73,0,0,85,99},
    {31,39,16,45,27,0,0,0,0,56,66,0,0,83,99},
    {30,37,13,44,23,0,0,0,0,53,58,61,89,82,99},
    {29,35,10,42,20,0,0,0,0,51,51,60,82,80,99},
    {28,34,7,40,16,0,0,0,0,49,44,58,75,78,99},
    {27,32,4,38,14,65,95,68,98,47,37,56,67,77,98},
    {26,30,3,36,11,63,91,66,95,45,31,54,60,75,98},
    {25,28,1,34,9,61,86,64,92,43,25,53,53,73,98},
    {24,26,1,32,7,59,79,62,87,41,20,51,47,72,97},
    {23,24,1,31,6,56,71,60,82,39,16,49,40,70,96},
    {22,23,1,29,4,54,62,58,76,37,12,47,35,68,96},
    {21,21,1,27,3,52,53,56,69,35,8,46,30,67,95},
    {20,19,1,25,2,49,45,54,62,33,6,44,25,65,93},
    {19,17,1,23,2,47,37,51,54,31,4,42,21,63,92},
    {18,15,1,21,1,45,29,49,47,28,2,40,18,62,89},
    {17,14,1,19,1,43,23,47,40,26,1,39,14,60,87},
    {16,12,1,17,1,40,17,45,33,24,1,37,12,58,83},
    {15,10,1,16,1,38,13,43,26,22,1,35,9,57,79},
    {14,10,1,14,1,36,9,41,21,20,1,33,7,55,74},
    {13,10,1,12,1,34,6,39,15,18,1,32,6,53,68},
    {12,10,1,10,1,31,4,37,11,16,1,30,4,52,61},
    {11,10,1,10,1,29,3,35,7,14,1,28,3,50,54},
    {10,10,1,10,1,27,2,33,4,12,1,26,2,48,45},
    {9,10,1,10,1,24,1,31,2,10,1,25,2,47,37},
    {8,10,1,10,1,22,1,28,1,10,1,23,1,45,29},
    {7,10,1,10,1,20,1,26,1,10,1,21,1,43,22},
    {6,10,1,10,1,18,1,24,1,10,1,19,1,42,16},
    {5,10,1,10,1,15,1,22,1,10,1,18,1,40,12},
    {4,10,1,10,1,13,1,20,1,10,1,16,1,38,8},
    {3,10,1,10,1,11,1,18,1,10,1,14,1,37,6},
    {2,10,1,10,1,10,1,16,1,10,1,12,1,35,4},
    {1,10,1,10,1,10,1,14,1,10,1,11,1,33,3},
    {0,10,1,10,1,10,1,12,1,10,1,10,1,32,2},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A4_6_9M[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {45,64,95,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,63,92,0,0,0,0,0,0,0,0,0,0,100,99},
    {43,61,88,0,0,0,0,0,0,0,0,0,0,100,99},
    {42,60,83,0,0,0,0,0,0,0,0,0,0,100,99},
    {41,58,78,0,0,0,0,0,0,0,0,0,0,100,99},
    {40,57,72,0,0,0,0,0,0,0,0,0,0,100,99},
    {39,55,66,63,94,0,0,0,0,0,0,0,0,99,99},
    {38,54,61,61,88,0,0,0,0,0,0,0,0,98,99},
    {37,53,55,59,81,0,0,0,0,0,0,0,0,96,99},
    {36,51,49,58,74,0,0,0,0,64,97,0,0,94,99},
    {35,50,44,56,66,0,0,0,0,62,93,0,0,93,99},
    {34,48,39,54,59,0,0,0,0,60,87,0,0,91,99},
    {33,47,34,52,52,0,0,0,0,58,80,0,0,89,99},
    {32,45,30,50,45,0,0,0,0,56,71,0,0,87,99},
    {31,44,26,49,39,0,0,0,0,55,63,0,0,86,99},
    {30,42,22,47,34,0,0,0,0,53,54,61,87,84,99},
    {29,41,19,45,29,0,0,0,0,51,46,59,80,82,99},
    {28,39,16,43,24,0,0,0,0,49,39,58,74,81,99},
    {27,38,13,41,20,64,96,69,98,47,32,56,67,79,99},
    {26,36,11,40,17,62,89,67,96,45,26,55,61,77,99},
    {25,35,9,38,13,60,81,65,94,43,22,53,55,76,99},
    {24,34,7,36,11,58,73,63,90,41,17,52,49,74,98},
    {23,32,6,34,9,55,64,61,86,39,14,50,44,72,98},
    {22,31,4,33,7,53,56,59,80,37,11,49,39,70,97},
    {21,29,3,31,5,51,48,57,74,35,9,47,35,69,96},
    {20,28,2,29,4,49,41,55,68,33,7,46,30,67,95},
    {19,26,2,27,3,47,34,53,61,31,5,44,27,65,93},
    {18,25,1,25,2,44,28,51,54,30,4,43,23,64,91},
    {17,23,1,24,1,42,23,50,47,28,3,42,20,62,88},
    {16,22,1,22,1,40,18,48,40,26,2,40,17,60,85},
    {15,20,1,20,1,38,14,46,34,24,2,39,15,59,81},
    {14,19,1,18,1,36,10,44,28,22,1,37,13,57,76},
    {13,17,1,16,1,33,7,42,22,20,1,36,11,55,71},
    {12,16,1,15,1,31,5,40,17,18,1,34,9,53,66},
    {11,15,1,13,1,29,3,38,13,16,1,33,7,52,60},
    {10,13,1,11,1,27,1,36,10,14,1,31,6,50,53},
    {9,12,1,10,1,25,1,34,7,12,1,30,5,48,46},
    {8,10,1,10,1,22,1,32,5,10,1,28,4,47,39},
    {7,10,1,10,1,20,1,30,3,10,1,27,3,45,33},
    {6,10,1,10,1,18,1,29,2,10,1,25,2,43,26},
    {5,10,1,10,1,16,1,27,1,10,1,24,1,42,21},
    {4,10,1,10,1,14,1,25,1,10,1,23,1,40,15},
    {3,10,1,10,1,12,1,23,1,10,1,21,1,38,11},
    {2,10,1,10,1,10,1,21,1,10,1,20,1,36,8},
    {1,10,1,10,1,10,1,19,1,10,1,18,1,35,5},
    {0,10,1,10,1,10,1,17,1,10,1,17,1,33,3},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A5_10_12F[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {45,67,99,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,65,97,0,0,0,0,0,0,0,0,0,0,99,99},
    {43,64,94,0,0,0,0,0,0,0,0,0,0,98,99},
    {42,63,91,0,0,0,0,0,0,0,0,0,0,96,99},
    {41,61,87,0,0,0,0,0,0,0,0,0,0,95,99},
    {40,60,62,0,0,0,0,0,0,0,0,0,0,94,99},
    {39,58,77,63,96,0,0,0,0,0,0,0,0,92,99},
    {38,57,71,62,89,0,0,0,0,0,0,0,0,91,99},
    {37,55,66,60,83,0,0,0,0,0,0,0,0,89,99},
    {36,54,60,59,76,0,0,0,0,66,97,0,0,88,99},
    {35,52,54,57,70,0,0,0,0,64,93,0,0,86,99},
    {34,51,49,56,64,0,0,0,0,61,87,0,0,85,99},
    {33,49,43,54,58,0,0,0,0,59,80,0,0,83,99},
    {32,48,38,53,53,0,0,0,0,57,73,0,0,82,99},
    {31,46,33,51,48,0,0,0,0,55,65,0,0,80,99},
    {30,45,29,50,43,0,0,0,0,53,57,62,90,79,99},
    {29,43,25,48,39,0,0,0,0,51,49,60,83,78,99},
    {28,42,21,47,35,0,0,0,0,49,42,59,76,76,99},
    {27,41,18,45,31,66,96,72,99,47,35,57,69,75,98},
    {26,39,15,44,27,63,91,70,98,44,29,55,62,73,98},
    {25,38,12,42,24,61,84,68,96,42,23,54,56,72,97},
    {24,36,10,40,20,58,77,66,94,40,18,52,51,70,97},
    {23,35,8,39,17,56,69,64,91,38,14,50,45,69,96},
    {22,33,6,37,14,53,60,62,87,36,10,49,40,67,95},
    {21,32,5,36,12,51,52,60,83,34,7,47,36,66,93},
    {20,30,4,34,9,48,44,59,78,32,5,46,32,65,92},
    {19,29,3,33,7,46,36,57,73,29,3,44,28,63,90},
    {18,27,2,31,5,43,28,55,67,27,1,42,24,62,87},
    {17,26,2,30,3,41,21,53,61,25,1,41,20,60,85},
    {16,24,1,28,2,38,15,51,55,23,1,39,17,59,82},
    {15,23,1,27,1,36,10,49,48,21,1,38,14,57,78},
    {14,21,1,25,1,33,6,47,41,19,1,36,12,56,75},
    {13,20,1,24,1,31,3,45,34,17,1,34,9,54,70},
    {12,19,1,22,1,28,1,43,28,14,1,33,7,53,65},
    {11,17,1,21,1,26,1,41,21,12,1,31,5,52,60},
    {10,16,1,19,1,23,1,39,15,10,1,30,4,50,55},
    {9,14,1,18,1,21,1,37,10,10,1,28,2,49,49},
    {8,13,1,16,1,18,1,35,5,10,1,26,1,47,43},
    {7,11,1,15,1,16,1,33,2,10,1,25,1,46,37},
    {6,10,1,13,1,13,1,31,1,10,1,23,1,44,31},
    {5,10,1,12,1,11,1,29,1,10,1,22,1,43,25},
    {4,10,1,10,1,10,1,27,1,10,1,20,1,41,20},
    {3,10,1,10,1,10,1,25,1,10,1,18,1,40,15},
    {2,10,1,10,1,10,1,23,1,10,1,17,1,39,11},
    {1,10,1,10,1,10,1,21,1,10,1,15,1,37,8},
    {0,10,1,10,1,10,1,19,1,10,1,14,1,36,5},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A6_10_12M[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {45,67,99,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,65,97,0,0,0,0,0,0,0,0,0,0,99,99},
    {43,64,94,0,0,0,0,0,0,0,0,0,0,98,99},
    {42,63,91,0,0,0,0,0,0,0,0,0,0,96,99},
    {41,61,87,0,0,0,0,0,0,0,0,0,0,95,99},
    {40,60,82,0,0,0,0,0,0,0,0,0,0,94,99},
    {39,58,77,63,96,0,0,0,0,0,0,0,0,92,99},
    {38,57,71,62,89,0,0,0,0,0,0,0,0,91,99},
    {37,55,66,60,83,0,0,0,0,0,0,0,0,89,99},
    {36,54,60,59,76,0,0,0,0,66,97,0,0,88,99},
    {35,52,54,57,70,0,0,0,0,64,93,0,0,86,99},
    {34,51,49,56,64,0,0,0,0,61,87,0,0,85,99},
    {33,49,43,54,58,0,0,0,0,59,80,0,0,83,99},
    {32,48,38,53,53,0,0,0,0,57,73,0,0,82,99},
    {31,46,33,51,48,0,0,0,0,55,65,0,0,80,99},
    {30,45,29,50,43,0,0,0,0,53,57,62,90,79,99},
    {29,43,25,48,39,0,0,0,0,51,49,60,83,78,99},
    {28,42,21,47,35,0,0,0,0,49,42,59,76,76,99},
    {27,41,18,45,31,66,96,72,99,47,35,57,69,75,98},
    {26,39,15,44,27,63,91,70,98,44,29,55,62,73,98},
    {25,38,12,42,24,61,84,68,96,42,23,54,56,72,97},
    {24,36,10,40,20,58,77,66,94,40,18,52,51,70,97},
    {23,35,8,39,17,56,69,64,91,38,14,50,45,69,96},
    {22,33,6,37,14,53,60,62,87,36,10,49,40,67,95},
    {21,32,5,36,12,51,52,60,83,34,7,47,36,66,93},
    {20,30,4,34,9,48,44,59,78,32,5,46,32,65,92},
    {19,29,3,33,7,46,36,57,73,29,3,44,28,63,90},
    {18,27,2,31,5,43,28,55,67,27,1,42,24,62,87},
    {17,26,2,30,3,41,21,53,61,25,1,41,20,60,85},
    {16,24,1,28,2,38,15,51,55,23,1,39,17,59,82},
    {15,23,1,27,1,36,10,49,48,21,1,38,14,57,78},
    {14,21,1,25,1,33,6,47,41,19,1,36,12,56,75},
    {13,20,1,24,1,31,3,45,34,17,1,34,9,54,70},
    {12,19,1,22,1,28,1,43,28,14,1,33,7,53,65},
    {11,17,1,21,1,26,1,41,21,12,1,31,5,52,60},
    {10,16,1,19,1,23,1,39,15,10,1,30,4,50,55},
    {9,14,1,18,1,21,1,37,10,10,1,28,2,49,49},
    {8,13,1,16,1,18,1,35,5,10,1,26,1,47,43},
    {7,11,1,15,1,16,1,33,2,10,1,25,1,46,37},
    {6,10,1,13,1,13,1,31,1,10,1,23,1,44,31},
    {5,10,1,12,1,11,1,29,1,10,1,22,1,43,25},
    {4,10,1,10,1,10,1,27,1,10,1,20,1,41,20},
    {3,10,1,10,1,10,1,25,1,10,1,18,1,40,15},
    {2,10,1,10,1,10,1,23,1,10,1,17,1,39,11},
    {1,10,1,10,1,10,1,21,1,10,1,15,1,37,8},
    {0,10,1,10,1,10,1,19,1,10,1,14,1,36,5},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A7_13_15F[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {45,65,97,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,64,94,0,0,0,0,0,0,0,0,0,0,100,99},
    {43,63,91,0,0,0,0,0,0,0,0,0,0,100,99},
    {42,61,87,0,0,0,0,0,0,0,0,0,0,100,99},
    {41,60,82,0,0,0,0,0,0,0,0,0,0,99,99},
    {40,58,77,0,0,0,0,0,0,0,0,0,0,97,99},
    {39,57,71,63,94,0,0,0,0,0,0,0,0,96,99},
    {38,55,66,62,90,0,0,0,0,0,0,0,0,94,99},
    {37,54,60,60,84,0,0,0,0,0,0,0,0,92,99},
    {36,52,54,58,77,0,0,0,0,66,98,0,0,91,99},
    {35,51,49,57,70,0,0,0,0,64,95,0,0,89,99},
    {34,49,44,55,63,0,0,0,0,62,89,0,0,87,99},
    {33,48,39,53,56,0,0,0,0,60,82,0,0,86,99},
    {32,47,34,51,49,0,0,0,0,58,74,0,0,84,99},
    {31,45,30,50,43,0,0,0,0,55,66,0,0,82,99},
    {30,44,26,48,37,0,0,0,0,53,57,63,92,81,99},
    {29,42,22,46,31,0,0,0,0,51,49,61,86,79,99},
    {28,41,19,44,27,0,0,0,0,49,41,59,79,77,98},
    {27,39,16,43,22,66,96,74,99,47,34,58,73,76,98},
    {26,38,13,41,18,64,92,72,98,45,28,56,66,74,98},
    {25,36,11,39,15,61,85,70,97,42,22,54,60,72,97},
    {24,35,9,37,12,59,78,68,95,40,18,53,54,71,97},
    {23,34,7,36,10,57,71,66,93,38,14,51,48,69,96},
    {22,32,5,34,8,54,63,64,91,36,10,49,42,67,95},
    {21,31,4,32,6,52,55,62,88,34,7,48,37,66,94},
    {20,29,3,31,5,49,46,60,84,32,5,46,33,64,92},
    {19,26,2,29,3,47,39,58,79,30,4,44,28,62,90},
    {18,25,2,27,2,45,31,56,73,27,2,43,24,61,88},
    {17,23,1,25,2,42,24,54,67,25,1,41,20,59,85},
    {16,22,1,24,1,40,18,52,60,23,1,39,17,57,81},
    {15,21,1,22,1,37,13,50,52,21,1,38,14,56,77},
    {14,19,1,20,1,35,8,48,44,19,1,36,11,54,72},
    {13,18,1,18,1,33,5,46,36,17,1,34,8,52,65},
    {12,16,1,17,1,30,2,44,28,15,1,33,6,51,58},
    {11,15,1,15,1,28,1,41,21,12,1,31,4,49,51},
    {10,13,1,13,1,25,1,39,15,10,1,29,3,47,43},
    {9,12,1,12,1,23,1,37,9,10,1,28,2,46,34},
    {8,10,1,10,1,21,1,35,5,10,1,26,1,44,27},
    {7,10,1,10,1,18,1,33,3,10,1,24,1,42,20},
    {6,10,1,10,1,16,1,31,1,10,1,23,1,41,14},
    {5,10,1,10,1,13,1,29,1,10,1,21,1,39,10},
    {4,10,1,10,1,11,1,27,1,10,1,19,1,37,7},
    {3,10,1,10,1,10,1,25,1,10,1,18,1,36,4},
    {2,10,1,10,1,10,1,23,1,10,1,16,1,34,3},
    {1,10,1,10,1,10,1,21,1,10,1,14,1,32,2},
    {0,10,1,10,1,10,1,19,1,10,1,13,1,31,1},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A8_13_15M[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {45,69,98,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,68,97,0,0,0,0,0,0,0,0,0,0,100,99},
    {43,66,95,0,0,0,0,0,0,0,0,0,0,100,99},
    {42,65,93,0,0,0,0,0,0,0,0,0,0,100,99},
    {41,64,91,0,0,0,0,0,0,0,0,0,0,100,99},
    {40,62,88,0,0,0,0,0,0,0,0,0,0,100,99},
    {39,61,84,66,96,0,0,0,0,0,0,0,0,100,99},
    {38,59,81,64,94,0,0,0,0,0,0,0,0,98,99},
    {37,58,77,63,90,0,0,0,0,0,0,0,0,97,99},
    {36,57,72,61,86,0,0,0,0,66,99,0,0,95,99},
    {35,55,67,60,82,0,0,0,0,64,96,0,0,93,99},
    {34,54,63,58,77,0,0,0,0,62,91,0,0,91,99},
    {33,52,58,57,72,0,0,0,0,60,84,0,0,90,99},
    {32,51,53,55,66,0,0,0,0,58,76,0,0,88,99},
    {31,49,48,54,61,0,0,0,0,56,67,0,0,86,99},
    {30,48,42,52,56,0,0,0,0,54,59,63,93,84,99},
    {29,47,38,51,50,0,0,0,0,52,51,62,88,83,99},
    {28,45,33,49,45,0,0,0,0,50,44,60,81,81,99},
    {27,44,28,48,40,67,97,75,99,48,37,58,75,79,99},
    {26,42,24,46,35,64,92,73,98,46,30,56,68,77,99},
    {25,41,20,45,30,62,87,71,98,43,25,54,62,76,99},
    {24,39,16,43,26,60,80,69,96,41,20,53,55,74,98},
    {23,38,13,42,22,57,73,67,95,39,16,51,49,72,98},
    {22,37,10,40,18,55,65,65,93,37,12,49,43,70,97},
    {21,35,8,39,15,52,57,63,90,35,9,47,37,68,96},
    {20,34,6,37,12,50,50,61,86,33,7,46,32,67,94},
    {19,32,4,36,10,48,42,59,82,31,5,44,27,65,93},
    {18,31,3,34,7,45,34,57,77,29,3,42,23,63,90},
    {17,30,2,33,6,43,27,55,71,27,2,40,19,61,87},
    {16,28,1,31,4,41,21,53,64,25,1,39,15,60,84},
    {15,27,1,30,3,38,14,51,57,23,1,37,12,58,79},
    {14,25,1,28,2,36,9,49,49,21,1,35,9,56,74},
    {13,24,1,27,1,33,5,47,42,19,1,33,7,54,69},
    {12,22,1,25,1,31,2,46,34,17,1,31,5,53,62},
    {11,21,1,24,1,29,1,44,27,14,1,30,3,51,55},
    {10,20,1,22,1,26,1,42,21,12,1,28,2,49,48},
    {9,18,1,21,1,24,1,40,15,10,1,26,1,47,41},
    {8,17,1,19,1,21,1,38,11,10,1,24,1,45,34},
    {7,15,1,18,1,19,1,36,7,10,1,23,1,44,27},
    {6,14,1,16,1,17,1,34,4,10,1,21,1,42,21},
    {5,12,1,15,1,14,1,32,3,10,1,19,1,40,16},
    {4,11,1,13,1,12,1,30,1,10,1,17,1,38,12},
    {3,10,1,12,1,10,1,28,1,10,1,16,1,37,8},
    {2,10,1,10,1,10,1,26,1,10,1,14,1,35,5},
    {1,10,1,10,1,10,1,24,1,10,1,12,1,33,3},
    {0,10,1,10,1,10,1,22,1,10,1,10,1,31,2},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A9_16_18F[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {45,66,97,0,0,0,0,0,0,0,0,0,0,100,99},
    {44,65,95,0,0,0,0,0,0,0,0,0,0,100,99},
    {43,63,92,0,0,0,0,0,0,0,0,0,0,100,99},
    {42,62,88,0,0,0,0,0,0,0,0,0,0,99,99},
    {41,61,84,0,0,0,0,0,0,0,0,0,0,97,99},
    {40,59,80,0,0,0,0,0,0,0,0,0,0,95,99},
    {39,58,75,63,96,0,0,0,0,0,0,0,0,94,99},
    {38,56,70,62,91,0,0,0,0,0,0,0,0,92,99},
    {37,55,65,60,85,0,0,0,0,0,0,0,0,91,99},
    {36,54,60,59,78,0,0,0,0,67,98,0,0,89,99},
    {35,52,55,57,71,0,0,0,0,65,95,0,0,87,99},
    {34,51,50,56,65,0,0,0,0,63,90,0,0,86,99},
    {33,49,45,54,59,0,0,0,0,60,83,0,0,84,99},
    {32,48,40,53,53,0,0,0,0,58,76,0,0,83,99},
    {31,46,35,51,48,0,0,0,0,56,68,0,0,81,99},
    {30,45,31,50,42,0,0,0,0,54,61,62,91,79,99},
    {29,44,27,48,38,0,0,0,0,52,53,60,85,78,99},
    {28,42,23,47,33,0,0,0,0,50,46,59,79,76,99},
    {27,41,20,45,29,67,96,74,99,48,39,57,73,75,98},
    {26,39,17,43,25,65,92,73,99,45,32,56,66,73,98},
    {25,38,14,42,22,63,87,71,98,43,26,54,60,71,97},
    {24,37,11,40,19,60,81,69,96,41,21,53,54,70,97},
    {23,35,9,39,16,58,75,67,94,39,16,51,49,68,96},
    {22,34,7,37,13,56,69,65,92,37,12,50,44,67,94},
    {21,32,5,36,11,54,62,63,89,35,8,48,39,65,93},
    {20,31,4,34,9,52,55,61,86,33,6,47,34,64,91},
    {19,30,2,33,7,49,48,60,82,31,4,45,30,62,88},
    {18,28,2,31,5,47,41,58,77,28,2,44,26,60,86},
    {17,27,1,30,4,45,34,56,72,26,1,42,22,59,82},
    {16,25,1,28,3,43,28,54,66,24,1,41,19,57,78},
    {15,24,1,27,2,41,21,52,60,22,1,39,16,56,73},
    {14,22,1,25,1,38,15,50,53,20,1,38,14,54,68},
    {13,21,1,23,1,36,10,48,47,18,1,36,11,52,62},
    {12,20,1,22,1,34,5,46,40,16,1,35,9,51,56},
    {11,18,1,20,1,32,2,45,33,14,1,33,7,49,49},
    {10,17,1,19,1,30,1,43,26,11,1,32,6,48,42},
    {9,15,1,17,1,28,1,41,20,10,1,30,4,46,36},
    {8,14,1,16,1,25,1,39,14,10,1,29,3,44,29},
    {7,13,1,14,1,23,1,37,9,10,1,27,2,43,23},
    {6,11,1,13,1,21,1,35,6,10,1,26,2,41,18},
    {5,10,1,11,1,19,1,33,3,10,1,24,1,40,14},
    {4,10,1,10,1,17,1,31,1,10,1,23,1,38,10},
    {3,10,1,10,1,14,1,30,1,10,1,21,1,36,7},
    {2,10,1,10,1,12,1,28,1,10,1,20,1,35,5},
    {1,10,1,10,1,10,1,26,1,10,1,18,1,33,3},
    {0,10,1,10,1,10,1,24,1,10,1,17,1,32,2},
};

const ScaleNormsPrq::Raw_T_Percent ScaleNormsPrq::A10_16_18M[] =
{
    {48,0,0,0,0,0,0,0,0,0,0,0,0,100,99},
    {47,0,0,0,0,0,0,0,0,0,0,0,0,98,99},
    {46,0,0,0,0,0,0,0,0,0,0,0,0,97,99},
    {45,72,99,0,0,0,0,0,0,0,0,0,0,95,99},
    {44,71,98,0,0,0,0,0,0,0,0,0,0,94,99},
    {43,69,97,0,0,0,0,0,0,0,0,0,0,93,99},
    {42,68,96,0,0,0,0,0,0,0,0,0,0,91,99},
    {41,66,95,0,0,0,0,0,0,0,0,0,0,90,99},
    {40,65,93,0,0,0,0,0,0,0,0,0,0,89,99},
    {39,63,90,68,99,0,0,0,0,0,0,0,0,87,99},
    {38,62,87,67,99,0,0,0,0,0,0,0,0,86,99},
    {37,60,84,65,97,0,0,0,0,0,0,0,0,85,99},
    {36,59,80,64,95,0,0,0,0,67,99,0,0,83,99},
    {35,57,75,62,92,0,0,0,0,65,97,0,0,82,99},
    {34,56,71,61,87,0,0,0,0,63,93,0,0,80,99},
    {33,54,65,59,82,0,0,0,0,61,87,0,0,79,99},
    {32,53,60,58,76,0,0,0,0,59,80,0,0,78,98},
    {31,51,54,56,70,0,0,0,0,57,73,0,0,76,98},
    {30,50,48,55,64,0,0,0,0,55,65,66,95,75,98},
    {29,48,42,53,57,0,0,0,0,53,57,64,92,74,97},
    {28,47,37,52,51,0,0,0,0,51,49,63,89,72,97},
    {27,45,31,50,45,70,98,82,99,49,42,61,84,71,96},
    {26,44,26,49,39,68,97,79,99,47,35,59,80,70,96},
    {25,42,22,47,34,66,94,77,99,45,29,57,74,68,95},
    {24,41,18,45,29,63,90,75,99,43,24,56,69,67,94},
    {23,39,14,44,24,61,86,73,98,41,19,54,63,65,93},
    {22,38,11,42,21,59,80,70,97,39,15,52,57,64,91},
    {21,36,9,41,17,56,73,68,96,37,11,51,51,63,90},
    {20,35,7,39,14,54,65,66,94,35,8,49,45,61,88},
    {19,33,5,38,12,52,56,64,92,33,6,47,39,60,86},
    {18,32,4,36,10,49,48,62,88,31,4,45,34,59,83},
    {17,30,3,35,8,47,39,59,84,28,3,44,28,57,80},
    {16,29,2,33,7,44,31,57,78,26,2,42,23,56,77},
    {15,27,1,32,5,42,23,55,70,24,1,40,19,55,73},
    {14,26,1,30,4,40,16,53,62,22,1,39,15,53,68},
    {13,24,1,29,3,37,11,50,53,20,1,37,11,52,63},
    {12,23,1,27,3,35,7,48,43,18,1,35,8,50,58},
    {11,21,1,26,2,33,4,46,34,16,1,33,5,49,52},
    {10,20,1,24,2,30,2,44,26,14,1,32,3,48,46},
    {9,18,1,23,1,28,1,41,18,12,1,30,2,46,40},
    {8,17,1,21,1,26,1,39,13,10,1,28,1,45,33},
    {7,15,1,20,1,23,1,37,8,10,1,27,1,44,27},
    {6,14,1,18,1,21,1,35,5,10,1,25,1,42,21},
    {5,12,1,17,1,18,1,32,3,10,1,23,1,41,16},
    {4,11,1,15,1,16,1,30,2,10,1,21,1,39,12},
    {3,10,1,14,1,14,1,28,1,10,1,20,1,38,8},
    {2,10,1,12,1,11,1,26,1,10,1,18,1,37,5},
    {1,10,1,10,1,10,1,23,1,10,1,16,1,35,3},
    {0,10,1,10,1,10,1,21,1,10,1,15,1,34,2},
};

const ScaleNormsPrq::Conf_Interval ScaleNormsPrq::conf_interval[] =
{
    {sdtAttachment, 6, 6, 6, 6, 5, 5, 5, 5, 5, 6},
    {sdtCommunication, 0, 0, 6, 6, 5, 5, 5, 5, 5, 6},
    {sdtDisciplinePractice, 7, 0, 7, 7, 7, 7, 8, 8, 7, 8},
    {sdtInvolvement, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6},
    {sdtParentingConfidence, 7, 7, 7, 7, 6, 6, 7, 7, 7, 7},
    {sdtSatisfactionWithSchool, 0, 0, 4, 4, 4, 4, 4, 4, 4, 4},
    {sdtRelationalFrustration, 5, 5, 5, 5, 4, 4, 5, 5, 5, 4}
};

/////////////////////////////////////////////////////////////////////////////////////////
// class ReportPrq
/////////////////////////////////////////////////////////////////////////////////////////
QString ScaleNormsPrq::SubDimensionTypeToName(SubDimensionType eSubDimensionType)
{
    QMap<SubDimensionType, QString> s_mapSubDimensionTypeToName;
    if(s_mapSubDimensionTypeToName.isEmpty())
    {
        s_mapSubDimensionTypeToName.insert(sdtAttachment, tr("Attachment"));
        s_mapSubDimensionTypeToName.insert(sdtCommunication, tr("Communication"));
        s_mapSubDimensionTypeToName.insert(sdtDisciplinePractice, tr("Discipline Practice"));
        s_mapSubDimensionTypeToName.insert(sdtInvolvement, tr("Involvement"));
        s_mapSubDimensionTypeToName.insert(sdtParentingConfidence, tr("Parenting Confidence"));
        s_mapSubDimensionTypeToName.insert(sdtSatisfactionWithSchool, tr("Satisfaction With School"));
        s_mapSubDimensionTypeToName.insert(sdtRelationalFrustration, tr("Relational Frustration"));
        s_mapSubDimensionTypeToName.insert(sdtUnknown, tr("Unknown"));
    }

    return s_mapSubDimensionTypeToName.value(eSubDimensionType, QObject::tr("Unknown"));
}

QString ScaleNormsPrq::LevelTypeToName(LevelType eLevelType)
{
    QMap<LevelType, QString> s_mapLevelTypeToName;
    if(s_mapLevelTypeToName.isEmpty())
    {
        s_mapLevelTypeToName.insert(ltAcceptable, tr("Acceptable"));
        s_mapLevelTypeToName.insert(ltCaution, tr("Caution"));
        s_mapLevelTypeToName.insert(ltExtremeCaution, tr("Extreme Caution"));
        s_mapLevelTypeToName.insert(ltUnknown, tr("Unknown"));
    }

    return s_mapLevelTypeToName.value(eLevelType, QObject::tr("Unknown"));
}

bool ScaleNormsPrq::A1_2_5F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A1_2_5F) / sizeof(Raw_T_Percent); i++)
        {
            if(A1_2_5F[i].uchRawScore == uchRawScore)
            {
                result = A1_2_5F[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A2_2_5M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A2_2_5M) / sizeof(Raw_T_Percent); i++)
        {
            if(A2_2_5M[i].uchRawScore == uchRawScore)
            {
                result = A2_2_5M[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A3_6_9F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A3_6_9F) / sizeof(Raw_T_Percent); i++)
        {
            if(A3_6_9F[i].uchRawScore == uchRawScore)
            {
                result = A3_6_9F[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A4_6_9M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A4_6_9M) / sizeof(Raw_T_Percent); i++)
        {
            if(A4_6_9M[i].uchRawScore == uchRawScore)
            {
                result = A4_6_9M[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A5_10_12F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A5_10_12F) / sizeof(Raw_T_Percent); i++)
        {
            if(A5_10_12F[i].uchRawScore == uchRawScore)
            {
                result = A5_10_12F[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A6_10_12M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A6_10_12M) / sizeof(Raw_T_Percent); i++)
        {
            if(A6_10_12M[i].uchRawScore == uchRawScore)
            {
                result = A6_10_12M[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A7_13_15F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A7_13_15F) / sizeof(Raw_T_Percent); i++)
        {
            if(A7_13_15F[i].uchRawScore == uchRawScore)
            {
                result = A7_13_15F[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A8_13_15M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A8_13_15M) / sizeof(Raw_T_Percent); i++)
        {
            if(A8_13_15M[i].uchRawScore == uchRawScore)
            {
                result = A8_13_15M[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A9_16_18F_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A9_16_18F) / sizeof(Raw_T_Percent); i++)
        {
            if(A9_16_18F[i].uchRawScore == uchRawScore)
            {
                result = A9_16_18F[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::A10_16_18M_FromRawAndType(unsigned char uchRawScore, SubDimensionType eSubDimensionType, T_Percent &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(A10_16_18M) / sizeof(Raw_T_Percent); i++)
        {
            if(A10_16_18M[i].uchRawScore == uchRawScore)
            {
                result = A10_16_18M[i].tp[int(eSubDimensionType)];
                return true;
            }
        }
    }

    return false;
}

bool ScaleNormsPrq::Conf_Interval_FromType(SubDimensionType eSubDimensionType, Conf_Interval &result)
{
    if(eSubDimensionType >= sdtAttachment && eSubDimensionType < sdtUnknown)
    {
        for(int i = 0; i < sizeof(conf_interval) / sizeof(Conf_Interval); i++)
        {
            if(conf_interval[i].eSubDimensionType == eSubDimensionType)
            {
                result = conf_interval[i];
                return true;
            }
        }
    }

    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
// class ReportPrq
/////////////////////////////////////////////////////////////////////////////////////////
ReportPrq::ReportPrq(int iUserEvaluationId, const QString &strReportFileName, bool bPP) :
    Report(iUserEvaluationId, strReportFileName), m_bPP(bPP)
{
    m_iFIndex = 0;
}

ReportPrq::~ReportPrq()
{
}

QString ReportPrq::GetHtmlTempalte() const
{
    return QLatin1String(":/selreport/html/rpt_prq.html");
}

bool ReportPrq::BuildReportData()
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
    QList<int> lstSuggestionQuestionIndexs;
    foreach(int iQuestionIndex, mapQuestionIndexToScore.keys())
    {
        const int iQuestionScore = mapQuestionIndexToScore.value(iQuestionIndex);

        ScaleNormsPrq::SubDimensionType eSubDimensionType = GetSubDimensionTypeFromQestionIndex(iQuestionIndex);
        if(eSubDimensionType != ScaleNormsPrq::sdtUnknown)
            m_mapSubDimensionTypeToRawScore[eSubDimensionType] += iQuestionScore;

        if(IsFIndex(iQuestionIndex, iQuestionScore))
            m_iFIndex++;

        if(IsSuggestionQuestion(iQuestionIndex, iQuestionScore))
            lstSuggestionQuestionIndexs.append(iQuestionIndex);
    }

    // Calculate T score and Percent
    if(m_ChildInfo.iAge <= 5)
    {
        if(m_UserInfo.eGenderType == gtFemale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A1_2_5F_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_2_5F;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_2_5F;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
        else if(m_UserInfo.eGenderType == gtMale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A2_2_5M_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_2_5M;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_2_5M;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
    }
    else if(m_ChildInfo.iAge >= 6 && m_ChildInfo.iAge <= 9)
    {
        if(m_UserInfo.eGenderType == gtFemale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A3_6_9F_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_6_9F;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_6_9F;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
        else if(m_UserInfo.eGenderType == gtMale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A4_6_9M_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_6_9M;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_6_9M;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
    }
    else if(m_ChildInfo.iAge >= 10 && m_ChildInfo.iAge <= 12)
    {
        if(m_UserInfo.eGenderType == gtFemale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A5_10_12F_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_10_12F;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_10_12F;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
        else if(m_UserInfo.eGenderType == gtMale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A6_10_12M_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_10_12M;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_10_12M;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
    }
    else if(m_ChildInfo.iAge >= 13 && m_ChildInfo.iAge <= 15)
    {
        if(m_UserInfo.eGenderType == gtFemale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A7_13_15F_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_13_15F;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_13_15F;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
        else if(m_UserInfo.eGenderType == gtMale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A8_13_15M_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_13_15M;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_13_15M;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
    }
    else if(m_ChildInfo.iAge >= 16)
    {
        if(m_UserInfo.eGenderType == gtFemale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A9_16_18F_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_16_18F;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_16_18F;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
        else if(m_UserInfo.eGenderType == gtMale)
        {
            foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
            {
                ScaleNormsPrq::T_Percent tp;
                if(ScaleNormsPrq::A10_16_18M_FromRawAndType(m_mapSubDimensionTypeToRawScore.value(eSubDimensionType), eSubDimensionType, tp))
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = tp.uchTScore;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = tp.uchPercent;
                }
                else
                {
                    m_mapSubDimensionTypeToTScore[eSubDimensionType] = 0;
                    m_mapSubDimensionTypeToPercent[eSubDimensionType] = 0;
                }

                ScaleNormsPrq::Conf_Interval ci;
                if(ScaleNormsPrq::Conf_Interval_FromType(eSubDimensionType, ci))
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) - ci.uch90_16_18M;
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType) + ci.uch90_16_18M;
                }
                else
                {
                    m_mapSubDimensionTypeTo90Lower[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                    m_mapSubDimensionTypeTo90Upper[eSubDimensionType] = m_mapSubDimensionTypeToTScore.value(eSubDimensionType);
                }
            }
        }
    }

    QStringList lstAcceptableSubdimens, lstCautionSubdimens, lstExtremeCautionSubdimens;
    foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
    {
        ScaleNormsPrq::LevelType eLevelType = GetSubDimensionLevelType(eSubDimensionType);
        if(eLevelType == ScaleNormsPrq::ltAcceptable)
            lstAcceptableSubdimens.append(ScaleNormsPrq::SubDimensionTypeToName(eSubDimensionType));
        else if(eLevelType == ScaleNormsPrq::ltCaution)
            lstCautionSubdimens.append(ScaleNormsPrq::SubDimensionTypeToName(eSubDimensionType));
        else if(eLevelType == ScaleNormsPrq::ltExtremeCaution)
            lstExtremeCautionSubdimens.append(ScaleNormsPrq::SubDimensionTypeToName(eSubDimensionType));

        m_mapSubDimensionTypeToLevelType[eSubDimensionType] = eLevelType;
    }

    QString strSuggestion1OnlyA = m_pSettings->value("prq_report_suggestion1/OnlyA").toString();
    QString strSuggestion1OnlyC = m_pSettings->value("prq_report_suggestion1/OnlyC").toString();
    QString strSuggestion1OnlyE = m_pSettings->value("prq_report_suggestion1/OnlyE").toString();
    QString strSuggestion1BothAC = m_pSettings->value("prq_report_suggestion1/BothAC").toString();
    QString strSuggestion1BothAE = m_pSettings->value("prq_report_suggestion1/BothAE").toString();
    QString strSuggestion1BothCE = m_pSettings->value("prq_report_suggestion1/BothCE").toString();
    QString strSuggestion1BothACE = m_pSettings->value("prq_report_suggestion1/BothACE").toString();

    QString strReportSuggestion1;
    // OnlyA
    if(!lstAcceptableSubdimens.isEmpty() && lstCautionSubdimens.isEmpty() && lstExtremeCautionSubdimens.isEmpty())
    {
        strReportSuggestion1 = strSuggestion1OnlyA.replace(QLatin1String("$list_a$"), lstAcceptableSubdimens.join(QLatin1Char(',')));
    }
    // OnlyC
    else if(lstAcceptableSubdimens.isEmpty() && !lstCautionSubdimens.isEmpty() && lstExtremeCautionSubdimens.isEmpty())
    {
        strReportSuggestion1 = strSuggestion1OnlyC.replace(QLatin1String("$list_c$"), lstCautionSubdimens.join(QLatin1Char(',')));
    }
    // OnlyE
    else if(lstAcceptableSubdimens.isEmpty() && lstCautionSubdimens.isEmpty() && !lstExtremeCautionSubdimens.isEmpty())
    {
        strReportSuggestion1 = strSuggestion1OnlyE.replace(QLatin1String("$list_e$"), lstExtremeCautionSubdimens.join(QLatin1Char(',')));
    }
    // BothAC
    else if(!lstAcceptableSubdimens.isEmpty() && !lstCautionSubdimens.isEmpty() && lstExtremeCautionSubdimens.isEmpty())
    {
        strReportSuggestion1 = strSuggestion1BothAC.replace(QLatin1String("$list_a$"), lstAcceptableSubdimens.join(QLatin1Char(',')));
        strReportSuggestion1 = strReportSuggestion1.replace(QLatin1String("$list_c$"), lstCautionSubdimens.join(QLatin1Char(',')));
    }
    // BothAE
    else if(!lstAcceptableSubdimens.isEmpty() && lstCautionSubdimens.isEmpty() && !lstExtremeCautionSubdimens.isEmpty())
    {
        strReportSuggestion1 = strSuggestion1BothAE.replace(QLatin1String("$list_a$"), lstAcceptableSubdimens.join(QLatin1Char(',')));
        strReportSuggestion1 = strReportSuggestion1.replace(QLatin1String("$list_e$"), lstExtremeCautionSubdimens.join(QLatin1Char(',')));
    }
    // BothCE
    else if(lstAcceptableSubdimens.isEmpty() && !lstCautionSubdimens.isEmpty() && !lstExtremeCautionSubdimens.isEmpty())
    {
        strReportSuggestion1 = strSuggestion1BothCE.replace(QLatin1String("$list_c$"), lstCautionSubdimens.join(QLatin1Char(',')));
        strReportSuggestion1 = strReportSuggestion1.replace(QLatin1String("$list_e$"), lstExtremeCautionSubdimens.join(QLatin1Char(',')));
    }
    // BothACE
    else if(!lstAcceptableSubdimens.isEmpty() && !lstCautionSubdimens.isEmpty() && !lstExtremeCautionSubdimens.isEmpty())
    {
        strReportSuggestion1 = strSuggestion1BothACE.replace(QLatin1String("$list_a$"), lstAcceptableSubdimens.join(QLatin1Char(',')));
        strReportSuggestion1 = strReportSuggestion1.replace(QLatin1String("$list_c$"), lstCautionSubdimens.join(QLatin1Char(',')));
        strReportSuggestion1 = strReportSuggestion1.replace(QLatin1String("$list_e$"), lstExtremeCautionSubdimens.join(QLatin1Char(',')));
    }

    QString strReportSuggestion2Data;
    foreach(int iQuestionIndex, lstSuggestionQuestionIndexs)
    {
        QString strKey = m_bPP ? QString("prq_report_suggestion2/pp_%1").arg(iQuestionIndex) : QString("prq_report_suggestion2/pca_%1").arg(iQuestionIndex);
        strReportSuggestion2Data = strReportSuggestion2Data + m_pSettings->value(strKey).toString();
    }

    QString strReportSuggestion2;
    if(!strReportSuggestion2Data.isEmpty())
        strReportSuggestion2 = m_pSettings->value("prq_report_suggestion2/suggestion").toString().replace("$list_data$", strReportSuggestion2Data);

    m_mapVariableData.insert(g_strSubdimenNum, QString::number(m_mapSubDimensionTypeToLevelType.size()));
    m_mapVariableData.insert(g_strAcceptableSubdimenNum, QString::number(lstAcceptableSubdimens.size()));
    m_mapVariableData.insert(g_strAcceptableSubdimenList, lstAcceptableSubdimens.isEmpty() ?  "" : QLatin1String(" : ") + lstAcceptableSubdimens.join(QLatin1String(", ")));
    m_mapVariableData.insert(g_strCautionSubdimenNum, QString::number(lstCautionSubdimens.size()));
    m_mapVariableData.insert(g_strCautionSubdimenList, lstCautionSubdimens.isEmpty() ?  "" : QLatin1String(" : ") + lstCautionSubdimens.join(QLatin1String(", ")));
    m_mapVariableData.insert(g_strExtremeCautionSubdimenNum, QString::number(lstExtremeCautionSubdimens.size()));
    m_mapVariableData.insert(g_strExtremeCautionSubdimenList, lstExtremeCautionSubdimens.isEmpty() ?  "" : QLatin1String(" : ") + lstExtremeCautionSubdimens.join(QLatin1String(", ")));
    m_mapVariableData.insert(g_strFIndex, QString::number(m_iFIndex));
    m_mapVariableData.insert(g_strVersionAge, (m_bPP ? tr("Preschool Verison, 3-6 Yeas old") : tr("School Verison, 6-18 Yeas old")));
    m_mapVariableData.insert(g_strSubdimenTableHeight, (m_bPP ? "620" : "820"));
    m_mapVariableData.insert(g_strReportSuggestion1, strReportSuggestion1);
    m_mapVariableData.insert(g_strReportSuggestion2, strReportSuggestion2);

    DrawImageForBasicInfo();
    DrawImageForSLevel();
    DrawImageSubDimenTable();
    DrawPercentGender();

    return true;
}

QJsonObject ReportPrq::BuildJsonResult()
{
    return QJsonObject();
}

int ReportPrq::AnswerToScore(int iQuestionIndex, const QString &strAnswer)
{
    static QList<int> lstNegativeQuestionIndex_PP = QList<int>() << 24 << 30 << 49;
    static QList<int> lstNegativeQuestionIndex_PCA = QList<int>() << 13 << 60 << 79 << 84;

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

bool ReportPrq::IsFIndex(int iQuestionIndex, int iQuestionScore)
{
    static QMap<int, int> mapIndexToScore_PP;
    if(mapIndexToScore_PP.isEmpty())
    {
        mapIndexToScore_PP.insert(1, 0);
        mapIndexToScore_PP.insert(6, 0);
        mapIndexToScore_PP.insert(8, 0);
        mapIndexToScore_PP.insert(11, 3);
        mapIndexToScore_PP.insert(14, 0);
        mapIndexToScore_PP.insert(16, 0);
        mapIndexToScore_PP.insert(28, 0);
        mapIndexToScore_PP.insert(30, 3);
        mapIndexToScore_PP.insert(34, 0);
        mapIndexToScore_PP.insert(35, 3);
        mapIndexToScore_PP.insert(39, 0);
        mapIndexToScore_PP.insert(44, 0);
        mapIndexToScore_PP.insert(48, 3);
        mapIndexToScore_PP.insert(55, 0);
        mapIndexToScore_PP.insert(59, 3);
    }

    static QMap<int, int> mapIndexToScore_PCA;
    if(mapIndexToScore_PCA.isEmpty())
    {
        mapIndexToScore_PCA.insert(4, 0);
        mapIndexToScore_PCA.insert(8, 0);
        mapIndexToScore_PCA.insert(12, 0);
        mapIndexToScore_PCA.insert(16, 3);
        mapIndexToScore_PCA.insert(19, 0);
        mapIndexToScore_PCA.insert(21, 0);
        mapIndexToScore_PCA.insert(23, 0);
        mapIndexToScore_PCA.insert(29, 0);
        mapIndexToScore_PCA.insert(35, 0);
        mapIndexToScore_PCA.insert(38, 0);
        mapIndexToScore_PCA.insert(54, 0);
        mapIndexToScore_PCA.insert(57, 0);
        mapIndexToScore_PCA.insert(63, 0);
        mapIndexToScore_PCA.insert(64, 0);
        mapIndexToScore_PCA.insert(71, 3);
        mapIndexToScore_PCA.insert(78, 3);
        mapIndexToScore_PCA.insert(80, 0);
        mapIndexToScore_PCA.insert(86, 3);
    }

    QMap<int, int> &mapIndexToScore = (m_bPP ? mapIndexToScore_PP : mapIndexToScore_PCA);
    return (mapIndexToScore.contains(iQuestionIndex) && mapIndexToScore.value(iQuestionIndex) == iQuestionScore);
}

bool ReportPrq::IsSuggestionQuestion(int iQuestionIndex, int iQuestionScore)
{
    static QList<int> lstIndexs_pp = QList<int>() << 2 << 7 << 17 << 18 << 19 << 20 << 34 << 37 << 59;
    static QList<int> lstIndexs_pca = QList<int>() << 1 << 2 << 5 << 11 << 26 << 33 << 34 << 38 << 39 << 46 << 57 << 65 << 70 << 78;

    if(iQuestionScore == 3)
        return false;

    QList<int> &lstIndexs = (m_bPP ? lstIndexs_pp : lstIndexs_pca);
    return lstIndexs.contains(iQuestionIndex);
}

ScaleNormsPrq::SubDimensionType ReportPrq::GetSubDimensionTypeFromQestionIndex(int iQuestionIndex) const
{
    static QMap<ScaleNormsPrq::SubDimensionType, QList<int> > s_mapSubDimensionTypeToQuestionIndex_PP;
    if(s_mapSubDimensionTypeToQuestionIndex_PP.isEmpty())
    {
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(ScaleNormsPrq::sdtAttachment, QList<int>() << 3 << 5 << 10 << 12 << 16 << 21 << 25 << 28 << 34 << 37 << 39 << 43 << 46 << 53);
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(ScaleNormsPrq::sdtDisciplinePractice, QList<int>() << 9 << 13 << 23 << 29 << 33 << 40 << 54 << 58);
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(ScaleNormsPrq::sdtInvolvement, QList<int>() << 1 << 6 << 20 << 26 << 32 << 36 << 41 << 45 << 55);
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(ScaleNormsPrq::sdtParentingConfidence, QList<int>() << 2 << 8 << 14 << 18 << 24 << 30 << 44 << 47 << 49 << 51 << 57 << 60);
        s_mapSubDimensionTypeToQuestionIndex_PP.insert(ScaleNormsPrq::sdtRelationalFrustration, QList<int>() << 4 << 11 << 15 << 19 << 22 << 27 << 31 << 35 << 38 << 42 << 48 << 50 << 52 << 56 << 59);
    }

    static QMap<ScaleNormsPrq::SubDimensionType, QList<int> > s_mapSubDimensionTypeToQuestionIndex_PCA;
    if(s_mapSubDimensionTypeToQuestionIndex_PCA.isEmpty())
    {
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(ScaleNormsPrq::sdtAttachment, QList<int>() << 1 << 7 << 15 << 19 << 24 << 30 << 34 << 36 << 41 << 44 << 52 << 54 << 64 << 82 << 87);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(ScaleNormsPrq::sdtCommunication, QList<int>() << 2 << 6 << 20 << 28 << 35 << 37 << 51 << 57 << 60 << 66 << 68 << 80 << 85);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(ScaleNormsPrq::sdtDisciplinePractice, QList<int>() << 3 << 9 << 25 << 27 << 31 << 43 << 48 << 61 << 83);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(ScaleNormsPrq::sdtInvolvement, QList<int>() << 4 << 10 << 16 << 38 << 45 << 47 << 55 << 62 << 75);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(ScaleNormsPrq::sdtParentingConfidence, QList<int>() << 8 << 13 << 21 << 39 << 53 << 56 << 63 << 70 << 72 << 76 << 79 << 84);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(ScaleNormsPrq::sdtSatisfactionWithSchool, QList<int>() << 12 << 18 << 23 << 29 << 33 << 40 << 50 << 59 << 73 << 77);
        s_mapSubDimensionTypeToQuestionIndex_PCA.insert(ScaleNormsPrq::sdtRelationalFrustration, QList<int>() << 11 << 14 << 17 << 22 << 32 << 42 << 49 << 58 << 65 << 67 << 69 << 71 << 74 << 78 << 81 << 86);
    }

    QMap<ScaleNormsPrq::SubDimensionType, QList<int> > &mapSubDimensionTypeToQuestionIndex = (m_bPP ? s_mapSubDimensionTypeToQuestionIndex_PP : s_mapSubDimensionTypeToQuestionIndex_PCA);
    foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, mapSubDimensionTypeToQuestionIndex.keys())
    {
        if(mapSubDimensionTypeToQuestionIndex.value(eSubDimensionType).contains(iQuestionIndex))
            return eSubDimensionType;
    }

    return ScaleNormsPrq::sdtUnknown;
}

ScaleNormsPrq::LevelType ReportPrq::GetSubDimensionLevelType(ScaleNormsPrq::SubDimensionType eSubDimensionType)
{
    ScaleNormsPrq::LevelType eLevelType = ScaleNormsPrq::ltUnknown;

    if(eSubDimensionType >= ScaleNormsPrq::sdtAttachment && eSubDimensionType < ScaleNormsPrq::sdtRelationalFrustration)
    {
        int iTScore = m_mapSubDimensionTypeToTScore.value(eSubDimensionType, 0);
        if(iTScore <= 30)
            eLevelType = ScaleNormsPrq::ltExtremeCaution;
        else if(iTScore > 30 && iTScore <= 40)
            eLevelType = ScaleNormsPrq::ltCaution;
        else
            eLevelType = ScaleNormsPrq::ltAcceptable;
    }
    else if(eSubDimensionType == ScaleNormsPrq::sdtRelationalFrustration)
    {
        int iTScore = m_mapSubDimensionTypeToTScore.value(eSubDimensionType, 0);
        if(iTScore >= 70)
            eLevelType = ScaleNormsPrq::ltExtremeCaution;
        else if(iTScore >= 60 && iTScore < 70)
            eLevelType = ScaleNormsPrq::ltCaution;
        else
            eLevelType = ScaleNormsPrq::ltAcceptable;
    }

    return eLevelType;
}

void ReportPrq::DrawImageForSLevel()
{
    const QRect rcImage(0, 0, 1400, 1000);
    const QRect rcPainter(0, 0, 1400, 1000);
    const QRect rcGraph(260, 50, 1050, 800);
    const int iSubDimensionNum = m_mapSubDimensionTypeToLevelType.size();
    const int iXGap = rcGraph.width() / iSubDimensionNum;
    const int iYGap = rcGraph.height() / 10;

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    QRect rcAcceptable = rcGraph.adjusted(0, 0, -iXGap, -rcGraph.height() / 10 * 4);
    painter.fillRect(rcAcceptable, QBrush(QColor(112, 173, 71)));
    QRect rcCaution = rcGraph.adjusted(0, rcGraph.height() / 10 * 6, -iXGap, -rcGraph.height() / 10 * 3);
    painter.fillRect(rcCaution, QBrush(QColor(255, 192, 0)));
    QRect rcExtremeCaution = rcGraph.adjusted(0, rcGraph.height() / 10 * 7, -iXGap, 0);
    painter.fillRect(rcExtremeCaution, QBrush(QColor(192, 0, 0)));

    rcExtremeCaution = rcGraph.adjusted(rcGraph.width() - iXGap, 0, 0, -rcGraph.height() / 10 * 7);
    painter.fillRect(rcExtremeCaution, QBrush(QColor(192, 0, 0)));
    rcCaution = rcGraph.adjusted(rcGraph.width() - iXGap, rcGraph.height() / 10 * 3, 0, -rcGraph.height() / 10 * 6);
    painter.fillRect(rcCaution, QBrush(QColor(255, 192, 0)));
    rcAcceptable = rcGraph.adjusted(rcGraph.width() - iXGap, rcGraph.height() / 10 * 4, 0, 0);
    painter.fillRect(rcAcceptable, QBrush(QColor(112, 173, 71)));

    painter.setPen(QPen(Qt::black, 5));
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(rcGraph.right() - iXGap, rcGraph.top() - 10, rcGraph.right() - iXGap, rcGraph.bottom() + 10);

    painter.setPen(QPen(Qt::black, 0));
    painter.drawRect(rcGraph);
    painter.drawLine(rcGraph.left(), rcGraph.top() + rcGraph.height() / 2, rcGraph.right(), rcGraph.top() + rcGraph.height() / 2);

    QFont font = qApp->font();
    font.setPointSize(20);
    painter.setFont(font);

    QTextOption TextOption;
    TextOption.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    painter.drawText(QRect(rcPainter.left(), rcGraph.top(), rcGraph.left() - 100 - rcPainter.left(), rcGraph.height()), tr("Score Value"), TextOption);
    painter.drawText(QRect(rcPainter.left(), rcGraph.bottom() + 100, rcGraph.left() - 100 - rcPainter.left(), 30), tr("Score"), TextOption);

    for(int i = 0; i <= 10; i++)
    {
        const int iY = rcGraph.bottom() - i * iYGap;
        painter.drawLine(rcGraph.left(), iY, rcGraph.left() - 20, iY);
        painter.drawLine(rcGraph.right(), iY, rcGraph.right() + 20, iY);

        QTextOption TextOption;
        TextOption.setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        painter.drawText(QRect(rcPainter.left(), iY - 50, rcGraph.left() - 25 - rcPainter.left(), 100), QString::number(i * 10), TextOption);

        TextOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        painter.drawText(QRect(rcGraph.right() + 25, iY - 50, rcPainter.right() - rcGraph.right() - 25, 100), QString::number(i * 10), TextOption);
    }

    QList<QPoint> lstPoints;
    int iX = rcGraph.left() + iXGap / 2;
    for(int i = ScaleNormsPrq::sdtAttachment; i < ScaleNormsPrq::sdtUnknown; i++)
    {
        ScaleNormsPrq::SubDimensionType eSubDimensionType = ScaleNormsPrq::SubDimensionType(i);
        if(m_mapSubDimensionTypeToLevelType.contains(eSubDimensionType))
        {
            painter.drawLine(iX, rcGraph.bottom(), iX, rcGraph.bottom() + 20);

            QTextOption TextOption;
            TextOption.setAlignment(Qt::AlignCenter);
            painter.drawText(QRect(iX - iXGap / 2, rcGraph.bottom() + 25, iXGap, 30), ScaleNormsPrq::SubDimensionTypeToName(eSubDimensionType), TextOption);
            painter.drawText(QRect(iX - iXGap / 2, rcGraph.bottom() + 100, iXGap, 30), QString::number(m_mapSubDimensionTypeToTScore.value(eSubDimensionType)), TextOption);

            int iY = rcGraph.bottom() - m_mapSubDimensionTypeToTScore.value(eSubDimensionType) * rcGraph.height() / 100;
            int iY90Lower = rcGraph.bottom() - m_mapSubDimensionTypeTo90Lower.value(eSubDimensionType) * rcGraph.height() / 100;
            int iY90Upper = rcGraph.bottom() - m_mapSubDimensionTypeTo90Upper.value(eSubDimensionType) * rcGraph.height() / 100;

            painter.setBrush(QColor(153, 204, 255));
            QRect rc90(0, 0, 40, iY90Upper - iY90Lower);
            rc90.moveCenter(QPoint(iX, iY));
            painter.drawRect(rc90);

            painter.setBrush(Qt::black);
            QRect rcPoint(0, 0, 20, 20);
            rcPoint.moveCenter(QPoint(iX, iY));
            painter.drawEllipse(rcPoint);
            lstPoints.append(QPoint(iX, iY));

            iX += iXGap;
        }
    }

    painter.setPen(QPen(Qt::black, 2));
    for(int i = 0; i < lstPoints.size() - 1; i++)
    {
        if(i != lstPoints.size() - 2)
            painter.drawLine(lstPoints.at(i), lstPoints.at(i + 1));
    }

    m_mapImageData.insert(QLatin1String("userdata://slevel.png"), image);
}

void ReportPrq::DrawImageSubDimenTable()
{
    QMap<ScaleNormsPrq::SubDimensionType, QString> mapSubDimensionTypeToDesc;
    mapSubDimensionTypeToDesc.insert(ScaleNormsPrq::sdtAttachment, m_pSettings->value("prq_subdimension_desc/sdtAttachment").toString());
    mapSubDimensionTypeToDesc.insert(ScaleNormsPrq::sdtCommunication, m_pSettings->value("prq_subdimension_desc/sdtCommunication").toString());
    mapSubDimensionTypeToDesc.insert(ScaleNormsPrq::sdtDisciplinePractice, m_pSettings->value("prq_subdimension_desc/sdtDisciplinePractice").toString());
    mapSubDimensionTypeToDesc.insert(ScaleNormsPrq::sdtInvolvement, m_pSettings->value("prq_subdimension_desc/sdtInvolvement").toString());
    mapSubDimensionTypeToDesc.insert(ScaleNormsPrq::sdtParentingConfidence, m_pSettings->value("prq_subdimension_desc/sdtParentingConfidence").toString());
    mapSubDimensionTypeToDesc.insert(ScaleNormsPrq::sdtSatisfactionWithSchool, m_pSettings->value("prq_subdimension_desc/sdtSatisfactionWithSchool").toString());
    mapSubDimensionTypeToDesc.insert(ScaleNormsPrq::sdtRelationalFrustration, m_pSettings->value("prq_subdimension_desc/sdtRelationalFrustration").toString());

    QMap<ScaleNormsPrq::SubDimensionType, QString> mapSubDimensionTypeToNormal;
    mapSubDimensionTypeToNormal.insert(ScaleNormsPrq::sdtAttachment, m_pSettings->value("prq_subdimension_nomal/sdtAttachment").toString());
    mapSubDimensionTypeToNormal.insert(ScaleNormsPrq::sdtCommunication, m_pSettings->value("prq_subdimension_nomal/sdtCommunication").toString());
    mapSubDimensionTypeToNormal.insert(ScaleNormsPrq::sdtDisciplinePractice, m_pSettings->value("prq_subdimension_nomal/sdtDisciplinePractice").toString());
    mapSubDimensionTypeToNormal.insert(ScaleNormsPrq::sdtInvolvement, m_pSettings->value("prq_subdimension_nomal/sdtInvolvement").toString());
    mapSubDimensionTypeToNormal.insert(ScaleNormsPrq::sdtParentingConfidence, m_pSettings->value("prq_subdimension_nomal/sdtParentingConfidence").toString());
    mapSubDimensionTypeToNormal.insert(ScaleNormsPrq::sdtSatisfactionWithSchool, m_pSettings->value("prq_subdimension_nomal/sdtSatisfactionWithSchool").toString());
    mapSubDimensionTypeToNormal.insert(ScaleNormsPrq::sdtRelationalFrustration, m_pSettings->value("prq_subdimension_nomal/sdtRelationalFrustration").toString());

    QMap<ScaleNormsPrq::SubDimensionType, QString> mapSubDimensionTypeToAbnormal;
    mapSubDimensionTypeToAbnormal.insert(ScaleNormsPrq::sdtAttachment, m_pSettings->value("prq_subdimension_abnomal/sdtAttachment").toString());
    mapSubDimensionTypeToAbnormal.insert(ScaleNormsPrq::sdtCommunication, m_pSettings->value("prq_subdimension_abnomal/sdtCommunication").toString());
    mapSubDimensionTypeToAbnormal.insert(ScaleNormsPrq::sdtDisciplinePractice, m_pSettings->value("prq_subdimension_abnomal/sdtDisciplinePractice").toString());
    mapSubDimensionTypeToAbnormal.insert(ScaleNormsPrq::sdtInvolvement, m_pSettings->value("prq_subdimension_abnomal/sdtInvolvement").toString());
    mapSubDimensionTypeToAbnormal.insert(ScaleNormsPrq::sdtParentingConfidence, m_pSettings->value("prq_subdimension_abnomal/sdtParentingConfidence").toString());
    mapSubDimensionTypeToAbnormal.insert(ScaleNormsPrq::sdtSatisfactionWithSchool, m_pSettings->value("prq_subdimension_abnomal/sdtSatisfactionWithSchool").toString());
    mapSubDimensionTypeToAbnormal.insert(ScaleNormsPrq::sdtRelationalFrustration, m_pSettings->value("prq_subdimension_abnomal/sdtRelationalFrustration").toString());

    QMap<ScaleNormsPrq::LevelType, QString> mapLevelTypeToDesc;
    mapLevelTypeToDesc.insert(ScaleNormsPrq::ltAcceptable, m_pSettings->value("prq_level_desc/ltAcceptable").toString());
    mapLevelTypeToDesc.insert(ScaleNormsPrq::ltCaution, m_pSettings->value("prq_level_desc/ltCaution").toString());
    mapLevelTypeToDesc.insert(ScaleNormsPrq::ltExtremeCaution, m_pSettings->value("prq_level_desc/ltExtremeCaution").toString());

    int iTableWidth = m_pSettings->value("prq_report_table/table_width").toString().toInt();
    int iTableHeight = m_pSettings->value(m_bPP ? "prq_report_table/table_height_pp" : "prq_report_table/table_height_pca").toString().toInt();

    QList<int> lstRowHeight;
    foreach(const QString &strRowHeight, m_pSettings->value(m_bPP ? "prq_report_table/row_height_pp" : "prq_report_table/row_height_pca").toStringList())
        lstRowHeight.append(strRowHeight.toInt());

    QList<int> lstColWidth;
    foreach(const QString &strColWidth, m_pSettings->value("prq_report_table/col_width").toStringList())
        lstColWidth.append(strColWidth.toInt());

    QStringList lstHeader = m_pSettings->value("prq_report_table/header").toStringList();
    QString strCol3 = m_pSettings->value("prq_report_table/col3").toString();

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

    font.setPointSize(28);
    font.setWeight(QFont::Normal);
    painter.setFont(font);

    int iRow = 0;
    int iY = rcPainter.top() + lstRowHeight.at(0);
    foreach(ScaleNormsPrq::SubDimensionType eSubDimensionType, m_mapSubDimensionTypeToRawScore.keys())
    {
        QTextOption TextOption;
        TextOption.setWrapMode(QTextOption::WordWrap);

        TextOption.setAlignment(Qt::AlignCenter);
        QRect rcCol1 = QRect(rcPainter.left(), iY, lstColWidth.at(0), lstRowHeight.at(iRow + 1));
        painter.drawText(rcCol1, ScaleNormsPrq::SubDimensionTypeToName(eSubDimensionType), TextOption);

        TextOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QRect rcCol2 = QRect(rcPainter.left() + lstColWidth.at(0) + 20, iY, lstColWidth.at(1) - 20, lstRowHeight.at(iRow + 1));
        painter.drawText(rcCol2, mapSubDimensionTypeToDesc.value(eSubDimensionType), TextOption);

        QString strCol3Value = strCol3;
        strCol3Value.replace(QLatin1String("$level$"), mapLevelTypeToDesc.value(m_mapSubDimensionTypeToLevelType.value(eSubDimensionType)));
        strCol3Value.replace(QLatin1String("$conlustion$"), m_mapSubDimensionTypeToLevelType.value(eSubDimensionType) == ScaleNormsPrq::ltAcceptable ? mapSubDimensionTypeToNormal.value(eSubDimensionType) : mapSubDimensionTypeToAbnormal.value(eSubDimensionType));

        TextOption.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        QRect rcCol3 = QRect(rcPainter.left() + lstColWidth.at(0) + lstColWidth.at(1) + 20, iY, lstColWidth.at(2) - 20, lstRowHeight.at(iRow + 1));
        painter.drawText(rcCol3, strCol3Value, TextOption);

        iY += lstRowHeight.at(iRow + 1);
        iRow++;
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

void ReportPrq::DrawPercentGender()
{
    const QRect rcImage(0, 0, 2020, 1400);
    const QRect rcPainter(10, 100, 2000, 1200);
    const int iSubDimensionNum = m_mapSubDimensionTypeToLevelType.size();
    const int iXGap = 1750 / iSubDimensionNum;

    QImage image(rcImage.size(), QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(rcImage, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(Qt::black, 5));
    painter.setBrush(Qt::NoBrush);
    painter.drawLine(rcPainter.left() + 200 + iXGap * (iSubDimensionNum - 1), rcPainter.top() + 30, rcPainter.left() + 200 + iXGap * (iSubDimensionNum - 1), rcPainter.bottom() - 130);

    painter.setPen(QPen(Qt::black, 0));
    painter.setBrush(Qt::NoBrush);
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

    font.setPointSize(30);
    painter.setFont(font);

    QColor crBar = m_UserInfo.eGenderType == Report::gtMale ? QColor(133, 204, 232) : QColor(238, 171, 202);
    int iX = rcPainter.left() + 200;
    for(int i = ScaleNormsPrq::sdtAttachment; i < ScaleNormsPrq::sdtUnknown; i++)
    {
        ScaleNormsPrq::SubDimensionType eSubDimensionType = ScaleNormsPrq::SubDimensionType(i);
        if(m_mapSubDimensionTypeToLevelType.contains(eSubDimensionType))
        {
            QTextOption TextOption;
            TextOption.setAlignment(Qt::AlignCenter);

            QRect rcXText(iX, rcPainter.bottom() - 150, iXGap, 150);
            painter.drawText(rcXText, ScaleNormsPrq::SubDimensionTypeToName(eSubDimensionType), TextOption);
            painter.drawLine(iX, rcPainter.bottom() - 150, iX, rcPainter.bottom() - 150 + 20);

            int iPecent = m_mapSubDimensionTypeToPercent.value(eSubDimensionType);
            int iYPecent = iPecent * 10;

            painter.save();
            painter.setPen(QPen(crBar, 0));
            painter.setBrush(crBar);
            painter.drawRect(iX + iXGap / 2 - 50, rcPainter.bottom() - 150 - iYPecent, 100, iYPecent);
            painter.restore();

            iX += iXGap;
        }
    }
    painter.drawLine(iX, rcPainter.bottom() - 150, iX, rcPainter.bottom() - 150 + 20);

    m_mapImageData.insert(QLatin1String("userdata://percent_gender.png"), image);
}


