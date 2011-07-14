/*
 *  Translation.h
 *
 *
 *  Created by valentina armenise on 3/15/11.
 *  Copyright 2011 Politecnico di Torino. All rights reserved.
 *
 */


namespace  TranslLib{
class Translation: public Ply {
public:
	Ply * translated_model;
	Point *origin;
	double object_value;
	int Id;
	bool flag;
	int orientation[3];
	int errors;

	float getPointDistance(const std::vector<Point> &pointcl, int nt, Point origin, int size, int max);//return the value of the objective function for a given pose
	static Translation* New(Ply* translated_model, Point*point);//translate the ply model
	void printPose(std::string inputFilename, int n_vertex, std::string output);//print the pose in a file

};
}
TranslLib::Translation* TranslLib::Translation::New(Ply*plymodel, Point *origin){
	Translation* newtrans=new Translation();
	newtrans->vertexes=plymodel->vertexes;
	newtrans->faces=plymodel->faces;
	newtrans->n_faces=plymodel->n_faces;
	newtrans->n_vertex=plymodel->n_vertex;
	newtrans->origin=origin;

	return newtrans;

}

float TranslLib::Translation::getPointDistance(const std::vector<Point> &pointcl,int nt, Point origin, int size, int max){

	Point point;
	float a,b,c,d;
	int it=0,nface,npoint=0;
	float min,dist=0,x,y,z;

	this->object_value=0;
	this->flag=0;
	this->errors=0;

	double total=0;

	double xa,xb,ya,yb,za,zb;
	double da, db;
	int entered=0;
	bool entered1, entered2,entered3,entered4;
	double minim;
	//FILE* pFile=fopen("inputCleanedframe2.xyz","r");
	int count=0;
	double pro;
	double late;
	double ent;
	int bad_detection=0;
	int filter=-1;//the filter is used to consider 1 point out of 10
	//std::cout<<"size"<<size<<std::endl;
	while(count<size){//for all the point cloud
		filter++;
		if(filter==10)
			filter=-1;
		count++;
		if(filter==0){
			//std::cout<<pointcl[count].px<<" "<<pointcl[count].py<<" "<<pointcl[count].pz<<std::endl;
			const Point &point=pointcl[count];
			double mindist=0;
			std::list<Face>::iterator iter = this->faces.begin();
			it=0;
			double tmp;
			while(iter!= this->faces.end()){//computing the distance point-face for each face
				entered=0;
				entered1=false;
				entered2=false;
				entered3=false;
				entered4=false;
				const Face &face=*iter;
				const Plane &plane=face.plane;
				a=plane.parameters[0];
				b=plane.parameters[1];
				c=plane.parameters[2];
				d=plane.parameters[3];

				dist=point.computeDistance(a,b,c,d);//distance between the point and the plane the face belongs to
				Vertex vtx=plane.point[0];

				double p[3] = {point.px, point.py, point.pz};
				double origin[3] = {vtx.vx, vtx.vy, vtx.vz};
				double mod=sqrt(a*a+b*b+c*c);
				double normal[3] = {a/mod, b/mod, c/mod};
				double projected[3];
				double projmatrix[4][4];

				double xo[3],t;

				//projecting the point p on the plane the face belongs to
				xo[0]=p[0]-origin[0];
				xo[1]=p[1]-origin[1];
				xo[2]=p[2]-origin[2];

				t=normal[0]*xo[0]+normal[1]*xo[1]+normal[2]*xo[2];

				projected[0]=p[0]-t*normal[0];
				projected[1]=p[1]-t*normal[1];
				projected[2]=p[2]-t*normal[2];
				

				if(face.n_vertexes==4){
					
					// if the face is rectangular: 
					//if the projected point is in the rectangle then the distance point-face is the distance point-plane.
					//if the projected point is outside the rectangle it can be in 8 different regions
					 
					 
				/*	
				 
				1		   2          3
					 _____________
					|			  |
				8	|             |   4
					|             |
					|_____________|
					             
				7	       6          5 
					 
					 
				*/
					 
					 
					 

					//std::cout<<"rectangular face"<<std::endl;

					xa=plane.point[0].vx;
					xb=plane.point[1].vx;
					ya=plane.point[0].vy;
					yb=plane.point[1].vy;
					za=plane.point[0].vz;
					zb=plane.point[1].vz;

					//if the projection of the projected point on the edge determined by the vertexes 0 and 1 of the plane
					//does not fall within the edge then the counter "entered" is incremented
					if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){

						dist+=0;

					}

					else {

						entered++;
						entered1=true;

					}

					xa=plane.point[1].vx;
					xb=plane.point[2].vx;
					ya=plane.point[1].vy;
					yb=plane.point[2].vy;
					za=plane.point[1].vz;
					zb=plane.point[2].vz;
					//if the projection of the projected point on the edge determined by the vertexes 1 and 2 of the plane
					//does not fall within the edge then the counter "entered" is incremented
					if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<0)||(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<0){

						entered++;
						entered2=true;

					}

					xa=plane.point[2].vx;
					xb=plane.point[3].vx;
					ya=plane.point[2].vy;
					yb=plane.point[3].vy;
					za=plane.point[2].vz;
					zb=plane.point[3].vz;
					//if the projection of the projected point on the edge determined by the vertexes 2 and 3 of the plane
					//does not fall within the edge then the counter "entered" is incremented
					if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<0)||(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<0){

						entered++;
						entered3=true;

					}

					xa=plane.point[3].vx;
					xb=plane.point[0].vx;
					ya=plane.point[3].vy;
					yb=plane.point[0].vy;
					za=plane.point[3].vz;
					zb=plane.point[0].vz;
					//if the projection of the projected point on the edge determined by the vertexes 3 and 0 of the plane 
					//does not fall within the edge then the counter "entered" is incremented
					if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<0)||(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<0){

						entered++;
						entered4=true;

					}

					double d[3];
					double va[3],vb[3],vc[3],vd[3];

					va[0]=plane.point[0].vx;
					va[1]=plane.point[0].vy;
					va[2]=plane.point[0].vz;

					vb[0]=plane.point[1].vx;
					vb[1]=plane.point[1].vy;
					vb[2]=plane.point[1].vz;

					vc[0]=plane.point[2].vx;
					vc[1]=plane.point[2].vy;
					vc[2]=plane.point[2].vz;

					vd[0]=plane.point[3].vx;
					vd[1]=plane.point[3].vy;
					vd[2]=plane.point[3].vz;

					Point pr;
					pr.px=projected[0];
					pr.py=projected[1];
					pr.pz=projected[2];

					if(entered==2){//if entered equals 2, it means the projected point falls in regions 2,4,6,8

						if(entered1==false){

							d[0]=Point::computeDistancePointLine(pr, plane.point[0], plane.point[1]);

						}
						else if(entered2==false){

							d[0]=Point::computeDistancePointLine(pr, plane.point[1], plane.point[2]);

						}

						if(entered3==false){

							d[1]=Point::computeDistancePointLine(pr, plane.point[2], plane.point[3]);

						}

						else if(entered4==false){

							d[1]=Point::computeDistancePointLine(pr, plane.point[3], plane.point[0]);

						}

						minim=d[0];
						for(int i=1;i<2;i++){
							if(d[i]<=minim)
								minim=d[i];
						}
					}

					if(entered==4){//if enterd equals 4, it means the projected point falls in regions  1,3,5,7
						d[0]=(projected[0]-va[0])*(projected[0]-va[0])+(projected[1]-va[1])*(projected[1]-va[1])+(projected[2]-va[2])*(projected[2]-va[2]);
						d[1]=(projected[0]-vb[0])*(projected[0]-vb[0])+(projected[1]-vb[1])*(projected[1]-vb[1])+(projected[2]-vb[2])*(projected[2]-vb[2]);
						d[2]=(projected[0]-vc[0])*(projected[0]-vc[0])+(projected[1]-vc[1])*(projected[1]-vc[1])+(projected[2]-vc[2])*(projected[2]-vc[2]);
						d[3]=(projected[0]-vd[0])*(projected[0]-vd[0])+(projected[1]-vd[1])*(projected[1]-vd[1])+(projected[2]-vd[2])*(projected[2]-vd[2]);
						minim=d[0];
						for(int i=1;i<4;i++){

							if(d[i]<=minim){
								minim=d[i];

							}
						}
					}
					if(entered==0){ 
						//if entered equals 0, then the projected point is in region 0 and the distance point-face coincides with the distance point-plane
						minim=0;
					}
				}
				if(face.n_vertexes==3){
					//if the face is triangular: 
					//if the projected point falls inside the triangle then the distance is the distance point-plane.
					//If the porjection of the point falls outside the triangle then the distance between the point and each vertex and each segment-line is computed.
					//The minimum among these distances is considered.
					//std::cout<<"triangular face"<<std::endl;

					double d[6];
					double va[3],vb[3],vc[3],vd[3];

					va[0]=plane.point[0].vx;
					va[1]=plane.point[0].vy;
					va[2]=plane.point[0].vz;

					vb[0]=plane.point[1].vx;
					vb[1]=plane.point[1].vy;
					vb[2]=plane.point[1].vz;

					vc[0]=plane.point[2].vx;
					vc[1]=plane.point[2].vy;
					vc[2]=plane.point[2].vz;

					int a[3],a2[3],b[3],flag=0;
					double cr1[3],cr2[3];
					a[0]=vb[0]-vc[0];
					a[1]=vb[1]-vc[1];
					a[2]=vb[2]-vc[2];

					b[0]=projected[0]-vb[0];
					b[1]=projected[1]-vb[1];
					b[2]=projected[2]-vb[2];
					cr1[0]=a[1]*b[2]-a[2]*b[1];
					cr1[1]=a[2]*b[0]-a[0]*b[2];
					cr1[2]=a[0]*b[1]-a[1]*b[0];

					a2[0]=a[0];
					a2[1]=a[1];
					a2[2]=a[2];

					b[0]=va[0]-vb[0];
					b[1]=va[1]-vb[1];
					b[2]=va[2]-vb[2];

					cr2[0]=a[1]*b[2]-a[2]*b[1];
					cr2[1]=a[2]*b[0]-a[0]*b[2];
					cr2[2]=a[0]*b[1]-a[1]*b[0];

					if((cr1[0]*cr2[0]+cr1[1]*cr2[1]+cr1[2]*cr2[2])>=0)
						flag++;

					a[0]=vc[0]-va[0];
					a[1]=vc[1]-va[1];
					a[2]=vc[2]-va[2];

					b[0]=projected[0]-va[0];
					b[1]=projected[1]-va[1];
					b[2]=projected[2]-va[2];
					cr1[0]=a[1]*b[2]-a[2]*b[1];
					cr1[1]=a[2]*b[0]-a[0]*b[2];
					cr1[2]=a[0]*b[1]-a[1]*b[0];

					a2[0]=a[0];
					a2[1]=a[1];
					a2[2]=a[2];

					b[0]=vb[0]-va[0];
					b[1]=vb[1]-va[1];
					b[2]=vb[2]-va[2];

					cr2[0]=a[1]*b[2]-a[2]*b[1];
					cr2[1]=a[2]*b[0]-a[0]*b[2];
					cr2[2]=a[0]*b[1]-a[1]*b[0];

					if((cr1[0]*cr2[0]+cr1[1]*cr2[1]+cr1[2]*cr2[2])>=0)
						flag++;


					a[0]=vb[0]-va[0];
					a[1]=vb[1]-va[1];
					a[2]=vb[2]-va[2];

					b[0]=projected[0]-va[0];
					b[1]=projected[1]-va[1];
					b[2]=projected[2]-va[2];
					cr1[0]=a[1]*b[2]-a[2]*b[1];
					cr1[1]=a[2]*b[0]-a[0]*b[2];
					cr1[2]=a[0]*b[1]-a[1]*b[0];

					a2[0]=a[0];
					a2[1]=a[1];
					a2[2]=a[2];

					b[0]=vc[0]-va[0];
					b[1]=vc[1]-va[1];
					b[2]=vc[2]-va[2];

					cr2[0]=a[1]*b[2]-a[2]*b[1];
					cr2[1]=a[2]*b[0]-a[0]*b[2];
					cr2[2]=a[0]*b[1]-a[1]*b[0];

					if((cr1[0]*cr2[0]+cr1[1]*cr2[1]+cr1[2]*cr2[2])>=0){
						flag++;
					}

					if(flag==3){
						minim=0;
					}

					else{
						//computing distances point-vertexes

						d[0]=(projected[0]-va[0])*(projected[0]-va[0])+(projected[1]-va[1])*(projected[1]-va[1])+(projected[2]-va[2])*(projected[2]-va[2]);
						d[1]=(projected[0]-vb[0])*(projected[0]-vb[0])+(projected[1]-vb[1])*(projected[1]-vb[1])+(projected[2]-vb[2])*(projected[2]-vb[2]);
						d[2]=(projected[0]-vc[0])*(projected[0]-vc[0])+(projected[1]-vc[1])*(projected[1]-vc[1])+(projected[2]-vc[2])*(projected[2]-vc[2]);
						//d[3]=(projected[0]-vd[0])*(projected[0]-vd[0])+(projected[1]-vd[1])*(projected[1]-vd[1])+(projected[2]-vd[2])*(projected[2]-vd[2]);
						
						//computing distances point-line

						Point pr;
						pr.px=projected[0];
						pr.py=projected[1];
						pr.pz=projected[2];
						xa=plane.point[0].vx;
						xb=plane.point[1].vx;
						ya=plane.point[0].vy;
						yb=plane.point[1].vy;
						za=plane.point[0].vz;
						zb=plane.point[1].vz;

						if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){

							d[3]=Point::computeDistancePointLine(pr, plane.point[0], plane.point[1]);

						}
						else d[3]=d[2];
						xa=plane.point[1].vx;
						xb=plane.point[2].vx;
						ya=plane.point[1].vy;
						yb=plane.point[2].vy;
						za=plane.point[1].vz;
						zb=plane.point[2].vz;
						if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){

							d[4]=Point::computeDistancePointLine(pr, plane.point[1], plane.point[2]);

						}
						else d[4]=d[2];

						xa=plane.point[2].vx;
						xb=plane.point[0].vx;
						ya=plane.point[2].vy;
						yb=plane.point[0].vy;
						za=plane.point[2].vz;
						zb=plane.point[0].vz;
						if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){

							d[5]=Point::computeDistancePointLine(pr, plane.point[2], plane.point[0]);

						}
						else d[5]=d[2];
						//d[3]=Point::computeDistancePointLine(pr, plane.point[0], plane.point[1]);
						//d[4]=Point::computeDistancePointLine(pr, plane.point[1], plane.point[2]);
						//d[5]=Point::computeDistancePointLine(pr, plane.point[2], plane.point[0]);
						//if((pointcl[count].px>2.3&&pointcl[count].px<2.4)&&(pointcl[count].py>254.6&&pointcl[count].py<254.7)&&(pointcl[count].pz>670.8&&pointcl[count].pz<670.9)){
						//	std::cout<<"\n DDD "<<sqrt(d[0])<<" "<<sqrt(d[1])<<" "<<sqrt(d[2])<<" "<<sqrt(d[3])<<" "<<sqrt(d[4])<<" "<<sqrt(d[5])<<std::endl;
						//	std::cout<<plane.point[3].vx<<" "<<plane.point[0].vx<<std::endl;
						//}
						//d[7]=Point::computeDistancePointLine(pr, plane.point[3], plane.point[0]);
						minim=d[0];
						for (int i=1;i<6;i++){
							if(d[i]<=minim){
								minim=d[i];
							}
						}
					}
				}

				//printf("\npro%f", dist);

				double proj=dist;
				double lat=sqrt(minim);
				double entr=entered;
				dist=sqrt(dist*dist+sqrt(minim)*sqrt(minim));
				//	if((pointcl[count].px>2.3&&pointcl[count].px<2.4)&&(pointcl[count].py>254.6&&pointcl[count].py<254.7)&&(pointcl[count].pz>670.8&&pointcl[count].pz<670.9)){
				//		std::cout<<pointcl[count].px<<" "<<pointcl[count].py<<" "<<pointcl[count].pz<<std::endl;
				//		printf("dist%f", dist);
				//		printf("pro%f", proj);
				//	}
				//printf("dist%f", dist);

				/*if(point.px>435 &&point.py>327 && point.pz>=1131){
					 std::cout<<point.px<<" "<<point.py<<" "<<point.pz<<std::endl;
					 std::cout<<"projection  "<<proj<<" on face"<<it<<std::endl;
					 std::cout<<"entered" <<entr<<std::endl;
					 }*/

				if (it==0){
					mindist=dist;
					nface=it;
					pro=proj;
					late=lat;
					ent=entr;
				}

				else if (dist<mindist){
					mindist=dist;
					nface=it;
					pro=proj;
					late=lat;
					ent=entr;
				}

				iter++;
				it++;

			}

			//std::cout<<mindist<<std::endl;

			total+=mindist;

			if(mindist>max){
				bad_detection++;
			}

			if(bad_detection>500){
				this->object_value=0;
				count=size;
			}

			if(mindist>27){
				this->errors=this->errors+1;
			}

			if(mindist>30){

				mindist=mindist*4;
				//mindist=mindist*8;
			}
			//	mindist=mindist*100;
			/*if (mindist<1){

				 mindist=0;//by 3??
				 }*/

			//double m=0.5*mindist;

			if(mindist<=200){
				this->object_value+=(1-mindist/200);
			}
			else {
				this->object_value+=0;
			}

			this->flag=1;

			npoint++;
		}
	}

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

