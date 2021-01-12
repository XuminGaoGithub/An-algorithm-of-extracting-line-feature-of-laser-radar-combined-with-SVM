#include "Coordinate.h"


Coordinate::Coordinate(void)
{
}


Coordinate::~Coordinate(void)
{
}


//已知四条直线如何计算变换参数.两条全局坐标系线段和两条雷达坐标系线段求出全局坐标系和雷达坐标系之间的转换参数，之后
//利用转换参数可以求出两条雷达坐标系线段中任意一点的全局坐标
//
void Coordinate::CalCoorTransPara(CoorTransPara &transPara,
                                  LinePara W1,
                                  LinePara W2, 
                                  LinePara R1, 
                                  LinePara R2)
{
    double theta = ( W1.Rho - R1.Rho + W2.Rho - R2.Rho )/2;//double Rho; 该段直线的倾角
    //double theta = ( W1.Rho - R1.Rho);
    //求解出Xw Yw Xr Yr
    double Xw = (double)(W1.b - W2.b)/(W2.a - W1.a);
    double Yw = W1.a*Xw + W1.b;

    double Xr = (double)(R1.b - R2.b)/(R2.a - R1.a);
    double Yr = R1.a*Xr + R1.b;


    int Tx = (int)(Xw - cos(theta)*Xr + sin(theta)*Yr);
    int Ty = (int)(Yw - sin(theta)*Xr - cos(theta)*Yr);
    //交点判定，场地上的几条直线都是有角点的
    iPoint crossPoint;//交点
    iPoint vectorW1,vectorR1;//向量
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
    //将激光雷达下的两个点旋转到W系下
    transPara.theta = theta;
    transPara.Tx = Tx;
    transPara.Ty = Ty;
    iPoint R1ToW;
    iPoint R2ToW;
    TransformCoord(transPara,R1.startPoint,R1ToW);
    TransformCoord(transPara,R2.startPoint,R2ToW);
    vectorR1.x = R1ToW.x - crossPoint.x;
    vectorR1.y = R1ToW.y - crossPoint.y;
    //判断是否在同一侧？
    if (vectorW1.x * vectorR1.x + vectorW1.y*vectorR1.y < 0)
    {
        //旋转角度差了180度，需要调转180度
        transPara.theta = theta + PI;
        transPara.Tx = (int)(Xw - cos(transPara.theta)*Xr + sin(transPara.theta)*Yr);
        transPara.Ty = (int)(Yw - sin(transPara.theta)*Xr - cos(transPara.theta)*Yr);
    }else{

    }
    //数据测试
    /*TransformCoord(transPara,R1.startPoint,R1ToW);
     cout<<"R1ToW.x "<<R1ToW.x<<"  R1ToW.y "<<R1ToW.y<<endl;
     TransformCoord(transPara,R1.endPoint,R1ToW);
     cout<<"R1ToW.x "<<R1ToW.x<<"  R1ToW.y "<<R1ToW.y<<endl;

     TransformCoord(transPara,R2.startPoint,R2ToW);
     cout<<"R2ToW.x "<<R2ToW.x<<"  R2ToW.y "<<R2ToW.y<<endl;

     TransformCoord(transPara,R2.endPoint,R2ToW);
     cout<<"R2ToW.x "<<R2ToW.x<<"  R2ToW.y "<<R2ToW.y<<endl;
*/
    //进行一次验证，看看交点进行坐标变换之后是否接近匹配的点
  /* iPoint R = ipoint(Xr,Yr);
    iPoint R2W;
    TransformCoord(transPara,R,R2W);
    cout<<"R2W.x "<<R2W.x<<"  R2W.y "<<R2W.y<<endl;*/
    
}

void Coordinate::CoortransTest()//先用已知的四条理想线段计算转换参数，之后的求实际转换参数与之可以对比
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
