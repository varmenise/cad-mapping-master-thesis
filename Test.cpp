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
		
	int 	xMin=250;
	int 	xMax=400;
	int 	yMin=300;
	int 	yMax=500;
	int 	zMin=1000;
	int 	zMax=1200;
	
	
	if(argc != 4)
    {
		std::cout << "Usage: " << argv[0] << "  Filename(.ply)" << std::endl;
		return EXIT_FAILURE;
    }
	
	std::string inputFilename = argv[1];
	std::string inputRangeScene=argv[2];
	
		
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
	Piano *piano=new Piano();
	int it=0,itr=0;
	int nt=0,c=0;


	while(iter!= plymodel->faces.end()){
		
		*face=*iter;
		
		
		piano=face->getPiano(plymodel->vertexes,*face);
		iter->piano=*piano;
		

		iter++;
	
		it++;
			
	}
	
	
	
	//MAIN ALGORITHM

	
	FILE* pFile=fopen (inputRangeScene.c_str(), "r");
	float x,y,z;
	int numOfTranslation=0;
	
	Ply *translated_model=plymodel->New(plymodel);
	Ply *rotated_model=plymodel->New(plymodel);
	using namespace TranslLib;
	std::list<Translation> translations;
	
	Point *point;
	int iteration=0;
	int cycle=0;
	srand((unsigned)time(NULL));
	TranslLib::Translation *pose=new Translation();
	
	FILE * ff=fopen("points.txt","w");
	std::list<Translation>::iterator iter2;
	
	int iterRotation=0;
	int tetax,tetay,tetaz;
	int count=0;
	int extiteration=0;
	
	
	using namespace nuklei;
	kernel::se3 k;
	
	
	k.loc_h_ = 1000; // Position standard deviation
	k.ori_h_ = 0.2; // Orientation standard deviation (radians)
	double teta[3];
	x=355;
	y=390;
	z=11300;
	
	tetax=-20;
	tetay=87;
	tetaz=0;
	
	while(extiteration<1){
		extiteration++;
		if(k.loc_h_>10)
		k.loc_h_=k.loc_h_-10;
		
		
	while(iteration<1){
		k.loc_ = Vector3(x, y, z);
		teta[0]=tetax*PI/180;
		teta[1]=tetay*PI/180;
		teta[2]=tetaz*PI/180;
		Matrix3 ori(cos(teta[1])*cos(teta[2]), -cos(teta[0])*sin(teta[2])+sin(teta[0])*sin(teta[1])*cos(teta[2]), sin(teta[0])*sin(teta[2])+cos(teta[0])*sin(teta[1])*cos(teta[2]),
					cos(teta[1])*sin(teta[2]), cos(teta[0])*cos(teta[2])+sin(teta[0])*sin(teta[1])*sin(teta[2]), -sin(teta[0])*cos(teta[2])+cos(teta[0])*sin(teta[1])*sin(teta[2]),
					-sin(teta[1]),           sin(teta[0])*cos(teta[1]),                                 cos(teta[0])*cos(teta[1]));
		
		k.ori_ = la::quaternionCopy(ori);
		kernel::se3 r = k.sample();
		
		Vector3 trasl=r.getLoc();
		//Matrix3 orient=la::matrixCopy(r.ori_);
		
		//teta[0]=atan2(2*(r.ori_[0]*r.ori_[1]+r.ori_[2]*r.ori_[3]), 1-2*(r.ori_[1]*r.ori_[1]+r.ori_[2]*r.ori_[2]));
		//teta[1]=asin(2*(r.ori_[0]*r.ori_[2]-r.ori_[3]*r.ori_[1]));
		//teta[2]=atan2(2*(r.ori_[0]*r.ori_[3]+r.ori_[1]*r.ori_[2]),1-2*(r.ori_[2]*r.ori_[2]+r.ori_[3]*r.ori_[3]));
		//std::cout<<teta[0]<<" "<<teta[1]<<" "<<teta[2]<<std::endl;
		//teta[0]=25.4806;
		//teta[1]=-30.2297;
		//teta[2]=-165.994;
		
		//x=rand() % (xMax-xMin) +xMin;
		//y=rand() % (yMax-yMin) +yMin;
		//z=rand() % (zMax-zMin) +zMin;
		
		//x=355;
		//y=390;
		//z=1130; 
		//x=344.589;
		//y=345.808;
		//z=1052.4; 
		x=trasl[0];
		y=trasl[1];
		z=trasl[2];
		
		
		
			
		point= new Point();
		point->px=x;
		point->py=y;
		point->pz=z;

		//translated_model=plymodel->translate_model(plymodel,point,translated_model);
		//translated_model=plymodel->rotate_model(plymodel,point,rotated_model,-20,87,0);
		translated_model=plymodel->rotate_model(plymodel,point,translated_model,teta[0]*180/PI,teta[1]*180/PI,teta[2]*180/PI);
		
		
		
		Face *face=new Face();
		std::list<Face>::iterator iter = translated_model->faces.begin();
		Piano *piano=new Piano();
		it=0;
		itr=0;
		
		
		
		while(iter!= translated_model->faces.end()){
			
			*face=*iter;
			
			
			piano=face->getPiano(translated_model->vertexes,*face);
			iter->piano=*piano;
			
			
			
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
		
		
		iterRotation=0;
		//debug->printFaces(translated_model->faces);
		//debug->printVertexPiano(translated_model->faces);
		//debug->printPlanes(translated_model->faces);
		
		
		while(iterRotation<0){
			//tetay=rand()%180-90;
			//tetax=rand()%180-90;
			//tetaz=rand()%180-90;
			tetax=teta[0];
			tetay=teta[1];
			tetaz=teta[2];
			//std::cout<<tetax<<" "<<tetay<<" "<<tetaz<<std::endl;
			rotated_model=plymodel->rotate_model(plymodel,point,rotated_model,tetax,tetay,tetaz);
			
			iter = rotated_model->faces.begin();
			
			while(iter!= rotated_model->faces.end()){
				
				*face=*iter;
				
				
				piano=face->getPiano(rotated_model->vertexes,*face);
				iter->piano=*piano;
				
				iter++;
				
				itr++;
				
			}
			
			numOfTranslation++;
			TranslLib::Translation *rot=TranslLib::Translation::New(rotated_model,point);
			rot->orientation[0]=tetax;
			rot->orientation[1]=tetay;
			rot->orientation[2]=tetaz;
		
			
			translations.push_back(*rot);
			iterRotation++;
		}
		
		
		delete face;
		delete piano;
		
		
		
		
		
		
	}
	
	
	fclose(pFile);
	
	std::cout<<"numoftransl"<<numOfTranslation<<std::endl;
	
	std::list<Vertex>::iterator iter1 = translated_model->vertexes.begin();
	
	
	iter2 = translations.begin();
	
	FILE *pFILE=fopen("iterations","w");
	nt=0;

	int contap=0;
	std::vector<Point> pointcl(90000);
	Point pointtmp;
		
	
	FILE *pFilein=fopen(inputRangeScene.c_str(),"r");
	int i=0;
	while(!feof(pFilein)){
		fscanf(pFilein,"%f %f %f",&x,&y,&z);
		
		pointtmp.px=x;
		pointtmp.py=y;
		pointtmp.pz=z;
		pointcl[i]=pointtmp;
		//std::cout<<i<<std::endl;
		i++;
	}
	std::cout<<i<<std::endl;
	
	//clock_t start,end;
	//start=clock();
	while(iter2!=translations.end()){
		
		//std::cout<<"getPointDistance"<<std::endl;
		
		
		iter2->getPointDistance(pointcl,nt,*(iter2->origin));
		
		
		
		iter2++;
		nt++;
	}
	//end=clock();
	//double tempo=double((double(end-start))/CLOCKS_PER_SEC);
	//std::cout<<"time"<<end<<" "<<tempo<<std::endl;
	fclose(pFILE);
	fclose(pFilein);
	
	
	
	 count=0;
	
	double max_value=0;
	iter2 = translations.begin();
	while(iter2!=translations.end()){
		//std::cout<<iter2->object_value<<std::endl;
		if(iter2->flag==1){	
			
			
			if(count==0){
				max_value=iter2->object_value;
				//std::cout<<"fuuuck"<<std::endl;
				*pose=*iter2;}
			
			else if(iter2->object_value>max_value){
				max_value=iter2->object_value;
				*pose=*iter2;
				//std::cout<<"fuuuck"<<std::endl;
			}
			
			
			
			count++;
		}
		iter2++;
	}
	
		
	std::cout<<"pose x="<<pose->origin->px<<"y="<<pose->origin->py<<"z="<<pose->origin->pz<<" "<<pose->object_value<<std::endl;
	//std::cout<<"orientation: "<<pose->orientation[0]<<" "<<pose->orientation[1]<<" "<<pose->orientation[2]<<std::endl;
	std::cout<<"orientation"<<pose->orientation[0]*180/PI<<" "<<pose->orientation[0]*180/PI<<" "<<pose->orientation[0]*180/PI<<std::endl;
	iteration=0;
	
	translations.clear();
	
	
	
	
	translations.push_back(*pose);
	
	
//	using namespace nuklei;
//	kernel::se3 k;
	
	
	x=pose->origin->px;
	y=pose->origin->py;
	z=pose->origin->pz;
	
	
	tetay=pose->orientation[0];
	tetax=pose->orientation[1];
	tetaz=pose->orientation[2];
		
	}
	
	pose->printPose(inputFilename,n_vertex, "poseFile.ply");
	
	iteration=0;
	
/*
 using namespace nuklei;
 kernel::se3 k;
 

 
 double tetax=tetax*PI/180;
 double tetay=tetay*PI/180;
 double tetaz=tetaz*PI/180;
 
 int count=0;
 k.loc_ = Vector3(x, y, z);
 Matrix3 ori(cos(tetay)*cos(tetaz), -cos(tetax)*sin(tetaz)+sin(tetax)*sin(tetay)*cos(tetaz), sin(tetax)*sin(tetaz)+cos(tetax)*sin(tetay)*cos(tetaz),
 cos(tetay)*sin(tetaz), cos(tetax)*cos(tetaz)+sin(tetax)*sin(tetay)*sin(tetaz), -sin(tetax)*cos(tetaz)+cos(tetax)*sin(tetay)*sin(tetaz),
 -sin(tetay),           sin(tetax)*cos(tetay),                                 cos(tetax)*cos(tetay));
 
 k.ori_ = la::quaternionCopy(ori);
 k.loc_h_ = 4; // Position standard deviation
 k.ori_h_ = 0.005; // Orientation standard deviation (radians)
 int count1=0;
 while(count1<100){
 count1++;
 
 while(count<5){
 count++;
 kernel::se3 r = k.sample();
 
 Vector3 trasl=r.getLoc();
 //Matrix3 orient=la::matrixCopy(r.ori_);
 double teta[3];
 teta[0]=atan2(2*(r.ori_[0]*r.ori_[1]+r.ori_[2]*r.ori_[3]), 1-2*(r.ori_[1]*r.ori_[1]+r.ori_[2]*r.ori_[2]));
 teta[1]=asin(2*(r.ori_[0]*r.ori_[2]-r.ori_[3]*r.ori_[1]));
 teta[2]=atan2(2*(r.ori_[0]*r.ori_[3]+r.ori_[1]*r.ori_[2]),1-2*(r.ori_[2]*r.ori_[2]+r.ori_[3]*r.ori_[3]));
 std::cout<<trasl<<std::endl;
 std::cout<<teta[0]*180/PI<<" "<<teta[1]*180/PI<<" "<<teta[2]*180/PI<<std::endl;
// std::cout<<cos(tetax)<<std::endl;
 
 
			point= new Point();
			point->px=x;
			point->py=y;
			point->pz=z;
		
			translated_model=plymodel->translate_model(plymodel,point,translated_model);
		
	
		
			face=new Face();
			iter = translated_model->faces.begin();
			piano=new Piano();
						  it=0;
						  itr=0;
		
			
		
			while(iter!= translated_model->faces.end()){
			
				*face=*iter;
			
			
				piano=face->getPiano(translated_model->vertexes,face);
				iter->piano=*piano;
			

			
				iter++;
			
				it++;
			
			
			}
		
	
		
		TranslLib::Translation *transl=TranslLib::Translation::New(translated_model,point);
		
		transl->Id=iteration;
	
				
		iteration++;
		//translations.push_back(*transl);

		
	
			
			//std::cout<<tetax<<" "<<tetay<<" "<<tetaz<<std::endl;
			rotated_model=plymodel->rotate_model(plymodel,point,rotated_model,tetax,tetay,tetaz);
			
			iter = rotated_model->faces.begin();
		
			while(iter!= rotated_model->faces.end()){
			
				*face=*iter;
			
			
				piano=face->getPiano(rotated_model->vertexes,face);
				iter->piano=*piano;
			
				iter++;
			
				itr++;
			
			}
			
		numOfTranslation++;
		
		TranslLib::Translation *rot=TranslLib::Translation::New(rotated_model,point);
		rot->orientation[0]=tetax;
		rot->orientation[1]=tetay;
		rot->orientation[2]=tetaz;
		translations.push_back(*rot);
	
		
		
	
		delete face;
		delete piano;
		
		
		
		
		
				
		}
	
	
	fclose(pFile);
	
	std::cout<<"numoftransl"<<numOfTranslation<<std::endl;
	
	 iter1 = translated_model->vertexes.begin();

	
	iter2 = translations.begin();
	nt=0;
	pFILE=fopen("iterations","w");
	
	while(iter2!=translations.end()){
		
		
		iter2->getPointDistance(inputRangeScene,nt,*(iter2->origin));
	

		
		iter2++;
		nt++;
		}
	
	fclose(pFILE);
	
	
	
	count=0;
	
	 max_value=0;
	iter2 = translations.begin();
	while(iter2!=translations.end()){
		//std::cout<<iter2->object_value<<std::endl;
		if(iter2->flag==1){	
			
	
			if(count==0){
			max_value=iter2->object_value;
				//std::cout<<"fuuuck"<<std::endl;
				*pose=*iter2;}
		
		else if(iter2->object_value>max_value){
			max_value=iter2->object_value;
			*pose=*iter2;
			//std::cout<<"fuuuck"<<std::endl;
		}
		
		
		
		count++;
		}
		iter2++;
	}
	
	std::cout<<"pose x="<<pose->origin->px<<"y="<<pose->origin->py<<"z="<<pose->origin->pz<<" "<<pose->object_value<<std::endl;
	std::cout<<"orientation: "<<pose->orientation[0]<<" "<<pose->orientation[1]<<" "<<pose->orientation[2]<<std::endl;
	
	pose->printPose(inputFilename,n_vertex, "poseFile.ply");
		
	
	
		
		k.loc_ = Vector3(pose->origin->px,pose->origin->py,pose->origin->pz);
		Matrix3 ori;
		ori.SetColumn(0, Vector3((double)pose->orientation[0],(double)pose->orientation[1],(double)pose->orientation[2]));
		k.ori_ = la::quaternionCopy(ori);
		k.loc_h_ = 20; // Position standard deviation
		k.ori_h_ = 3; // Orientation standard deviation (radians)
		

	}delete plymodel;
	delete translated_model;
	delete point;
	

	*/
	
	return EXIT_SUCCESS;
	
}