/*
 *  Ply.h
 *  
 *
 *  Created by valentina armenise on 3/9/11.
 *  Copyright 2011 Politecnico di Torino. All rights reserved.
 *
 */


#define MAXLINE 30 


class Ply {
	public:
	int n_faces;
	int n_vertex;
	
	
	std::list <Vertex> vertexes;
	std::list <Face> faces;
	
	int getNumVertex(std::string inputFilename);
	int getNumFaces(std::string inputFilename);
	std::list <Vertex> getListVertex(std::string inputFilename, int n_vertex);
	Ply* translate_model(Ply* ply,Point* point);
	std::list<Face> getListFaces(std::string inputFilename, int n_vertex, int n_faces);
	//Vertex getElement(int el, Ply ply);
	
};





