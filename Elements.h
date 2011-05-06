/*
 *  Test.h
 *  
 *
 *  Created by valentina armenise on 3/7/11.
 *  Copyright 2011 Politecnico di Torino. All rights reserved.
 *
 */

class Vertex{
	public:
	float vx;
	float vy;
	float vz;

};

class Point{
	public:
	float px;
	float py;
	float pz;
	
	float computeDistance( const float &a,const float &b,const float & c,const float & d) const;
	
	static float computeDistance2P(const Point &point,const Vertex &vertex);
	static float computeDistance2P(const Point &point,const Point &origin);
	static float computeDistance2P(const Vertex &point,const Vertex &vertex);
	static float computeDistancePointLine(const Point &point,const Vertex &v1,const Vertex &v2);
};

class Piano{
	public:
	float parameters[4];
	Vertex point[4];
	float A[4][4];
	float B[3];
	float m[4][4];
	
	float* getParameters(Piano*piano);
//	float correctDistance(Point point);
	float correctDistance(const Point &point,const Point &origin);
	
	
	
};

class Face {
	public:
	int n_vertexes;
	int face_vertexes[4];
	Piano piano;
	Piano* getPiano(const std::list<Vertex> &vertexes,const Face &face);

	
};


inline Piano* Face::getPiano(const std::list<Vertex> &vertexes,const Face &face){
	int it=0;
	int itVert=0;
	int offset;
	Piano *piano=new Piano();
	
	int iter2 = 0;
	
	while(it<=3){
		
		offset=face.face_vertexes[iter2];
		itVert=0;
		
		for(std::list<Vertex>::const_iterator iterVert=vertexes.begin();iterVert!=vertexes.end();++iterVert)
		{
			if (itVert==offset){
				piano->point[it]=*iterVert;
				//std::cout<<"ciao"<<indexToVertex->vx<<" "<<indexToVertex->vy<<std::endl;
				piano->A[it][0]=iterVert->vx;
				piano->A[it][1]=iterVert->vy;
				piano->A[it][2]=iterVert->vz;
				//std::cout<<piano->A[it][0]<<piano->A[it][1]<<piano->A[it][2]<<std::endl;
				
			}
			itVert++;
		}
		
		/**indexToVertex=*index;
		 face->piano->point[it]=indexToVertex;
		 
		 std::cout<<"face"<<it<<" "<<indexToVertex<<std::endl;*/
		iter2++;
		it++;
		
	}
	
	for(int i=0;i<3;i++){
		piano->B[i]=0;
	}
	
	
	piano->getParameters(piano);
	
	return piano;	
	
}



inline float * Piano::getParameters(Piano*piano){
	
	
	double x[3];
	double U[3];
	double V[3];
	double n[3];
	double v1[3];
	double v2[3];
	double v3[3];
	
	v1[0]=piano->A[0][0];
	v1[1]=piano->A[0][1];
	v1[2]=piano->A[0][2];
	
	v2[0]=piano->A[1][0];
	v2[1]=piano->A[1][1];
	v2[2]=piano->A[1][2];
	
	v3[0]=piano->A[2][0];
	v3[1]=piano->A[2][1];
	v3[2]=piano->A[2][2];
	
	/*v1[0]=3;
	v1[1]=0;
	v1[2]=4;
	
	v2[0]=2;
	v2[1]=2;
	v2[2]=-2;
	
	v3[0]=-1;
	v3[1]=2;
	v3[2]=3;*/
	
	U[0]=v2[0]-v1[0];
	U[1]=v2[1]-v1[1];
	U[2]=v2[2]-v1[2];
	
	V[0]=v3[0]-v1[0];
	V[1]=v3[1]-v1[1];
	V[2]=v3[2]-v1[2];
	
	//computing cross product UxV
	//x = Ay * Bz - By * Az
	//y = Az * Bx - Bz * Ax
	//z = Ax * By - Bx * Ay
	
	
	n[0]=U[1]*V[2]-V[1]*U[2];
	n[1]=U[2]*V[0]-V[2]*U[0];
	n[2]=U[0]*V[1]-V[0]*U[1];
	
	double a=n[0];
	double b=n[1];
	double c=n[2];
	double d=-v3[0]*n[0]-v3[1]*n[1]-v3[2]*n[2];
	
	//std::cout<<"a="<<a<<std::endl;
	//std::cout<<"b="<<b<<std::endl;
	//std::cout<<"c="<<c<<std::endl;
	//std::cout<<"d="<<d<<std::endl;
	
	piano->parameters[0]=a;
	piano->parameters[1]=b;
	piano->parameters[2]=c;
	piano->parameters[3]=d;
	

	
	
}



	
	inline float Point::computeDistance(const float &a, const float &b, const float &c, const float &d) const{
		float num, den;
		float dist;
		
		num=a*this->px+b*this->py+c*this->pz+d;
		num=fabs(num);
		den=sqrt(a*a+b*b+c*c);
		
		dist=num/den;
		
		return 3*dist;
	}
		

/*float Piano::correctDistance(Point point){
	float dist=0;
	float tmp;
			//old version	
	for(int i=0;i<=3;i++){
		tmp=Point::computeDistance2P(point,this->point[i]);
		if(i==0)
			dist=tmp;
		else if(tmp>dist)
			dist=tmp;
			
		}
	

		return dist;
	}*/

inline float Piano::correctDistance(const Point &point, const Point &origin){
	float dist=0;
	
	
	
	dist=Point::computeDistance2P(point, origin);	
	
	
	
	return dist;
}


inline float Point::computeDistance2P(const Point &point, const Vertex &vertex){
	float dist;
	
	/*float a=pow(point.px-vertex.vx,2);
	float b=pow(point.py-vertex.vy,2);
	float c=pow(point.pz-vertex.vz,2);*/
	
	float a=(point.px-vertex.vx)*(point.px-vertex.vx);
	float b=(point.py-vertex.vy)*(point.py-vertex.vy);
	float c=(point.pz-vertex.vz)*(point.pz-vertex.vz);
	dist=sqrt(a+b+c);
		//std::cout<<"what the hell"<<dist<<std::endl;
	return dist;
}

inline float Point::computeDistance2P(const Vertex &point,const Vertex &vertex){
	float dist;
	
	/*float a=pow(point.vx-vertex.vx,2);
	float b=pow(point.vy-vertex.vy,2);
	float c=pow(point.vz-vertex.vz,2);*/
	
	float a=(point.vx-vertex.vx)*(point.vx-vertex.vx);
	float b=(point.vy-vertex.vy)*(point.vy-vertex.vy);
	float c=(point.vz-vertex.vz)*(point.vz-vertex.vz);
	dist=sqrt(a+b+c);
	//std::cout<<"what the hell"<<dist<<std::endl;
	return dist;
}


		
			
inline float Point::computeDistance2P(const Point &point,const Point &origin){
	float dist;
	
	/*float a=pow(point.px-origin.px,2);
	float b=pow(point.py-origin.py,2);
	float c=pow(point.pz-origin.pz,2);*/
	
	float a=(point.px-origin.px)*(point.px-origin.px);
	float b=(point.py-origin.py)*(point.py-origin.py);
	float c=(point.pz-origin.pz)*(point.pz-origin.pz);
	dist=sqrt(a+b+c);
	//std::cout<<"what the hell"<<dist<<std::endl;
	return dist;
}				
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					
					



inline float Point::computeDistancePointLine(const Point &p0, const Vertex &p1,const Vertex &p2){
	
	double cr_p_x, cr_p_y,cr_p_z;
	double modcr;
	
//	p1p0=computeDistance2P(p0,p1);
//	p2p1=computeDistance2P(p2,p1);
	

	//dist=pow(p1p0,2)*pow(p2p1,2)-pow((p1p0*p2p1),2);
	//dist=dist/pow(p2p1,2);
	//dist=sqrt(dist);
	
	//|p1p0xp2p1|/|p2p1| 
	//p2p1_x=p2.vx-p1.vx;
	//p2p1_y=p2.vy-p1.vy;
	//p2p1_z=p2.vz-p1.vz;
	cr_p_x=(p2.vy-p1.vy)*(p1.vz-p0.pz)-(p2.vz-p1.vz)*(p1.vy-p0.py);
	cr_p_y=(p2.vz-p1.vz)*(p1.vx-p0.px)-(p2.vx-p1.vx)*(p1.vz-p0.pz);
	cr_p_z=(p2.vx-p1.vx)*(p1.vy-p0.py)-(p2.vy-p1.vy)*(p1.vx-p0.px);
	
	modcr=cr_p_x*cr_p_x+cr_p_y*cr_p_y+cr_p_z*cr_p_z;
	
	return modcr/((p2.vx-p1.vx)*(p2.vx-p1.vx)+(p2.vy-p1.vy)*(p2.vy-p1.vy)+(p2.vz-p1.vz)*(p2.vz-p1.vz));
}
			
