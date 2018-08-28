#include <iostream>
#include <fstream>
#include <vector>

#include <boost/format.hpp>

#include <opencv2/opencv.hpp>

#include <Eigen/Geometry>

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>

// #inlcude
using namespace std;

void Learn_Mat_Definiton();

int main(int argc, char **argv)
{
    vector<cv::Mat> colorImgs, depthImgs;
    vector<Eigen::Isometry3d> poses;

    ifstream fin("./pose.txt");

    if (!fin)
    {
        cerr << "请放入相机外参文件pose.txt" << endl;
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        boost::format fmt("./%s/%d.%s");
        colorImgs.push_back(cv::imread((fmt % "color" % (i + 1) % "png").str()));
        depthImgs.push_back(cv::imread((fmt % "depth" % (i + 1) % "pgm").str(), -1));

        double data[7] = {0};
        for (auto &d : data)
            fin >> d;

        Eigen::Quaterniond q(data[6], data[3], data[4], data[5]);
        Eigen::Isometry3d t(q);
        t.pretranslate(Eigen::Vector3d(data[0], data[1], data[2]));

        poses.push_back(t);

        // cout << q << endl;
    }

    //计算点云
    double cx = 325.3;
    double cy = 253.5;
    double fx = 518.0;
    double fy = 519.0;
    double depthScale = 1000.0;

    cout << "正在进行转换:" << endl;

    cout << "poses.size: " << poses.size() << endl;

    typedef pcl::PointXYZRGB PointT;
    typedef pcl::PointCloud<PointT> PointCloud;

    PointCloud::Ptr pointCloud(new PointCloud);
    for (int i = 0; i < 5; i++)
    {
        cout << "转换图像中: " << i + 1 << endl;
        cv::Mat color = colorImgs[i];
        cv::Mat depth = depthImgs[i];
        Eigen::Isometry3d T = poses[i];
        for (int v = 0; v < color.rows; v++)
        {
            for (int u = 0; u < color.cols; u++)
            {
                unsigned int d = depth.ptr<unsigned short>(v)[u]; //深度值
                if (d == 0)
                    continue;
                Eigen::Vector3d point;
                point[2] = double(d) / depthScale;
                point[0] = (u - cx) * point[2] / fx;
                point[1] = (v - cy) * point[2] / fy;
                Eigen::Vector3d pointWorld = T * point;

                PointT p;
                p.x = pointWorld[0];
                p.y = pointWorld[1];
                p.z = pointWorld[2];
                p.b = color.data[v * color.step + u * color.channels()];
                p.g = color.data[v * color.step + u * color.channels() + 1];
                p.r = color.data[v * color.step + u * color.channels() + 2];

                pointCloud->points.push_back(p);
            }
        }
    }

    pointCloud->is_dense = false;

    cout << "pointCloud共有" << pointCloud->size() << "个点" << endl;

    pcl::io::savePCDFileBinary("map.pcd", *pointCloud);

    Learn_Mat_Definiton();

    return 0;
}

void Learn_Mat_Definiton() //测试一下step[]的各个维度大小
{

    //////////////////Demo1(3维矩阵)///////////////////////////////////////////
    printf("//////////////////////Demo1(3维矩阵)////////////////////////\n");
    //最后面的两个数：(行，列),确定了一个面
    //是一个依次降维的过程
    //8,10组成了面，5个面，组成了立方体
    int matSize[] = {5, 8, 10}; //每一维元素的个数：8：行，10：列
    cv::Mat mat1(3, matSize, CV_16UC3, cv::Scalar::all(0));

    //求step[i]的大小:每一维元素的大小(单位字节)
    printf("\n///////step[i]的大小//////////\n");
    printf("step[0]:%d\n", mat1.step[0]); //480：面的大小(第一维)
    printf("step[1]:%d\n", mat1.step[1]); //60：线的大小(第二维)
    printf("step[2]:%d\n", mat1.step[2]); //6：点的大小(第三维)

    //求size[i]：每一维元素的个数
    printf("\n///////size[i]的大小///////\n");
    printf("size[0]:%d\n", mat1.size[0]); //5：面
    printf("size[1]:%d\n", mat1.size[1]); //8：线
    printf("size[2]:%d\n", mat1.size[2]); //10：点

    //求step1(i)：每一维元素的通道数
    printf("\n///////step1(i)的大小///////\n");
    printf("step1(0):%d\n", mat1.step1(0)); //240:面
    printf("step1(1):%d\n", mat1.step1(1)); //30:线
    printf("step1(2):%d\n", mat1.step1(2)); //3:点

    //求elemSize：每个元素的大小(单位字节)
    printf("\n///////elemSize的大小///////\n");
    printf("elemSize:%d\n", mat1.elemSize()); //6：每个元素的大小

    //求elemSize1：每个通道的大小(单位字节)
    printf("\n///////elemSize1的大小///////\n");
    printf("elemSize1:%d\n", mat1.elemSize1()); //2：每个通道的大小
}