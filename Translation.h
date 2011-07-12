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
			
			float getPointDistance(const std::vector<Point> &pointcl, int nt, Point origin, int size, int max);
			static Translation* New(Ply* translated_model, Point*point);
			void printPose(std::string inputFilename, int n_vertex, std::string output);
			
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
		//u can replace translated_model with this
		
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
		//while(!feof(pFile)){
		//	fscanf(pFile,"%f %f %f",&x,&y,&z);
		
		//	point.px=x;
		//	point.py=y;
		//	point.pz=z;
		int filter=-1;
		//std::cout<<"size"<<size<<std::endl;
		while(count<size){
			filter++;
			if(filter==10)
				filter=-1;
			count++;
			if(filter==0){
				//std::cout<<pointcl[count].px<<" "<<pointcl[count].py<<" "<<pointcl[count].pz<<std::endl;
				const Point &point=pointcl[count];
				//std::cout<<x<<" "<<y<<" "<<z<<std::endl;
				
				double mindist=0;
				std::list<Face>::iterator iter = this->faces.begin();
				it=0;
				double tmp;
				//std::cout<<"HERE YOU ARE"<<count<<std::endl;
				while(iter!= this->faces.end()){//for each face
					entered=0;
					entered1=false;
					entered2=false;
					entered3=false;
					entered4=false;
					const Face &face=*iter;
					const Piano &piano=face.piano;
					a=piano.parameters[0];
					b=piano.parameters[1];
					c=piano.parameters[2];
					d=piano.parameters[3];
					
					dist=point.computeDistance(a,b,c,d);
					Vertex vtx=piano.point[0];
					
					double p[3] = {point.px, point.py, point.pz};
					double origin[3] = {vtx.vx, vtx.vy, vtx.vz};
					double mod=sqrt(a*a+b*b+c*c);
					double normal[3] = {a/mod, b/mod, c/mod};
					double projected[3];
					double projmatrix[4][4];
					
					
					double xo[3],t;
					
					//projecting the point p
					xo[0]=p[0]-origin[0];
					xo[1]=p[1]-origin[1];
					xo[2]=p[2]-origin[2];
					
					t=normal[0]*xo[0]+normal[1]*xo[1]+normal[2]*xo[2];
					
					projected[0]=p[0]-t*normal[0];
					projected[1]=p[1]-t*normal[1];
					projected[2]=p[2]-t*normal[2];
					
					if(face.n_vertexes==4){
						
						//std::cout<<"rectangular face"<<std::endl;
						
						xa=piano.point[0].vx;
						xb=piano.point[1].vx;
						ya=piano.point[0].vy;
						yb=piano.point[1].vy;
						za=piano.point[0].vz;
						zb=piano.point[1].vz;
						
						
						
						if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
							
							dist+=0;
							
						}
						
						else {
							
							entered++;
							entered1=true;
							
						}
						
						
						xa=piano.point[1].vx;
						xb=piano.point[2].vx;
						ya=piano.point[1].vy;
						yb=piano.point[2].vy;
						za=piano.point[1].vz;
						zb=piano.point[2].vz;
						if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<0)||(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<0){
							
							entered++;
							entered2=true;
						}
						
						
						
						xa=piano.point[2].vx;
						xb=piano.point[3].vx;
						ya=piano.point[2].vy;
						yb=piano.point[3].vy;
						za=piano.point[2].vz;
						zb=piano.point[3].vz;
						
						if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<0)||(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<0){
							
							entered++;
							entered3=true;
							
						}
						
						
						xa=piano.point[3].vx;
						xb=piano.point[0].vx;
						ya=piano.point[3].vy;
						yb=piano.point[0].vy;
						za=piano.point[3].vz;
						zb=piano.point[0].vz;
						if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)<0)||(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)<0){
							
							entered++;
							entered4=true;
							
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
						
						
						
						if(entered==2){
							if(entered1==false){
								d[0]=Point::computeDistancePointLine(pr, piano.point[0], piano.point[1]);//sto ritornando il quadrato per farl auguale ma toglilo
								
							}
							else if(entered2==false){
								d[0]=Point::computeDistancePointLine(pr, piano.point[1], piano.point[2]);
								
							}
							
							
							
							
							if(entered3==false){
								
								d[1]=Point::computeDistancePointLine(pr, piano.point[2], piano.point[3]);
								
							}
							
							
							else if(entered4==false){
								d[1]=Point::computeDistancePointLine(pr, piano.point[3], piano.point[0]);
							}
							
							minim=d[0];
							for(int i=1;i<2;i++){
								if(d[i]<=minim)
									minim=d[i];
							}
							
						}
						if(entered==4){
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
							minim=0;
						}
						
					
						
					}
					if(face.n_vertexes==3){
						//std::cout<<"triangular face"<<std::endl;
						
						double d[6];
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
						
						
						
						/*int zab,zbc,zca;
						
						zab=(projected[0]-va[0])*(vb[1]-va[1])-(projected[1]-va[1])*(vb[0]-va[0]);
						zbc=(projected[0]-vb[0])*(vc[1]-vb[1])-(projected[1]-vb[1])*(vc[0]-vb[0]);
						zca=(projected[0]-vc[0])*(va[1]-vc[1])-(projected[1]-vc[1])*(va[0]-vc[0]);
						
						if((zab>0&&zbc>0&&zca>0)||(zab<0&&zbc<0&&zca<0)){
							minim=0;
							//std::cout<<"here"<<std::endl;
						}*/
						
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
							xa=piano.point[0].vx;
							xb=piano.point[1].vx;
							ya=piano.point[0].vy;
							yb=piano.point[1].vy;
							za=piano.point[0].vz;
							zb=piano.point[1].vz;
							
							if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
								d[3]=Point::computeDistancePointLine(pr, piano.point[0], piano.point[1]);
								
							}
							else d[3]=d[2];
							xa=piano.point[1].vx;
							xb=piano.point[2].vx;
							ya=piano.point[1].vy;
							yb=piano.point[2].vy;
							za=piano.point[1].vz;
							zb=piano.point[2].vz;
							if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
								d[4]=Point::computeDistancePointLine(pr, piano.point[1], piano.point[2]);
								
							}
							else d[4]=d[2];
							
							xa=piano.point[2].vx;
							xb=piano.point[0].vx;
							ya=piano.point[2].vy;
							yb=piano.point[0].vy;
							za=piano.point[2].vz;
							zb=piano.point[0].vz;
							if(((projected[0]-xa)*(xb-xa)+(projected[1]-ya)*(yb-ya)+(projected[2]-za)*(zb-za)>=0)&&(projected[0]-xb)*(xa-xb)+(projected[1]-yb)*(ya-yb)+(projected[2]-zb)*(za-zb)>=0){
								d[5]=Point::computeDistancePointLine(pr, piano.point[2], piano.point[0]);
								
							}
							else d[5]=d[2];
							//d[3]=Point::computeDistancePointLine(pr, piano.point[0], piano.point[1]);
							//d[4]=Point::computeDistancePointLine(pr, piano.point[1], piano.point[2]);
							//d[5]=Point::computeDistancePointLine(pr, piano.point[2], piano.point[0]);
							//if((pointcl[count].px>2.3&&pointcl[count].px<2.4)&&(pointcl[count].py>254.6&&pointcl[count].py<254.7)&&(pointcl[count].pz>670.8&&pointcl[count].pz<670.9)){
							//	std::cout<<"\n DDD "<<sqrt(d[0])<<" "<<sqrt(d[1])<<" "<<sqrt(d[2])<<" "<<sqrt(d[3])<<" "<<sqrt(d[4])<<" "<<sqrt(d[5])<<std::endl;
							//	std::cout<<piano.point[3].vx<<" "<<piano.point[0].vx<<std::endl;
							//}
							//d[7]=Point::computeDistancePointLine(pr, piano.point[3], piano.point[0]);
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
					//if(point.px>435 &&point.py>327 && point.pz>=1131){
					//	std::cout<<"maior than "<<std::endl;
					//
					//	std::cout<<mindist<<std::endl;
					//std::cout<<point.px<<" "<<point.py<<" "<<point.pz<<std::endl;
					//std::cout<<"pro"<<pro<<std::endl;
					//std::cout<<"lat"<<late<<std::endl;
					
					mindist=mindist*4;
					//mindist=mindist*8;
				}
				
					//std::cout<<"AAAAAAAAAAAAAAAAA"<<std::endl;
				
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
				
				
			}}
		
		//std::cout<<"total "<<total<<std::endl;
		
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


		