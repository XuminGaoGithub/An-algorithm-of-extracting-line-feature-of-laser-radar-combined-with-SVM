
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
	//char fileName[32] = "csv\\6-lvbo.txt";//遇到干扰性（密度极大）的点群需要滤波
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
    
    while (access(fileName,0) == 0)///**如果想获取数据需要屏蔽while（），一共需要屏蔽四处////
    {

	
		 resizeAllWindow();////调节参数
        //sprintf(fileName,"csv\\data_%d.csv",frameCnt);
		sprintf(fileName,"csv\\6.txt",frameCnt);
		//sprintf(fileName,"csv\\6-lvbo.txt",frameCnt);
		
        openRadar.RadarRead(fileName);  



		/////////////////////////////////////////////////////////
		openRadar.MedFilter(openRadar.RadarRho,openRadar.RadarTheta);
			openRadar.CreateRadarImage(RadarImage,openRadar.RadarRho,openRadar.RadarTheta);  
			cvShowImage("Radar",RadarImage);
			
			//创建地图
			openRadar.CreateFieldImage(FieldImage);
			//cvShowImage("Field",FieldImage);

			//cvWaitKey(0);
			
			/////////////////////////////////////////////////////////////////
			openRadar.BreakRadarRho();
			openRadar.CreateRadarImage(RadarImage,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			//cvShowImage("BreakedRadar",RadarImage);
			//在此处添加必要的算法
			
			

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
			///////////拟合圆
			//openRadar.SelectRadarRho(openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			/*openRadar.FitArc(openRadar.FittedArc,openRadar.BreakedRadarRho,openRadar.BreakedRadarTheta);
			//openRadar.DrawRadarArc(BreakedRadarImage,openRadar.FittedArc);
			//cvShowImage("BreakedRadar",BreakedRadarImage);
			openRadar.DrawRadarArc(RadarImage,openRadar.FittedArc);
			cvShowImage("ARC",RadarImage);*/





			
		//--------------------------------------------------------------------------------------------------------//
			
			//coord.CoortransTest();//用已知的四条理想线段计算转换参数，可以与下面实际的进行对比

			//通过调节拟合好的直线剔除参数--使得FittedLine.size() == 2，从而进行定位。或者由printf修改if (openRadar.FittedLine.size() == x)
			
			printf("openRadar.FittedLine.size=%d \n",openRadar.FittedLine.size());
			//printf("FittedLine.size=%d \n",openRadar.FittedLine_merge.size());
			//计算雷达的位置姿态
			if (openRadar.FittedLine.size() == 6)
			{
				if (openRadar.FittedLine.at(0).Rho < 0 
					&& openRadar.FittedLine.at(1).Rho > 0)
				{
					coord.CalCoorTransPara(coord.RadarCoordTransPara,
						FieldLine3,//FieldLine可以使用不同的组合，但不能平行
						FieldLine6,
						openRadar.FittedLine.at(0),//FittedLine.at(x)可以使用不同的组合，但不能平行
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

			openRadar.DrawRadar(FieldImage,coord.RadarCoordTransPara);//根据转变参数画出雷达在全局坐标下的定位位置
			////cvShowImage("Field",FieldImage);

		
		//--------------------------------------------------------------------------------------------------------//	



        key = cvWaitKey(10);
           
        if (key == 27)//esc退出
        {
			break;
            
        }
        frameCnt++;


		cvWaitKey(3000);//控制循环间隔时间以便截取数据
    }
    
  

	cvReleaseImage(&RadarImage);//释放radarimage 对象
	cvDestroyWindow("Radar");//关闭窗口
	
	cvDestroyWindow("SepRadar");//关闭窗口
	cvDestroyWindow("SepRadarFitLine");//关闭窗口
	cvDestroyWindow("SepRadarFitLine");//关闭窗口
	
	cvDestroyWindow("BreakedRadarLine");//关闭窗口
	


    return 0;
}


