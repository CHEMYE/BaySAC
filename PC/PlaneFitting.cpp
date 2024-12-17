#include "include/PlaneFitting.h"
#include<iostream>
int ran=0;
int bay =0;
td::PlaneFitting::PlaneFitting()
	: inliers_threshold(0)
{

}

td::PlaneFitting::PlaneFitting(td::PointCloud pc)
{
	cloud = pc;
}

td::PlaneFitting::~PlaneFitting()
{

}

//�������
void td::PlaneFitting::setInputCloud(td::PointCloud pc)
{
	cloud = pc;
}


//RANSAC�������
bool td::PlaneFitting::computeByRANSAC(double threshold)
{
	srand((unsigned)time(NULL));
	if (cloud.size() <= 3)
		return false;
	//��ʼ��������
	int initial_iter = int(ceil(log10(0.01) / log10(1.0 - (double)model.needNum / cloud.size())));
	int iter(0);  //RANSACѭ������
	int inliers_l(0), inliers_n(0);//��һ��ѭ�����ڵ���Ŀ������ѭ�����ڵ���Ŀ 

	int randnum = model.needNum;
	int* nIndex=new int [randnum];    //���������

	while (iter < initial_iter)
	{

		//��ȡ�����
		for (int i = 0; i < randnum; i++)
		{
			nIndex[i] = rand() % cloud.size();
		}

		//�жϳ�ȡ������Ƿ��ظ�
		bool allsame(true);
		for (int i = 1; i < randnum; i++)
		{
			allsame = allsame&&nIndex[i] == nIndex[i - 1];
		}
		if (allsame)
			continue;

		Plane pl;
		PointCloud randcloud;
		for (int i = 0; i < randnum; i++)
		{
			randcloud.push_back(cloud[nIndex[i]]);
		}
		pl.computeFromPoints(randcloud);

		double d2plane(0);
		inliers_n = 0;
		for (size_t i = 0; i < cloud.size(); ++i)
		{
			d2plane = pl.point2plane(cloud[i]);
			if (d2plane <= threshold)
			{
				inliers_n++;
			}
		}


		if (inliers_n > inliers_l)
		{
			inliers_l = inliers_n;
			initial_iter = int(ceil(log10(0.01) / log10(1.0 - pow((double)inliers_n / cloud.size(), 3))));//����ѭ������
			model.setPara(pl.getA(), pl.getB(), pl.getC(), pl.getD());
			iter = 0;
			continue;//������һ��ѭ��
		}
		iter++;


		
	}
	delete[]nIndex;
	inliers_threshold = threshold;
	ran++;
		std::cout<<ran<<std::endl;
	return true;

}

//BAYSAC�������
bool td::PlaneFitting::computeByBAYSAC(double threshold)
{
	srand((unsigned)time(NULL));
	if (cloud.size() <= 3)
		return false;

	std::vector<double> pro(cloud.size(), 0);//����ֵ���������һһ��Ӧ

	//��ʼ��������
	int initial_iter = int(ceil(log10(0.01) / log10(1.0 - (double)model.needNum / cloud.size())));
	int stas_num = int(0.1*(double)initial_iter);


	int randnum = model.needNum;
	int* nIndex = new int[randnum];    //���������


	double para_percent = 0;//���Ų���ռ�ܲ����ı���

	//���ڲ���ͳ��
	std::vector<double> ins, plnum; //��¼ƽ�����
	std::vector<Plane> planes;

	//����ͳ��
	for (int iter = 0; iter <6; )
	{

		//��ȡ�����
		for (int i = 0; i < randnum; i++)
		{
			nIndex[i] = rand() % cloud.size();
		}

		//�жϳ�ȡ������Ƿ��ظ�
		bool allsame(true);
		for (int i = 1; i < randnum; i++)
		{
			allsame = allsame&&nIndex[i] == nIndex[i - 1];
		}
		if (allsame)
			continue;

		Plane pl;
		PointCloud randcloud;
		for (int i = 0; i < randnum; i++)
		{
			randcloud.push_back(cloud[nIndex[i]]);
		}
		pl.computeFromPoints(randcloud);

		double d2plane(0);
		int inliers_n = 0;
		for (size_t i = 0; i < cloud.size(); ++i)
		{
			d2plane = pl.point2plane(cloud[i]);
			if (d2plane <= threshold)
			{
				inliers_n++;
			}
		}

		iter++;

		//ÿ�μ���Ĳ�������ͳ��������
		planes.push_back(pl);
		ins.push_back(inliers_n);//�ò����¾��ڵ�ĸ���
		plnum.push_back(1);


		//���Ų���ͳ��,��ǰ������ǰ��Ĳ������бȽ�,
		if (iter >= 1)
		{
			for (size_t i = 0; i < planes.size() - 1; ++i)
			{
				double para_dif = acos(pl.getA() * planes[i].getA() + pl.getB() * planes[i].getB()
					+ pl.getC() * planes[i].getC()) * 180 /3.14;
				para_dif = para_dif > 90 ? 180 - para_dif : para_dif;
				if (para_dif < 10)/*�㵽���ȸ߽Ƕ�С����ľ��Ȳ�Ƕȴ�*/
				{
					plnum[i]++;//ͳ�Ƶ�i�ײ�������Ŀ
					if (inliers_n>ins[i])
					{
						planes[i].setPara(pl.getA(), pl.getB(), pl.getC(), pl.getD());
						ins[i] = inliers_n;
					}
					planes.pop_back();
					ins.pop_back();
					plnum.pop_back();
				}
			}
			para_percent = *max_element(plnum.begin(), plnum.end()) / (double)(iter);
		}
	}
	//��Ҷ˹����
	std::vector<double>::iterator it_f = plnum.begin();
	std::vector<double>::iterator it_s = max_element(plnum.begin(), plnum.end());
	size_t para_best_index = it_s - it_f;

	//ͨ������ͳ��Ϊ�㸳�������
	for (size_t i = 0; i < cloud.size(); i++)
	{
		double d2plane = planes[para_best_index].point2plane(cloud[i]);
		if (d2plane <= threshold)
		{
			pro[i] = 1 - d2plane / threshold;
		}
	}

	//���¿�ʼѭ��������ͳ�Ʋ����Ѿ�����
	int iter = 0;
	int inliers_l = 0;
	int inliers_n = 0;

	while (iter < initial_iter)
	{
		//�ҳ�����ֵ��ߵ�������
		std::vector<double> temp = pro;
		for (int i = 0; i < randnum; i++)
		{
			std::vector<double>::iterator it;
			it = max_element(temp.begin(), temp.end());
			nIndex[i] = it - temp.begin();
			temp.erase(it);
		}
		temp.clear();

		Plane pl_;
		PointCloud randcloud_;
		for (int i = 0; i < randnum; i++)
		{
			randcloud_.push_back(cloud[nIndex[i]]);
		}
		pl_.computeFromPoints(randcloud_);


		inliers_n = 0;
		for (size_t i = 0; i < cloud.size(); ++i)
		{
			double d2plane = pl_.point2plane(cloud[i]);
			if (d2plane <= threshold)
			{
				inliers_n++;
			}
		}

		//���¼���㼯�������
		for (size_t i = 0; i < cloud.size(); ++i)
		{
			double d2plane = pl_.point2plane(cloud[i]);
			if (d2plane <= threshold)
			{
				pro[i] = pro[i] * (double)inliers_n / cloud.size();
			}
		}


		//Ѱ�Ҿ��ڵ�
		if (inliers_n > inliers_l)
		{
			inliers_l = inliers_n;
			initial_iter = int(ceil(log10(0.01) / log10(1.0 - pow((double)inliers_n / cloud.size(), 3))));//����Kֵ
			iter = 0;
			model.setPara(pl_.getA(), pl_.getB(), pl_.getC(), pl_.getD());
			continue;//������һ��ѭ��
		}
		iter++;
	
		
	}

	delete[]nIndex;
	inliers_threshold = threshold;
	bay++;
	std::cout<<bay<<std::endl;
	return true;
}


bool td::PlaneFitting::computeByLMedS()
{
	srand((unsigned)time(NULL));
	if (cloud.size() <= 3)
		return false;

	int randnum = model.needNum;
	int* nIndex = new int[randnum];    //���������
	//����ͳ�Ʋ����Ѿ�����
	int initial_iter = 100;
	int iter(0);  //RANSACѭ������
	double min_median(1000), mid_deviation(0), bstd(0);//��С��ֵ������ѭ�����ڵ���Ŀ 


	//��ѭ�������͸��ʵĸ��»�����Ҫ��ֵ
	while (iter < initial_iter)
	{
		//�ҳ�����ֵ��ߵ�������
		for (int i = 0; i < randnum; i++)
		{
			nIndex[i] = rand() % cloud.size();
		}

		//�жϳ�ȡ������Ƿ��ظ�
		bool allsame(true);
		for (int i = 1; i < randnum; i++)
		{
			allsame = allsame&&nIndex[i] == nIndex[i - 1];
		}
		if (allsame)
			continue;

		Plane pl_;
		PointCloud randcloud_;
		for (int i = 0; i < randnum; i++)
		{
			randcloud_.push_back(cloud[nIndex[i]]);
		}

		pl_.computeFromPoints(randcloud_);

		std::vector<double> model_deviation_p(0);//ͳ��ƫ����ֵ

		mid_deviation = 0;//��ֵ

		for (size_t i = 0; i < cloud.size(); ++i)
		{
			double d2plane = pl_.point2plane(cloud[i]);
			model_deviation_p.push_back(d2plane);
		}

		sort(model_deviation_p.begin(), model_deviation_p.end());
		if (model_deviation_p.size() % 2 == 0)
			mid_deviation = (model_deviation_p[model_deviation_p.size() / 2 - 1] +
			model_deviation_p[model_deviation_p.size() / 2]) / 2;
		else
			mid_deviation = model_deviation_p[model_deviation_p.size() / 2];

		//���¼���㼯�������
		double std_ = 0;
		std_ = computeStd(pl_, cloud);

		//Ѱ����С��ֵ��������ģ�Ͳ���
		if (mid_deviation < min_median)
		{
			min_median = mid_deviation;
			int inliers = 0;

			for (size_t i = 0; i < cloud.size(); ++i)
			{
				double d2plane = pl_.point2plane(cloud[i]);
				if (d2plane <= 2 * std_)
				{
					inliers++;
				}
			}
			bstd = std_;
			model.setPara(pl_.getA(), pl_.getB(), pl_.getC(), pl_.getD());
			initial_iter = int(ceil(log10(0.01) / log10(1.0 - pow((double)inliers / cloud.size(), 3))));
			iter = 0;
			continue;
		}
		iter++;
	
	}


	delete[]nIndex;
	inliers_threshold = 2 * bstd;
	return true;
}


//BAYLMEDS�������
bool td::PlaneFitting::computeByBayLMedS()
{
	srand((unsigned)time(NULL));
	if (cloud.size() <= 3)
		return false;

	//��ʼ��������
	int initial_iter = int(ceil(log10(0.01) / log10(1.0 - (double)model.needNum / cloud.size())));
	int stas_num = int(0.1*(double)initial_iter);
	std::vector<double> pro(cloud.size(), 0);//����ֵ���������һһ��Ӧ
	int randnum = model.needNum;
	

	double para_percent = 0;//���Ų���ռ�ܲ����ı���

	//���ڲ���ͳ��

	Plane stat_best_plane;
	double stat_min_penalty(std::numeric_limits<double>::max());
	
	//���˸о����в���ͳ�Ʋ����ף�ͨ��Ѱ�����Ų�����ȷ���������
	for (int iter = 0; iter < 6;)
	{
		std::set<int> nIndex;//���������
		for (int i = 0; i < randnum; i++)
		{
			int curIndex = rand() % cloud.size();
			nIndex.insert(curIndex);
		}
		//�ж������ȡ�ĵ��Ƿ��ظ�
		if (nIndex.size() < randnum)
			continue;

		Plane pl;
		PointCloud randcloud;
		std::set<int>::const_iterator it_nIndex;
		for (it_nIndex = nIndex.begin(); it_nIndex != nIndex.end(); ++it_nIndex)
		{
			randcloud.push_back(cloud[*it_nIndex]);
		}
		pl.computeFromPoints(randcloud);

		std::vector<double> model_deviation(0);//ͳ��ƫ����ֵ

		double cur_penalty = 0;
		for (size_t i = 0; i < cloud.size(); i++)
		{
			double d2plane = pl.point2plane(cloud[i]);
			model_deviation.push_back(d2plane);
		}

		sort(model_deviation.begin(), model_deviation.end());

		//��ȡ��ֵ
		if (model_deviation.size() % 2 == 0)
			cur_penalty = (model_deviation[model_deviation.size() / 2 - 1] + 
			model_deviation[model_deviation.size() / 2]) / 2;
		else
			cur_penalty = model_deviation[model_deviation.size() / 2];

		//����ͳ���е�������ֵ
		if (cur_penalty < stat_min_penalty)
		{
			stat_min_penalty = cur_penalty;
			stat_best_plane = pl;
		}

		iter++;	
	}


	//��Ҷ˹����
	/*std::vector<double>::iterator it_f = plnum.begin();
	std::vector<double>::iterator it_s = max_element(plnum.begin(), plnum.end());
	size_t para_best_index = it_s - it_f;*/

	//ͨ������ͳ��Ϊ�㸳�������
	//�����׼��
	//double std = computeStd(planes[para_best_index], cloud);
	double th = computeMStd(stat_best_plane, cloud, stat_min_penalty);
	for (size_t i = 0; i < cloud.size(); i++)
	{
		double d2plane = stat_best_plane.point2plane(cloud[i]); 

		if (d2plane <= th)
		{
			pro[i] = 1 - d2plane / th;
		}
	}

	//����ͳ�Ʋ����Ѿ�����
	
	//double best_median(1000), mid_deviation(0), bstd(0);//��С��ֵ������ѭ�����ڵ���Ŀ 
	double best_penalty(std::numeric_limits<double>::max());
	double best_inliers(0);
	double best_th(th);

	//��ѭ�������͸��ʵĸ��»�����Ҫ��ֵ
	std::vector<int> g_index(randnum, 0);
	int iter(0);  //��ʼѭ������
	while (iter < initial_iter)
	{
		//�ҳ�����ֵ��ߵ�������
		std::vector<int> cur_index(randnum, 0);
		std::vector<double> temp = pro;

		for (int i = 0; i < randnum; i++)
		{
			std::vector<double>::iterator it;
			it = max_element(temp.begin(), temp.end());
			int iIndex = it - temp.begin();
			cur_index[i] = iIndex;
			temp.erase(it);
		}

		bool goon = true;
		
		//����
		std::sort(cur_index.begin(), cur_index.end());
		std::sort(g_index.begin(), g_index.end());

		for (int i = 0; i < randnum; i++)
		{
			goon = goon&&cur_index[i] == g_index[i];
		}

		if (goon&&iter>0)
			break;

		for (int i = 0; i < randnum; i++)
		{
			g_index[i] = cur_index[i];
		}

		Plane pl_;
		PointCloud randcloud_;
		for (int i = 0; i < randnum; i++)
		{
			randcloud_.push_back(cloud[cur_index[i]]);
		}

		pl_.computeFromPoints(randcloud_);

		std::vector<double> model_deviation_p(0);//ͳ��ƫ����ֵ

		//mid_deviation = 0;//��ֵ
		double cur_penalty(0);

		for (size_t i = 0; i < cloud.size(); ++i)
		{
			double d2plane = pl_.point2plane(cloud[i]);
			model_deviation_p.push_back(d2plane);
		}

		sort(model_deviation_p.begin(), model_deviation_p.end());
		if (model_deviation_p.size() % 2 == 0)
			cur_penalty = (model_deviation_p[model_deviation_p.size() / 2 - 1] + 
			model_deviation_p[model_deviation_p.size() / 2]) / 2;
		else
			cur_penalty = model_deviation_p[model_deviation_p.size() / 2];

		//���¼���㼯�������
		double k = 0;     //kΪ��ǰ��ֵ�����ڵ�ĸ���
		double cur_th = 0;
		cur_th = computeMStd(pl_, cloud, cur_penalty);

		for (size_t i = 0; i < cloud.size(); ++i)
		{
			double d2plane = pl_.point2plane(cloud[i]);
			if (d2plane <= best_th)
			{
				k = k + 1.0;
			}
		}

		for (size_t i = 0; i < cloud.size(); ++i)
		{
			double d2plane = pl_.point2plane(cloud[i]);
			if (d2plane <= best_th)
			{
				pro[i] = pro[i] * k / cloud.size();
			}
		}

		//Ѱ����С��ֵ��������ģ�Ͳ���
		if (cur_penalty < best_penalty)
		{
			best_penalty = cur_penalty;
			best_inliers = k;
			best_th = cur_th;
			/*for (size_t i = 0; i < cloud.size(); ++i)
			{
				double d2plane = pl_.point2plane(cloud[i]);
				if (d2plane <= kk * std_)
				{
					inliers++;
				}
			}
			bstd = std_;
			model.setPara(pl_.getA(), pl_.getB(), pl_.getC(), pl_.getD());*/
			model = pl_;
			//initial_iter = int(ceil(log10(0.01) / log10(1.0 - pow((double)best_inliers / cloud.size(), 3))));
			//iter = 0;
		}
		iter++;
	}

	inliers_threshold = best_th;
	return true;
}

// ����ƽ�����
td::Plane td::PlaneFitting::getModel()
{
	return model;
}


// ���ؾ��ڵ�
td::PointCloud td::PlaneFitting::getInliers()
{
	PointCloud inliers;
	for (size_t i = 0; i < cloud.size(); ++i)
	{
		double d2plane = model.point2plane(cloud[i]);
		if (d2plane <= inliers_threshold)
		{
			inliers.push_back(cloud[i]);
		}
	}
	return inliers;
}


// ����ģ�ͱ�׼��ֵ
double td::PlaneFitting::getInlierThershold()
{
	return inliers_threshold;
}


// ���ؾ����
td::PointCloud td::PlaneFitting::getOutliers()
{
	PointCloud outliers;
	for (size_t i = 0; i < cloud.size(); ++i)
	{
		double d2plane = model.point2plane(cloud[i]);
		if (d2plane > inliers_threshold)
		{
			outliers.push_back(cloud[i]);
		}
	}
	return outliers;
}


// ����㵽��ı�׼��
double td::PlaneFitting::computeStd(Plane& pl, PointCloud& cloud)
{
	double avg_d(0), avg_d2(0);
	for (size_t i = 0; i < cloud.size(); i++)
	{
		double d2plane = pl.point2plane(cloud[i]); 
		avg_d += d2plane;
		avg_d2 += pow(d2plane, 2.0);
	}
	double ptnum = (double)cloud.size();
	/*avg_d /= ptnum;
	avg_d2 /= ptnum;*/
	double std = sqrt(avg_d2/ptnum - pow(avg_d/ptnum, 2.0));
	return std;
}

// ��С��ֵ��Ӧ�ı�׼��,��������С��ֵ����ֵ�ļ���
// derta = 1.4856[1 + 5/(n-p)]/sqrt(Mj)
double td::PlaneFitting::computeMStd(Plane& pl, PointCloud& cloud, double penalty)
{
	double num = (double)cloud.size();
	double sigma = 1.4826 * (1 + 5.0 / (num - 3.0))*penalty;
	return 2.5*sigma;
}