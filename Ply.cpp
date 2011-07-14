/*
 *  Ply.cpp
 *
 *
 *  Created by valentina armenise on 3/11/11.
 *  Copyright 2011 Politecnico di Torino. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <math.h>
#include "Elements.h"
#include "Ply.h"

#define PI 3.14159265

Ply* Ply::New(Ply*plymodel){
	Ply* newmodel=new Ply();
	newmodel->n_faces=plymodel->n_faces;
	newmodel->n_vertex=plymodel->n_vertex;
	newmodel->faces=plymodel->faces;

	return newmodel;
}

int	Ply::getNumVertex(std::string inputFilename){

	std::list <Vertex> vertexes;
	Vertex *vertex= new Vertex();

	bool end_header=false;
	char buf[200],el_vert[20],el_faces[20],s1[20],s2[20];
	//std::string buf;
	float x,y,z;
	int n_vertex;

	FILE *pFile = fopen (inputFilename.c_str() ,"r");

	while(!feof(pFile)) {
		//model_vertex=new float *[6];
		if (end_header==false){
			//fgets ((char*)buf.c_str() , MAXLINE , pFile);
			fgets (buf , MAXLINE , pFile);
			if (strstr(buf, "element vertex")!=NULL){
				sscanf(buf, "%s %s %d",s1,s2,&n_vertex);
				//std::cout<<buf<<" "<<n_vertex<<std::endl;

			}

			if (strstr(buf,"end_header")!=NULL){
				end_header=true;
				//std::cout<<"fuck"<<std::endl;

			}
		}
		else {

			fscanf(pFile,"%f %f %f",&x,&y,&z);
			vertex->vx=x;
			vertex->vy=y;
			vertex->vz=z;
			vertexes.push_back (*vertex);
			//std::cout<<x<<" "<<y<<" "<<z<<"\n"<<std::endl;

		}
	}
	fclose(pFile);
	return n_vertex;

}

int	Ply::getNumFaces(std::string inputFilename){

	std::list <Vertex> vertexes;
	Vertex *vertex= new Vertex();
	bool end_header=false;
	char buf[200],el_vert[20],el_faces[20],s1[20],s2[20];
	//std::string buf;
	float x,y,z;
	int n_faces;

	FILE *pFile = fopen (inputFilename.c_str() ,"r");

	while(!feof(pFile)) {
		//model_vertex=new float *[6];
		if (end_header==false){
			//fgets ((char*)buf.c_str() , MAXLINE , pFile);
			fgets (buf , MAXLINE , pFile);
			if (strstr(buf, "element face")!=NULL){
				sscanf(buf, "%s %s %d",s1,s2,&n_faces);
				//std::cout<<buf<<" "<<n_vertex<<std::endl;

			}

			if (strstr(buf,"end_header")!=NULL){
				end_header=true;

			}
		}
		else {

			fscanf(pFile,"%f %f %f",&x,&y,&z);
			vertex->vx=x;
			vertex->vy=y;
			vertex->vz=z;
			vertexes.push_back (*vertex);
			//std::cout<<x<<" "<<y<<" "<<z<<"\n"<<std::endl;

		}
	}
	fclose(pFile);
	return n_faces;

}

std::list <Vertex> Ply::getListVertex(std::string inputFilename, int n_vertex){

	std::list <Vertex> vertexes;
	Vertex *vertex= new Vertex();
	bool end_header=false;
	char buf[200],el_vert[20],el_faces[20],s1[20],s2[20];
	//std::string buf;
	float x,y,z;
	int line=0;

	FILE *pFile = fopen (inputFilename.c_str() ,"r");

	while(!feof(pFile)) {
		//model_vertex=new float *[6];
		if (end_header==false){
			//fgets ((char*)buf.c_str() , MAXLINE , pFile);
			fgets (buf , MAXLINE , pFile);

			if (strstr(buf,"end_header")!=NULL){
				end_header=true;
			}
		}
		else {
			if (line<n_vertex){
				line++;
				fscanf(pFile,"%f %f %f",&x,&y,&z);
				vertex->vx=x*1000;
				vertex->vy=y*1000;
				vertex->vz=z*1000;
				vertexes.push_back (*vertex);
			}
			fscanf(pFile,"%f %f %f",&x,&y,&z);
		}
	}

	fclose(pFile);
	return vertexes;

}

Ply* Ply::translate_model(Ply* plymodel,Point *point,Ply *translated_model){

	//the reference frame of the object is centered in the center of the object

	std::list<Vertex>::iterator iter=plymodel->vertexes.begin();
	Vertex *vertex= new Vertex();

	std::list<Vertex> translated_list;
	float tx,ty,tz;

	while(iter!=plymodel->vertexes.end()){

		*vertex=*iter;

		tx=vertex->vx+point->px;
		ty=vertex->vy+point->py;
		tz=vertex->vz+point->pz;
		//std::cout<<"vertex"<<vertex->vx<< " " << vertex->vy<< " " << vertex->vz<<"\n"<<std::endl;
		//std::cout<<"point"<<point->px<< " " << point->py<< " " << point->pz<<"\n"<<std::endl;
		//std::cout<<"transl"<<tx<< " " << ty<< " " << tz<<"\n"<<std::endl;
		Vertex *translated_vertex=new Vertex();
		translated_vertex->vx=tx;
		translated_vertex->vy=ty;
		translated_vertex->vz=tz;
		translated_list.push_back(*translated_vertex);
		iter++;

	}

	translated_model->vertexes=translated_list;

	return translated_model;

}

Ply* Ply::rotate_model(Ply* plymodel,Point *point,Ply *rotated_model, int tetax, int tetay, int tetaz){

	//the reference frame of the object is centered in the center of the object

	std::list<Vertex>::iterator iter=plymodel->vertexes.begin();
	Vertex *vertex= new Vertex();
	double radiantx,radianty,radiantz;
	std::list<Vertex> rotated_list;
	float rx,ry,rz;

	radiantx=tetax*PI/180;
	radianty=tetay*PI/180;
	radiantz=tetaz*PI/180;

	while(iter!=plymodel->vertexes.end()){

		*vertex=*iter;

		Vertex *rotated_vertex=new Vertex();
		//rotation around y
		rotated_vertex->vx=vertex->vx*cos(radianty)+vertex->vz*sin(radianty);
		rotated_vertex->vz=vertex->vz*cos(radianty)-vertex->vx*sin(radianty);
		rotated_vertex->vy=vertex->vy;
		rx=rotated_vertex->vx;
		ry=rotated_vertex->vy;
		rz=rotated_vertex->vz;
		//rotation around x
		rotated_vertex->vx=rx;
		rotated_vertex->vy=ry*cos(radiantx)-rz*sin(radiantx);
		rotated_vertex->vz=ry*sin(radiantx)+rz*cos(radiantx);
		rx=rotated_vertex->vx;
		ry=rotated_vertex->vy;
		rz=rotated_vertex->vz;
		//rotation around z
		rotated_vertex->vx=rx*cos(radiantz)-ry*sin(radiantz);
		rotated_vertex->vy=rx*sin(radiantz)+ry*cos(radiantz);
		rotated_vertex->vz=rz;
		rx=rotated_vertex->vx;
		ry=rotated_vertex->vy;
		rz=rotated_vertex->vz;
		//translation
		rotated_vertex->vx=rotated_vertex->vx+point->px;
		rotated_vertex->vz=rotated_vertex->vz+point->pz;
		rotated_vertex->vy=rotated_vertex->vy+point->py;;
		/*rotated_vertex->vx=rotated_vertex->vx-tx;
				rotated_vertex->vz=rotated_vertex->vz-tz;
				rotated_vertex->vy=rotated_vertex->vy-ty;*/
		//std::cout<<" rotation "<<rotated_vertex->vx<<" "<<rotated_vertex->vy<<" "<<rotated_vertex->vz<<std::endl;
		rotated_list.push_back(*rotated_vertex);
		iter++;

	}

	rotated_model->vertexes=rotated_list;

	return rotated_model;

}


std::list <Face> Ply::getListFaces(std::string inputFilename, int n_vertex, int n_faces){

	std::list <Face> faces;

	int index[4];
	bool end_header=false;
	int num,n;
	char buf[200],buf2[200];
	//std::string buf;
	float x,y,z,nn;
	int line=0;
	int line2=0;

	FILE *pFile = fopen (inputFilename.c_str() ,"r");

	while(!feof(pFile)) {
		//model_vertex=new float *[6];
		if (end_header==false){
			//fgets ((char*)buf.c_str() , MAXLINE , pFile);
			fgets (buf , MAXLINE , pFile);

			if (strstr(buf,"end_header")!=NULL){
				end_header=true;
			}
		}
		else {
			if (line<n_vertex){
				line++;
				fscanf(pFile,"%f %f %f %f %f %f",&x,&y,&z,&nn,&nn,&nn);
				fgets(buf,MAXLINE,pFile);
			}

			else {

				if (line2<n_faces){
					char buf3[100];
					line2++;
					fgets (buf3 , 200 , pFile);
					sscanf(buf3 ,"%d %d %d %d %d",&n,&index[0],&index[1],&index[2],&index[3]);
					std::cout<<n<<" "<<index[0]<<" "<<index[1]<<" "<<index[2]<<" "<<index[3]<<std::endl;

					Face *face= new Face();
					face->n_vertexes=n;
					for (int i=0;i<n;i++){
						face->face_vertexes[i]=index[i];
						//std::cout<<"ciao"<<index[i]<<std::endl;
					}
					//std::cout<<"\n"<<std::endl;
					faces.push_back(*face);
				}
				else fgets (buf , MAXLINE , pFile);
			}
		}
	}

	fclose(pFile);

	return faces;

}






