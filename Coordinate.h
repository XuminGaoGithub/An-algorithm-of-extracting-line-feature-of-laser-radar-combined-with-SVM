#pragma once
#include "WeightedFit.h"
#include <iostream>
using namespace std;
//����ϵ�Ķ����Լ������볡�غͳ������й�

//#define ManualRobot  //�ֶ�������
//#define AutoRobot  //�Զ�������
//#define BlueSquare
#define  RedSquare

//�����еĹؼ���
static iPoint FieldPointA = ipoint(9192,0);
static iPoint FieldPointB = ipoint(0,9192); 
static iPoint FieldPointC = ipoint(-9192,0);
static iPoint FieldPointD = ipoint(0,-9192);
//�����е�ֱ�߱���
static LinePara FieldLine1 = linePara(-1.0,9192.3881554,FieldPointA,FieldPointB);
static LinePara FieldLine2 = linePara(1.0,9192.3881554,FieldPointB,FieldPointC);
static LinePara FieldLine3 = linePara(-1.0,-9192.3881554,FieldPointC,FieldPointD);
static LinePara FieldLine4 = linePara(1.0,-9192.3881554,FieldPointD,FieldPointA);
static LinePara FieldLine5 = linePara(100000.0,0.0,FieldPointB,FieldPointD);
static LinePara FieldLine6 = linePara(-1/100000.0,0.0,FieldPointC,FieldPointA);

//�����е�Բ
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

//����ϵ�࣬��������ϵ��صļ���
typedef struct{
    int Tx;
    int Ty;
    double theta;//��ת��
}CoorTransPara;  //����任����

class Coordinate
{
public:

    Coordinate(void);
    ~Coordinate(void);
    //��֪����ֱ����μ���任����
    void CalCoorTransPara(CoorTransPara &transPara,
                          LinePara W1,
                          LinePara W2, 
                          LinePara R1, 
                          LinePara R2);
    void CoortransTest();
    void CalRadarCoord();

    CoorTransPara RadarCoordTransPara;//ȫ������ϵ���״�����ϵ֮���ת������
    void printRadarCoordtransPara(CoorTransPara coordtrans);
    void TransformCoord(CoorTransPara transPara,iPoint R,iPoint& W);
 

};

