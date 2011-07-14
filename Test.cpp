#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSimplePointsReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkPolyData.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkOutlineSource.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkRendererCollection.h>
#include <vtkWorldPointPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkImageViewer2.h>
#include <vtkImageBlend.h>
#include <vtkPlane.h>
#include <vtkLine.h>
#include <vtkMatrix3x3.h>

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

#include <iostream>
#include <fstream> 
#include <vector>
#include <list>
#include <ctime>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <math.h>
#include "Elements.h"
#include "Ply.h"
#include "Clusters_Set.h"

#include "Debug.h"
#include "Translation.h"

#include "MouseInteractorStyle.h"
#include <stdio.h>
#include <opencv/cv.h>
#include <nuklei/Kernel.h>
#include <nuklei/GenericKernel.h>
#include <nuklei/KernelCollection.h>
#include <time.h>
#include <ctime>

#define PI 3.14159265
#define MAXLINE 30

// input .ply .asc .xyz

int main ( int argc, char *argv[] )
{	
	if(argc != 3)
	{
		std::cout << "Usage: " << argv[0] << "  Filename(.ply)" << std::endl;
		return EXIT_FAILURE;
	}

	std::string inputFilename = argv[1];
	std::string inputRangeScene=argv[2];

	int n_cluster=Clusters_Set::clusters_extraction(inputRangeScene); //extract clusters from the scene

	vector <TranslLib::Translation> cluster_pose(n_cluster);

	float x,y,z,xmin,xmax,ymin,ymax,zmin,zmax;

	//LOAD PLYMODEL

	Ply *plymodel=new Ply();
	Debug *debug=new Debug();

	int n_vertex;
	int n_faces;
	n_vertex=plymodel->getNumVertex(inputFilename);
	n_faces=plymodel->getNumFaces(inputFilename);
	plymodel->n_vertex=n_vertex;
	plymodel->n_faces=n_faces;
	std::cout<<plymodel->n_faces<<" "<<plymodel->n_vertex<<std::endl;
	plymodel->vertexes=plymodel->getListVertex(inputFilename, n_vertex);
	plymodel->faces=plymodel->getListFaces(inputFilename,n_vertex,n_faces);

	Vertex *vertex= new Vertex();
	Face *face=new Face();
	std::list<Face>::iterator iter = plymodel->faces.begin();
	Plane *plane=new Plane();
	int it=0,itr=0;
	int c=0;

	while(iter!= plymodel->faces.end()){

		*face=*iter;

		plane=face->getPlane(plymodel->vertexes,*face);
		iter->plane=*plane;

		iter++;
		it++;

	}

	for(int icluster=0;icluster<n_cluster;icluster++){//for each cluster on the table

		stringstream ss;
		ss << "cloud_cluster_" << icluster << ".asc";
		std::string sst=ss.str();

		FILE *pFilein=fopen(sst.c_str(),"r");
		int nt=0;

		c=0;

		while(!feof(pFilein)){
			fscanf(pFilein,"%f %f %f",&x,&y,&z);
			if(c==0){
				xmax=xmin=x;
				ymax=ymin=y;
				zmax=zmin=z;
				c++;
			}

			if(x>=xmax)
				xmax=x;
			if(x<xmin)
				xmin=x;
			if(y>=ymax)
				ymax=y;
			if(y<ymin)
				ymin=y;
			if(z>=zmax)
				zmax=z;
			if(z<zmin)
				zmin=z;
		}

		int 	xMin=(xmax-xmin)/2+xmin-3;
		int 	xMax=(xmax-xmin)/2+xmin+3;
		int 	yMin=(ymax-ymin)/2+ymin-3;
		int 	yMax=(ymax-ymin)/2+ymin+3;
		int 	zMin=(zmax-zmin)/2+zmin-3;
		int 	zMax=(zmax-zmin)/2+zmin+3;

		//MAIN ALGORITHM

		FILE* pFile=fopen (sst.c_str(), "r");

		int numOfTranslation=0;

		Ply *translated_model=plymodel->New(plymodel);
		Ply *model=plymodel->New(plymodel);
		Ply *rotated_model=plymodel->New(plymodel);
		using namespace TranslLib;
		std::list<Translation> translations;

		Point *point;
		int iteration=0;
		int cycle=0;
		srand((unsigned)time(NULL));
		TranslLib::Translation *pose=new Translation();
		TranslLib::Translation *worstpose=new Translation();

		FILE * ff=fopen("points.txt","w");
		std::list<Translation>::iterator iter2;
		std::list<Translation> best;
		std::list<Translation> besttransl;

		int iterRotation=0;
		int tetax,tetay,tetaz;
		int count=0;
		int extiteration=0;
		double curr_obj_value=-1;
		double prev_obj_value=-1;
		int lmin_detector=0;

		using namespace nuklei;
		kernel::se3 k;

		k.loc_h_ = 50; // Position standard deviation
		k.ori_h_ = 180*PI/180; // Orientation standard deviation (radians)
		double teta[3];

		int extiteration2=0;
		int alpha=2;
		cycle=0;

		extiteration=0;
		int trasliteration=0,traslcounter=0;
		TranslLib::Translation *best_pose=new TranslLib::Translation();
		double xbest=rand() % (xMax-xMin) +xMin;
		double ybest=rand() % (yMax-yMin) +yMin;
		double zbest=rand() % (zMax-zMin) +zMin;

		best_pose->object_value=-1;

		int lminbest_detector=0;
		double curr_best_value,prev_best_value;

		while(trasliteration<50){ // iterations on the translations
			trasliteration++;

			if(lminbest_detector==0){
				if (k.loc_h_>10)
					k.loc_h_=k.loc_h_-10;

			}

			if(lminbest_detector>3&&best_pose->object_value!=0){
				k.loc_h_=20;
				std::cout<<"too many times equal"<<std::endl;
				xbest=rand() % (xMax-xMin) +xMin;
				ybest=rand() % (yMax-yMin) +yMin;
				zbest=rand() % (zMax-zMin) +zMin;
				besttransl.push_back(*best_pose);
				best_pose->object_value=0;

			}

			k.loc_ = Vector3(xbest, ybest, zbest);
			kernel::se3 r = k.sample();
			Vector3 trasl=r.getLoc();
			x=trasl[0];
			y=trasl[1];
			z=trasl[2];

			point= new Point();
			point->px=x;
			point->py=y;
			point->pz=z;
			model=plymodel->translate_model(plymodel,point,model);//generate a plymodel centred in a specific translation

			Face *face=new Face();
			std::list<Face>::iterator iter = model->faces.begin();
			Plane *plane=new Plane();

			while(iter!= model->faces.end()){

				*face=*iter;

				plane=face->getPlane(model->vertexes,*face);
				iter->plane=*plane;

				iter++;
				it++;

			}

			TranslLib::Translation *translmodel=TranslLib::Translation::New(model,point);

			std::vector<Point> pointcl;
			Point pointtmp;

			FILE *pFilein=fopen(sst.c_str(),"r");
			int size=0;
			float xf,yf,zf;

			while(!feof(pFilein)){
				fscanf(pFilein,"%f %f %f",&xf,&yf,&zf);
				pointtmp.px=xf;
				pointtmp.py=yf;
				pointtmp.pz=zf;
				pointcl.push_back(pointtmp);
				size++;
			}

			//std::cout<<x<<" "<<y<<" "<<z<<std::endl;

			translmodel->getPointDistance(pointcl,nt,*(translmodel->origin),size, 50);
			if(translmodel->object_value>best_pose->object_value){
				*best_pose=*translmodel;
			}

			extiteration=0;
			iteration=0;
			tetay=rand()%180-90;
			tetax=rand()%180-90;
			tetaz=rand()%180-90;
			while(extiteration<100){//cycle of 100 iterations for each translation

				extiteration++;
				extiteration2++;

				if(k.ori_h_>10*PI/180){
					k.ori_h_=k.ori_h_-10*PI/180;
				}
				if(k.ori_h_<=10*PI/180){
					k.ori_h_=2*PI/180;
				}

				if(lmin_detector>3){
					best.push_back(*translations.begin());
					translations.clear();
					k.ori_h_=180*PI/180;

					tetay=rand()%180-90;
					tetax=rand()%180-90;
					tetaz=rand()%180-90;
				}

				while(iteration<20){// for each combination of translation and orientation

					teta[0]=tetax*PI/180;
					teta[1]=tetay*PI/180;
					teta[2]=tetaz*PI/180;
					Matrix3 ori(cos(teta[1])*cos(teta[2]), -cos(teta[0])*sin(teta[2])+sin(teta[0])*sin(teta[1])*cos(teta[2]), sin(teta[0])*sin(teta[2])+cos(teta[0])*sin(teta[1])*cos(teta[2]),
							cos(teta[1])*sin(teta[2]), cos(teta[0])*cos(teta[2])+sin(teta[0])*sin(teta[1])*sin(teta[2]), -sin(teta[0])*cos(teta[2])+cos(teta[0])*sin(teta[1])*sin(teta[2]),
							-sin(teta[1]),           sin(teta[0])*cos(teta[1]),                                 cos(teta[0])*cos(teta[1]));

					k.ori_ = la::quaternionCopy(ori);
					kernel::se3 r = k.sample();

					teta[0]=atan2(2*(r.ori_[0]*r.ori_[1]+r.ori_[2]*r.ori_[3]), 1-2*(r.ori_[1]*r.ori_[1]+r.ori_[2]*r.ori_[2]));
					teta[1]=asin(2*(r.ori_[0]*r.ori_[2]-r.ori_[3]*r.ori_[1]));
					teta[2]=atan2(2*(r.ori_[0]*r.ori_[3]+r.ori_[1]*r.ori_[2]),1-2*(r.ori_[2]*r.ori_[2]+r.ori_[3]*r.ori_[3]));

					point= new Point();
					point->px=x;
					point->py=y;
					point->pz=z;

					translated_model=plymodel->rotate_model(plymodel,point,translated_model,teta[0]*180/PI,teta[1]*180/PI,teta[2]*180/PI);

					Face *face=new Face();
					std::list<Face>::iterator iter = translated_model->faces.begin();
					Plane *plane=new Plane();
					it=0;
					itr=0;

					while(iter!= translated_model->faces.end()){

						*face=*iter;

						plane=face->getPlane(translated_model->vertexes,*face);
						iter->plane=*plane;

						iter++;
						it++;

					}

					TranslLib::Translation *transl=TranslLib::Translation::New(translated_model,point);

					transl->orientation[0]=teta[0]*180/PI;
					transl->orientation[1]=teta[1]*180/PI;
					transl->orientation[2]=teta[2]*180/PI;

					transl->Id=iteration;
					numOfTranslation++;

					iteration++;
					translations.push_back(*transl);


					delete face;
					delete plane;

				}

				fclose(pFile);

				std::list<Vertex>::iterator iter1 = translated_model->vertexes.begin();

				iter2 = translations.begin();

				FILE *pFILE=fopen("iterations","w");
				nt=0;

				int contap=0;
				std::vector<Point> pointcl;
				Point pointtmp;

				FILE *pFilein=fopen(sst.c_str(),"r");
				int size=0;

				while(!feof(pFilein)){
					fscanf(pFilein,"%f %f %f",&xf,&yf,&zf);
					pointtmp.px=xf;
					pointtmp.py=yf;
					pointtmp.pz=zf;
					pointcl.push_back(pointtmp);
					//std::cout<<i<<std::endl;
					size++;
				}

				count=0;

				while(iter2!=translations.end()){

					iter2->getPointDistance(pointcl,nt,*(iter2->origin),size, 50);//compute the objective function

					iter2++;
					nt++;
				}

				fclose(pFILE);
				fclose(pFilein);

				count=0;

				double max_value=0;
				double min_value=0;;
				iter2 = translations.begin();
				while(iter2!=translations.end()){

					if(iter2->flag==1){

						if(count==0){
							max_value=iter2->object_value;
							min_value=iter2->object_value;
							*pose=*iter2;}

						else if(iter2->object_value>max_value){
							max_value=iter2->object_value;
							*pose=*iter2;
						}
						else if (iter2->object_value<min_value){
							min_value=iter2->object_value;
							*worstpose=*iter2;//not used
						}
						count++;
					}
					iter2++;
				}

				iteration=0;
				prev_obj_value=curr_obj_value;
				curr_obj_value=pose->object_value;
				if(curr_obj_value==prev_obj_value){
					lmin_detector++;

				}
				else {
					lmin_detector=0;
				}

				translations.clear();
				translations.push_back(*pose);

				tetay=pose->orientation[0];
				tetax=pose->orientation[1];
				tetaz=pose->orientation[2];

			}

			best.push_back(*pose);
			iter2=best.begin();
			double max_value=0;
			max_value=iter2->object_value;
			*pose=*iter2;
			while(iter2!=best.end()){

				if (iter2->object_value>max_value){
					max_value=iter2->object_value;
					*pose=*iter2;
				}

				iter2++;
			}

			if(pose->object_value>=best_pose->object_value){

				*best_pose=*pose;
				xbest=best_pose->origin->px;
				ybest=best_pose->origin->py;
				zbest=best_pose->origin->pz;
				std::cout<<"orientation"<<best_pose->orientation[0]<<" "<<best_pose->orientation[1]<<" "<<best_pose->orientation[2]<<std::endl;
			}

			else {
				std::cout<<"else"<<std::endl;
				if(translmodel->object_value>=best_pose->object_value){
					std::cout<<translmodel->object_value<<std::endl;
					*best_pose=*translmodel;
					xbest=best_pose->origin->px;
					ybest=best_pose->origin->py;
					zbest=best_pose->origin->pz;
				}
			}

			std::cout<<"bestpose"<<best_pose->object_value<<std::endl;
			std::cout<<curr_best_value<<" "<<prev_best_value<<std::endl;
			prev_best_value=curr_best_value;
			curr_best_value=best_pose->object_value;
			if(curr_best_value==prev_best_value){

				std::cout<<"weird"<<std::endl;
				lminbest_detector++;

			}
			else {
				lminbest_detector=0;
			}


		}
		besttransl.push_back(*best_pose);
		int max_value=0;
		iter2=besttransl.begin();
		while(iter2!=besttransl.end()){
			if (iter2->object_value>max_value){
				max_value=iter2->object_value;
				*pose=*iter2;
			}

			iter2++;
		}

		std::cout<<"BEST SOLUTION"<<std::endl;
		std::cout<<"pose x="<<pose->origin->px<<"y="<<pose->origin->py<<"z="<<pose->origin->pz<<" "<<pose->object_value<<std::endl;
		//std::cout<<"orientation: "<<pose->orientation[0]<<" "<<pose->orientation[1]<<" "<<pose->orientation[2]<<std::endl;
		std::cout<<"orientation"<<pose->orientation[0]<<" "<<pose->orientation[1]<<" "<<pose->orientation[2]<<std::endl;
		std::cout<<"errors"<<pose->errors<<std::endl;

		iteration=0;

		cluster_pose[icluster]=*pose;

	}

	TranslLib::Translation *best_cluster_matching=new TranslLib::Translation();
	best_cluster_matching->object_value=0;;
	std::cout<<"primo"<<best_cluster_matching->object_value<<std::endl;
	for(int i=0;i<n_cluster;i++){
		std::cout<<best_cluster_matching->object_value<<std::endl;
		if((best_cluster_matching->object_value<cluster_pose[i].object_value)&&(cluster_pose[i].errors<5)){
			std::cout<<"right"<<cluster_pose[i].object_value<<std::endl;
			*best_cluster_matching=cluster_pose[i];
		}
	}

	std::cout<<"BEST SOLUTION"<<std::endl;
	std::cout<<"pose x="<<best_cluster_matching->origin->px<<"y="<<best_cluster_matching->origin->py<<"z="<<best_cluster_matching->origin->pz<<" "<<best_cluster_matching->object_value<<std::endl;
	//std::cout<<"orientation: "<<pose->orientation[0]<<" "<<pose->orientation[1]<<" "<<pose->orientation[2]<<std::endl;
	std::cout<<"orientation"<<best_cluster_matching->orientation[0]<<" "<<best_cluster_matching->orientation[1]<<" "<<best_cluster_matching->orientation[2]<<std::endl;
	std::cout<<"errors"<<best_cluster_matching->errors<<std::endl;
	best_cluster_matching->printPose(inputFilename,n_vertex, "poseFile.ply");
	return EXIT_SUCCESS;

}
