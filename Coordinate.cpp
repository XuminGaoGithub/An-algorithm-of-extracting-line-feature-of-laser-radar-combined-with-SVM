#include "Coordinate.h"


Coordinate::Coordinate(void)
{
}


Coordinate::~Coordinate(void)
{
}


//��֪����ֱ����μ���任����.����ȫ������ϵ�߶κ������״�����ϵ�߶����ȫ������ϵ���״�����ϵ֮���ת��������֮��
//����ת������������������״�����ϵ�߶�������һ���ȫ������
//
void Coordinate::CalCoorTransPara(CoorTransPara &transPara,
                                  LinePara W1,
                                  LinePara W2, 
                                  LinePara R1, 
                                  LinePara R2)
{
    double theta = ( W1.Rho - R1.Rho + W2.Rho - R2.Rho )/2;//double Rho; �ö�ֱ�ߵ����
    //double theta = ( W1.Rho - R1.Rho);
    //����Xw Yw Xr Yr
    double Xw = (double)(W1.b - W2.b)/(W2.a - W1.a);
    double Yw = W1.a*Xw + W1.b;

    double Xr = (double)(R1.b - R2.b)/(R2.a - R1.a);
    double Yr = R1.a*Xr + R1.b;


    int Tx = (int)(Xw - cos(theta)*Xr + sin(theta)*Yr);
    int Ty = (int)(Yw - sin(theta)*Xr - cos(theta)*Yr);
    //�����ж��������ϵļ���ֱ�߶����нǵ��
    iPoint crossPoint;//����
    iPoint vectorW1,vectorR1;//����
    iPoint vectorR2,vectorW2;
    if (W1.startPoint.x == W2.startPoint.x && W1.startPoint.y == W2.startPoint.y)
    {
        crossPoint = ipoint(W1.startPoint.x,W1.startPoint.y);
        vectorW1 = ipoint(W1.endPoint.x - W1.startPoint.x, W1.endPoint.y - W1.startPoint.y);
        vectorW2 = ipoint(W2.endPoint.x - W2.startPoint.x, W2.endPoint.y - W2.startPoint.y);
    }else if (W1.endPoint.x == W2.startPoint.x && W1.endPoint.y == W2.startPoint.y)
    {
        crossPoint = ipoint(W1.endPoint.x,W1.endPoint.y);
        vectorW1 = ipoint(W1.startPoint.x - W1.endPoint.x, W1.startPoint.y - W1.endPoint.y);
        vectorW2 = ipoint(W2.endPoint.x - W2.startPoint.x, W2.endPoint.y - W2.startPoint.y);
    }else if (W1.startPoint.x == W2.endPoint.x && W1.startPoint.y == W2.endPoint.y)
    {
         crossPoint = ipoint(W1.startPoint.x,W1.startPoint.y);
         vectorW1 = ipoint(W1.endPoint.x - W1.startPoint.x, W1.endPoint.y - W1.startPoint.y);
         vectorW2 = ipoint(W2.startPoint.x - W2.endPoint.x, W2.startPoint.y - W2.endPoint.y);
    }else if (W1.endPoint.x == W2.endPoint.x && W1.endPoint.y == W2.endPoint.y)
    {
         crossPoint = ipoint(W1.endPoint.x,W1.endPoint.y);
         vectorW1 = ipoint(W1.startPoint.x - W1.endPoint.x, W1.startPoint.y - W1.endPoint.y);
         vectorW2 = ipoint(W2.startPoint.x - W2.endPoint.x, W2.startPoint.y - W2.endPoint.y);
    }
    //�������״��µ���������ת��Wϵ��
    transPara.theta = theta;
    transPara.Tx = Tx;
    transPara.Ty = Ty;
    iPoint R1ToW;
    iPoint R2ToW;
    TransformCoord(transPara,R1.startPoint,R1ToW);
    TransformCoord(transPara,R2.startPoint,R2ToW);
    vectorR1.x = R1ToW.x - crossPoint.x;
    vectorR1.y = R1ToW.y - crossPoint.y;
    //�ж��Ƿ���ͬһ�ࣿ
    if (vectorW1.x * vectorR1.x + vectorW1.y*vectorR1.y < 0)
    {
        //��ת�ǶȲ���180�ȣ���Ҫ��ת180��
        transPara.theta = theta + PI;
        transPara.Tx = (int)(Xw - cos(transPara.theta)*Xr + sin(transPara.theta)*Yr);
        transPara.Ty = (int)(Yw - sin(transPara.theta)*Xr - cos(transPara.theta)*Yr);
    }else{

    }
    //���ݲ���
    /*TransformCoord(transPara,R1.startPoint,R1ToW);
     cout<<"R1ToW.x "<<R1ToW.x<<"  R1ToW.y "<<R1ToW.y<<endl;
     TransformCoord(transPara,R1.endPoint,R1ToW);
     cout<<"R1ToW.x "<<R1ToW.x<<"  R1ToW.y "<<R1ToW.y<<endl;

     TransformCoord(transPara,R2.startPoint,R2ToW);
     cout<<"R2ToW.x "<<R2ToW.x<<"  R2ToW.y "<<R2ToW.y<<endl;

     TransformCoord(transPara,R2.endPoint,R2ToW);
     cout<<"R2ToW.x "<<R2ToW.x<<"  R2ToW.y "<<R2ToW.y<<endl;
*/
    //����һ����֤�����������������任֮���Ƿ�ӽ�ƥ��ĵ�
  /* iPoint R = ipoint(Xr,Yr);
    iPoint R2W;
    TransformCoord(transPara,R,R2W);
    cout<<"R2W.x "<<R2W.x<<"  R2W.y "<<R2W.y<<endl;*/
    
}

void Coordinate::CoortransTest()//������֪�����������߶μ���ת��������֮�����ʵ��ת��������֮���ԶԱ�
{
    Coordinate coord;
    CoorTransPara coordtrans;
    coord.CalCoorTransPara(coordtrans,FieldLine1,FieldLine5,FieldLine2,FieldLine5);
    cout<<"bingding : "<<"theta : "<<coordtrans.theta*180/PI<<" Tx: "<<coordtrans.Tx<<"  Ty: "<<coordtrans.Ty<<endl;
}

void Coordinate::printRadarCoordtransPara(CoorTransPara coordtrans){
    cout<<"theta : "<<coordtrans.theta*180/PI<<" Tx: "<<coordtrans.Tx<<"  Ty: "<<coordtrans.Ty<<endl;
}


void Coordinate::TransformCoord(CoorTransPara transPara,iPoint R,iPoint& W){
    
    W.x = (int)(R.x*cos(transPara.theta) - R.y*sin(transPara.theta) );
    W.y = (int)(R.x*sin(transPara.theta) + R.y*cos(transPara.theta) );
    W.x = W.x + transPara.Tx ;
    W.y = W.y + transPara.Ty ;
}
