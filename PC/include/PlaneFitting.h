/*
*��Ҫ���ڵ��Ƶ����
*LMS����С���˷�
*RANSAC���������һ����
*BaySAC�����ڱ�Ҷ˹������ʳ���һ����
*BayLMedS:���ڱ�Ҷ˹���������Сƽ����ֵ
*/
#ifndef fitting_h
#define fitting_h


#include "bayPoint.h"
#include "bayPlane.h"

#include <vector>
#include <set>
#include <ctime>

namespace td
{
	
	class PlaneFitting
	{
	public:
		PlaneFitting();
		PlaneFitting(td::PointCloud pc);
		~PlaneFitting();

		//�������
		void setInputCloud(PointCloud pc);


		//RANSAC�������
		bool computeByRANSAC(double );

		//BAYSAC�������
		bool computeByBAYSAC(double );

		//BAYLMEDS�������
		bool computeByBayLMedS();

		// LMEDS
		bool computeByLMedS();

	private:
		PointCloud cloud;
		Plane model;
	public:
		// ����ƽ�����
		Plane getModel();
	private:
		// ���ڵ���ֵ
		double inliers_threshold;
	public:
		// ���ؾ��ڵ�
		PointCloud getInliers();
		// ����ģ�ͱ�׼��ֵ
		double getInlierThershold();
		// ���ؾ����
		td::PointCloud getOutliers();

	private:
		// ����㵽��ı�׼��
		double computeStd(Plane& pl, PointCloud& cloud);

		// ��С��ֵ��Ӧ�ı�׼��
		// derta = 1.4826[1 + 5/(n-p)]/sqrt(Mj)
		double computeMStd(Plane& pl, PointCloud& cloud, double penalty);
	};
}

#endif
