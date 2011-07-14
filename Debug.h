/*
 *  Debug.h
 *
 *
 *  Created by valentina armenise on 3/11/11.
 *  Copyright 2011 Politecnico di Torino. All rights reserved.
 *
 */

class Debug{
public:

	void printFaces(std::list<Face> faces);
	void printVertexPlane(std::list<Face> faces);
	void printPlanes(std::list<Face> faces);

};

void Debug::printFaces(std::list<Face> faces){

	/*	int it=0;
	std::list<Face>::iterator iter =faces.begin();


	while(iter!=faces.end()){
		std::cout<<"#face "<<it<<std::endl;
		std::list<int>::iterator iter2=iter->face_vertexes.begin();
		while(iter2!=iter->face_vertexes.end()){
			std::cout<<*iter2<<std::endl;
			iter2++;
		}
		it++;
		iter++;
	}
	 */}


void Debug::printVertexPlane(std::list<Face> faces){
	int it=0;
	std::list<Face>::iterator iter =faces.begin();
	Plane *plane=new Plane();


	while(iter!=faces.end()){
		std::cout<<"#face "<<it<<std::endl;
		*plane=iter->plane;
		for(int i=0;i<=3;i++){
			std::cout<<plane->point[i].vx<<" "<<plane->point[i].vy<<" "<<plane->point[i].vz<<std::endl;
		}
		iter++;
		it++;

	}
}



void Debug::printPlanes(std::list<Face> faces){
	int it=0;
	std::list<Face>::iterator iter =faces.begin();
	Plane *plane=new Plane();


	while(iter!=faces.end()){
		std::cout<<"#plane "<<it<<std::endl;
		*plane=iter->plane;

		std::cout<<"a= "<<plane->parameters[0]<<std::endl;
		std::cout<<"b= "<<plane->parameters[1]<<std::endl;
		std::cout<<"c= "<<plane->parameters[2]<<std::endl;
		std::cout<<"d= "<<plane->parameters[3]<<std::endl;

		iter++;
		it++;

	}
}

