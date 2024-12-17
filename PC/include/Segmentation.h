/*
*�Ե��ƽ��зָ�
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

		//���õ���
		void setInputCloud(PointCloud);

		//���������ָ�
		void regionGrow(METHOD );

		// multi-RANSAC�ָ�
		void multiRansac();

		// �������������뾶
		void setRadius(double r);
	
		// ���������ֵ����Ҫ���RANSAC��BAYSAC���
		void setThreshold(double threshold);

		// ���Ʒָ���Ϻ����
		void outPut(std::string);

		// ����k����ֵ
		void setKNearest(int);

		// ����ģ�͸���
		int getModelNum(); 

		// ����ƽ�����
		double getAverageError();

	private:
		// k����ֵ
		int k;

		// ƽ��������
		double averageError;

		PointCloud cloud;//����
		

		double radius;//���������뾶

		// �����ֵ,��Ҫ���RANSAC��BaySAC
		double threshold;

		bool selectMethod(PlaneFitting& fit, METHOD method);
	public:
		std::vector<PointCloud> indices;//�ָ��ĵ�����Ƭ
	};
}
#endif
