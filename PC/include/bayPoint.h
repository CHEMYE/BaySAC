/*
*基本点类型
*/

#ifndef point_h
#define point_h

#include <vector>

namespace td
{
	class Point
	{
	public:
		Point();
		//构造函数:Point(x,y,z)
		Point(double, double, double);
		~Point();

		//可以用来sort和作为map的key
		friend bool operator<(const Point &_pnta, const Point &_pntb);

		//设置点的属性
		void setPoint(double, double, double);
		

	public:
		double x;
		double y;
		double z;
		bool operator==(const Point& pnt);
	};

	typedef std::vector<td::Point> PointCloud;
}
#endif
