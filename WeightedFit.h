
#ifndef WEIGHTED_FIT_H
#define WEIGHTED_FIT_H
#include <cmath>
#include <cstdlib>
#include "QSort.h"
#define MAX_FITPOINTS_CNT 1000
#define PI 3.141592653
#include <vector>
using namespace std;



#define K   5.0//(4.685 / 0.6745)     /// 加权拟合中的系数
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
    double Rho; // 该段直线的倾角
    iPoint startPoint;
    iPoint endPoint;
}LinePara;

//LinePara的构造内联函数
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
    int r;//外径
    int r2;//内径，默认为0
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
int Med(int R[] , int Cnt);// 求取中值
int CalW(int X[] , int Y[] , int Cnt , LinePara * EstLinePara , int W[]);
int FitPara(int X[] , int Y[] , int Cnt ,LinePara * EstLinePara , int W[]);
int WeightedFit(int X[] , int Y[] , int Cnt , LinePara * EstLinePara);
int HoughArc(int X[] , int Y[] , int Cnt ,int r, ArcPara * Arc);
//int HoughArc2(int X[] , int Y[] , int Cnt ,int r, ArcPara * Arc);
int HArc(int X[] , int Y[] , int Cnt ,int r, ArcPara * Arc);
#define cmp_pts( x, y )   ( x < y )    //  用于快速排序比较x < y , 得到的结果问哦升序排列
#endif
