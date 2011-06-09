	/*
	 *  MouseInteractorStyle.h
	 *  
	 *
	 *  Created by valentina armenise on 3/18/11.
	 *  Copyright 2011 Politecnico di Torino. All rights reserved.
	 *
	 */


	class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
	{
	public:
		static MouseInteractorStyle* New();
		vtkTypeRevisionMacro(MouseInteractorStyle, vtkInteractorStyleTrackballCamera);
		
		virtual void OnLeftButtonDown() 
		{
			
			std::cout << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1] << std::endl;
			this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0], 
												this->Interactor->GetEventPosition()[1], 
												0,  // always zero.
												this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
			double picked[3];
			this->Interactor->GetPicker()->GetPickPosition(picked);
			std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
			// Forward events
			vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
		}
		
	};


	vtkCxxRevisionMacro(MouseInteractorStyle, "$Revision: 1.1 $");
	vtkStandardNewMacro(MouseInteractorStyle);