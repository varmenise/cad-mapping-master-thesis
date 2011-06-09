	/*
	 *  Translation.cpp
	 *  
	 *
	 *  Created by valentina armenise on 3/15/11.
	 *  Copyright 2011 Politecnico di Torino. All rights reserved.
	 *
	 */


	#include <iostream>
	#include <fstream.h> 
	#include <list>
	#include <string>
	#include <cstdio>
	#include <stdio.h>
	#include <math.h>
	#include <vtkSmartPointer.h>
	#include <vtkPlane.h>
	#include <vtkLine.h>
	#include "Elements.h"
	#include "Ply.h"
	#include "Translation.h"



	TranslLib::Translation* TranslLib::Translation::New(Ply*plymodel, Point *origin){
		Translation* newtrans=new Translation();
		newtrans->vertexes=plymodel->vertexes;
		newtrans->faces=plymodel->faces;
		newtrans->n_faces=plymodel->n_faces;
		newtrans->n_vertex=plymodel->n_vertex;
		newtrans->origin=origin;
		
		
		return newtrans;
		
	}

	float TranslLib::Translation::getPointDistance(std::string rangeFilename,int nt, Point origin){
		//u can replace translated_model with this
		FILE *pFile=fopen(rangeFilename.c_str(),"r");
		Point *point=new Point();
		Face *face=new Face();
		Piano piano;
		float a,b,c,d;
		int it=0,nface,npoint=0;
		std::list<Point> points;
		float min,dist=0,x,y,z;
		
		this->object_value=0;
		this->flag=0;
		point->nface==-1;
		FILE* pFile1=fopen ("distPoint-Face.txt", "w");
		
		double xa,xb,ya,yb,za,zb;
		double da, db;
		int entered=0;
		double minim;


		
		while(!feof(pFile)){//for each point

			fscanf(pFile,"%f %f %f",&x,&y,&z);
			
			point->px=x;
			point->py=y;
			point->pz=z;

			//std::cout<<"point"<<x<<" "<<y<<" "<<z<<std::endl;
			point->mindist=0;
			//point->nface==-1;
			std::list<Face>::iterator iter = this->faces.begin();
			it=0;
				double tmp;
			
			
			
				
			while(iter!= this->faces.end()){//for each face
			//while(it<2){
				//std::cout<<" "<<std::endl;
				entered=0;
				*face=*iter;
				piano=face->piano;
				a=piano.parameters[0];
				b=piano.parameters[1];
				c=piano.parameters[2];
				d=piano.parameters[3];
		
				/*a=0;
				b=1;
				c=1;
				d=0;*/
				dist=point->computeDistance(a,b,c,d);
				//std::cout<<"piano"<<dist<<std::endl;
				
				Vertex vtx=piano.point[0];
				/*std::cout<<vtx.vx<<std::endl;
				std::cout<<vtx.vy<<std::endl;
				std::cout<<vtx.vz<<std::endl;*/
				
			
			
				
				//vtkSmartPointer<vtkPlane> plane = 
				//vtkSmartPointer<vtkPlane>::New();
				//plane->SetOrigin(vtx.vx, vtx.vy, vtx.vz);
				//plane->SetOrigin(0, 0, 0);
				//plane->SetNormal(a/sqrt(pow(a,2)+pow(b,2)+pow(c,2)), b/sqrt(pow(a,2)+pow(b,2)+pow(c,2)), c/sqrt(pow(a,2)+pow(b,2)+pow(c,2)));
				
				double p[3] = {point->px, point->py, point->pz};
				double origin[3] = {vtx.vx, vtx.vy, vtx.vz};
				//double origin[3] = {0.0, 0.0, 0.0};
				double normal[3] = {a/sqrt(pow(a,2)+pow(b,2)+pow(c,2)), b/sqrt(pow(a,2)+pow(b,2)+pow(c,2)), c/sqrt(pow(a,2)+pow(b,2)+pow(c,2))};
				double projected[3];
				//double projected2[3];
				double projmatrix[4][4];
				
				
				/*projmatrix[0][0]=1-pow(a,2)/(pow(a,2)+pow(b,2)+pow(c,2));
				std::cout<<"matrix"<<projmatrix[0][0]<<std::endl;
				projmatrix[0][1]=-a*b/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[0][2]=-a*c/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[0][3]=-a*d/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[1][0]=-b*a/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[1][1]=1-pow(b,2)/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[1][2]=-b*c/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[1][3]=-b*d/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[2][0]=-c*a/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[2][1]=-c*b/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[2][2]=1-pow(c,2)/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[2][3]=-c*d/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[3][0]=-d*a/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[3][1]=-d*b/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[3][2]=-d*c/(pow(a,2)+pow(b,2)+pow(c,2));
				projmatrix[3][3]=1-pow(d,2)/(pow(a,2)+pow(b,2)+pow(c,2));
				
				projected2[0]=p[0]*projmatrix[0][0]+p[0]*projmatrix[1][0]+p[0]*projmatrix[2][0]+p[0]*projmatrix[3][0];
				projected2[1]=p[1]*projmatrix[0][1]+p[1]*projmatrix[1][1]+p[1]*projmatrix[2][1]+p[1]*projmatrix[3][1];
				projected2[2]=p[2]*projmatrix[0][2]+p[2]*projmatrix[1][2]+p[2]*projmatrix[2][2]+p[2]*projmatrix[3][2];
				 std::cout<<"proj"<<projected2[0]<<" "<<projected2[1]<<" "<<projected2[2]<<std::endl;
				std::cout<<"pro11 "<<projmatrix[0][1]*p[1]<<std::endl;
				std::cout<<"pro11 "<<projmatrix[1][1]*p[1]<<std::endl;
				std::cout<<"pro11 "<<projmatrix[2][1]*p[1]<<std::endl;
				std::cout<<"pro11 "<<projmatrix[3][1]*p[1]<<std::endl;*/
				
				double xo[3],t;
				xo[0]=p[0]-origin[0];
				xo[1]=p[1]-origin[1];
				xo[2]=p[2]-origin[2];
				
				t=normal[0]*xo[0]+normal[1]*xo[1]+normal[2]*xo[2];
				
				projected[0]=p[0]-t*normal[0];
				projected[1]=p[1]-t*normal[1];
				projected[2]=p[2]-t*normal[2];
				
				 //std::cout<<"proj"<<projected2[0]<<" "<<projected2[1]<<" "<<projected2[2]<<std::endl;
				
				
				
				
				//plane->ProjectPoint(p, origin, normal, projected);
				//std::cout<<"proj"<<projected[0]<<" "<<projected[1]<<" "<<projected[2]<<std::endl;
				//std::cout<<" "<<std::endl;
				
				
				xa=piano.point[0].vx;
				xb=piano.point[1].vx;
				ya=piano.point[0].vy;
				yb=piano.point[1].vy;
				za=piano.point[0].vz;
				zb=piano.point[1].vz;
					//std::cout<<xa<<" "<<xb<<" "<<ya<<" "<<yb<<" "<<za<<" "<<zb<<std::endl;
							
		
				
				if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
					
					dist+=0;
					
				}
				
				else {
				
					entered++;
					//std::cout<<"en1"<<std::endl;
					//std::cout<<(projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<<std::endl;
					//std::cout<<(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<<std::endl;
				
					
				}

				
				xa=piano.point[1].vx;
				xb=piano.point[2].vx;
				ya=piano.point[1].vy;
				yb=piano.point[2].vy;
				za=piano.point[1].vz;
				zb=piano.point[2].vz;
				//std::cout<<xa<<" "<<xb<<" "<<ya<<" "<<yb<<" "<<za<<" "<<zb<<std::endl;
				
				if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
					
					dist+=0;
					//std::cout<<(projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<<std::endl;
					//std::cout<<(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<<std::endl;
					
				}
				else {
					
					entered++;
						//std::cout<<"en2"<<std::endl;
					//std::cout<<(projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<<std::endl;
					//std::cout<<(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<<std::endl;
				
					
				}
				
				
				xa=piano.point[2].vx;
				xb=piano.point[3].vx;
				ya=piano.point[2].vy;
				yb=piano.point[3].vy;
				za=piano.point[2].vz;
				zb=piano.point[3].vz;
					//std::cout<<xa<<" "<<xb<<" "<<ya<<" "<<yb<<" "<<za<<" "<<zb<<std::endl;
			
				
				if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
					
					dist+=0;
					
				}
				else {
					
					entered++;
					//	std::cout<<"en3"<<std::endl;
					//std::cout<<(projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<<std::endl;
					//std::cout<<(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<<std::endl;
					
				}
				
				xa=piano.point[3].vx;
				xb=piano.point[0].vx;
				ya=piano.point[3].vy;
				yb=piano.point[0].vy;
				za=piano.point[3].vz;
				zb=piano.point[0].vz;
				//	std::cout<<xa<<" "<<xb<<" "<<ya<<" "<<yb<<" "<<za<<" "<<zb<<std::endl;
				//std::cout<<"zb"<<zb<<std::endl;
				
				if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
					
					dist+=0;
					
				}
				else {
					
					entered++;
						//std::cout<<"en4"<<std::endl;
					//std::cout<<(projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<<std::endl;
					//std::cout<<(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<<std::endl;
				
					
				}
				
			
			double d[3];
			double va[3],vb[3],vc[3],vd[3];

			va[0]=piano.point[0].vx;
			va[1]=piano.point[0].vy;
			va[2]=piano.point[0].vz;
			
			vb[0]=piano.point[1].vx;
			vb[1]=piano.point[1].vy;
			vb[2]=piano.point[1].vz;
			
			vc[0]=piano.point[2].vx;
			vc[1]=piano.point[2].vy;
			vc[2]=piano.point[2].vz;
			
			vd[0]=piano.point[3].vx;
			vd[1]=piano.point[3].vy;
			vd[2]=piano.point[3].vz;
				
				Point pr;
				pr.px=projected[0];
				pr.py=projected[1];
				pr.pz=projected[2];
			
			
				//std::cout<<"ent"<<entered<<std::endl;
			if(entered==2){
				//std::cout<<"vale"<<std::endl;
				//d[0]=vtkLine::DistanceToLine(projected, va, vb);//returns squared distance
				//std::cout<<"old"<<d[0]<<std::endl;
				d[0]=Point::computeDistancePointLine(pr, piano.point[0], piano.point[1]);//sto ritornando il quadrato per farl auguale ma toglilo
				//std::cout<<"new"<<d[0]<<std::endl;
				//std::cout<<" "<<std::endl;
				//d[1]=vtkLine::DistanceToLine(projected, vb, vc);
				
				d[1]=Point::computeDistancePointLine(pr, piano.point[1], piano.point[2]);
				
				
				
			
				//d[2]=vtkLine::DistanceToLine(projected, vc, vd);
				
				d[2]=Point::computeDistancePointLine(pr, piano.point[2], piano.point[3]);
				
				
				//d[3]=vtkLine::DistanceToLine(projected, vd, va);
				//std::cout<<"old2 "<<d[3]<<std::endl;
				d[3]=Point::computeDistancePointLine(pr, piano.point[3], piano.point[0]);
				//std::cout<<"new2 "<<d[3]<<std::endl;
				//std::cout<<" "<<std::endl;
				minim=d[0];
				for(int i=0;i<4;i++){
					if(d[i]<=minim)
						minim=d[i];
				}
				//std::cout<<minim<<std::endl;
			}
			if(entered==4){
				//std::cout<<"vale"<<std::endl;
				d[0]=sqrt(pow(projected[0]-va[0],2)+pow(projected[1]-va[1],2)+pow(projected[2]-va[2],2));
				d[1]=sqrt(pow(projected[0]-vb[0],2)+pow(projected[1]-vb[1],2)+pow(projected[2]-vb[2],2));
				d[2]=sqrt(pow(projected[0]-vc[0],2)+pow(projected[1]-vc[1],2)+pow(projected[2]-vc[2],2));
				d[3]=sqrt(pow(projected[0]-vd[0],2)+pow(projected[1]-vd[1],2)+pow(projected[2]-vd[2],2));
				minim=d[0];
				for(int i=0;i<4;i++){
					if(d[i]<=minim){
						minim=d[i];
						//std::cout<<i<<std::endl;
					}
				}
				//std::cout<<minim<<std::endl;
			}
						
			dist+=minim;
				//std::cout<<"entered"<<entered<<std::endl;	
			
				
				
				if (it==0){
					point->mindist=dist;
					point->nface=it;
				}
				
				else if (dist<point->mindist){
					point->mindist=dist;
					point->nface=it;
				}
				
				iter++;
				
				it++;
				
			}


	//std::cout<<"dist"<<dist<<std::endl;
			//fprintf(pFile1, "%f\n", point->mindist);
			if (point->mindist>=10){
				
				point->mindist=point->mindist*1;//by 3??
			}
			
			double m=0.5*point->mindist;
				
			this->object_value+=(1-m);
			
			this->flag=1;
			
			
			npoint++;
		
			
	}
		//std::cout<<"obj "<<this->object_value<<std::endl;
		
		delete point;
		delete face;
		fclose(pFile);
		return nface;
	}


	void TranslLib::Translation::printPose(std::string inputFilename, int n_vertex, std::string output){
		
		std::list <Vertex> vertexes;
		Vertex *vertex= new Vertex();
		bool end_header=false;
		char buf[200],el_vert[20],el_faces[20],s1[20],s2[20];
		//std::string buf;
		float x,y,z;
		int line=0;
		float a,b,c,d,e,f;
		
		
		std::list<Vertex>::iterator iter1 = this->vertexes.begin();
		
		FILE *pFile = fopen (inputFilename.c_str() ,"r");
		FILE *poseFile= fopen (output.c_str(), "w");
		
		
		while(!feof(pFile)) {
			
			
			if (end_header==false){
				//fgets ((char*)buf.c_str() , MAXLINE , pFile);
				fgets (buf , MAXLINE , pFile);
				fputs (buf, poseFile);
		 
				if (strstr(buf,"end_header")!=NULL){
					end_header=true;
		 //std::cout<<"fuck"<<std::endl;
		 
		 
				}	
			}
			else {
				if (line<n_vertex){
					line++;
					fscanf(pFile, "%f %f %f %f %f %f", &a, &b, &c, &d, &e, &f);
					
					*vertex=*iter1;
					
					if(line==n_vertex)
						fprintf(poseFile, "%f %f %f %f %f %f", vertex->vx, vertex->vy, vertex->vz, d,e,f);
					else	
						fprintf(poseFile, "%f %f %f %f %f %f\n", vertex->vx, vertex->vy, vertex->vz, d,e,f);
					
				
					iter1++;
		 
					}
				else {
					if(fgets (buf , MAXLINE , pFile)!=NULL)
						fputs (buf, poseFile);
				}
		 
				}
			}
		 
		 fclose(pFile);
		 fclose (poseFile);
		
		
		
	}


