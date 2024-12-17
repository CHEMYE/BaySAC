/*
*点云的读取
*/

#ifndef pointio_h
#define pointio_h

#include "bayPoint.h"
#include <string>
#include <fstream>
#include <iostream>

namespace td
{
	class PointIO
	{
	public:
		PointIO();
		PointIO(std::string );
		~PointIO();
	private:
		// 点云数据
		PointCloud cloud;
	public:
		PointCloud getPointCloud();
		// 读入文件
		void open(std::string path);
		void save(std::string path);
	};
}
#endif

