
#ifndef WEIGHTED_FIT_H
#define WEIGHTED_FIT_H
#include <cmath>
#include <cstdlib>
#include "QSort.h"
#define MAX_FITPOINTS_CNT 1000
#define PI 3.141592653
#include <vector>
using namespace std;



#define K   5.0//(4.685 / 0.6745)     /// ��Ȩ����е�ϵ��
typedef struct
{
    int x;
    int y;
}iPoint;

static inline iPoint
    ipoint(int x, int y){
        iPoint point;
        point.x = x;
        point.y = y;
        return point;
}
typedef struct{
    double a;//y = a*x + b
    double b;
    double Rho; // �ö�ֱ�ߵ����
    iPoint startPoint;
    iPoint endPoint;
}LinePara;

//LinePara�Ĺ�����������
static inline LinePara
       linePara(double a,double b)
{
    LinePara para;
    para.a = a;
    para.b = b;
    if (abs(a) >= 100000)
    {
        para.Rho = PI/2;
    }else {
        para.Rho = atan(a);
    }
    iPoint start,end;
    start.x = 0;
    start.y = static_cast<int>(b);

    end.x = static_cast<int>(-b/a);
    end.y = 0;
    para.startPoint  = start;
    para.endPoint = end;
    return para;
}

static inline LinePara
    linePara(double a,double b,iPoint startPoint, iPoint endPoint)
{
    LinePara para;
    para.a = a;
    para.b = b;
    if (abs(a) >= 100000)
    {
        para.Rho = PI/2;
    }else {
        para.Rho = atan(a);
    }   
    para.startPoint  = startPoint;
    para.endPoint = endPoint;
    return para;
}

typedef struct{
    int x;
    int y;
    int r;//�⾶
    int r2;//�ھ���Ĭ��Ϊ0
}CirclePara;
static inline CirclePara
    circlePara(int x,int y,int r ,int r2)
{
    CirclePara circle;
    circle.x = x;
    circle.y = y;
    circle.r = r;
    circle.r2 = r2;
    return circle;
}

typedef struct{
	iPoint center;
	int r;
	double startAngle;
	double endAngle;
}ArcPara; 
int Med(int R[] , int Cnt);// ��ȡ��ֵ
int CalW(int X[] , int Y[] , int Cnt , LinePara * EstLinePara , int W[]);
int FitPara(int X[] , int Y[] , int Cnt ,LinePara * EstLinePara , int W[]);
int WeightedFit(int X[] , int Y[] , int Cnt , LinePara * EstLinePara);
int HoughArc(int X[] , int Y[] , int Cnt ,int r, ArcPara * Arc);
//int HoughArc2(int X[] , int Y[] , int Cnt ,int r, ArcPara * Arc);
int HArc(int X[] , int Y[] , int Cnt ,int r, ArcPara * Arc);
#define cmp_pts( x, y )   ( x < y )    //  ���ڿ�������Ƚ�x < y , �õ��Ľ����Ŷ��������
#endif
