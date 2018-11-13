#include "isv_fusion_gpu.h"

isv_fusion_gpu::isv_fusion_gpu(QWidget *parent)
	: QWidget(parent)
{

	m_last_path = QApplication::applicationDirPath();
	ui.setupUi(this);
}

isv_fusion_gpu::~isv_fusion_gpu()
{
	//octree_device.clear();
}

void isv_fusion_gpu::slot_LoadClouds()
{
	QStringList list_cloud_path = QFileDialog::getOpenFileNames(this, "Open cloud files", m_last_path, "PLY(*.ply);;Ascii(*.asc;*.txt;*.xyz)");
	if (list_cloud_path.isEmpty())
	{
		return;
	}

	m_last_path = list_cloud_path.last();

	QDateTime time_begin = QDateTime::currentDateTime();
	int pos_id = 0;

	for (auto it = list_cloud_path.begin(); it != list_cloud_path.end(); it++)
	{
		QString base_name = QFileInfo(*it).completeBaseName();
		if (base_name.contains("Raw"))
		{
			pos_id = base_name.remove("Raw").toInt();
		}
		else if (base_name.contains("Sparse"))
		{
			pos_id = base_name.remove("Sparse").toInt();
		}

		else
		{
			pos_id++;
		}

		if (m_map_cloud_ptr.contains(pos_id))
		{
			qDebug() << "Position id " << pos_id << " conflict!";
			delete m_map_cloud_ptr[pos_id];
			m_map_cloud_ptr.remove(pos_id);
			m_map_pt_num.remove(pos_id);
			//			continue;
		}

		int pt_num = 0;
		double* cloud_ptr = LoadClouds_PLY_Bin(*it, pt_num);
		if (cloud_ptr != 0)
		{
			m_map_cloud_ptr[pos_id] = cloud_ptr;
			m_map_pt_num[pos_id] = pt_num;
			qDebug() << "Position id " << pos_id << " load succeed! Point number " << pt_num;
		}
		else
		{
			qDebug() << "Position id " << pos_id << " load failed!";
		}
		//----------------------------
		id_min = std::min(id_min, pos_id);
		id_max = std::max(id_max, pos_id);
		//----------------------------
	}

	QDateTime time_end = QDateTime::currentDateTime();

	qDebug() << "Load succeed! Time: " << time_begin.msecsTo(time_end) << " ms.";
	qDebug() << "Load point cloud from : " << id_min << " to " << id_max;

	refreshNumber();
}

void isv_fusion_gpu::slot_ClearClouds()
{
	QDateTime time_begin = QDateTime::currentDateTime();
	for (auto it = m_map_cloud_ptr.begin(); it != m_map_cloud_ptr.end(); it++)
	{
		delete *it;
	}
	m_map_cloud_ptr.clear();
	m_map_pt_num.clear();
	//------------------------

	int id_min = INT_MAX;
	int id_max = 0;
	//------------------------
	QDateTime time_end = QDateTime::currentDateTime();
	qDebug() << "Clear succeed! Time: " << time_begin.msecsTo(time_end) << " ms.";
	refreshNumber();
}

void isv_fusion_gpu::slot_MergeClouds()
{
	if (m_map_cloud_ptr.isEmpty() || m_map_pt_num.isEmpty())
	{
		return;
	}

	int total_num = 0;
	foreach(int pt_num, m_map_pt_num)
	{
		total_num += pt_num;
	}

	template_cloud.resize(total_num);

	int current_num = 0;
	QList<int> list_pos_id = m_map_cloud_ptr.keys();

	xmin = xmax = m_map_cloud_ptr[list_pos_id[0]][0];
	ymin = ymax = m_map_cloud_ptr[list_pos_id[0]][1];
	zmin = zmax = m_map_cloud_ptr[list_pos_id[0]][2];

	for (int it_id = 0; it_id < list_pos_id.size(); it_id++)
	{
		int pos_id = list_pos_id[it_id];
		double* cloud_ptr = m_map_cloud_ptr[pos_id];
		if (!m_map_pt_num.contains(pos_id))
		{
			qDebug() << "Position id " << pos_id << " have no point number!";
			continue;
		}
		int pt_num = m_map_pt_num[pos_id];
		for (int idx = 0; idx < pt_num; idx++)
		{
			template_cloud[current_num].x = cloud_ptr[idx * 3 + 0];
			template_cloud[current_num].y = cloud_ptr[idx * 3 + 1];
			template_cloud[current_num].z = cloud_ptr[idx * 3 + 2];
			current_num++;
			//-----------------
			xmin = std::min(cloud_ptr[idx * 3 + 0], xmin);
			xmax = std::max(cloud_ptr[idx * 3 + 0], xmax);
			ymin = std::min(cloud_ptr[idx * 3 + 1], ymin);
			ymax = std::max(cloud_ptr[idx * 3 + 1], ymax);
			zmin = std::min(cloud_ptr[idx * 3 + 2], zmin);
			zmax = std::max(cloud_ptr[idx * 3 + 2], zmax);
			//-----------------

		}
	}
	if (current_num != total_num)
	{
		qDebug() << "Final point number error! Total: " << total_num << " Current:" << current_num;
	}

	qDebug() << "min" << xmin << " " << ymin << " " << zmin;

	qDebug() << "max" << xmax << " " << ymax << " " << zmax;

	qDebug() << "X dimension:" << xmax - xmin;
	qDebug() << "Y dimension:" << ymax - ymin;
	qDebug() << "Z dimension:" << zmax - zmin;


	//m_octree.setInputCloud(template_cloud.makeShared());
	//m_octree.addPointsFromInputCloud();

	// 往GPU放入点云
	pcl::gpu::Octree::PointCloud cloud_device;
	cloud_device.upload(template_cloud.points);


	//点云放入八叉树中并建立
	QDateTime time_begin_build = QDateTime::currentDateTime();

	
	octree_device.setCloud(cloud_device);
	octree_device.build();


	if (octree_device.isBuilt())
	{
		QDateTime time_end_build = QDateTime::currentDateTime();
		qDebug() << "Constructing GPU Octree Tims is: " << time_begin_build.msecsTo(time_end_build) << " ms.";
	}
	else
	{
		qDebug() << "Constructing GPU Octree failed...";
	}
	//// 搜索点
	//std::vector<pcl::PointXYZ> query_host;
	//query_host.resize(template_cloud.points.size());

	//for (int i = 0; i < template_cloud.points.size(); i++)
	//{
	//	query_host[i].x = template_cloud.points[i].x;
	//	query_host[i].y = template_cloud.points[i].y;
	//	query_host[i].z = template_cloud.points[i].z;
	//}

	//qDebug() << "start to upload queries to device....There are " << template_cloud.size()<<"query points";

	//size_t buffer_size = sizeof(template_cloud);
	//
	//qDebug() << "the buffer size is" << 3 * sizeof(template_cloud[0].x) * template_cloud.size() << " bytes";
	//
	//pcl::gpu::Octree::Queries queries_device;

	//queries_device.upload(query_host);
	//

	//// Output buffer on the device

	//max_answers = ui.doubleSpinBox_Resolution->value();

	//qDebug() << "There are : " << max_answers << " points to considerate";

	//pcl::gpu::NeighborIndices result_device(queries_device.size(), max_answers);


	//QDateTime time_begin_search = QDateTime::currentDateTime();
	//
	//// Do the actual search on gpu octree
	//octree_device.radiusSearch(queries_device, radius_m, max_answers, result_device);

	//

	//QDateTime time_end_search = QDateTime::currentDateTime();
	//qDebug() << "Octree radius search Tims is: " << time_begin_search.msecsTo(time_end_search) << " ms.";

	//result_device.sizes.download(sizes);
	//result_device.data.download(data);

	//std::cout << "INFO: data size : " << data.size() << " data.size" << std::endl;
	//std::cout << "INFO: searched  : " << sizes.size() << " Points" << std::endl;
	//std::cout << "INFO: Data generated" << std::endl;
}

void isv_fusion_gpu::slot_AddClouds()
{
	if (m_map_cloud_ptr.isEmpty() || m_map_pt_num.isEmpty())
	{
		return;
	}
	QDateTime time_begin = QDateTime::currentDateTime();
	QList<int> list_pos_id = m_map_cloud_ptr.keys();

	for (int it_id = 0; it_id < list_pos_id.size(); it_id++)
	{
		int pos_id = list_pos_id[it_id];
		double* cloud_ptr = m_map_cloud_ptr[pos_id];
		if (!m_map_pt_num.contains(pos_id))
		{
			qDebug() << "Position id " << pos_id << " have no point number!";
			continue;
		}
		int pt_num = m_map_pt_num[pos_id];
		pcl::PointCloud<pcl::PointXYZ> template_cloud;
		template_cloud.resize(pt_num);
		for (int idx = 0; idx < pt_num; idx++)
		{
			template_cloud[idx].x = cloud_ptr[idx * 3 + 0];
			template_cloud[idx].y = cloud_ptr[idx * 3 + 1];
			template_cloud[idx].z = cloud_ptr[idx * 3 + 2];
		}


	}

	QDateTime time_end = QDateTime::currentDateTime();
	qDebug() << "Constructing Octree Tims is: " << time_begin.msecsTo(time_end) << " ms.";
}

void isv_fusion_gpu::slot_startfusion()
{
	QDateTime time_begin = QDateTime::currentDateTime();

	pcl::PointCloud<pcl::PointXYZ> overlap;

	pcl::PointCloud<pcl::PointXYZ> not_overlap;
	pcl::PointCloud<pcl::PointXYZ> fusion_part;

	std::vector <bool> flags(sizes.size(), true);

	//const int max_answers = 3;



	int cnt_not_overlap = 0;
	int cnt_fusion = 0;

	std::vector<float> fusion;


	for (size_t i = 0; i < sizes.size(); ++i)
	{
		if (sizes[i] >= 2)
		{
			//overlap.push_back(cloud.points[i]);
			//overlap_parts << cloud.points[i] << std::endl;
		}

		if (flags[i] == true)
		{
			if (sizes[i] >= 2)
			{
				//out << "INFO: sizes : " << i << " size " << sizes[i] << std::endl;

				flags[i] = false;
				float tmpX = 0;
				float tmpY = 0;
				float tmpZ = 0;

				for (size_t j = 0; j < sizes[i]; ++j)
				{
					tmpX = tmpX + template_cloud.points[data[j + i * max_answers]].x;
					tmpY = tmpY + template_cloud.points[data[j + i * max_answers]].y;
					tmpZ = tmpZ + template_cloud.points[data[j + i * max_answers]].z;
					flags[data[j + i * max_answers]] = false;
				}

				tmpX = tmpX / sizes[i];
				tmpY = tmpY / sizes[i];
				tmpZ = tmpZ / sizes[i];
				fusion.emplace_back(tmpX);
				fusion.emplace_back(tmpY);
				fusion.emplace_back(tmpZ);
				cnt_fusion++;

				//fusion_result << tmpX << " " << tmpY << " " << tmpZ << std::endl;
				//fusion_part.push_back(pcl::PointXYZ(tmpX, tmpY, tmpZ));
			}
		}

		if (sizes[i] < 2)
		{
			//not_overlap.push_back(cloud.points[i]);
			//not_overlap_parts << template_cloud[i].x << " " << template_cloud[i].y << " " << template_cloud[i].z << std::endl;

			fusion.emplace_back(template_cloud[i].x);
			fusion.emplace_back(template_cloud[i].y);
			fusion.emplace_back(template_cloud[i].z);

			cnt_not_overlap++;
		}
	}

	// 由于蓝光计算导入单片点云数量上限是800w，所以需要拆分。
	const int pt_limits = 7500000;
	int pts_num = fusion.size() / 3;

	if (pts_num < pt_limits)
	{
		//生成融合后的点云文件名
		fusion_filename = "result\\Raw" + std::to_string(id_max) + ".ply";

		FILE *fp_fusion = fopen(fusion_filename.c_str(), "wb");
		fprintf(fp_fusion, "ply\n");
		fprintf(fp_fusion, "format binary_little_endian 1.0\n");
		fprintf(fp_fusion, "comment ISV generated\n");
		fprintf(fp_fusion, "element vertex %d\n", fusion.size() / 3);
		fprintf(fp_fusion, "property float64 x\n");
		fprintf(fp_fusion, "property float64 y\n");
		fprintf(fp_fusion, "property float64 z\n");
		fprintf(fp_fusion, "end_header\n");

		for (int i = 0; i < pts_num; i++)
		{
			double tmp2[3] = { fusion[3 * i + 0], fusion[3 * i + 1], fusion[3 * i + 2] };
			fwrite(tmp2, sizeof(double), 3, fp_fusion);
		}
		fclose(fp_fusion);
		fp_fusion = NULL;
	}

	if (pts_num >= pt_limits)
	{
		std::vector <std::string> parts(pts_num / pt_limits);

		for (int i = 0; i < parts.size(); i++)
		{
			if (pts_num - (i)* pt_limits > 0)
			{
				parts[i] = std::to_string(i);

				FILE *fp_fusion = fopen(("result\\Raw" + std::to_string(id_min) + "_" + std::to_string(i) + ".ply").c_str(), "wb");
				//FILE *fp_fusion = fopen(("result\\Raw" + std::to_string(id_min) + ".ply").c_str(), "wb");

				fprintf(fp_fusion, "ply\n");
				fprintf(fp_fusion, "format binary_little_endian 1.0\n");
				fprintf(fp_fusion, "comment ISV generated\n");
				fprintf(fp_fusion, "element vertex %d\n", pt_limits);
				fprintf(fp_fusion, "property float64 x\n");
				fprintf(fp_fusion, "property float64 y\n");
				fprintf(fp_fusion, "property float64 z\n");
				fprintf(fp_fusion, "end_header\n");

				for (int j = i * pt_limits; j < (i + 1)*pt_limits; j++)
				{
					double tmp2[3] = { fusion[3 * j + 0], fusion[3 * j + 1], fusion[3 * j + 2] };
					fwrite(tmp2, sizeof(double), 3, fp_fusion);
				}
				fclose(fp_fusion);
				fp_fusion = NULL;
			}


			if (pts_num - ((i + 2)* pt_limits) < 0)
			{
				FILE *fp_fusion2 = fopen(("result\\Raw" + std::to_string(id_max) + ".ply").c_str(), "wb");
				fprintf(fp_fusion2, "ply\n");
				fprintf(fp_fusion2, "format binary_little_endian 1.0\n");
				fprintf(fp_fusion2, "comment ISV generated\n");
				fprintf(fp_fusion2, "element vertex %d\n", pts_num - (i + 1)* pt_limits);
				fprintf(fp_fusion2, "property float64 x\n");
				fprintf(fp_fusion2, "property float64 y\n");
				fprintf(fp_fusion2, "property float64 z\n");
				fprintf(fp_fusion2, "end_header\n");

				for (int j = (i + 1) * pt_limits; j < pts_num; j++)
				{
					double tmp2[3] = { fusion[3 * j + 0], fusion[3 * j + 1], fusion[3 * j + 2] };
					fwrite(tmp2, sizeof(double), 3, fp_fusion2);
				}

				fclose(fp_fusion2);
				fp_fusion2 = NULL;
			}
		}

	}

	std::cout << "fused part: " << cnt_fusion << std::endl << std::endl;
	std::cout << "not overlap part: " << cnt_not_overlap << std::endl << std::endl;

	QDateTime time_end = QDateTime::currentDateTime();
	qDebug() << "Fusion Tims is: " << time_begin.msecsTo(time_end) << " ms.";
}

void isv_fusion_gpu::refreshNumber()
{
	int total_num = 0;
	foreach(int num, m_map_pt_num)
	{
		total_num += num;
	}

	ui.label_PointsNum->setText(QString::number(total_num));
	ui.label_CloudsNum->setText(QString::number(m_map_cloud_ptr.size()));
}

double* isv_fusion_gpu::LoadClouds_PLY_Bin(QString filePath, int &ptNum)
{
	QFile cloudFile(filePath);
	if (!cloudFile.open(QIODevice::ReadOnly))
	{
		return false;
	}

	QString strSizeX, strSizeY, strSizeZ;
	while (!cloudFile.atEnd())
	{
		QString strLine = cloudFile.readLine();
		if (strLine.simplified() == "end_header")
		{
			break;
		}
		else
		{
			QStringList list_Line = strLine.simplified().split(" ");
			if (list_Line.indexOf("format") != -1)
			{
				if (list_Line[list_Line.indexOf("format") + 1].compare("binary_little_endian", Qt::CaseInsensitive) != 0)
				{
					cloudFile.close();
					return false;
				}
			}
			else if (list_Line.indexOf("element") != -1)
			{
				if (list_Line.indexOf("vertex") != -1)
				{
					ptNum = list_Line[list_Line.indexOf("vertex") + 1].toInt();
				}
			}
			else if (list_Line.indexOf("property") != -1)
			{
				if (list_Line.indexOf("x") != -1)
				{
					strSizeX = list_Line[list_Line.indexOf("x") - 1];
				}
				else if (list_Line.indexOf("y") != -1)
				{
					strSizeY = list_Line[list_Line.indexOf("y") - 1];
				}
				else if (list_Line.indexOf("z") != -1)
				{
					strSizeZ = list_Line[list_Line.indexOf("z") - 1];
				}
			}
		}
	}

	if (cloudFile.atEnd())
	{
		cloudFile.close();
		return false;
	}




	if (strSizeX.isEmpty() || strSizeX != strSizeY || strSizeX != strSizeZ)
	{
		cloudFile.close();
		return false;
	}

	double *ptCloud = new double[ptNum * 3];
	if (strSizeX == "float" || strSizeX == "float32")
	{
		float *data_temp = new float[ptNum * 3];
		int maxLen = ptNum * 3 * sizeof(float);
		if (maxLen != cloudFile.read((char*)data_temp, maxLen))
		{
			delete[]ptCloud;
			delete[]data_temp;
			cloudFile.close();
			return false;
		}
		for (int i = 0; i < ptNum * 3; i++)
	
		{
			ptCloud[i] = data_temp[i];
		}
		delete[]data_temp;
	}
	else if (strSizeX == "double" || strSizeX == "float64")
	{
		int maxLen = ptNum * 3 * sizeof(double);
		if (maxLen != cloudFile.read((char*)ptCloud, maxLen))
		{
			delete[]ptCloud;
			cloudFile.close();
			return false;
		}
	}
	else
	{
		delete[]ptCloud;
		cloudFile.close();
		return false;
	}

	cloudFile.close();
	return ptCloud;
}
