#include "OpenRadar.h"
#define pi 3.141592653
using namespace cv;


//int Dmax = 10;//����������Ч������Ϊ 10 �ף����ҽǶȷֱ���Ϊ 0.25�ȣ�������ɨ���֮��ľ�����СΪ��2��10m��sin(0.125��)=0.0436m�����ݸ�ֵ�����趨���ʵķָ���ֵ
int Dmax = 3;
int p=3,n=10;
int pn=5;
//int b=2;
int b=5;
int c=2;
int halfWindowSize = 2;
int h;
void resizeAllWindow()
{
	cvNamedWindow("��������", 0);
	cvResizeWindow("��������", 600, 600);

	/*cΪ��ͼ���Ķ�λ*/
	    cvCreateTrackbar("���Ķ�λ", "��������", &c, 10, 0); 

	cvCreateTrackbar("�˲�����", "��������", &halfWindowSize, 30, 0);

	/*����ָ���Dj,������ֵdelta,����Ϊ��(x,y)����������ķָ��*/
	cvCreateTrackbar("�ָֵ", "��������", &Dmax, 500, 0);

	/*����Ŀ�Ƚ��ٵ�����ֱ������*/
	cvCreateTrackbar("�ָ��޳�", "��������", &p, 300, 0);

	/*�ǵ���
	������һ�����ߣ�ֻ�е����ǵ㣬��ô���ǿ��Բ��ö������Ϸ�ʽȷ���ǵ��λ�á�
	���Ƚ������ڵĵ���ϳ�һ��ֱ�ߣ�Ȼ���ҳ���ֱ����Զ�ĵ㣬��������������ĳ����ֵ��
	�������Ϊ�����ߣ����õ�������ߵķָ�㣬���߾���һ��ֱ�ߡ�
	*/
	cvCreateTrackbar("�ǵ㷧ֵ", "��������", &n, 1000, 0);
	  
	/*pn ����Ŀ�㹻��ĵ��вŽ���ֱ�����*/
	    cvCreateTrackbar("ֱ���޳�", "��������", &pn, 360, 0);
		
		/*bΪ��ͼ���ڱ���*/
	    cvCreateTrackbar("��ͼ����", "��������", &b, 100, 0);

	
}


//int dx =  RadarImageWdith*2/4;
//int dy =  RadarImageHeight*3/4;

/*
//������ֵ�໥��ԼDmax�� pointCnt > 3��PolyContourFit(X , Y, pointCnt, 300)
//int dx =  RadarImageWdith/2;
//    int dy =  RadarImageHeight/2;

//theta = (90 + 90)*pi/180;//***������ϳ���ֱ�߽Ƕȣ�90Ϊɨ�跶Χ��һ��
      // deltaTeta = -0.5*pi/180;//***0.5(�״�ֱ���)������ϳ���ֱ�߱���
//��ͬ��Ⱥ��Ҫ��ͬ�Ĳ���
*/
OpenRadar::OpenRadar(void)
{
}
OpenRadar::~OpenRadar(void)
{
}
bool OpenRadar::RadarRead(char *fileName){
    FILE* fp = NULL;
    int rho = 0;
    double theta;
    double deltaTeta;
#ifdef UTM30LX
       /*theta = (135.0 + 90)*pi/180;
       deltaTeta = -0.25*pi/180;*/
	theta = (90 + 90)*pi/180;//***������ϳ���ֱ�߽Ƕȣ�90Ϊɨ�跶Χ��һ��
       deltaTeta = -0.5*pi/180;//***0.5(�״�ֱ���)������ϳ���ֱ�߱���
#elif defined URG04LX
    theta = (120.0 + 90)*pi/180;
    deltaTeta = -0.36*pi/180;
#endif

    int totalCnt = 0;
    fp = fopen(fileName,"r");
    if (fp == NULL)
    {
        //cout<<"failed to read"<<endl;
        return false;
    }else {
        //cout<<"successed to read"<<endl;
        RadarRho.clear();
        RadarTheta.clear();

        while(!feof(fp))
        {

            fscanf(fp, "%d, ", &rho);
            RadarRho.push_back(rho);
            RadarTheta.push_back(theta);
            theta += deltaTeta;
            //printf("%d  ", rho);
        }
        //cout<<"Total Count: "<<RadarRho.size()<<endl;
        //
    }
    fclose(fp);
    return true;
}

void OpenRadar::CreateRadarImage(IplImage* RadarImage,vector<int>& RadarRho,vector<double>& RadarTheta){
    //RadarImage = cvCreateImage(cvSize(RadarImageWdith,RadarImageHeight),IPL_DEPTH_8U,1);
    cvZero(RadarImage);
    //�����ļ���һ��Բ��
    //int dx  = RadarImageWdith/2;//***������޸�
    //int dy = RadarImageHeight/2;//***������޸�
    int dx =  RadarImageWdith*c/4;
    int dy =  RadarImageHeight*c/4;

    cvCircle(RadarImage, cvPoint(dx,dy),3, CV_RGB(0,255,255), -1, 8,0);

    int x,y;
    double theta,rho;
    unsigned char * pPixel = 0;
    


    //��ɫ
    int colorIndex = 0,colorRGB;
    int R = 255,G = 0,B = 0;
    int pointCnt = 0;
  

    for (int i = 0; i < RadarRho.size();i++)
    {
        theta = (-1)*(pointCnt/2+180)*pi/180;//***2��Ҫ��***-1��ʾ�״ﵹ��***180��Ҫ��
        //theta = RadarTheta.at(i);
        rho = RadarRho.at(i);

		printf("%f ",rho);

        if (rho < 0)
        {
           //�״����ݶϵ��־
            colorRGB = usualColor[colorIndex];
            R = colorRGB/65536;
            G = (colorRGB%65536)/256;
            B = colorRGB%256; 
            colorIndex = (colorIndex + 1)%10;
        }else {
            pointCnt++;
        }

        x = (int)(rho*cos(theta)/b) + dx;
        y = (int)(-rho*sin(theta)/b)+ dy;

        if (x >= 0 && x < RadarImageWdith && y >= 0 && y < RadarImageHeight)
        {
			//դ��
           //cvLine(RadarImage,cvPoint(dx,dy),cvPoint(x,y),CV_RGB(255,255,255),1,8,0);

			pPixel = (unsigned char*)RadarImage->imageData + y*RadarImage->widthStep + 3*x;
            pPixel[0] = B;
            pPixel[1] = G;
            pPixel[2] = R;
			
			
			//����һ�ֻ��Ƽ���ķ���
			//cvCircle(RadarImage, cvPoint(x,y),1, CV_RGB(255,0,0), -1, 8,0);
			
			




        }else{
            //cout<<"x: "<<x<<"  y: "<<y<<endl;
        }





    }

}
//�̶���ֵ�ָ�
//int OpenRadar::BreakRadarRho(){
//    int breakCnt = 0;
//    int rho = 0;
//    
//    int lastRho = RadarRho.at(0);
//    double theta = RadarTheta.at(0);
//    int dis = 0;//***
//   // double Dmax = 5;//***���Dj,������ֵdelta,����Ϊ��(x,y)����������ķָ��,��ֵ��Ӱ��ָ���ͼ����ת�Ƕ�
//
//    BreakedRadarRho.clear();
//    BreakedRadarTheta.clear();
//
//    BreakedRadarRho.push_back(lastRho);
//    BreakedRadarTheta.push_back(theta);
//
//    for (int i = 1; i< RadarRho.size();i++)
//    {
//       rho = RadarRho.at(i);
//       theta = RadarTheta.at(i);
//
//       BreakedRadarRho.push_back(rho);
//       BreakedRadarTheta.push_back(theta);
//       dis = abs(rho - lastRho);
//       if (dis < Dmax)
//       {
//          
//       }else {
//          BreakedRadarRho.push_back(-1);
//          BreakedRadarTheta.push_back(361);
//          breakCnt++;
//       }
//
//       lastRho = rho;
//    }
//    BreakedRadarRho.push_back(-1);
//    BreakedRadarTheta.push_back(361);
//    //cout<<"breakCnt: "<<breakCnt<<endl;
//    return breakCnt;
//}

//��̬��ֵ�ָ�
int OpenRadar::BreakRadarRho() {
	int breakCnt = 0;
	int  rho = 0;

	int lastRho = RadarRho.at(0);
	double theta = RadarTheta.at(0);
	int dis = 0;
	//int Dmax =30;//***************************************************************//80

	BreakedRadarRho.clear();
	BreakedRadarTheta.clear();

	BreakedRadarRho.push_back(lastRho);
	BreakedRadarTheta.push_back(theta);

	for (int i = 1; i < RadarRho.size(); i++)
	{
		try
		{
			theta = RadarTheta.at(i);
			rho = RadarRho.at(i);
		}
		catch (std::out_of_range & exc)
		{
			std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << endl;
		}

		

		BreakedRadarRho.push_back(rho);
		BreakedRadarTheta.push_back(theta);
		dis = abs(rho - lastRho);
		
		//��̬��ֵ����ָ�ֵ
		/*���統ĳ��ɨ����봫�������ĵľ���Ϊ D ʱ���ָ���ֵѡ��Ϊ d��
		��ɨ����봫�������ĵľ���Ϊ3D ʱ����ֵѡ��Ϊ 3d
		*/
		 h=rho/100;
		//printf("%d \n",h);
		float D=Dmax;
		D=(h+0.5)*D;
		//printf("%f \n",D);
		if (dis < D)
		
		
		/*printf("%d \n",Dmax);*/
		/*if (dis < Dmax)*/
		{

		}
		else {
			BreakedRadarRho.push_back(-1);
			BreakedRadarTheta.push_back(1000.0);
			breakCnt++;
		}

		lastRho = rho;
	}
	BreakedRadarRho.push_back(-1);
	BreakedRadarTheta.push_back(1000.0);
	//cout<<"breakCnt: "<<breakCnt<<endl;
	return breakCnt;
}

// ���ж������ϣ� Points : �����ϵĵ�      n -- ��������Ŀ  Eps -- ��Ͼ���
// ����ֵ�� ������������Ҫ�ֶΣ��򷵻طֶε��ڸ����������е����������򣬷��� 0 ��ʾ����Ҫ�ֶ�
// �����������㷨���㸴��������һ���ط�
// Ϊ����߳�������Ч�ʣ��Ե㵽ֱ�ߵľ��������иĽ���
// ���������е�ֱ�����ɵ����еĵ������
// Ϊ�˼���㵽ֱ�ߵľ��룬
// ��������ϵ��ת����ֱ����ת��x�᷽�������㵽ֱ�ߵľ��뼴Ϊ������
// ��������ת���yֵ�ľ���ֵ
// ͬʱ��������ת�����ڸô�������Ϊ��ֵ��ֻ��һ�μ��㣬����Ҫ��εĿ��������Ǽ���
int OpenRadar::PolyContourFit( int* X, int* Y, int n , float Eps ) // ���������㣬�ö������ϸ�������    
{
    double dis = sqrt((double)(((X[0] - X[n - 1])*(X[0] - X[n - 1])) +  
                     ((Y[0] - Y[n - 1])* (Y[0] - Y[n - 1]))));
    double cosTheta = (X[n- 1] - X[0]) / dis;
    double sinTheta = - ( Y[n- 1] - Y[0] )/dis;
    double MaxDis = 0;
    int i ;
    int MaxDisInd = -1;
    double dbDis;
    for(i = 1 ; i < n - 1 ; i++)
    {
        // ����������ת������ת��ĵ㵽x��ľ���
        dbDis = abs( (Y[i] - Y[0]) * cosTheta + (X[i] - X[0])* sinTheta);
        if( dbDis > MaxDis)
        {
            MaxDis = dbDis;
            MaxDisInd = i;
        }
    }
    if(MaxDis > Eps)
    {
        return MaxDisInd;
               /* cout << "Line 1 : " << endl;
                cout << "Start :" << Points[0].x << "  " << Points[0].y  << " --- " << Points[MaxDisInd].x << "  " << Points[MaxDisInd].y << endl;
                cout << "�Ƕȣ� "<<180 * atan2(Points[0].y - Points[MaxDisInd].y , Points[0].x - Points[MaxDisInd].x ) / 3.1415926;
                cout << "Line 2 :" << endl;
                cout << "Start :" << Points[MaxDisInd].x << "  " << Points[MaxDisInd].y  << " --- " << Points[n - 1].x << "  " << Points[n - 1].y << endl;
                cout << "�Ƕȣ� "<< 180 * atan2(Points[n - 1].y - Points[MaxDisInd].y , Points[n - 1].x - Points[MaxDisInd].x ) / 3.1415926;*/
    }
       /* else{
            cout << "Line 1 : " << endl;
            cout << "Start :" << Points[0].x << "  " << Points[0].y  << " --- " << Points[n - 1].x << "  " << Points[n - 1].y << endl;
            cout << "�Ƕȣ� "<<180 * atan2(Points[n - 1].y - Points[0].y , Points[n - 1].x - Points[0].x ) / 3.1415926;

        }*/
    return 0;
}














//�����߲������
int OpenRadar::BreakPolyLine(vector<int>& BreakedRadarRho,
                             vector<double>& BreakedRadarTheta,
                             vector<int>& SepRadarRho ,   
                             vector<double>&SepRadarTheta)
{
    int rho = 0;
    double theta = 0.0;
    int X[1200] = {0};
    int Y[1200] = {0};
    int rhoCopy[1200] = {0};
    double thetaCopy[1200] = {0};
    int pointCnt = 0;
    int lineCnt = 0;
    int N = 0;
    SepRadarRho.clear();
    SepRadarTheta.clear();
    Corners.clear();

    //���ж�ε����������е����߶���ֳ�ֱ�߶�
   
   vector<int>CornerIndex;
   int CornerCnt = 0;
   int tempIndex = 0;
    for (int i = 0; i < static_cast<int>(BreakedRadarRho.size());i++)
    {
        rho   = BreakedRadarRho.at(i);
        theta = BreakedRadarTheta.at(i);

        if (rho < 0)
        {
            if (pointCnt > p)//��Ŀ�Ƚ��ٵĵ�ֱ������
            {
                CornerIndex.clear();
                CornerCnt = FindCorners(CornerIndex,X,Y,0,pointCnt,n);///////////////////////////////////////////n

                if (CornerIndex.size() == 0)
                {
                    for (int k = 0 ; k < pointCnt;k++)
                    {
                        SepRadarRho.push_back(rhoCopy[k]);
                        SepRadarTheta.push_back(thetaCopy[k]);
                    }
                    SepRadarRho.push_back(-1);
                    SepRadarTheta.push_back(1000.0);
                    lineCnt++;
                }else
                {
                    tempIndex = 0;
                    for (int k = 0 ; k < pointCnt;k++)
                    {
                        SepRadarRho.push_back(rhoCopy[k]);
                        SepRadarTheta.push_back(thetaCopy[k]);
                        if (k == CornerIndex.at(tempIndex))
                        {
                            SepRadarRho.push_back(-1);
                            SepRadarTheta.push_back(1000.0);
                            lineCnt++;
                            if (tempIndex < static_cast<int>(CornerIndex.size()) -1)
                            {
                                tempIndex++;
                            }  
                        }
                    }
                    SepRadarRho.push_back(-1);
                    SepRadarTheta.push_back(1000.0);
                    lineCnt++;
                }
            }
            pointCnt = 0;
            continue;
        }
        X[pointCnt] = static_cast<int>(rho*cos(theta));
        Y[pointCnt] = static_cast<int>(rho*sin(theta));
        rhoCopy[pointCnt]   = rho;
        thetaCopy[pointCnt] = theta;
        pointCnt++;
    }
    
    //cout<<"lineCnt: "<<lineCnt<<endl;
    return lineCnt;
}

//����ֱ�����
void OpenRadar::FitLine(vector<LinePara>& FittedLine,vector<LinePara>& FittedLine_save,vector<LinePara>& FittedLine_merge,vector<int>& RadarRho,vector<double>& RadarTheta,IplImage* RadarImage){
	
	int dx =  RadarImageWdith*c/4;
    int dy =  RadarImageHeight*c/4;
	int colorIndex = 0,colorRGB;
	int x1,y1,x2,y2;
    int rho = 0;
	int R = 255,G = 0,B = 0;
    double theta = 0.0;
    int X[1200] = {0};
    int Y[1200] = {0};
    int pointCnt = 0;
    LinePara tmpLinePara;
	LinePara tmpLinePara0;
	LinePara  merge;
    FittedLine.clear();
	 FittedLine_merge.clear();
    for (int i = 0 ; i < static_cast<int>(RadarRho.size());i++)
    {
        rho = RadarRho.at(i);
        theta = RadarTheta.at(i);

        if (rho < 0)
        {
            if (pointCnt > pn )//����Ŀ�㹻��ĵ��вŽ���ֱ�����
            {
                WeightedFit(X ,Y ,pointCnt,&tmpLinePara);
                FittedLine.push_back(tmpLinePara);

                //�洢��ϵ����ݺ���Ͻ��
               /*FILE* pF = fopen("line.txt","w");
                fprintf(pF,"[a]\n");
                fprintf(pF,"%f\n",tmpLinePara.a);
                fprintf(pF,"[b]\n");
                fprintf(pF,"%f\n",tmpLinePara.b);
                fprintf(pF,"[x]\n");
                for (int j = 0; j < pointCnt ;j++)
                {
                    fprintf(pF,"%d,",X[j]);
                }
                fprintf(pF,"\n[y]\n");
                for (int j = 0; j < pointCnt ;j++)
                {
                    fprintf(pF,"%d,",Y[j]);
                }
                fclose(pF);*/

                pointCnt = 0;
                continue;
            }else {
                pointCnt = 0;
                continue;
            }   
        }

        X[pointCnt] = static_cast<int>(rho*cos(theta));
        Y[pointCnt] = static_cast<int>(rho*sin(theta));
        pointCnt++;
    }


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	printf("FittedLine.size=%d \n",FittedLine.size());///�����ã�ע��ʹ��BreakedRadarLine��SepRadarFittedLine_corn�����ֵ��һ��
	FittedLine_save.assign(FittedLine.begin(), FittedLine.end());
	
//--------------------------------------------------------------------------------------------------------//
	//////////////////////////////////�ϲ�����////////////////////////////////////////////////
	predict1(tmpLinePara,tmpLinePara0, merge, merge);
	ofstream ofs("data_0.txt");


	
 for (int i = 0; i < FittedLine.size()-2;i++)
 {                 
	                 s0=0;s1=0;s2=0;s3=0;
	                 /////////////////////������
	                 s0=d0(FittedLine.at(i),FittedLine.at(i+1));
				     cout<<"s0����Ϊ��"<<s0<<endl;
		            s1=d1(FittedLine_save.at(i).endPoint.x,FittedLine_save.at(i).endPoint.y,FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y);
		            cout<<"s1����Ϊ��"<<s1<<endl;
					// s2=d2(FittedLine_save.at(i),FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y,FittedLine_save.at(i+1).endPoint.x,FittedLine_save.at(i+1).endPoint.y);
		            //cout<<"s2����Ϊ��"<<s2<<endl;
					s3=d3(FittedLine_save.at(i),FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y,FittedLine_save.at(i+1).endPoint.x,FittedLine_save.at(i+1).endPoint.y);
		            cout<<"s3����Ϊ��"<<s3<<endl;
	                //s=(d(FittedLine.at(i),FittedLine.at(i+1),FittedLine_save.at(i),FittedLine_save.at(i+1)));
					//cout<<"sΪ��"<<s<<endl;
	               s=s0*s1*s3;
					cout<<"s����Ϊ��"<<s<<endl;

					 //�����Ҫ��ȡ��������Ҫ�������������while
					 ofs<< setprecision(6)<<s0<<",";
					 ofs<< setprecision(6)<<s1<<",";
					 //ofs<< setprecision(6)<<s2<<",";
					 ofs<< setprecision(6)<<s3<<",";
					 ofs<< setprecision(6)<<s<<",";
					 ofs<< endl;
					 /////////////////////////







	 //if( //(abs((FittedLine.at(i).Rho*180/PI)-(FittedLine.at(i+1).Rho*180/PI))<=10)
		//    (d0(FittedLine.at(i),FittedLine.at(i+1))<=10)
		// && (d1(FittedLine_save.at(i).endPoint.x,FittedLine_save.at(i).endPoint.y,FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y))<=20
		// && (d2(FittedLine_save.at(i),FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y,FittedLine_save.at(i+1).endPoint.x,FittedLine_save.at(i+1).endPoint.y))<=0.5
		// && ((d3(FittedLine_save.at(i),FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y,FittedLine_save.at(i+1).endPoint.x,FittedLine_save.at(i+1).endPoint.y))<=1
		//      &&(d3(FittedLine_save.at(i),FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y,FittedLine_save.at(i+1).endPoint.x,FittedLine_save.at(i+1).endPoint.y))>=0.9
		//	)
	 //  )

	 if(d(FittedLine.at(i),FittedLine.at(i+1),FittedLine_save.at(i),FittedLine_save.at(i+1))==1)//����������ж����Ϊһ������
       //if(d(FittedLine_save.at(i),FittedLine_save.at(i+1),FittedLine_save.at(i),FittedLine_save.at(i+1))==1)
			//if( abs((atan(FittedLine.at(i).a)*180/PI)-(atan(FittedLine.at(i+1).a)*180/PI))<10)
	 {             
		              ////////////////////////////������
		            
					 s0=d0(FittedLine.at(i),FittedLine.at(i+1));
				     cout<<"s0����Ϊ��"<<s0<<endl;
		            s1=d1(FittedLine_save.at(i).endPoint.x,FittedLine_save.at(i).endPoint.y,FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y);
		            cout<<"s1����Ϊ��"<<s1<<endl;
					//s2=d2(FittedLine_save.at(i),FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y,FittedLine_save.at(i+1).endPoint.x,FittedLine_save.at(i+1).endPoint.y);
		            //cout<<"s2����Ϊ��"<<s2<<endl;
					s3=d3(FittedLine_save.at(i),FittedLine_save.at(i+1).startPoint.x,FittedLine_save.at(i+1).startPoint.y,FittedLine_save.at(i+1).endPoint.x,FittedLine_save.at(i+1).endPoint.y);
		            cout<<"s3����Ϊ��"<<s3<<endl;
		            //s=(d(FittedLine.at(i),FittedLine.at(i+1),FittedLine_save.at(i),FittedLine_save.at(i+1)));
					//cout<<"sΪ��"<<s<<endl;
	               s=s0*s1*s3;
				   cout<<"s����Ϊ��"<<s<<endl;
				     ofs<< endl;
				     ofs<< endl;
					 ofs<< "merge start:"<<endl;
		             ofs<< setprecision(6)<<s0<<",";
					 ofs<< setprecision(6)<<s1<<",";
					 //ofs<< setprecision(6)<<s2<<",";
					 ofs<< setprecision(6)<<s3<<",";
					 ofs<< setprecision(6)<<s<<","<<endl;;
					 ofs<< "merge finish!"<<endl;
					 ofs<< endl;
					 ofs<< endl;
					///////////////////////////////////////


					//merge.a=(FittedLine.at(i).startPoint.y-FittedLine.at(i+1).endPoint.y)/(FittedLine.at(i).startPoint.x-FittedLine.at(i+1).endPoint.x);
					//merge.b=FittedLine.at(i).startPoint.y-merge.a*FittedLine.at(i).startPoint.x;
					merge.a=(FittedLine.at(i).a+FittedLine.at(i+1).a)/2;
					merge.b=FittedLine.at(i).startPoint.y-merge.a*FittedLine.at(i).startPoint.x;
							  int FlagFlip = 0;// �Ƿ��X,Y���з�ת��
    
								if( merge.a>0 )
								{
        
									FlagFlip = 1;  // ��ת
								}
    
								if(FlagFlip == 0)
								{
									// δ��ת
									merge.Rho = atan(merge.a);
								}
								else
								{
									// ��ת��
									if(abs(merge.a) < 0.00001)
									{
										merge.a = 100000;
									}	     
									else
									{
										//merge.a =1.0/ merge.a;
									}	 
									//merge.b = - merge.b * (merge.a);
									merge.Rho = atan(merge.a);
								}

								//X Y����ת�����ٷ�ת��ȥ
   
								//�����߶ε������˵�
							   if (abs(merge.a) >= 1)
							   {
								   merge.startPoint.y = FittedLine.at(i).startPoint.y;
								   merge.startPoint.x = (FittedLine.at(i).startPoint.y - merge.b)/merge.a;
        
								   merge.endPoint.y = FittedLine.at(i+1).endPoint.y;
								   merge.endPoint.x = (FittedLine.at(i+1).endPoint.y - merge.b)/merge.a;
							   }else {
								   merge.startPoint.x = FittedLine.at(i).startPoint.x;
								   merge.startPoint.y = merge.a* FittedLine.at(i).startPoint.x + merge.b;

								   merge.endPoint.x = FittedLine.at(i+1).endPoint.x;
								  merge.endPoint.y = merge.a* FittedLine.at(i+1).endPoint.x + merge.b;
							   }

  
					              FittedLine.at(i+1)=merge;
										

						//if( abs((FittedLine.at(i+1).Rho*180/PI)-(FittedLine.at(i+2).Rho*180/PI))<=10)
                        if(d(FittedLine.at(i+1),FittedLine.at(i+2),FittedLine_save.at(i+1),FittedLine_save.at(i+2))==1)
						//if(d(FittedLine_save.at(i+1),FittedLine_save.at(i+2),FittedLine_save.at(i+1),FittedLine_save.at(i+2))==1)
						{

							/*if(i==FittedLine.size()-3)
							{FittedLine_merge.push_back(FittedLine.at(i+1));}
							else{}*/

						}
						else 
						{ 
											
							//FittedLine_merge.push_back(FittedLine.at(i+1));
							FittedLine_merge.push_back(merge);
										
						}
									

                               //FittedLine_merge.push_back(merge);
		}

				

	else
	   {
								//if((i>1)&&(abs((FittedLine.at(i-1).Rho*180/PI)-(FittedLine.at(i).Rho*180/PI))<10))
								if((i>1)&&d(FittedLine.at(i-1),FittedLine.at(i),FittedLine_save.at(i-1),FittedLine_save.at(i))==1)
								//if((i>1)&&d(FittedLine_save.at(i-1),FittedLine_save.at(i),FittedLine_save.at(i-1),FittedLine_save.at(i))==1)
								{


									//if(abs((FittedLine.at(i+1).Rho*180/PI)-(FittedLine.at(i+2).Rho*180/PI))<10)
                                    if(d(FittedLine.at(i+1),FittedLine.at(i+2),FittedLine_save.at(i+1),FittedLine_save.at(i+2))==1)
									//if(d(FittedLine_save.at(i+1),FittedLine_save.at(i+2),FittedLine_save.at(i+1),FittedLine_save.at(i+2))==1)
									{      } 
									else {
									merge=FittedLine_save.at(i+1);
									i++;
									//printf("i=%d \n",i);
									}
									FittedLine_merge.push_back(merge);
								}

								else if((i==1)&&(d(FittedLine.at(i-1),FittedLine.at(i),FittedLine_save.at(i-1),FittedLine_save.at(i))==1))
								//else if((i==1)&&(d(FittedLine_save.at(i-1),FittedLine_save.at(i),FittedLine_save.at(i-1),FittedLine_save.at(i))==1))
								{}
					
								else
								{merge=FittedLine.at(i);
								FittedLine_merge.push_back(merge);
								}
			
							//FittedLine_merge.push_back(merge);
				

       }
				
		
  }

                 ////////////////////////////������
					 s0=d0(FittedLine.at(FittedLine.size()-3),FittedLine.at(FittedLine.size()-2));
				     cout<<"s0����Ϊ��"<<s0<<endl;
		            s1=d1(FittedLine_save.at(FittedLine_save.size()-3).endPoint.x,FittedLine_save.at(FittedLine_save.size()-3).endPoint.y,FittedLine_save.at(FittedLine_save.size()-2).startPoint.x,FittedLine_save.at(FittedLine_save.size()-2).startPoint.y);
		            cout<<"s1����Ϊ��"<<s1<<endl;
					//s2=d2(FittedLine_save.at(FittedLine.size()-3),FittedLine_save.at(FittedLine.size()-2).startPoint.x,FittedLine_save.at(FittedLine.size()-2).startPoint.y,FittedLine_save.at(FittedLine.size()-2).endPoint.x,FittedLine_save.at(FittedLine.size()-2).endPoint.y);
		            //cout<<"s2����Ϊ��"<<s2<<endl;
					s3=d3(FittedLine_save.at(FittedLine_save.size()-3),FittedLine_save.at(FittedLine_save.size()-2).startPoint.x,FittedLine_save.at(FittedLine_save.size()-2).startPoint.y,FittedLine_save.at(FittedLine_save.size()-2).endPoint.x,FittedLine_save.at(FittedLine_save.size()-2).endPoint.y);
		            cout<<"s3����Ϊ��"<<s3<<endl;
		            //s=(d(FittedLine.at(i),FittedLine.at(i+1),FittedLine_save.at(i),FittedLine_save.at(i+1)));
					//cout<<"sΪ��"<<s<<endl;
	               s=s0*s1*s3;
					cout<<"s����Ϊ��"<<s<<endl;

		             ofs<< setprecision(6)<<s0<<",";
					 ofs<< setprecision(6)<<s1<<",";
					 //ofs<< setprecision(6)<<s2<<",";
					 ofs<< setprecision(6)<<s3<<",";
					 ofs<< setprecision(6)<<s<<",";
					 ofs<< endl;

					 s0=d0(FittedLine.at(FittedLine.size()-2),FittedLine.at(FittedLine.size()-1));
				     cout<<"s0����Ϊ��"<<s0<<endl;
		            s1=d1(FittedLine_save.at(FittedLine_save.size()-2).endPoint.x,FittedLine_save.at(FittedLine_save.size()-2).endPoint.y,FittedLine_save.at(FittedLine_save.size()-1).startPoint.x,FittedLine_save.at(FittedLine_save.size()-1).startPoint.y);
		            cout<<"s1����Ϊ��"<<s1<<endl;
					//s2=d2(FittedLine_save.at(FittedLine_save.size()-2),FittedLine_save.at(FittedLine_save.size()-1).startPoint.x,FittedLine_save.at(FittedLine_save.size()-1).startPoint.y,FittedLine_save.at(FittedLine_save.size()-1).endPoint.x,FittedLine_save.at(FittedLine_save.size()-1).endPoint.y);
		            //cout<<"s2����Ϊ��"<<s2<<endl;
					s3=d3(FittedLine_save.at(FittedLine_save.size()-2),FittedLine_save.at(FittedLine_save.size()-1).startPoint.x,FittedLine_save.at(FittedLine_save.size()-1).startPoint.y,FittedLine_save.at(FittedLine_save.size()-1).endPoint.x,FittedLine_save.at(FittedLine_save.size()-1).endPoint.y);
		            cout<<"s3����Ϊ��"<<s3<<endl;
		            //s=(d(FittedLine.at(i),FittedLine.at(i+1),FittedLine_save.at(i),FittedLine_save.at(i+1)));
					//cout<<"sΪ��"<<s<<endl;
	               s=s0*s1*s3;
					cout<<"s����Ϊ��"<<s<<endl;

		             ofs<< setprecision(6)<<s0<<",";
					 ofs<< setprecision(6)<<s1<<",";
					//ofs<< setprecision(6)<<s2<<",";
					 ofs<< setprecision(6)<<s3<<",";
					 ofs<< setprecision(6)<<s<<",";
					 ofs<< endl;

					/////////////////////////////////////////

  ofs.close();



           ////////////////////�������Ԫ�ش���////////////////////////

//if( abs((FittedLine.at(FittedLine.size()-3).Rho*180/PI)-(FittedLine.at(FittedLine.size()-2).Rho*180/PI))<=10)
  if(d(FittedLine.at(FittedLine.size()-3),FittedLine.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-3),FittedLine_save.at(FittedLine.size()-2))==1)
 //if(d(FittedLine_save.at(FittedLine.size()-3),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-3),FittedLine_save.at(FittedLine.size()-2))==1)

{
		//if( abs((FittedLine.at(i).Rho*180/PI)-(FittedLine.at(i+1).Rho*180/PI))<=10)	
		merge.a=(FittedLine.at(FittedLine.size()-3).a+FittedLine.at(FittedLine.size()-2).a)/2;
		merge.b=FittedLine.at(FittedLine.size()-3).startPoint.y-merge.a*FittedLine.at(FittedLine.size()-3).startPoint.x;

					int FlagFlip = 0;// �Ƿ��X,Y���з�ת��
    
						if( merge.a>0 )
						{
        
							FlagFlip = 1;  // ��ת
						}
    
						if(FlagFlip == 0)
						{
							// δ��ת
							merge.Rho = atan(merge.a);
						}
						else
						{
							// ��ת��
							if(abs(merge.a) < 0.00001)
							{
								merge.a = 100000;
							}	     
							else
							{
								//merge.a =1.0/ merge.a;
							}	 
							//merge.b = - merge.b * (merge.a);
							merge.Rho = atan(merge.a);
						}

						//X Y����ת�����ٷ�ת��ȥ
   
						//�����߶ε������˵�
					   if (abs(merge.a) >= 1)
					   {
						   merge.startPoint.y = FittedLine.at(FittedLine.size()-3).startPoint.y;
						   merge.startPoint.x = (FittedLine.at(FittedLine.size()-3).startPoint.y - merge.b)/merge.a;
        
						   merge.endPoint.y = FittedLine.at(FittedLine.size()-2).endPoint.y;
						   merge.endPoint.x = (FittedLine.at(FittedLine.size()-2).endPoint.y - merge.b)/merge.a;
					   }else {
						   merge.startPoint.x = FittedLine.at(FittedLine.size()-3).startPoint.x;
						   merge.startPoint.y = merge.a* FittedLine.at(FittedLine.size()-3).startPoint.x + merge.b;

						   merge.endPoint.x = FittedLine.at(FittedLine.size()-2).endPoint.x;
						  merge.endPoint.y = merge.a* FittedLine.at(FittedLine.size()-2).endPoint.x + merge.b;
					   }

							  FittedLine.at(FittedLine.size()-2)=merge;
          
				  //if( abs((FittedLine.at(FittedLine.size()-2).Rho*180/PI)-(FittedLine.at(FittedLine.size()-1).Rho*180/PI))<=10)
                  if(d(FittedLine.at(FittedLine.size()-2),FittedLine.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==1)
				   // if(d(FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==1)
                        {
							merge.a=(FittedLine.at(FittedLine.size()-2).a+FittedLine.at(FittedLine.size()-1).a)/2;
							merge.b=FittedLine.at(FittedLine.size()-2).startPoint.y-merge.a*FittedLine.at(FittedLine.size()-2).startPoint.x;

								int FlagFlip = 0;// �Ƿ��X,Y���з�ת��
    
								if( merge.a>0 )
							{
        
								FlagFlip = 1;  // ��ת
							}
    
								if(FlagFlip == 0)
							{
								// δ��ת
								merge.Rho = atan(merge.a);
								}
							else
							{
								// ��ת��
								if(abs(merge.a) < 0.00001)
								{
									merge.a = 100000;
								}	     
								else
									{
									//merge.a =1.0/ merge.a;
									}	 
								//merge.b = - merge.b * (merge.a);
								merge.Rho = atan(merge.a);
								}

							//X Y����ת�����ٷ�ת��ȥ
   
							//�����߶ε������˵�
								if (abs(merge.a) >= 1)
								{
								merge.startPoint.y = FittedLine.at(FittedLine.size()-2).startPoint.y;
								merge.startPoint.x = (FittedLine.at(FittedLine.size()-2).startPoint.y - merge.b)/merge.a;
        
								merge.endPoint.y = FittedLine.at(FittedLine.size()-1).endPoint.y;
								merge.endPoint.x = (FittedLine.at(FittedLine.size()-1).endPoint.y - merge.b)/merge.a;
								}else {
								merge.startPoint.x = FittedLine.at(FittedLine.size()-2).startPoint.x;
								merge.startPoint.y = merge.a* FittedLine.at(FittedLine.size()-2).startPoint.x + merge.b;

								merge.endPoint.x = FittedLine.at(FittedLine.size()-1).endPoint.x;
								merge.endPoint.y = merge.a* FittedLine.at(FittedLine.size()-1).endPoint.x + merge.b;
								}
									FittedLine_merge.push_back(merge);
					}
				else 
				   {

								//FittedLine_merge.push_back(merge);
								//FittedLine_merge.push_back(FittedLine_save.back());
					               

								/* merge=FittedLine.at(FittedLine.size()-2);
								FittedLine_merge.push_back(merge);*/
								FittedLine_merge.push_back(FittedLine.back());
				  }	

 }

else
{
	//if(abs((FittedLine.at(FittedLine.size()-4).Rho*180/PI)-(FittedLine.at(FittedLine.size()-3).Rho*180/PI))>10)
	if(d(FittedLine.at(FittedLine.size()-4),FittedLine.at(FittedLine.size()-3),FittedLine_save.at(FittedLine.size()-4),FittedLine_save.at(FittedLine.size()-3))==1)
	//if(d(FittedLine_save.at(FittedLine.size()-4),FittedLine_save.at(FittedLine.size()-3),FittedLine_save.at(FittedLine.size()-4),FittedLine_save.at(FittedLine.size()-3))==1)
	{
	                            
	             //if( abs((FittedLine.at(FittedLine.size()-2).Rho*180/PI)-(FittedLine.at(FittedLine.size()-1).Rho*180/PI))<=10)
		           if(d(FittedLine.at(FittedLine.size()-2),FittedLine.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==1)
		           //if(d(FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==1)
				   { 
		                            merge=FittedLine.at(FittedLine.size()-4);
								    FittedLine_merge.push_back(merge);


									merge.a=(FittedLine.at(FittedLine.size()-2).a+FittedLine.at(FittedLine.size()-1).a)/2;
									merge.b=FittedLine.at(FittedLine.size()-2).startPoint.y-merge.a*FittedLine.at(FittedLine.size()-2).startPoint.x;

									 int FlagFlip = 0;// �Ƿ��X,Y���з�ת��
    
									 if( merge.a>0 )
									{
        
									   FlagFlip = 1;  // ��ת
									}
    
									 if(FlagFlip == 0)
								  {
									  // δ��ת
									 merge.Rho = atan(merge.a);
									  }
								   else
								   {
									  // ��ת��
										if(abs(merge.a) < 0.00001)
									  {
										 merge.a = 100000;
									  }	     
									  else
										  {
										  // merge.a =1.0/ merge.a;
										  }	 
										// merge.b = - merge.b * (merge.a);
										merge.Rho = atan(merge.a);
									 }

							//X Y����ת�����ٷ�ת��ȥ
   
						 //�����߶ε������˵�
							 if (abs(merge.a) >= 1)
							 {
								merge.startPoint.y = FittedLine.at(FittedLine.size()-2).startPoint.y;
								merge.startPoint.x = (FittedLine.at(FittedLine.size()-2).startPoint.y - merge.b)/merge.a;
        
							   merge.endPoint.y = FittedLine.at(FittedLine.size()-1).endPoint.y;
							   merge.endPoint.x = (FittedLine.at(FittedLine.size()-1).endPoint.y - merge.b)/merge.a;
							 }else {
								merge.startPoint.x = FittedLine.at(FittedLine.size()-2).startPoint.x;
								merge.startPoint.y = merge.a* FittedLine.at(FittedLine.size()-2).startPoint.x + merge.b;

								merge.endPoint.x = FittedLine.at(FittedLine.size()-1).endPoint.x;
								merge.endPoint.y = merge.a* FittedLine.at(FittedLine.size()-1).endPoint.x + merge.b;
							 }

								FittedLine_merge.push_back(merge);
		          }

			    //else if(abs((FittedLine.at(FittedLine.size()-2).Rho*180/PI)-(FittedLine.at(FittedLine.size()-1).Rho*180/PI))>10)
                else if(d(FittedLine.at(FittedLine.size()-2),FittedLine.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==0)
				//else if(d(FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==0)
				   {
					           
					           /*merge=FittedLine.at(FittedLine.size()-2);
								FittedLine_merge.push_back(merge);*/
								merge=FittedLine.at(FittedLine.size()-1);
								FittedLine_merge.push_back(merge);
				 }
	}
	else
	{ 

		//if(d(FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==1)
     if(d(FittedLine.at(FittedLine.size()-2),FittedLine.at(FittedLine.size()-1),FittedLine_save.at(FittedLine.size()-2),FittedLine_save.at(FittedLine.size()-1))==1)
		 {                          merge.a=(FittedLine.at(FittedLine.size()-2).a+FittedLine.at(FittedLine.size()-1).a)/2;
									merge.b=FittedLine.at(FittedLine.size()-2).startPoint.y-merge.a*FittedLine.at(FittedLine.size()-2).startPoint.x;

									 int FlagFlip = 0;// �Ƿ��X,Y���з�ת��
    
									 if( merge.a>0 )
									{
        
									   FlagFlip = 1;  // ��ת
									}
    
									 if(FlagFlip == 0)
								  {
									  // δ��ת
									 merge.Rho = atan(merge.a);
									  }
								   else
								   {
									  // ��ת��
										if(abs(merge.a) < 0.00001)
									  {
										 merge.a = 100000;
									  }	     
									  else
										  {
										  // merge.a =1.0/ merge.a;
										  }	 
										// merge.b = - merge.b * (merge.a);
										merge.Rho = atan(merge.a);
									 }

							//X Y����ת�����ٷ�ת��ȥ
   
						 //�����߶ε������˵�
							 if (abs(merge.a) >= 1)
							 {
								merge.startPoint.y = FittedLine.at(FittedLine.size()-2).startPoint.y;
								merge.startPoint.x = (FittedLine.at(FittedLine.size()-2).startPoint.y - merge.b)/merge.a;
        
							   merge.endPoint.y = FittedLine.at(FittedLine.size()-1).endPoint.y;
							   merge.endPoint.x = (FittedLine.at(FittedLine.size()-1).endPoint.y - merge.b)/merge.a;
							 }else {
								merge.startPoint.x = FittedLine.at(FittedLine.size()-2).startPoint.x;
								merge.startPoint.y = merge.a* FittedLine.at(FittedLine.size()-2).startPoint.x + merge.b;

								merge.endPoint.x = FittedLine.at(FittedLine.size()-1).endPoint.x;
								merge.endPoint.y = merge.a* FittedLine.at(FittedLine.size()-1).endPoint.x + merge.b;
							 }

								FittedLine_merge.push_back(merge);
		          }
		else{
			merge=FittedLine.at(FittedLine.size()-2);
			FittedLine_merge.push_back(merge);
			merge=FittedLine.at(FittedLine.size()-1);
			FittedLine_merge.push_back(merge);
		  }   
    }

}

                 
             ////////////////////////////////////////////////// 


      //////////////////////////////////�ϲ�����////////////////////////////////////////////////
	 //--------------------------------------------------------------------------------------------------------//
	


	  //�������Ԫ�����뻯����
	            //FittedLine_merge.push_back(FittedLine_save.at(FittedLine_save.size()-2));
				//FittedLine_merge.push_back(FittedLine_save.back());//���һ��Ԫ�ش���
      /////////////////////////////////////////////////////////////////////////////////////

	printf("FittedLine_merge.size=%d \n",FittedLine_merge.size());///������

						//for (int i = 0; i < static_cast<int>(FittedLine_merge.size());i++)///������
						//	{
						//		cout<<" a: "<<FittedLine_merge.at(i).a
						//			<<" b: "<<FittedLine_merge.at(i).b
						//			<<" Rho:"<<FittedLine_merge.at(i).Rho*180/PI<<endl;///**�ص�
						//		//<<" Rho:"<<atan(FittedLine_merge.at(i).a)*180/PI<<endl;///**�ص�
						//	}

						//for (int i = 0; i < static_cast<int>(FittedLine_merge.size());i++)///������
						//	//for (int i = 0; i < 2;i++)  
						//{

						//	//�״����ݶϵ��־
						//	colorRGB = usualColor[colorIndex];
						//	R = colorRGB/65536;
						//	G = (colorRGB%65536)/256;
						//	B = colorRGB%256; 
						//	colorIndex = (colorIndex + 1)%10;//��ϳ����߶ε���ɫ�����10�֣�

						//	x1 = FittedLine_merge.at(i).startPoint.x;
						//	y1 = FittedLine_merge.at(i).startPoint.y;

						//	x2 = FittedLine_merge.at(i).endPoint.x;
						//	y2 = FittedLine_merge.at(i).endPoint.y;
		
						//	//ת��ΪRadarͼ����ϵ��
						//	/* x1 = x1/b  + dx;
						//	y1 = -y1/b + dy;
						//	x2 = x2/b  + dx;
						//	y2 = -y2/b + dy;
						//	cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<endl;*/
						//	x1 = x1/b+dx  ;
						//	y1 = -y1/b+dy ;
						//	x2 = x2/b +dx;
						//	y2 = -y2/b+dy;
						//	cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<endl;

						//	//cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,8,0);


						//	//cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,CV_AA,0);///������
						//}
		
}







void OpenRadar::DrawRadarLine(vector<LinePara>& FittedLine,IplImage* RadarImage){
    //�����ļ���һ��Բ��
    int dx =  RadarImageWdith*c/4;
    int dy =  RadarImageHeight*c/4;
    cvCircle(RadarImage, cvPoint(dx,dy),3, CV_RGB(0,255,255), -1, 8,0);
    int x1,y1,x2,y2;
    //��ɫ
    int colorIndex = 0,colorRGB;
    int R = 255,G = 0,B = 0;
	LinePara  merge;
	//FittedLine_merge.clear();

	/////////////*������
	
	for (int i = 0; i < static_cast<int>(FittedLine.size());i++)
     {
         cout<<" a: "<<FittedLine.at(i).a
             <<" b: "<<FittedLine.at(i).b
             <<" Rho:"<<FittedLine.at(i).Rho*180/PI<<endl;
     }
	////////////////////////////////////////////////////
	////////////////////////*������
	//float k1=abs(FittedLine.at(5).Rho*180/PI);
	//printf("k1=%f",k1);
	//float k2=abs(FittedLine.at(6).Rho*180/PI);
	//printf("k2=%f",k2);
	//float k3=abs(abs(FittedLine.at(2).Rho*180/PI)-abs(FittedLine.at(3).Rho*180/PI));
	//printf("k3=%f",k3);

	//
	//if( abs((FittedLine.at(1).Rho*180/PI)-(FittedLine.at(2).Rho*180/PI))<100)
	//			{

	//				 x1 = FittedLine.at(1).startPoint.x;
 //       y1 = FittedLine.at(1).startPoint.y;

 //       x2 = FittedLine.at(2).endPoint.x;
 //       y2 = FittedLine.at(2).endPoint.y;
	//	
 //       //ת��ΪRadarͼ����ϵ��
 //       x1 = x1/b  + dx;
 //       y1 = -y1/b + dy;
 //       x2 = x2/b  + dx;
 //       y2 = -y2/b + dy;
	//	cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,8,0);
	//			}
	//			else
	//			{

	//				 x1 = FittedLine.at(1).startPoint.x;
 //       y1 = FittedLine.at(1).startPoint.y;

 //       x2 = FittedLine.at(1).endPoint.x;
 //       y2 = FittedLine.at(1).endPoint.y;
	//	
 //       //ת��ΪRadarͼ����ϵ��
 //       x1 = x1/b  + dx;
 //       y1 = -y1/b + dy;
 //       x2 = x2/b  + dx;
 //       y2 = -y2/b + dy;
	//	cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(255,0,0),2,8,0);

	//			}
	////////////////////////////////////////////////////



    for (int i = 0; i < static_cast<int>(FittedLine.size());i++)
		//for (int i = 0; i < 2;i++)  ///������
    {

        //�״����ݶϵ��־
        colorRGB = usualColor[colorIndex];
        R = colorRGB/65536;
        G = (colorRGB%65536)/256;
        B = colorRGB%256; 
        colorIndex = (colorIndex + 1)%10;//��ϳ����߶ε���ɫ�����10�֣�

        x1 = FittedLine.at(i).startPoint.x;
        y1 = FittedLine.at(i).startPoint.y;

        x2 = FittedLine.at(i).endPoint.x;
        y2 = FittedLine.at(i).endPoint.y;
		
		////////////��������///////////////////////
		int x_1 = FittedLine.at(i).startPoint.x;
        int y_1 = FittedLine.at(i).startPoint.y;
        int x_2 = FittedLine.at(i).endPoint.x;
        int y_2 = FittedLine.at(i).endPoint.y;
		//cout<<"x_1: "<<x_1<<" y_1: "<<y_1<<" x2: "<<x_2<<" y_2: "<<y_2<<endl;
		///////////////////////////////////////////

        //ת��ΪRadarͼ����ϵ��
        x1 = x1/b  + dx;
        y1 = -y1/b + dy;
        x2 = x2/b  + dx;
        y2 = -y2/b + dy;
        cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<endl;
		 
        cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,8,0);


		//cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,CV_AA,0);///������
    }
}

void OpenRadar::DrawRadarLine_merge(vector<LinePara>& FittedLine,IplImage* RadarImage){
    //�����ļ���һ��Բ��
    int dx =  RadarImageWdith*c/4;
    int dy =  RadarImageHeight*c/4;
    cvCircle(RadarImage, cvPoint(dx,dy),3, CV_RGB(0,255,255), -1, 8,0);
    int x1,y1,x2,y2;
    //��ɫ
    int colorIndex = 0,colorRGB;
    int R = 255,G = 0,B = 0;
	LinePara  merge;
	//FittedLine_merge.clear();

	
    for (int i = 0; i < static_cast<int>(FittedLine_merge.size());i++)
		//for (int i = 0; i < 2;i++)  ///������
    {

        //�״����ݶϵ��־
        colorRGB = usualColor[colorIndex];
        R = colorRGB/65536;
        G = (colorRGB%65536)/256;
        B = colorRGB%256; 
        colorIndex = (colorIndex + 1)%10;//��ϳ����߶ε���ɫ�����10�֣�
	if(i<FittedLine_merge.size()-1)
	{
		if((FittedLine_merge.at(i).startPoint.x==FittedLine_merge.at(i+1).startPoint.x)&&(FittedLine_merge.at(i).startPoint.y==FittedLine_merge.at(i+1).startPoint.y))
		{
        x1 = FittedLine.at(i).startPoint.x;
        y1 = FittedLine.at(i).startPoint.y;

        x2 = FittedLine.at(i+1).endPoint.x;
        y2 = FittedLine.at(i+1).endPoint.y;
		
        //ת��ΪRadarͼ����ϵ��
        x1 = x1/b  + dx;
        y1 = -y1/b + dy;
        x2 = x2/b  + dx;
        y2 = -y2/b + dy;
        cout<<"x1: "<<x1<<" y1: "<<y1<<" x2: "<<x2<<" y2: "<<y2<<endl;

       // cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,8,0);
		cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,CV_AA,0);///������

		}
	}
		//cvLine(RadarImage,cvPoint(x2,y2),cvPoint(x1,y1),CV_RGB(R,G,B),2,CV_AA,0);///������
    }
}



int OpenRadar::FindCorners(vector<int>& CornerIndex,int* X, int* Y, int start, int Cnt , float Eps){
  
    int N = 0;
    int N1 = 0,N2 = 0;
    N = PolyContourFit(X, Y, Cnt ,Eps);
    if (N ==0)
    {
        return 0;
    }else if (N > 0 && N < Cnt)
    {
        CornerIndex.push_back(start+N);
        if (N > 100)
        {
            N1 = FindCorners(CornerIndex,X, Y, start,N ,Eps);
        }
        
        if (Cnt - N > 100)
        {
            N2 = FindCorners(CornerIndex,X + N, Y + N, start + N ,Cnt - N ,Eps);
        }    
    }

    //��������еĽǵ㣬Ȼ��Խǵ��˳���������
    int temp;
    for (int i = 0; i < static_cast<int>(CornerIndex.size());i++)
    {
        for (int j = i + 1; j < static_cast<int>(CornerIndex.size());j++)
        {
            if (CornerIndex.at(i) > CornerIndex.at(j))
            {
                temp = CornerIndex.at(i);
                CornerIndex.at(i) = CornerIndex.at(j);
                CornerIndex.at(j) = temp;
            }
        }
    }
    iPoint CornerPoint;
    //cout<<"Corner index: ";
    for (int i = 0; i < static_cast<int>(CornerIndex.size());i++)
    {
        //cout<<CornerIndex.at(i)<<"  ";
        CornerPoint.x = X[CornerIndex.at(i)];
        CornerPoint.y = Y[CornerIndex.at(i)];
        Corners.push_back(CornerPoint);
    }
    //cout<<"\n";

    /*if (CornerIndex.size() >= 2)
    {
        cout<<"cornerCnt >= 2:"<<endl;
    }*/
    return CornerIndex.size();
}

 void OpenRadar::DrawRadarCorners(vector<iPoint>& Corners,IplImage* RadarImage){

     int x,y;
     int dx =  RadarImageWdith*c/4;
     int dy =  RadarImageHeight*c/4;

     for (int i = 0; i < static_cast<int>(Corners.size());i++)
     {
         x = Corners.at(i).x;
         y = Corners.at(i).y;
         x = x/b  + dx;
         y = -y/b + dy;
         cvCircle(RadarImage, cvPoint(x,y),3, CV_RGB(0,255,255), 3, 8,0);
     }
 }

 void OpenRadar::printRadarLine(vector<LinePara> FittedLine){

     for (int i = 0; i < static_cast<int>(FittedLine.size());i++)
     {
         cout<<" a: "<<FittedLine.at(i).a
             <<" b: "<<FittedLine.at(i).b
             <<" Rho:"<<FittedLine.at(i).Rho*180/PI<<endl;
     }

     if (FittedLine.size() == 2)
     {
         cout<<" Rho Sub:"
             <<(FittedLine.at(0).Rho - FittedLine.at(1).Rho)*180/PI <<endl;
     }
 }

 //��ֵ�˲� ֻ�ܶԳ�ʼ�����������˲�
 //�˲��������������ݣ����˻�����ӵ���������
 void OpenRadar::MedFilter(vector<int>& RadarRho, vector<double>& RadarTheta){
     vector<int>rho;
     vector<double>theta;
     //int halfWindowSize = 2;
     int *neighbor = new int[2*halfWindowSize+1];
     int temp;
     for (int i = halfWindowSize; i< (int)RadarRho.size() - halfWindowSize;i++)
      {
          for (int j = -halfWindowSize;j <= halfWindowSize;j++)
          {
              neighbor[j + halfWindowSize] = RadarRho.at(i + j); 
          }
          //����
          for (int m = 0; m < 2*halfWindowSize +1;m++)
          {
              for (int n = m + 1;n < 2*halfWindowSize + 1;n++)
              {
                  if (neighbor[m]> neighbor[n])
                  {
                      temp = neighbor[m];
                      neighbor[m] = neighbor[n];
                      neighbor[n] = temp;
                  }
              }
          }
          rho.push_back(neighbor[halfWindowSize]);
          theta.push_back(RadarTheta.at(i));
      }

     RadarRho.clear();
     RadarTheta.clear();

     for (int i = 0; i < (int)(rho.size());i++)
     {
         RadarRho.push_back(rho.at(i));
         RadarTheta.push_back(theta.at(i));
     }
 }


 void OpenRadar::CreateFieldImage(IplImage* FieldImage){
     int width = FieldImage->width;
     int height = FieldImage->height;
     int thickness  = 1;
     int dx = width/2;
     int dy = width/2;
     int ratio = 20000/height;
     cvZero(FieldImage);
     DrawLine(FieldImage,FieldPointA,FieldPointB,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawLine(FieldImage,FieldPointB,FieldPointC,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawLine(FieldImage,FieldPointC,FieldPointD,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawLine(FieldImage,FieldPointD,FieldPointA,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawLine(FieldImage,FieldPointB,FieldPointD,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     //���Ƴ����е�Բ
     DrawCircle(FieldImage,FieldCircle1,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle2,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle3,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle4,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle5,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle6,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle7,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle8,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle9,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle10,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle11,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle12,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle13,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);
     DrawCircle(FieldImage,FieldCircle14,ratio,dx,dy,CV_RGB(255,255,255),thickness,8,0);

 }

//��ĳ�߶���С����Ƶ�image
 void OpenRadar::DrawLine(IplImage *image,iPoint W1,iPoint W2,int ratio,int dx,
                          int dy,CvScalar color,int thickness, int line_type, 
                          int shift)
 {
    CvPoint pt1,pt2;
    pt1.x = W1.x/ratio + dx;
    pt1.y = -W1.y/ratio+ dy;
    pt2.x = W2.x/ratio + dx;
    pt2.y = -W2.y/ratio+ dy;
    cvLine(image,pt1,pt2,color,thickness,line_type,shift);
 }

 void OpenRadar::DrawCircle(IplImage *image,CirclePara circle,int ratio,int dx,int dy,CvScalar color,
     int thickness, int line_type, int shift){
         CvPoint center;
         center.x = circle.x/ratio + dx;
         center.y = -circle.y/ratio+ dy;

         int r = circle.r/ratio;
         int r2 = circle.r2/ratio;
         
         cvCircle(image,center,r,color,thickness,line_type,shift);
         //cvCircle(image,center,r2,color,thickness,line_type,shift);
 }

 void OpenRadar::DrawRadar(IplImage *image,CoorTransPara transPara){

     CvPoint radarPoint;
     int thickness = 3;
     int dx = image->width/2;
     int dy = image->height/2;
     int ratio = 20000/image->height;
     radarPoint.x = transPara.Tx/ratio + dx;
     radarPoint.y = -transPara.Ty/ratio + dy;
     CvScalar color = CV_RGB(0,255,255);
     cvCircle(image,radarPoint,3,color,-1,8,0);
     //���Ƽ�ͷ
     int arrowLength = 400;
     CvPoint endPoint;
     endPoint.x = (transPara.Tx + arrowLength*cos(transPara.theta+ PI/2))/ratio + dx;
     endPoint.y = -(transPara.Ty + arrowLength*sin(transPara.theta+ PI/2))/ratio + dy;
     cvLine(image,radarPoint,endPoint,color,1,8,0);
 }

 void OpenRadar::RadarInit( urg_state_t state ,int* data,int n){
     string model = state.model;     
     int distance_min = state.distance_min;           
     int distance_max = state.distance_max;           
     int area_total = state.area_total;               
     int area_min = state.area_min;                
     int area_max = state.area_max;                 
     int area_front = state.area_front;               
     int scan_rpm = state.scan_rpm;   

     
     double deltaTheta = 360.0/area_total*PI/180;
     #ifdef CLOCKWISE
     deltaTheta *= -1;
     #endif
	 double theta;
	 int rho;

	 RadarRho.clear();
	 RadarTheta.clear();
	 for (int i = 0; i < n;i++)
	 {
		 theta = (i + area_min - area_front)*deltaTheta + 90*PI/180;
		 rho = data[i];
		 RadarRho.push_back(rho);
		 RadarTheta.push_back(theta);
	 }
 }

 //���ļ��ж�ȡ�״������
 bool OpenRadar::ReadRadarFile(urg_state_t state,char *fileName){
	 FILE* fp = NULL;
	 int rho = 0;
	 double theta;
	 string model = state.model;     
	 int distance_min = state.distance_min;           
	 int distance_max = state.distance_max; 

	 int area_total = state.area_total;               
	 int area_min = state.area_min;                
	 int area_max = state.area_max;                 
	 int area_front = state.area_front;               
	 int scan_rpm = state.scan_rpm;   


	 double deltaTheta = 360.0/area_total*PI/180;
	 #ifdef CLOCKWISE
	 deltaTheta *= -1;
     #endif

//	 //ע���״���ʱ�ǵ���װ��
//	 #ifdef UTM30LX
//	 theta = (135.0 + 90)*PI/180;
//	 deltaTeta = -0.25*PI/180;
//#elif defined URG04LX
//	 theta = (120.0 + 90)*PI/180;
//	 deltaTeta = -0.352422907488987*PI/180;
//#endif

	 int pointCnt = 0;
	 fp = fopen(fileName,"r");
	 if (fp == NULL)
	 {
		 //cout<<"failed to read"<<endl;
		 return false;
	 }else {
		 //cout<<"successed to read"<<endl;
		 RadarRho.clear();
		 RadarTheta.clear();

		 while(!feof(fp))
		 {
			 fscanf(fp, "%d, ", &rho);
			 RadarRho.push_back(rho);

			 theta = (pointCnt + area_min - area_front)*deltaTheta + 90*PI/180;
			 RadarTheta.push_back(theta);
			 //printf("%d  ", rho);
			 pointCnt++;
		 }
	 }
	 fclose(fp);
	 return true;
 }
 //���ļ��ж�ȡ���״��״̬��Ϣ
 bool OpenRadar::ReadRadarInitFile(urg_state_t & state,char *fileName){
	 string s;
	 char sNum[20];
	 ifstream pFile;
	 pFile.open(fileName);
	 if (!pFile.is_open())
	 {
		 return false;
	 }
	 //getline(pFile,s);
	 while (pFile.good())
	 {
		 getline(pFile,s);
		 if (s.empty())
		 {
			 continue;
		 }

		 if (s == string("[model]"))
		 {
			 getline(pFile,s);
			 state.model = s;
		 }else if (s == string("[frontStep]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.area_front = atoi(sNum);
		 }else if (s == string("[minDistance]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.distance_min = atoi(sNum);
		 }else if (s == string("[maxDistance]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.distance_max = atoi(sNum);
		 }else if (s == string("[motorSpeed]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.scan_rpm = atoi(sNum);
		 }else if (s == string("[totalSteps]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.area_total = atoi(sNum);
		 }else if (s == string("[captureMode]"))
		 {
			 getline(pFile,s);
			
		 }else if (s == string("[scanMsec]"))
		 {
			 getline(pFile,s);
			
		 }else if (s == string("[startStep]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.area_min = atoi(sNum);
		 }else if (s == string("[endStep]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.area_max = atoi(sNum);
		 }else if (s == string("[serialNumber]"))
		 {
			 getline(pFile,s);
		 }else if (s == string("[timestamp]"))
		 {
			 getline(pFile,s);
			 myStrcpy(s,sNum);
			 state.last_timestamp = atoi(sNum);
		 }else 
		 {

		 }
	 }
	 pFile.close();
	 return true;
 }

 void OpenRadar::myStrcpy(string s,char *array){
	 int length = s.length();
	 for (int i = 0; i < length;i++)
	 {
		 array[i] = s.at(i);
	 }
	 array[length] = '\n';
 }

 int OpenRadar::FitArc(vector<ArcPara>& FittedArc,
						vector<int>& RadarRho,
						vector<double>& RadarTheta)
 {
	int rho = 0;
    double theta = 0.0;
    int X[1200] = {0};
    int Y[1200] = {0};
    int pointCnt = 0;
    ArcPara tmpArcPara;
    FittedArc.clear();
	int ArcCnt = 0;
    for (int i = 0 ; i < (int)(RadarRho.size());i++)
    {
        rho = RadarRho.at(i);
        theta = RadarTheta.at(i);

        if (rho < 0)
        {
            if (pointCnt > 3 && pointCnt < 360 )//����Ŀ�㹻��ĵ��вŽ���Բ���
            {
                if (HArc(X ,Y ,pointCnt,80,&tmpArcPara))
                {
					FittedArc.push_back(tmpArcPara);
					ArcCnt++;
                }
                pointCnt = 0;
                continue;
            }else {
                pointCnt = 0;
                continue;
            }   
        }
		if (rho > 100 && rho < 3000)
		{
			X[pointCnt] = static_cast<int>(rho*cos(theta));
			Y[pointCnt] = static_cast<int>(rho*sin(theta));
			pointCnt++;
		}
      
    }
    for (int i = 0; i < FittedArc.size();i++)
    {
       cout<<" x: "<<FittedArc.at(i).center.x 
		   <<" y: "<<FittedArc.at(i).center.y
		   <<" r: "<<FittedArc.at(i).r<<endl;
    }
	return ArcCnt;
 }

 void OpenRadar::DrawRadarArc(IplImage* image,vector<ArcPara>& FittedArc)
 {
	 CvPoint center;
	 int r;
	 int ratio = 10;
	 int dx =  RadarImageWdith*c/4;
	 int dy =  RadarImageHeight*c/4;
	 for (int i = 0; i < (int)(FittedArc.size());i++)
	 {
		 center.x = FittedArc.at(i).center.x/ratio + dx;
		 center.y = -FittedArc.at(i).center.y/ratio+ dy;
		  r = FittedArc.at(i).r/ratio;
		 cvCircle(image,center,r,CV_RGB(0,255,0),1,8,0);
	 }
	 
 }

 void OpenRadar::SelectRadarRho(vector<int>& RadarRho,vector<double>& RadarTheta){
	 int rho = 0;
	 double theta = 0.0;
	 int X[1200] = {0};
	 int Y[1200] = {0};
	 int RHO[1200] = {0};
	 double THETA[1200] = {0};
	 int pointCnt = 0;

	 vector<int> tempRadarRho;
	 vector<double> tempRadarTheta;
	 tempRadarRho.clear();
	 tempRadarTheta.clear();

	
	 for (int i = 0 ; i < static_cast<int>(RadarRho.size());i++)
	 {
		 rho = RadarRho.at(i);
		 theta = RadarTheta.at(i);

		 if (rho < 0)
		 {
			 if (pointCnt > 30 && pointCnt < 100 )//����Ŀ�㹻��ĵ��вŽ���ֱ�����
			 {
				
				 for (int k = 0; k < pointCnt;k++)
				 {
					 tempRadarRho.push_back(RHO[k]);
					 tempRadarTheta.push_back(THETA[k]);
				 }
				 tempRadarRho.push_back(-1);
				 tempRadarTheta.push_back(1000.0);

				 pointCnt = 0;
				 continue;
			 }else {
				 pointCnt = 0;
				 continue;
			 }   
		 }
		 if (rho > 100 && rho < 3000)
		 {
			 X[pointCnt] = static_cast<int>(rho*cos(theta));
			 Y[pointCnt] = static_cast<int>(rho*sin(theta));
			 RHO[pointCnt] = rho;
			 THETA[pointCnt] = theta;
			 pointCnt++;
		 }

	 }

	 RadarRho.clear();
	 RadarTheta.clear();
	 for (int i = 0; i < (int)tempRadarRho.size();i++)
	 {
		 RadarRho.push_back(tempRadarRho.at(i));
		 RadarTheta.push_back(tempRadarTheta.at(i));
	 }

 }


 void OpenRadar::Contours(IplImage* image,vector<int>& RadarRho,vector<double>& RadarTheta)
 {
	 //�鿴����������
	 CvMemStorage *storage = cvCreateMemStorage(0);
	 CvSeq * seq = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint), storage);
	 int x,y;
	 CvPoint point;
	 int rho;
	 double theta;
	 int ratio = 20000/image->height;
	 int dx =  RadarImageWdith*c/4;
	 int dy =  RadarImageHeight*c/4;
	 int* added;
	 for(int i = 0; i < RadarRho.size(); i++ )
	 {
		 rho = RadarRho.at(i);
		 theta = RadarTheta.at(i);
		 if (rho < 0)
		 {
			 continue;
		 }
		 x = rho*cos(theta);
		 y = rho*sin(theta);

		 point.x = x/ratio + dx;
		 point.y = -y/ratio + dy;

		 cvSeqPush( seq, (CvPoint*)&point);
	 }
	cvDrawContours(image,seq,CV_RGB(255,255,255),CV_RGB(255,255,255),-1,CV_FILLED,8);
	CvPoint * pPoint;
	for (;seq != 0;seq = seq->h_next)
	 {
		 for (int k = 0; k < seq->total;k++)
		 {

			 pPoint = CV_GET_SEQ_ELEM(CvPoint,seq,k);
			 //cout<<"get: "<<pPoint->x << "  "<<pPoint->y<<endl;
		 }

	 }
	 cvReleaseMemStorage( &storage );
 }


 void  OpenRadar::SendArc(vector<ArcPara> FittedArc){
	 //message_buffer[0] = 0x55;
	 //message_buffer[1] = (unsigned char)FittedArc.size();
	 //Com2_SendLength = 2;
	 //int tempX = 0,tempY = 0;
	 //unsigned char *p;
	 //for (int i = 0; i< FittedArc.size();i++)
	 //{
		// tempX = FittedArc.at(i).center.x;
		// tempY = FittedArc.at(i).center.y;
		// p = (unsigned char *)&tempX;
		// message_buffer[Com2_SendLength] = p[0];
		// message_buffer[Com2_SendLength+1] = p[1];
		// message_buffer[Com2_SendLength+2] = p[2];
		// message_buffer[Com2_SendLength+3] = p[3];
		// Com2_SendLength += 4;

		// p = (unsigned char *)&tempY;
		// message_buffer[Com2_SendLength] = p[0];
		// message_buffer[Com2_SendLength+1] = p[1];
		// message_buffer[Com2_SendLength+2] = p[2];
		// message_buffer[Com2_SendLength+3] = p[3];
		// Com2_SendLength += 4;
	 //}

	 //message_buffer[Com2_SendLength] = 0xaa;
	 //Com2_SendLength++;

	 //message_buffer[Com2_SendLength] = '\0';
	 //_snprintf(message_buffer, Com2_LineLength,
		// "Fucking\n");
	
	 _snprintf(message_buffer, Com2_LineLength,
		 "sending\n");
	 com2_send(message_buffer,Com2_LineLength);
 }




 //--------------------------------------------------------------------------------------------------------//
 //---�������������-----//
   
 //������-�ǶȲ�ֱ��ʹ��

 double OpenRadar:: d0(LinePara& line1,LinePara& line2)
 {
	 double a;

	 
	 if((((85<=line1.Rho*180/PI)&&(line1.Rho*180/PI<=90))&&((-90<=line2.Rho*180/PI)&&(line2.Rho*180/PI<=-85)))
		||(((85<=line2.Rho*180/PI)&&(line2.Rho*180/PI<=90))&&((-90<=line1.Rho*180/PI)&&(line1.Rho*180/PI<=-85)))
       )
	 {
	a = abs(abs(line1.Rho*180/PI)-abs(line2.Rho*180/PI));
	 }
	 else
	 {a =abs((line1.Rho*180/PI)-(line2.Rho*180/PI));}
     return a;
 }


   //�ڽ��Ծ���
 double OpenRadar:: d1(double x1,double y1,double x2,double y2)
{
    double d=0;
    d=  sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return d;
}

 //������-�������

 double OpenRadar:: d2(LinePara& line,double x1,double y1,double x3,double y3)
{
    double d1,d2,d3,d;
	double x2=0,y2=0;
	x2=(x1+x2)/2;
	y2=(y1+y2)/2;
    d1=  abs((line.a*x1-y1+line.b)/ (sqrt(line.a*line.a+1)));
	d2=  abs((line.a*x2-y2+line.b)/ (sqrt(line.a*line.a+1)));
	d3=  abs((line.a*x3-y3+line.b)/ (sqrt(line.a*line.a+1)));
	
	d= (d1+d2+d3)/3;
	//d= d2;

	d=d/(sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3)));
    return d;
}

 //�ص���
 double OpenRadar:: d3(LinePara& line,double x1,double y1,double x2,double y2)
{
	double mx1,my1;
	double mx2,my2;
	mx1 = (line.a*(y1-line.b)+x1)/(line.a*line.a+1);
	my1 = line.a*mx1+line.b;
	mx2 = (line.a*(y2-line.b)+x2)/(line.a*line.a+1);
	my2 = line.a*mx2+line.b;



    double d1,d2,d;
	d1=(sqrt((mx1-line.endPoint.x)*(mx1-line.endPoint.x)+(my1-line.endPoint.y)*(my1-line.endPoint.y)));
	d2=(sqrt((mx2-line.endPoint.x)*(mx2-line.endPoint.x)+(my2-line.endPoint.y)*(my2-line.endPoint.y)));
	d=(d2-d1)/sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	//d=(d2/d1)/sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    return d;
}



 double OpenRadar:: d(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2)
 {

	 int f;


	  if( //(abs((FittedLine.at(i).Rho*180/PI)-(FittedLine.at(i+1).Rho*180/PI))<=10)
		    (d0(line1,line2)<=6)
		 && (d1(line_save1.endPoint.x,line_save1.endPoint.y,line_save2.startPoint.x,line_save2.startPoint.y))<=15
		 //&& (d2 (line_save1,line_save2.startPoint.x,line_save2.startPoint.y,line_save2.endPoint.x,line_save2.endPoint.y))<=0.5
		 && (((d3(line_save1,line_save2.startPoint.x,line_save2.startPoint.y,line_save2.endPoint.x,line_save2.endPoint.y))<=1)
		      &&((d3(line_save1,line_save2.startPoint.x,line_save2.startPoint.y,line_save2.endPoint.x,line_save2.endPoint.y))>=0.98))
		)
	   { f=1; }
	  else{f=0;}

	  return f;

 }


 //------------------------------------------------------------------------------------------------------------------//

            ///////////////////////////////svm���//////////////////////////////////////////
			 //svm��˵ 
			//https://www.cnblogs.com/chenzhefan/p/7662315.html 
			//https://blog.csdn.net/yansmile1/article/details/50215809


 void OpenRadar::Train()
 {
	 double p[20][4]=
	 {
		 {},{},{},{},
		 {},{},{},{},
		 {},{},{},{},
		 {},{},{},{},
		 {},{},{},{},
	 };
	double trainingData[20][4];
	int labels[20] = {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1};//////////////...................
    Mat labelsMat(20, 1, CV_32FC1, labels);

	double v[4];
	
	char s[100]="no connection";
	char t[100]="connection";
	

	for(int m=0;m<20;m++)
   {
	   for(int j=0;j<4;j++)
	   {
		   v[j]=p[m][j];
		   trainingData[m][j]=v[j];
		   
	   }
	       printf("train_feature:");
	       printf(" v[0]=%f ",trainingData[m][0]);
	       printf(" v[1]=%f",trainingData[m][1]);
		   printf(" v[2]=%f",trainingData[m][2]);
	       printf(" v[3]=%f \n",trainingData[m][3]);
			if(labels[m]==0)
			{printf(" label=%s \n ",s);}
			else if(labels[m]==1)
			{printf(" label= %s\n",t);}
			v[0]=0;v[1]=0;v[2]=0;v[3]=0;
   }
	
	Mat trainingDataMat(20, 4, CV_32FC1, trainingData);
	
	
	//��ͬ��������svm�����ܲ�ͬ
	
	//*************************
    CvSVM svm ;   
	CvSVMParams param;   
	CvTermCriteria criteria;  
	criteria= cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);   
	param= CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 10.0, 8.0, 1.0, 10.0, 0.5, 0.1, NULL, criteria);   

	svm.train(trainingDataMat, labelsMat, Mat(), Mat(), param);  
	svm.save( "SVM_DATA.xml" ); 


	
	//*************************
    //// ����SVM����
 //   CvSVMParams params;
 //   params.svm_type = CvSVM::C_SVC;
 //   params.kernel_type = CvSVM::LINEAR;
 //   params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

 //   // ��SVM����ѵ��
 //   CvSVM SVM;
 //   SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);

	//SVM.save( "SVM_DATA.xml" );


 }


void OpenRadar::test()
{
	 double p[10][4]=
	 {
		 {},{},{},{},
		 {},{},{},{},
		 {},{},
	 };
	double testData[10][4];
	int labels[10] = {0,0,0,0,0,1,1,1,1,1};//////////////...................
    

	double v[4];
	
	char s[100]="no connection";
	char t[100]="connection";
	
	CvSVM svm;
	svm.load( "SVM_DATA.xml" );

	Mat testDataMat(1, 4, CV_32FC1, testData);
	int ap=0;
	int ep=0;
	float ap_result;
	float ep_result;

	for(int m=0;m<10;m++)
   { 
	   for(int j=0;j<4;j++)
	   {
		   v[j]=p[m][j];
		   //testData[m][j]=v[j];   
	   }
	       
		   printf("test_feature:");
	       printf(" v[0]=%f",v[0]);
	       printf(" v[1]=%f",v[1]);
		   printf(" v[2]=%f",v[2]);
	       printf(" v[3]=%f \n",v[3]);
		   
		   testData[0][0]=v[0];
	       testData[0][1]=v[1];
	       testData[0][2]=v[2];
	       testData[0][3]=v[3];

		   Mat testDataMat(1, 4, CV_32FC1, testData);
		   float result = (float)svm.predict(testDataMat);
		   
		   if(labels[m]==result)
		   { ap=ap++; }
	       else if(labels[m]!=result)
	       {ep=ep++;}


		   v[0]=0;v[1]=0;v[2]=0;v[3]=0;
     }

	ap_result=ap/10;
	ep_result=ep/10;
	printf("������ȷ��= %f",ap_result);
	printf("���Դ�����= %f",ep_result);


}







 int OpenRadar:: predict(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2)
 {
	 int f;
	 double v[4];
	 double testData[1][4];
	 char s[100]="no connection";
	 char t[100]="connection";

	 CvSVM svm;
	 svm.load( "SVM_DATA.xml" );


	v[0]=d0(line1,line2);
	v[1]=d1(line_save1.endPoint.x,line_save1.endPoint.y,line_save2.startPoint.x,line_save2.startPoint.y);
	v[2]=d2(line_save1,line_save2.startPoint.x,line_save2.startPoint.y,line_save2.endPoint.x,line_save2.endPoint.y);
	v[3]=d3(line_save1,line_save2.startPoint.x,line_save2.startPoint.y,line_save2.endPoint.x,line_save2.endPoint.y);
	
	testData[0][0]=v[0];
	testData[0][1]=v[1];
	testData[0][2]=v[2];
	testData[0][3]=v[3];
	
	printf("predict_feature:");
	printf(" v[0]=%f ",testData[0][0]);
	printf(" v[1]=%f",testData[0][1]);
	printf(" v[2]=%f",testData[0][2]);
	printf(" v[3]=%f \n",testData[0][3]);

	Mat testDataMat(1, 4, CV_32FC1, testData);

	float result = (float)svm.predict(testDataMat);

	if(result==0)
	{printf(" label=%s \n ",s);}
	else if(result==1)
	{printf(" label= %s\n",t);}
	
	v[0]=0;v[1]=0;v[2]=0;v[3]=0;

	f=result;
	 
	return f;
 }

 int OpenRadar:: predict1(LinePara& line1,LinePara& line2,LinePara& line_save1,LinePara& line_save2)
 {
	 int f;
	 double v[3];
	 double testData[1][3];
	 char s[100]="no connection";
	 char t[100]="connection";

	 CvSVM svm;
	 svm.load( "SVM_DATA.xml" );


	v[0]=d0(line1,line2);
	v[1]=d1(line_save1.endPoint.x,line_save1.endPoint.y,line_save2.startPoint.x,line_save2.startPoint.y);
	v[2]=d2(line_save1,line_save2.startPoint.x,line_save2.startPoint.y,line_save2.endPoint.x,line_save2.endPoint.y);
	
	
	testData[0][0]=v[0];
	testData[0][1]=v[1];
	testData[0][2]=v[2];
	
	
	/*printf("predict_feature:");
	printf(" v[0]=%f ",testData[0][0]);
	printf(" v[1]=%f",testData[0][1]);
	printf(" v[2]=%f",testData[0][2]);*/
	

	Mat testDataMat(1, 3, CV_32FC1, testData);

	float result = (float)svm.predict(testDataMat);

	if(result==0)
	{
		//printf(" label=%s \n ",s);
	}
	else if(result==1)
	{//printf(" label= %s\n",t);
	d(line1,line2,line_save1,line_save2)==1;
	}
	
	v[0]=0;v[1]=0;v[2]=0;

	f=result;
	 
	return 0;
 }
           //////////////////////////////////////////////////////////////////////////////////////
 //-------------------------------------------------------------------------------------------------------------------//


 














