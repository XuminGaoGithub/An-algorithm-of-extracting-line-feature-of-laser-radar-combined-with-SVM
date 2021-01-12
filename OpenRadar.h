#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <stdlib.h>
#include<iostream>  
#include <algorithm>  
#include<fstream>
#include <iomanip>
using namespace cv;
using namespace std;




#include <iostream>
#include <cmath>
#include<cv.h>
#include<highgui.h>
using namespace std;
#include <vector>
#include "WeightedFit.h"
#include"Coordinate.h"
#include"URG.h"
#include <fstream>
#include "Serial.h"
#define UTM30LX
//#define URG04LX
#define CLOCKWISE//倒着装的时候是逆时针
void resizeAllWindow();
static int RadarImageWdith  = 600;
static int RadarImageHeight = 600;
//常用颜色
/*Colour      Red      Green      Blue      值     
    白色   White    255    255    255    16777215     
    红色    Red    255    0    0    255     
    深红色    Dark    Red    128    0    0    128     
    绿色    Green    0    255    0    65280     
    深绿色    Dark    Green    0    128    0    32768     
    蓝色    Blue    0    0    255    16711680       
    紫红色    Magenta    255    0    255    16711935            
    深紫    Dark    Cyan    0    128    128    8421376     
    黄色    Yellow    255    255    0    65535     
    棕色    Brown    128    128    0    32896  
 */
static int usualColor[15] = {16777215,255,128,65280,32768,
                             16711680,16711935,8421376,65535,32896 }; /*<10种常用的颜色*/

class OpenRadar
{
public:
    OpenRadar(void);
    ~OpenRadar(void);

    vector<int>RadarRho;
    vector<int>BreakedRadarRho;

    vector<double>RadarTheta;
    vector<double>BreakedRadarTheta;

    vector<int>SepRadarRho;//分段之后的Rho,折线也进行拆分
    vector<double>SepRadarTheta;

    vector<int>RadarX;
    vector<int>RadarY;
	LinePara line;
    vector<int>BreakIndex;
    vector<LinePara>FittedLine;//拟合出的直线
	vector<LinePara>FittedLine_save;
	vector<LinePara>FittedLine_merge;
	vector<ArcPara> FittedArc;//拟合出的圆弧
	double s0,s1,s2,s3,s;
    
    bool RadarRead(char *fileName);//数据读取
    void CreateRadarImage(IplImage* RadarImage,vector<int>& RadarRho,vector<double>& RadarTheta);
    //数据分段
    int BreakRadarRho();//阈值如何选取
    int OpenRadar::PolyContourFit(int* X, int* Y, int n , float Eps);
    int BreakPolyLine( vector<int>& BreakedRadarRho,vector<double>& BreakedRadarTheta,
                       vector<int>& SepRadarRho ,   vector<double>&SepRadarTheta);//将折线拆成两段

    void FitLine(vector<LinePara>& FittedLine,vector<LinePara>& FittedLine_save,vector<LinePara>& FittedLine_merge,vector<int>& RadarRho,vector<double>& RadarTheta,IplImage* RadarImage);//直线拟合
    void DrawRadarLine(vector<LinePara>& FittedLine,IplImage* RadarImage);
	void DrawRadarLine_merge(vector<LinePara>& FittedLine,IplImage* RadarImage);
    int FindCorners(vector<int>& Corner,int* X, int* Y, int start, int Cnt , float Eps);
    vector<iPoint>Corners;
    void DrawRadarCorners(vector<iPoint>& Corners,IplImage* RadarImage);
    void printRadarLine(vector<LinePara> FittedLine);
    void MedFilter(vector<int>& RadarRho, vector<double>& RadarTheta);
    void CreateFieldImage(IplImage* FieldImage);
    void DrawLine(IplImage *image,iPoint W1,iPoint W2,int ratio,int dx,int dy,CvScalar color,
                  int thickness, int line_type, int shift);//将某线段缩小后绘制到image
    void DrawCircle(IplImage *image,CirclePara circle,int ratio,int dx,int dy,CvScalar color,
        int thickness, int line_type, int shift);//将圆缩小后绘制到image
    void DrawRadar(IplImage *image,CoorTransPara transPara);
    void RadarInit( urg_state_t ,int* data,int n);

	bool ReadRadarFile(urg_state_t state,char *fileName);//从文件中读取雷达的数据
	bool ReadRadarInitFile(urg_state_t & state,char *fileName);//从文件中读取出雷达的状态信息
    void myStrcpy(string s,char *array); 
	
	int FitArc(vector<ArcPara>& FittedArc,vector<int>& RadarRho,vector<double>& RadarTheta);
	void DrawRadarArc(IplImage* image,vector<ArcPara>& FittedArc);

	void SelectRadarRho(vector<int>& RadarRho,vector<double>& RadarTheta);

	void Contours(IplImage* image,vector<int>& RadarRho,vector<double>& RadarTheta);
	void SendArc(vector<ArcPara> FittedArc);
	
	//svm设计
	void Train();
	void test();
	int predict(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2);
	int predict1(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2);
	//多特征提取和判断设计
	double d0(LinePara& line1,LinePara& line2);
	double d1(double x1,double y1,double x2,double y2);
	double d2(LinePara& line,double x1,double y1,double x3,double y3);
	double d3(LinePara& line,double x1,double y1,double x2,double y2);
	double d(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2);

	


};





