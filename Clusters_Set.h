#include "pcl/ModelCoefficients.h"
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/features/normal_3d.h>
#include <pcl/filters/extract_indices.h>
#include "pcl/filters/voxel_grid.h"
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include "pcl/segmentation/extract_clusters.h"

using namespace std;
using namespace pcl;

class Clusters_Set{
public:
	static int clusters_extraction(std::string inputFilename);
};
// Read in the cloud data


int Clusters_Set::clusters_extraction(std::string inputFilename){
	PCDReader reader;
	PointCloud<PointXYZ>::Ptr cloud (new PointCloud<PointXYZ>);
	reader.read (inputFilename.c_str(), *cloud);
	cerr << "PointCloud before filtering has: " << cloud->points.size () << " data points." << endl; //*

	// Create the filtering object: downsample the dataset using a leaf size of 1cm
	VoxelGrid<PointXYZ> vg;
	PointCloud<PointXYZ>::Ptr cloud_filtered (new PointCloud<PointXYZ>);
	vg.setInputCloud (cloud);
	vg.setLeafSize (0.01, 0.01, 0.01);
	vg.filter (*cloud_filtered);
	cerr << "PointCloud after filtering has: " << cloud_filtered->points.size ()  << " data points." << endl; //*

	// Create the segmentation object for the planar model and set all the parameters
	SACSegmentation<PointXYZ> seg;
	PointIndices::Ptr inliers (new PointIndices);
	ModelCoefficients::Ptr coefficients (new ModelCoefficients);
	PointCloud<PointXYZ>::Ptr cloud_plane (new PointCloud<PointXYZ> ());
	PCDWriter writer;
	seg.setOptimizeCoefficients (true);
	seg.setModelType (SACMODEL_PLANE);
	seg.setMethodType (SAC_RANSAC);
	seg.setMaxIterations (100);
	seg.setDistanceThreshold (0.02);

	int i=0, nr_points = cloud_filtered->points.size ();
	while (cloud_filtered->points.size () > 0.3 * nr_points)
	{
		// Segment the largest planar component from the remaining cloud
		seg.setInputCloud(cloud_filtered);
		seg.segment (*inliers, *coefficients); //*
		if (inliers->indices.size () == 0)
		{
			cout << "Could not estimate a planar model for the given dataset." << endl;
			break;
		}

		// Extract the planar inliers from the input cloud
		ExtractIndices<PointXYZ> extract;
		extract.setInputCloud (cloud_filtered);
		extract.setIndices (inliers);
		extract.setNegative (false);

		// Write the planar inliers to disk
		extract.filter (*cloud_plane); //*
		cerr << "PointCloud representing the planar component: " << cloud_plane->points.size () << " data points." << endl;

		// Remove the planar inliers, extract the rest
		extract.setNegative (true);
		extract.filter (*cloud_filtered); //*
	}

	// Creating the KdTree object for the search method of the extraction
	KdTree<PointXYZ>::Ptr tree (new KdTreeFLANN<PointXYZ>);
	tree->setInputCloud (cloud_filtered);

	vector<PointIndices> cluster_indices;
	EuclideanClusterExtraction<PointXYZ> ec;
	ec.setClusterTolerance (0.02); // 2cm
	ec.setMinClusterSize (100);
	ec.setMaxClusterSize (25000);
	ec.setSearchMethod (tree);
	ec.setInputCloud( cloud_filtered);
	ec.extract (cluster_indices);

	std::cout<<cluster_indices.size()<<std::endl;

	int j = 0;
	FILE*pfile;
	for (vector<PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it)
	{	
		stringstream ss;
		ss << "cloud_cluster_" << j << ".asc";
		std::string sst=ss.str();
		pfile=fopen(sst.c_str(),"w");

		stringstream ssxyz;
		ssxyz << "cloud_cluster_" << j << ".xyz";
		std::string sstxyz=ssxyz.str();
		FILE*pfilexyz=fopen(sstxyz.c_str(),"w");

		PointCloud<PointXYZ>::Ptr cloud_cluster (new PointCloud<PointXYZ>);
		for (vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); pit++){
			cloud_cluster->points.push_back (cloud_filtered->points[*pit]); //*
			std::cout<<cloud_filtered->points[*pit]<<std::endl;
			fprintf(pfile, "%f %f %f\n", cloud_filtered->points[*pit].x*1000, cloud_filtered->points[*pit].y*1000, cloud_filtered->points[*pit].z*1000);
			fprintf(pfilexyz, "%f %f %f\n", cloud_filtered->points[*pit].x*1000, cloud_filtered->points[*pit].y*1000, cloud_filtered->points[*pit].z*1000);

		}
		fclose(pfile);

		cerr << "PointCloud representing the Cluster: " << cloud_cluster->points.size () << " data points." << endl;

		//ss << "cloud_cluster_" << j << ".pcd";

		/*for(int lol = 0; lol < cloud_cluster->points.size (); lol++){
		 std::cout<<cloud_cluster->points.x[lol].x<<std::endl;
		 }*/


		//writer.write<PointXYZ> (ss.str (), *cloud_cluster, false); //*
		j++;
	}

	return j;
}
