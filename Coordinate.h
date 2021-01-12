#pragma once
#include "WeightedFit.h"
#include <iostream>
using namespace std;
//坐标系的定义以及计算与场地和车种类有关

//#define ManualRobot  //手动机器人
//#define AutoRobot  //自动机器人
//#define BlueSquare
#define  RedSquare

//场地中的关键点
static iPoint FieldPointA = ipoint(9192,0);
static iPoint FieldPointB = ipoint(0,9192); 
static iPoint FieldPointC = ipoint(-9192,0);
static iPoint FieldPointD = ipoint(0,-9192);
//场地中的直线变量
static LinePara FieldLine1 = linePara(-1.0,9192.3881554,FieldPointA,FieldPointB);
static LinePara FieldLine2 = linePara(1.0,9192.3881554,FieldPointB,FieldPointC);
static LinePara FieldLine3 = linePara(-1.0,-9192.3881554,FieldPointC,FieldPointD);
static LinePara FieldLine4 = linePara(1.0,-9192.3881554,FieldPointD,FieldPointA);
static LinePara FieldLine5 = linePara(100000.0,0.0,FieldPointB,FieldPointD);
static LinePara FieldLine6 = linePara(-1/100000.0,0.0,FieldPointC,FieldPointA);

//场地中的圆
static CirclePara FieldCircle1 = circlePara(-3000,1301,400,350);
static CirclePara FieldCircle2 = circlePara(-1951,880,400,350);
static CirclePara FieldCircle3 = circlePara(-651,815,400,350);
static CirclePara FieldCircle4 = circlePara(-495,2416,400,350);
static CirclePara FieldCircle5 = circlePara(-3347,-997,400,350);
static CirclePara FieldCircle6 = circlePara(-2400,-2848,400,350);
static CirclePara FieldCircle7 = circlePara(-1499,-2499,400,350);

static CirclePara FieldCircle8 = circlePara(3000,1301,400,350);
static CirclePara FieldCircle9 = circlePara(1951,880,400,350);
static CirclePara FieldCircle10 = circlePara(651,815,400,350);
static CirclePara FieldCircle11= circlePara(495,2416,400,350);
static CirclePara FieldCircle12 = circlePara(3347,-997,400,350);
static CirclePara FieldCircle13 = circlePara(2400,-2848,400,350);
static CirclePara FieldCircle14 = circlePara(1499,-2499,400,350);

//坐标系类，进行坐标系相关的计算
typedef struct{
    int Tx;
    int Ty;
    double theta;//旋转角
}CoorTransPara;  //坐标变换参数

class Coordinate
{
public:

    Coordinate(void);
    ~Coordinate(void);
    //已知四条直线如何计算变换参数
    void CalCoorTransPara(CoorTransPara &transPara,
                          LinePara W1,
                          LinePara W2, 
                          LinePara R1, 
                          LinePara R2);
    void CoortransTest();
    void CalRadarCoord();

    CoorTransPara RadarCoordTransPara;//全局坐标系和雷达坐标系之间的转换参数
    void printRadarCoordtransPara(CoorTransPara coordtrans);
    void TransformCoord(CoorTransPara transPara,iPoint R,iPoint& W);
 

};

