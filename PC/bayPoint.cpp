#include "include/bayPoint.h"
#include <cmath>

td::Point::Point():
x(0),
y(0),
z(0)
{

}

td::Point::Point(double x,double y,double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

td::Point::~Point()
{
}

//设置点的属性
void td::Point::setPoint(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

//可以用来sort和作为map的key
 bool td::operator<(const Point &_pnta, const Point &_pntb)
{
	if (_pnta.x<_pntb.x){
		return true;
	}
	else if (_pnta.x>_pntb.x){
		return false;
	}
	else if (_pnta.y<_pntb.y){
		return true;
	}
	else if (_pnta.y>_pntb.y){
		return false;
	}
	else if (_pnta.z<_pntb.z){
		return true;
	}
	else if (_pnta.z>_pntb.z){
		return false;
	}
	else{
		return false;
	}
 }

 bool td::Point::operator==(const Point& pnt)
 {
	 if (x == pnt.x)
	 if (y == pnt.y)
	 if (z == pnt.z)
		 return true;
	 return false;
 }
