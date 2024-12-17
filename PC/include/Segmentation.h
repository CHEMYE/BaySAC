/*
*对点云进行分割
*/

#ifndef segmentation_h
#define segmentation_h

#include "bayPoint.h"
#include "PlaneFitting.h"
#include "bayPlane.h"
#include "baykdtree.h"

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>
#include<iostream>

namespace td
{
	class Segmentation
	{
	public:
		Segmentation();
		Segmentation(PointCloud);
		~Segmentation();

		enum METHOD{RANSAC,BAYSAC,BAYLMEDS};

		//设置点云
		void setInputCloud(PointCloud);

		//区域生长分割
		void regionGrow(METHOD );

		// multi-RANSAC分割
		void multiRansac();

		// 设置邻域搜索半径
		void setRadius(double r);
	
		// 设置拟合阈值，主要针对RANSAC和BAYSAC拟合
		void setThreshold(double threshold);

		// 点云分割拟合后输出
		void outPut(std::string);

		// 设置k邻域值
		void setKNearest(int);

		// 返回模型个数
		int getModelNum(); 

		// 返回平均误差
		double getAverageError();

	private:
		// k邻域值
		int k;

		// 平均拟合误差
		double averageError;

		PointCloud cloud;//点云
		

		double radius;//邻域搜索半径

		// 拟合阈值,主要针对RANSAC和BaySAC
		double threshold;

		bool selectMethod(PlaneFitting& fit, METHOD method);
	public:
		std::vector<PointCloud> indices;//分割后的点云面片
	};
}
#endif
