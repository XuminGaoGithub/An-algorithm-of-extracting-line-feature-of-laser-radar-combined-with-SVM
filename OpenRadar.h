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
#define CLOCKWISE//����װ��ʱ������ʱ��
void resizeAllWindow();
static int RadarImageWdith  = 600;
static int RadarImageHeight = 600;
//������ɫ
/*Colour      Red      Green      Blue      ֵ     
    ��ɫ   White    255    255    255    16777215     
    ��ɫ    Red    255    0    0    255     
    ���ɫ    Dark    Red    128    0    0    128     
    ��ɫ    Green    0    255    0    65280     
    ����ɫ    Dark    Green    0    128    0    32768     
    ��ɫ    Blue    0    0    255    16711680       
    �Ϻ�ɫ    Magenta    255    0    255    16711935            
    ����    Dark    Cyan    0    128    128    8421376     
    ��ɫ    Yellow    255    255    0    65535     
    ��ɫ    Brown    128    128    0    32896  
 */
static int usualColor[15] = {16777215,255,128,65280,32768,
                             16711680,16711935,8421376,65535,32896 }; /*<10�ֳ��õ���ɫ*/

class OpenRadar
{
public:
    OpenRadar(void);
    ~OpenRadar(void);

    vector<int>RadarRho;
    vector<int>BreakedRadarRho;

    vector<double>RadarTheta;
    vector<double>BreakedRadarTheta;

    vector<int>SepRadarRho;//�ֶ�֮���Rho,����Ҳ���в��
    vector<double>SepRadarTheta;

    vector<int>RadarX;
    vector<int>RadarY;
	LinePara line;
    vector<int>BreakIndex;
    vector<LinePara>FittedLine;//��ϳ���ֱ��
	vector<LinePara>FittedLine_save;
	vector<LinePara>FittedLine_merge;
	vector<ArcPara> FittedArc;//��ϳ���Բ��
	double s0,s1,s2,s3,s;
    
    bool RadarRead(char *fileName);//���ݶ�ȡ
    void CreateRadarImage(IplImage* RadarImage,vector<int>& RadarRho,vector<double>& RadarTheta);
    //���ݷֶ�
    int BreakRadarRho();//��ֵ���ѡȡ
    int OpenRadar::PolyContourFit(int* X, int* Y, int n , float Eps);
    int BreakPolyLine( vector<int>& BreakedRadarRho,vector<double>& BreakedRadarTheta,
                       vector<int>& SepRadarRho ,   vector<double>&SepRadarTheta);//�����߲������

    void FitLine(vector<LinePara>& FittedLine,vector<LinePara>& FittedLine_save,vector<LinePara>& FittedLine_merge,vector<int>& RadarRho,vector<double>& RadarTheta,IplImage* RadarImage);//ֱ�����
    void DrawRadarLine(vector<LinePara>& FittedLine,IplImage* RadarImage);
	void DrawRadarLine_merge(vector<LinePara>& FittedLine,IplImage* RadarImage);
    int FindCorners(vector<int>& Corner,int* X, int* Y, int start, int Cnt , float Eps);
    vector<iPoint>Corners;
    void DrawRadarCorners(vector<iPoint>& Corners,IplImage* RadarImage);
    void printRadarLine(vector<LinePara> FittedLine);
    void MedFilter(vector<int>& RadarRho, vector<double>& RadarTheta);
    void CreateFieldImage(IplImage* FieldImage);
    void DrawLine(IplImage *image,iPoint W1,iPoint W2,int ratio,int dx,int dy,CvScalar color,
                  int thickness, int line_type, int shift);//��ĳ�߶���С����Ƶ�image
    void DrawCircle(IplImage *image,CirclePara circle,int ratio,int dx,int dy,CvScalar color,
        int thickness, int line_type, int shift);//��Բ��С����Ƶ�image
    void DrawRadar(IplImage *image,CoorTransPara transPara);
    void RadarInit( urg_state_t ,int* data,int n);

	bool ReadRadarFile(urg_state_t state,char *fileName);//���ļ��ж�ȡ�״������
	bool ReadRadarInitFile(urg_state_t & state,char *fileName);//���ļ��ж�ȡ���״��״̬��Ϣ
    void myStrcpy(string s,char *array); 
	
	int FitArc(vector<ArcPara>& FittedArc,vector<int>& RadarRho,vector<double>& RadarTheta);
	void DrawRadarArc(IplImage* image,vector<ArcPara>& FittedArc);

	void SelectRadarRho(vector<int>& RadarRho,vector<double>& RadarTheta);

	void Contours(IplImage* image,vector<int>& RadarRho,vector<double>& RadarTheta);
	void SendArc(vector<ArcPara> FittedArc);
	
	//svm���
	void Train();
	void test();
	int predict(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2);
	int predict1(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2);
	//��������ȡ���ж����
	double d0(LinePara& line1,LinePara& line2);
	double d1(double x1,double y1,double x2,double y2);
	double d2(LinePara& line,double x1,double y1,double x3,double y3);
	double d3(LinePara& line,double x1,double y1,double x2,double y2);
	double d(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2);

	


};





