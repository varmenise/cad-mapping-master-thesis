/*
 *  Visualize.cpp
 *  
 *
 *  Created by valentina armenise on 4/7/11.
 *  Copyright 2011 Politecnico di Torino. All rights reserved.
 *
 */

#include "Visualize.h"


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

#include "MouseInteractorStyle.h"

int main ( int argc, char *argv[] )

{	int 	xMin=250;
int 	xMax=400;
int 	yMin=300;
int 	yMax=500;
int 	zMin=1000;
int 	zMax=1200;

if(argc != 2)
{
	std::cout << "Usage: " << argv[0] << "  Filename(.ply)" << std::endl;
	return EXIT_FAILURE;
}


// Read the file
vtkSmartPointer<vtkSimplePointsReader> reader =
		vtkSmartPointer<vtkSimplePointsReader>::New();
reader->SetFileName ( argv[1] );
//reader->SetFileName ( "inputCleaned.txt" );

reader->Update();

vtkSmartPointer<vtkOutlineSource> outline =
		vtkSmartPointer<vtkOutlineSource>::New();

outline->SetBounds(xMin, xMax, yMin,
		yMax,
		zMin,
		zMax );

// Visualize image
vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
mapper->SetInputConnection(reader->GetOutputPort());

// Visualize box
vtkSmartPointer<vtkPolyDataMapper> mapper1 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
mapper1->SetInputConnection(outline->GetOutputPort());

vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
actor->GetProperty()->SetColor(0,0,0);
actor->SetMapper(mapper);
actor->GetProperty()->SetPointSize(4);

vtkSmartPointer<vtkActor> actor1 =
		vtkSmartPointer<vtkActor>::New();
actor1->GetProperty()->SetColor(255,0,255);
actor1->SetMapper(mapper1);
actor1->GetProperty()->SetPointSize(4);

//visualize point
vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
//const float p[3] = {306.46,306.46,279.17};
const float p[3] = {365.895,419.341,945.256};


// Create the topology of the point (a vertex)
vtkSmartPointer<vtkCellArray> vertices =
		vtkSmartPointer<vtkCellArray>::New();
vtkIdType pid[1];
pid[0] = points->InsertNextPoint(p);
//vertices->InsertNextCell(1,pid);

// Create a polydata object
vtkSmartPointer<vtkPolyData> point1 =
		vtkSmartPointer<vtkPolyData>::New();

// Set the points and vertices we created as the geometry and topology of the polydata
point1->SetPoints(points);
point1->SetVerts(vertices);

vtkSmartPointer<vtkPolyDataMapper> mapper2 =
		vtkSmartPointer<vtkPolyDataMapper>::New();
mapper2->SetInput(point1);
vtkSmartPointer<vtkActor> actor2 =
		vtkSmartPointer<vtkActor>::New();
actor2->GetProperty()->SetColor(255,000,000);
actor2->SetMapper(mapper2);
actor2->GetProperty()->SetPointSize(4);

const float p1[3]={365.895,419.341,945.256};

// Create the topology of the point (a vertex)

pid[0] = points->InsertNextPoint(p1);

vertices->InsertNextCell(1,pid);

// Create a polydata object

// Set the points and vertices we created as the geometry and topology of the polydata
point1->SetPoints(points);
point1->SetVerts(vertices);


mapper2->SetInput(point1);
vtkSmartPointer<vtkActor> actor4 =
		vtkSmartPointer<vtkActor>::New();
actor4->GetProperty()->SetColor(000,100,000);
actor4->SetMapper(mapper2);
actor4->GetProperty()->SetPointSize(4);


//visualize iterations

/*iter2 = translations.begin();
	 while(iter2!=translations.end()){
	 if(iter2->origin->px!=pose->origin->px||iter2->origin->py!=pose->origin->py||iter2->origin->pz!=pose->origin->pz){
	 std::cout<<iter2->origin->px<< " " <<iter2->origin->py<< " "<<iter2->origin->pz<< " "<<std::endl;

	 vtkSmartPointer<vtkPoints> points =
	 vtkSmartPointer<vtkPoints>::New();

	 const float  p[3] = {iter2->origin->px,iter2->origin->py,iter2->origin->pz};

	 // Create the topology of the point (a vertex)
	 vertices =
	 vtkSmartPointer<vtkCellArray>::New();
	 pid[0] = points->InsertNextPoint(p);
	 vertices->InsertNextCell(1,pid);

	 // Create a polydata object
	 point1 =
	 vtkSmartPointer<vtkPolyData>::New();

	 // Set the points and vertices we created as the geometry and topology of the polydata
	 point1->SetPoints(points);
	 point1->SetVerts(vertices);

	 vtkSmartPointer<vtkPolyDataMapper> mapper3 =
	 vtkSmartPointer<vtkPolyDataMapper>::New();

	 mapper3->SetInput(point1);
	 vtkSmartPointer<vtkActor> actor3 =
	 vtkSmartPointer<vtkActor>::New();
	 actor3->GetProperty()->SetColor(000,100,000);
	 actor3->SetMapper(mapper3);
	 actor3->GetProperty()->SetPointSize(4);
	 renderer->AddActor(actor3);


	 }

	 iter2++;
	 }

	 std::list<Vertex>::iterator iter3 = plymodel->vertexes.begin();
	 // translation of the model (every point in the list)
	 //while(iter1!=translated_model->vertexes.end()){
	 //std::cout<<"ciao"<<iter1->vx<< " " << iter1->vy<< " " << iter1->vz<<std::endl;

	 //	fprintf(pFile1,"%f  %f  %f\n", iter1->vx,iter1->vy,iter1->vz);
	 //	iter1++;
	 //}


	 //visualize RGB
	 /*FILE* pfile=fopen("RGBBOX.rgb", "r");
	 FILE *pfile1=fopen(inputRangeScene.c_str(),"r");
	 FILE *pfile2=fopen("cleaned.txt", "w");
	 inputcleaned=fopen("inputCleaned.txt","r");
	 rgbcleaned=fopen("RGBcleaned.txt","r");
	 int red, green, blue;
	 int count1=0;

	 while(!feof(inputcleaned)){


	 fscanf(inputcleaned, "%f %f %f", &x, &y, &z);
	 //	std::cout<<"count"<<count1<<std::endl;
	 fscanf(rgbcleaned, "%d %d %d ", &red, &green, &blue);

	 if(red>=96 && red<=122 && green>=30 && green<=55 && blue>=4 &&blue<=25){
	 std::cout<<"ciao"<<std::endl;
	 fprintf(pfile2, "%f %f %f\n", x,y,z);
	 }
	 }

	 //from rgb to hsv
	 /*	double maxC = blue;
	 if (maxC < green)
	 maxC = green;
	 if (maxC < red)
	 maxC = red;
	 double minC = blue;
	 if (minC > green)
	 minC = green;
	 if (minC > red)
	 minC = red;

	 double delta = maxC - minC;

	 double V = maxC;
	 double S = 0;
	 double H = 0;

	 if (delta == 0) {
	 H = 0;
	 S = 0;
	 } else {
	 S = delta / maxC;

	 double dR = 60 * (maxC - red) / delta + 180;
	 double dG = 60 * (maxC - green) / delta + 180;
	 double dB = 60 * (maxC - blue) / delta + 180;
	 if (red == maxC)
	 H = dB - dG;
	 else if (green == maxC)
	 H = 120 + dR - dB;
	 else
	 H = 240 + dG - dR;
	 }

	 if (H < 0)

	 H += 360;
	 if (H >= 360)
	 H -= 360;

	 V=V/256;*/
/*		count1++;
	 //std::cout<<count1<<std::endl;

	 //if((H==240||H==210)&&(5/8<=V<=1)&&(3/4<=S<=1)){
	 //if(10000<count1<17000){	
	 std::cout<<count1<<std::endl;

	 vtkSmartPointer<vtkPoints> points =
	 vtkSmartPointer<vtkPoints>::New();

	 const float  p[3] = {x,y,z};

	 // Create the topology of the point (a vertex)
	 vertices =
	 vtkSmartPointer<vtkCellArray>::New();
	 pid[0] = points->InsertNextPoint(p);
	 vertices->InsertNextCell(1,pid);

	 // Create a polydata object
	 point1 =
	 vtkSmartPointer<vtkPolyData>::New();

	 // Set the points and vertices we created as the geometry and topology of the polydata
	 point1->SetPoints(points);
	 point1->SetVerts(vertices);

	 vtkSmartPointer<vtkPolyDataMapper> mapper3 =
	 vtkSmartPointer<vtkPolyDataMapper>::New();

	 mapper3->SetInput(point1);
	 vtkSmartPointer<vtkActor> actor3 =
	 vtkSmartPointer<vtkActor>::New();


	 actor3->GetProperty()->SetColor(red/255.0,green/255.0,blue/255.0);
	 actor3->SetMapper(mapper3);
	 actor3->GetProperty()->SetPointSize(4);
	 renderer->AddActor(actor3);


	 }*/


vtkSmartPointer<vtkPLYReader> reader2 =
		vtkSmartPointer<vtkPLYReader>::New();
reader2->SetFileName ("/Users/valentinaarmenise/Applications/VTKlib/VTKBin/myCode/Test/build/poseFile.ply");

vtkSmartPointer<vtkPolyDataMapper> map=
		vtkSmartPointer<vtkPolyDataMapper>::New();
map->SetInputConnection(reader2->GetOutputPort());

vtkSmartPointer<vtkActor> actorr =
		vtkSmartPointer<vtkActor>::New();
//actor2->GetProperty()->SetColor(255,000,000);

actorr->GetProperty()->SetColor(198/255.0,226/255.0,255/255.0);
actorr->GetProperty()->SetOpacity(0.7);

actorr->SetMapper(map);


vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();



renderer->AddActor(actor);
//renderer->AddActor(actor1);
//renderer->AddActor(actor2);
//renderer->AddActor(actor4);
renderer->AddActor(actorr);
renderer->SetBackground(1, 1, 1); // Background color green



vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
renderWindow->AddRenderer(renderer);
vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
renderWindowInteractor->SetRenderWindow(renderWindow);
vtkSmartPointer<MouseInteractorStyle> style =
		vtkSmartPointer<MouseInteractorStyle>::New();
renderWindowInteractor->SetInteractorStyle( style );

renderWindow->Render();
renderWindowInteractor->Start();


}
