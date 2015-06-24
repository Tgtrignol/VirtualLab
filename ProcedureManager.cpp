#include "ProcedureManager.h"
#include "ControlEnum.h"
#include "DSLReader.h"
#include "KeyPoint.h"
#include "ProcedureInformation.h"
#include "ProcedureObject.h"
#include "Control.h"
#include "GameManager.h"
#include "Scene.h"
#include "hud.h"
#include <iostream>

void ProcedureManager::init()
{
	procedureFileLocations = DSLReader().readProcedureLocationFromFile();
	if (procedureFileLocations.size() > 0)//Test: instead of menu.
	{
		currentProcedureInformation = DSLReader().readProcedureFromFile(procedureFileLocations[3]);
	}

	for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
	{
			procedureObject->init();
	}
}

void ProcedureManager::draw()
{
	for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
	{
		if (!procedureObject->isChangeObject)
			procedureObject->draw();
	}
}

void ProcedureManager::update(ControlEnum controlEnum)
{
	for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
	{
		if (!procedureObject->isChangeObject)
			procedureObject->update();
	}

	if (controlEnum == ControlEnum::None)
	{
		return;
	}

	GameManager::getInstance()->scene->hud->buttonText = std::string(ControlEnumToString(controlEnum));

	for each (KeyPoint *keyPoint in currentProcedureInformation->m_keyPoints)
	{
		if (keyPoint->m_isSuccessTriggered && procedure)
			continue;

		bool selectedHydraLeft = false;
		ProcedureObject *contextObject = 0;
		Control *contextControl = 0;
		bool objectSelected = false;

		for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
		{
			if (procedureObject->grabbed)
			{
				if (RightLeft == procedureObject->LeftRight)
					objectSelected = true;
			}
			else if (righternSelectedProcedureObject == procedureObject || lefternSelectedProcedureObject == procedureObject)
			{
				if (righternSelectedProcedureObject == procedureObject && RightLeft == "Right")
					selectedHydraLeft = false;
				else if (lefternSelectedProcedureObject == procedureObject && RightLeft == "Left")
					selectedHydraLeft = true;
				else
					return;

				righternSelectedProcedureObject = NULL;
				lefternSelectedProcedureObject = NULL;

				objectSelected = true;
			}

			if (objectSelected)
			{
				bool isBreakCalled = false;
				if (procedure)
				{	
					for each (Control *control in procedureObject->controls)
					{			
						if (control->m_primitive == "GrabRelease" && control->m_control == controlEnum)
						{
							contextObject = procedureObject;
							contextControl = control;

							isBreakCalled = true;
							break;
						}
						if (control->m_primitive == keyPoint->m_primitive)
						{
							contextObject = procedureObject;
							contextControl = control;

							isBreakCalled = true;
							break;
						}
					}
				}
				else
				{
					isBreakCalled = true;
					for each (Control *control in procedureObject->controls)
					{
						if (control->m_control == controlEnum)
						{
							contextObject = procedureObject;
							contextControl = control;
							break;
						}
					}
				}
				if (isBreakCalled)
					break;
			}
		}

		if (contextControl == 0 || contextObject == 0)
			return;
	
		//Getting appliedObject and changingObject
		ProcedureObject* appliedObject = 0;
		ProcedureObject* changingObject = 0;

		if (keyPoint->m_params.size() > 1 && procedure)
		{
			for (int i = 1; i < keyPoint->m_params.size(); i++)
			{
				std::string objectName = *keyPoint->m_params[i];
				for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
				{
					if (objectName == procedureObject->name)
					{
						if (i == 1)
							appliedObject = procedureObject;
						else if (i == 2)
							changingObject = procedureObject;
					}
				}
			}
		}
		else if (!procedure)
		{
			//Code for testing code for changing 2 objects into 1
			string appliedName;
			string changingName;
			if (!changingObjectTest)
			{
				appliedName = "Volumetric_flask";
				changingName = "Volumetric_flask_with_Funnel";
			}
			else
			{
				appliedName = "";
				changingName = "Volumetric_flask";
			}

			for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
			{
				if (appliedName == procedureObject->name)
					appliedObject = procedureObject;
				else if (changingName == procedureObject->name)
					changingObject = procedureObject;
			}
		}

		//Checking and performing control
		if (contextControl->m_primitive == "GrabRelease")
		{
			if (contextControl->m_control == controlEnum)
			{
				bool canPickup = false;
				//Check if a hand is empty
				bool leftHandEmpty = true;
				bool rightHandEmpty = true;
				for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
				{
					if (procedureObject->grabbed)
					{
						if (procedureObject->LeftRight == "Right")
							rightHandEmpty = false;
						else if (procedureObject->LeftRight == "Left")
							leftHandEmpty = false;
					}
				}
				
				if (RightLeft == "Right")
				{
					if (rightHandEmpty)
						canPickup = true;
				}
				else if (RightLeft == "Left")
				{
					if (leftHandEmpty)
						canPickup = true;
				}

				if (!contextObject->grabbed && canPickup)
				{
					contextObject->grabbed = true;
					if (selectedHydraLeft)
						contextObject->LeftRight = "Left";
					else
						contextObject->LeftRight = "Right";
					contextObject->update();
				}
				else
				{
					contextObject->grabbed = false;
					contextObject->update();
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Rinse" && procedure) || contextControl->m_primitive == "Rinse")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					
					//ContextObjects: Bottle_of_demineralized_water, Beaker
					//AppliedObjects: Volumetric_flask, Funnel(Not visible), Weighing_Boat(Not visible), Erlenmeyer_flask, Burette_with_Funnel
					if (contextObject->name == "Bottle_of_demineralized_water")
					{
						appliedObject->useWaterOverlay = true;
					}
					else if (contextObject->name == "Beaker")
					{
						appliedObject->useWaterOverlay = true;
					}
					
					if (appliedObject->name == "Funnel" || appliedObject->name == "Weighing_boat")
					{
						appliedObject->useWaterOverlay = false;
					}
					
					
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Rinse water in appliedObject
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "FlushLiquid" && procedure) || contextControl->m_primitive == "FlushLiquid")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (appliedObject->grabbed)
				{
					appliedObject->useWaterOverlay = false;
					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "FillHalfway"&& procedure) || contextControl->m_primitive == "FillHalfway")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					//AppliedObject: Volumetric_flask
					//Max height = 14
					//Half = 7

					appliedObject->useWaterOverlay = true;
					appliedObject->waterDirectionMax->setY(appliedObject->waterDirectionMax->y() / 2);

					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "FillUntilMark" && procedure) || contextControl->m_primitive == "FillUntilMark")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					//AppliedObject: Volumetric_flask
					//Max height = 14

					appliedObject->useWaterOverlay = true;

					//With 14 is looks like the water is at the mark
					appliedObject->waterDirectionMax->setY(14);

					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Liquefy"&& procedure) || contextControl->m_primitive == "Liquefy")
		{
			//CONTROL NOT USED
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					//AppliedObject: Volumetric_flask
					//appliedObject->useWaterOverlay = true;

					//Make the water a little bit higher
					//appliedObject->waterDirectionMax->setY(appliedObject->waterDirectionMax->y() + 1);
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Indicate flask with some liquid in it
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Attach"&& procedure) || contextControl->m_primitive == "Attach")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed && changingObject != NULL)
				{
					//TODO: Change appliedObject into changingObject and dont show the Funnel anymore
					
					changingObject->isChangeObject = false;
					changingObject->origin = appliedObject->origin;
					changingObject->LeftRight = appliedObject->LeftRight;
					changingObject->grabbed = true;

					//Funnel
					contextObject->isChangeObject = true;
					contextObject->grabbed = false;
					contextObject->LeftRight = "None";
					contextObject->deleteRigidBodyFromWorld();

					appliedObject->isChangeObject = true;
					appliedObject->grabbed = false;
					appliedObject->LeftRight = "None";
					appliedObject->deleteRigidBodyFromWorld();

					changingObjectTest = true;

					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Swerve"&& procedure) || contextControl->m_primitive == "Swerve")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed)
				{
					//ContextObject: Volumetric_flask
					
					contextObject->rotate("Y", 90);
					if (contextObject->rotation->x() == 45)
						anglePositive = false;
					else if (contextObject->rotation->x() == -45)
						anglePositive = true;
					
					if (anglePositive)
						contextObject->rotate("X", 45);
					else
						contextObject->rotate("X", -45);

					//Swerve volumetric flask: perform 4 times for full 360
					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Rotate"&& procedure) || contextControl->m_primitive == "Rotate")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed)
				{
					//ContextObject: Volume_pipette, Volumetric_flask

					//contextObject->rotate("X", 90);
					contextObject->rotate("X", 120);
					//Rotate object vertical 360 degrees: Perform 3 times for 360
					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Detach_Funnel"&& procedure) || contextControl->m_primitive == "Detach_Funnel")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && changingObject != NULL)
				{
					//Check if other hand is empty
					string pickedRightOrLeft = contextObject->LeftRight;
					bool otherHandEmpty = true;
					string checkOtherHand = "";

					if (pickedRightOrLeft == "Left")
						checkOtherHand = "Right";
					else if (pickedRightOrLeft == "Right")
						checkOtherHand = "Left";

					for each (ProcedureObject* prob in currentProcedureInformation->m_procedureObjects)
					{
						if (prob->grabbed && prob->LeftRight == checkOtherHand)
							otherHandEmpty = false;
					}

					if (otherHandEmpty)
					{
						for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
						{
							if (procedureObject->name == "Funnel")
								appliedObject = procedureObject;
						}
						
						//TODO: Change contextObject back into 2 object, the funnel and changingObject
						changingObject->isChangeObject = false;
						changingObject->origin = contextObject->origin;
						changingObject->LeftRight = contextObject->LeftRight;
						changingObject->grabbed = true;						

						//Funnel
						appliedObject->isChangeObject = false;
						appliedObject->grabbed = true;
						appliedObject->LeftRight = checkOtherHand;

						contextObject->isChangeObject = true;
						contextObject->grabbed = false;
						contextObject->LeftRight = "None";
						contextObject->deleteRigidBodyFromWorld();

						keyPoint->m_isSuccessTriggered = true;
					}
					else
					{
						//TODO: Show error sign
					}
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Cork" && procedure) || contextControl->m_primitive == "Cork")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					//TODO: Change flask into corked flask and dont display cork anymore

					//Corked Flask
					changingObject->isChangeObject = false;
					changingObject->origin = appliedObject->origin;
					changingObject->LeftRight = appliedObject->LeftRight;
					changingObject->useWaterOverlay = appliedObject->useWaterOverlay;
					changingObject->grabbed = true;

					//Cork
					contextObject->isChangeObject = true;
					contextObject->grabbed = false;
					contextObject->LeftRight = "None";
					contextObject->deleteRigidBodyFromWorld();

					//Flask
					appliedObject->isChangeObject = true;
					appliedObject->grabbed = false;
					appliedObject->LeftRight = "None";
					appliedObject->deleteRigidBodyFromWorld();

					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Dump" && procedure) || contextControl->m_primitive == "Dump")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL && changingObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					//TODO: Dump solid into appliedObject

					//Empty Weighing boat
					changingObject->isChangeObject = false;
					changingObject->origin = contextObject->origin;
					changingObject->LeftRight = contextObject->LeftRight;
					changingObject->grabbed = true;

					//Full Weighing boat
					contextObject->isChangeObject = true;
					contextObject->grabbed = false;
					contextObject->LeftRight = "None";
					contextObject->deleteRigidBodyFromWorld();

					//Change appliedobject


					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "SuckLiquid" && procedure) || contextControl->m_primitive == "SuckLiquid")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed)
				{
					//ContextObject: Volume_Pipette
					contextObject->useWaterOverlay = true;

					switch (contextObject->controlStep)
					{
					case 0:
						//Half: 5.3
						contextObject->waterDirectionMax->setY(contextObject->waterDirectionMax->y() - 4);
						contextObject->controlStep++;
						break;
					case 1 :
						//Until mark = 9.3
						contextObject->waterDirectionMax->setY(contextObject->waterDirectionMax->y() + 4);
						contextObject->controlStep++;
						break;
					}
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Suck liquid from appliedObject and show liquid in contextObject
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "TurnSideWay" && procedure) || contextControl->m_primitive == "TurnSideWay")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed)
				{
					if (contextObject->rotation->z() == 90)
					{
						contextObject->rotate("Z", -90);
					}
					else if (contextObject->rotation->z() == 0)
					{
						contextObject->rotate("Z", 90);
					}
					//Rotate volume pipette 90 degrees horizontal: first press is 90 degree rotation, second press is back at 0 degrees
					keyPoint->m_isSuccessTriggered = true;

				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "PourWithLiquid" && procedure) || contextControl->m_primitive == "PourWithLiquid")
		{
			//CONTROL NOT USED
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Pour liquid in beaker
					//This keypoint can be skipped by making the beaker a beaker with liquid from the start
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Dry" && procedure) || contextControl->m_primitive == "Dry")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject-> grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Dry appliedObject with contextObject
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Turn45Degree" && procedure) || contextControl->m_primitive == "Turn45Degree")
		{
			if (contextControl->m_control == controlEnum )//&& appliedObject != NULL)
			{
				if (contextObject->grabbed )//&& appliedObject->grabbed)
				{
					if (contextObject->rotation->z() == 0)
						contextObject->rotate("Z", -45);
					else if (contextObject->rotation->z() == -45)
						contextObject->rotate("Z", 45);
					//Rotate contextObject 45 degrees horizontal: First press is 45 degree, second is back
					keyPoint->m_isSuccessTriggered = true;
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "LowerLiquid"&& procedure) || contextControl->m_primitive == "LowerLiquid")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					appliedObject->useWaterOverlay = false;
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Liquid goes out of volume pipette
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "OpenCloseTap"&& procedure) || contextControl->m_primitive == "OpenCloseTap")
		{
			if (contextControl->m_control == controlEnum)
			{
				switch (contextObject->controlStep)
				{
				case 0:
					//Open the buret
					contextObject->controlStep = 1;
					contextObject->useWaterOverlay = false;
					//TODO: Liquid goes out of burette
					break;
				case 1:
					//Close the buret
					contextObject->controlStep = 0;
					contextObject->useWaterOverlay = true;
					//TODO: Liquid stops going out of burette
					break;
				}

				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Pour10Ml"&& procedure) || contextControl->m_primitive == "Pour10Ml")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					//AppliedObject: Burette
					//Add some liquid in the buret
					appliedObject->waterDirectionMax->setY(appliedObject->waterDirectionMin->getY() + 40);
					
					keyPoint->m_isSuccessTriggered = true;
					//Change watermax height

					//TODO: Liquid goes into appliedObject
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Fill"&& procedure) || contextControl->m_primitive == "Fill")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (contextObject->grabbed && appliedObject->grabbed && changingObject != NULL)
				{
					//AppliedObject: Burette
					appliedObject->useWaterOverlay = true;
					//Max height buret: 240 - 60 = 180
					//Max height buret with funnel: 219- 39 = 180

					appliedObject->waterDirectionMax->setY(219);
					//Change watermax height

					keyPoint->m_isSuccessTriggered = true;
					//TODO: Fill burette with liquid and detach the funnel form burette
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Titrate1Ml"&& procedure) || contextControl->m_primitive == "Titrate1Ml")
		{
			if (contextControl->m_control == controlEnum && appliedObject != NULL)
			{
				if (appliedObject->grabbed)
				{
					//ContextObject: Burette
					appliedObject->useWaterOverlay = true;
					appliedObject->waterDirectionMax->setY(appliedObject->waterDirectionMax->y() - 20);

					keyPoint->m_isSuccessTriggered = true;
					//Change watermax height

					//TODO: Titrate liquid into the erlenmeyer
				}
				else
				{
					keyPoint->m_isSuccessTriggered = false;
					//TODO: Show error sign
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "ReadAmount"&& procedure) || contextControl->m_primitive == "ReadAmount")
		{
			if (contextControl->m_control == controlEnum)
			{
				//ContextObject: Burette
				int amount = (appliedObject->waterDirectionMax->getY() - appliedObject->waterDirectionMin->getY());

				//Get watermax height
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		break;
	}

	bool procedureCompleted = true;
	for each (KeyPoint *keyPoint in currentProcedureInformation->m_keyPoints)
	{
		if (keyPoint->m_isSuccessTriggered == false)
			procedureCompleted = false;
	}

	if (procedureCompleted)
	{
		//TODO: Do something for procedure completion.
	}
}