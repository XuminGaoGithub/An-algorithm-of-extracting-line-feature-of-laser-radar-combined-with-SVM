
#include"OpenRadar.h"
#include <iostream>
#include <cmath>
#include "io.h"
using namespace cv;

using namespace std;
const int MAX_POINT_COUNT = 361;
int Rho[MAX_POINT_COUNT] = {0}; 
Coordinate coord;

int main(){
    OpenRadar openRadar;
    //char fileName[32] = "csv\\data_0.csv";
	char fileName[32]= "csv\\6.txt";
	//char fileName[32] = "csv\\6-lvbo.txt";//���������ԣ��ܶȼ��󣩵ĵ�Ⱥ��Ҫ�˲�
    int frameCnt = 0;
    char key;
    
    int lineCnt = 0;
	IplImage* RadarImage = cvCreateImage(cvSize(RadarImageWdith,
		RadarImageHeight),
		IPL_DEPTH_8U,
		3);
	IplImage* BreakedRadarImage = cvCreateImage(cvSize(RadarImageWdith,
		RadarImageHeight),
		IPL_DEPTH_8U,
		3);
		IplImage* FieldImage = cvCreateImage(cvSize(RadarImageWdith,
		RadarImageHeight),
		IPL_DEPTH_8U,
		3);
    
    while (access(fileName,0) == 0)///**������ȡ������Ҫ����while������һ����Ҫ�����Ĵ�////
    {

	
		 resizeAllWindow();////���ڲ���
        //sprintf(fileName,"csv\\data_%d.csv",frameCnt);
		sprintf(fileName,"csv\\6.txt",frameCnt);
		//sprintf(fileName,"csv\\6-lvbo.txt",frameCnt);
		
        openRadar.RadarRead(fileName);  



		/////////////////////////////////////////////////////////
		openRadar.MedFilter(openRadar.RadarRho,openRadar.RadarTheta);
			openRadar.CreateRadarImage(RadarImage,openRadar.RadarRho,openRadar.RadarTheta);  
			cvShowImage("Radar",RadarImage);
			
			//������ͼ
			openRadar.CreateFieldImage(FieldImage);
			//cvShowImage("Field",FieldImage);

			//cvWaitKey(0);
			
			/////////////////////////////////////////////////////////////////
			openRadar.BreakRadarRho();
			openRadar.CreateRadarImage(RadarImage,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			//cvShowImage("BreakedRadar",RadarImage);
			//�ڴ˴���ӱ�Ҫ���㷨
			
			

			//////////////////////////////////////////////////
			//openRadar.CreateRadarImage(RadarImage,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			////openRadar.CreateRadarImage(RadarImage, openRadar.RadarRho, openRadar.RadarTheta);
			//openRadar.FitLine(openRadar.FittedLine,openRadar.FittedLine_merge,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			//openRadar.DrawRadarLine(openRadar.FittedLine,RadarImage);
			//cvShowImage("BreakedRadarLine",RadarImage);
			////openRadar.printRadarLine(openRadar.FittedLine);


			//cvWaitKey(0);

			lineCnt = openRadar.BreakPolyLine(openRadar.BreakedRadarRho,
				openRadar.BreakedRadarTheta,
				openRadar.SepRadarRho ,   
				openRadar.SepRadarTheta);
			cout<<"lineCnt: "<<lineCnt<<endl;

			/////////////////////////////////////////////////////////////////////////////
			//openRadar.SepRadarRho();
			openRadar.CreateRadarImage(RadarImage,openRadar.SepRadarRho,openRadar.SepRadarTheta);
			cvShowImage("SepRadar",RadarImage);

			

		

			/////////////////////////////////////////////////////////////////
			
			//openRadar.DrawRadarCorners(openRadar.Corners,RadarImage);
			openRadar.FitLine(openRadar.FittedLine,openRadar.FittedLine_save,openRadar.FittedLine_merge,openRadar.SepRadarRho,openRadar.SepRadarTheta, RadarImage);
        openRadar.DrawRadarLine(openRadar.FittedLine_save,RadarImage);
		//openRadar.DrawRadarLine(openRadar.FittedLine_merge,RadarImage);
			cvShowImage("SepRadarFittedLine_save",RadarImage);
			//openRadar.printRadarLine(openRadar.FittedLine);


			////////////////////////////////////////////////////////////

			openRadar.CreateRadarImage(RadarImage,openRadar.SepRadarRho,openRadar.SepRadarTheta);
			//openRadar.DrawRadarCorners(openRadar.Corners,RadarImage);
			openRadar.FitLine(openRadar.FittedLine,openRadar.FittedLine_save,openRadar.FittedLine_merge,openRadar.SepRadarRho,openRadar.SepRadarTheta,RadarImage);
		    //openRadar.DrawRadarCorners(openRadar.Corners,RadarImage);
			openRadar.DrawRadarLine(openRadar.FittedLine_merge,RadarImage);
			cvShowImage("SepRadarFittedLine_merge",RadarImage);



				///////////////////////////////////////////////////////////////////
			//openRadar.CreateRadarImage(RadarImage,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			////openRadar.CreateRadarImage(RadarImage, openRadar.RadarRho, openRadar.RadarTheta);
			//openRadar.FitLine(openRadar.FittedLine,openRadar.FittedLine_merge,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			//openRadar.DrawRadarLine(openRadar.FittedLine,RadarImage);
			//cvShowImage("BreakedRadarLine",RadarImage);
			////openRadar.printRadarLine(openRadar.FittedLine);

			/*
			///////////���Բ
			//openRadar.SelectRadarRho(openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			/*openRadar.FitArc(openRadar.FittedArc,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			//openRadar.DrawRadarArc(BreakedRadarImage,openRadar.FittedArc);
			//cvShowImage("BreakedRadar",BreakedRadarImage);
			openRadar.DrawRadarArc(RadarImage,openRadar.FittedArc);
			cvShowImage("ARC",RadarImage);*/





			
		//--------------------------------------------------------------------------------------------------------//
			
			//coord.CoortransTest();//����֪�����������߶μ���ת������������������ʵ�ʵĽ��жԱ�

			//ͨ��������Ϻõ�ֱ���޳�����--ʹ��FittedLine.size() == 2���Ӷ����ж�λ��������printf�޸�if (openRadar.FittedLine.size() == x)
			
			printf("openRadar.FittedLine.size=%d \n",openRadar.FittedLine.size());
			//printf("FittedLine.size=%d \n",openRadar.FittedLine_merge.size());
			//�����״��λ����̬
			if (openRadar.FittedLine.size() == 6)
			{
				if (openRadar.FittedLine.at(0).Rho < 0 
					&& openRadar.FittedLine.at(1).Rho > 0)
				{
					coord.CalCoorTransPara(coord.RadarCoordTransPara,
						FieldLine3,//FieldLine����ʹ�ò�ͬ����ϣ�������ƽ��
						FieldLine6,
						openRadar.FittedLine.at(0),//FittedLine.at(x)����ʹ�ò�ͬ����ϣ�������ƽ��
						openRadar.FittedLine.at(1));
					coord.printRadarCoordtransPara(coord.RadarCoordTransPara);

				}else if (openRadar.FittedLine.at(0).Rho > 0 
					&& openRadar.FittedLine.at(1).Rho < 0)
				{
					coord.CalCoorTransPara(coord.RadarCoordTransPara,
						FieldLine3,
						FieldLine6,
						openRadar.FittedLine.at(1),
						openRadar.FittedLine.at(0));
					coord.printRadarCoordtransPara(coord.RadarCoordTransPara);
				}else {
					cout<<"don't match"<<endl;
				}         
			}

			openRadar.DrawRadar(FieldImage,coord.RadarCoordTransPara);//����ת����������״���ȫ�������µĶ�λλ��
			////cvShowImage("Field",FieldImage);

		
		//--------------------------------------------------------------------------------------------------------//	



        key = cvWaitKey(10);
           
        if (key == 27)//esc�˳�
        {
			break;
            
        }
        frameCnt++;


		cvWaitKey(3000);//����ѭ�����ʱ���Ա��ȡ����
    }
    
  

	cvReleaseImage(&RadarImage);//�ͷ�radarimage ����
	cvDestroyWindow("Radar");//�رմ���
	
	cvDestroyWindow("SepRadar");//�رմ���
	cvDestroyWindow("SepRadarFitLine");//�رմ���
	cvDestroyWindow("SepRadarFitLine");//�رմ���
	
	cvDestroyWindow("BreakedRadarLine");//�رմ���
	


    return 0;
}


