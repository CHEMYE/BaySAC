#include "include/PlaneFitting.h"
#include "include/3dKDtree.h"
#include "include/baykdtree.h"
#include "include/Matrix.h"
#include "include/Model.h"
#include "include/bayPlane.h"
#include "include/PointIO.h"
#include "include/bayPlane.h"
#include "include/Segmentation.h"
#include <unistd.h>
#include<iostream>

#include<sys/time.h>
#include<unistd.h>

int main(){

    td::Segmentation test;
    td::PointIO path1;

    path1.td::PointIO::open("/home/ye/test.pcd");
    std::cout<<"shuru点云"<<std::endl;
    test.setInputCloud(path1.getPointCloud());//设置点云
    std::cout<<"设置点云"<<std::endl;
    test.setRadius(0.5);// 设置邻域搜索半径
    std::cout<<"设置邻域搜索半径"<<std::endl;
    test.setKNearest(30);// 设置k邻域值
    std::cout<<"设置k邻域值"<<std::endl;
    test.setThreshold(0.08);// 设置拟合阈值
    std::cout<<"设置拟合阈值"<<std::endl;
 
    //时间测量
    struct timeval tstart,tend;
    double timeUsed;
    gettimeofday(&tstart,NULL);
    /*待测试代码*/    
        test.regionGrow(td::Segmentation::BAYSAC);//区域生长分割
    /*待测试代码*/    
    gettimeofday(&tend,NULL);
    timeUsed=1000000*(tend.tv_sec-tstart.tv_sec)+tend.tv_usec-tstart.tv_usec;
    std::cout<<"BAYSAC Time="<<timeUsed/1000<<" ms"<<std::endl;

    std::cout<<"BAYSAC返回模型个数:"<<test.getModelNum()<<std::endl;
    std::cout<<"BAYSAC返回平均误差:"<<test.getAverageError()<<std::endl;

    //时间测量
    struct timeval tstart1,tend1;
    double timeUsed1;
    gettimeofday(&tstart1,NULL);
    /*待测试代码*/    
   test.regionGrow(td::Segmentation::RANSAC);//区域生长分割
    /*待测试代码*/    
    gettimeofday(&tend1,NULL);
    timeUsed1=1000000*(tend1.tv_sec-tstart1.tv_sec)+tend1.tv_usec-tstart1.tv_usec;
    std::cout<<"RANSAC Time="<<timeUsed1/1000<<" ms"<<std::endl;

    std::cout<<"RANSAC返回模型个数:"<<test.getModelNum()<<std::endl;
    std::cout<<"RANSAC返回平均误差:"<<test.getAverageError()<<std::endl;
  //  test.outPut(std::string);
    pause();
    return  0;
}

