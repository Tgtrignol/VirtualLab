#include "ProcedureManager.h"
#include "ControlEnum.h"
#include "DSLReader.h"
#include "KeyPoint.h"
#include "ProcedureInformation.h"
#include "ProcedureObject.h"
#include "Control.h"
#include "GameManager.h"
#include "Scene.h"

void ProcedureManager::init()
{
	procedureFileLocations = DSLReader().readProcedureLocationFromFile();
	if (procedureFileLocations.size() > 0)//Test: instead of menu.
	{
		currentProcedureInformation = DSLReader().readProcedureFromFile(procedureFileLocations[0]);
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
		procedureObject->draw();
	}
}

void ProcedureManager::update(ControlEnum controlEnum)
{
	for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
	{
		procedureObject->update();
	}

	if (controlEnum == ControlEnum::None)
	{
		return;
	}

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
					for each (Control *control in procedureObject->controls)
					{
						if (control->m_control == controlEnum)
						{
							contextObject = procedureObject;
							contextControl = control;

							isBreakCalled = true;
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

		if (keyPoint->m_params.size() > 1)
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

		//Checking and performing control
		if ((keyPoint->m_primitive == "Rinse" && procedure) || contextControl->m_primitive == "Rinse")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Rinse appliedObject
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
		else if ((keyPoint->m_primitive == "GrabRelease" && procedure) || contextControl->m_primitive == "GrabRelease")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
				if (!contextObject->grabbed)
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
					//contextObject->setGravity(&btVector3(0, -9.81f * 20.0f, 0));
					contextObject->update();
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
				if (contextObject->grabbed)
				{
					//TODO Check if hydra is near sink
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Indicate volumetric flask is halfway full
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Indicate volumetric flask is full
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed && changingObject != NULL)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Change appliedObject into changingObject and dont show the Funnel anymore
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
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Swerve volumetric flask
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
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Rotate object vertical 360 degrees
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
		else if ((keyPoint->m_primitive == "DetachFunnel"&& procedure) || contextControl->m_primitive == "DetachFunnel")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && changingObject != NULL)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Change contextObject back into 2 object, the funnel and changingObject
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Change contextObject back into 2 object, the funnel and changingObject
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Dump solid into appliedObject
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed)
				{
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
		else if ((keyPoint->m_primitive == "TurnSideWay" && procedure) || contextControl->m_primitive == "TurnSideway")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Rotate volume pipette 90 degrees horizontal
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
			if (contextControl->m_control == controlEnum)
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
					//TODO: Rotate contextObject 45 degrees horizontal
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
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
				//TODO: Check if hydra is near burette
				keyPoint->m_isSuccessTriggered = true;
				if (contextObject->closed)
				{
					//if buret is closed, open it
					contextObject->closed = false;
					//TODO: Liquid goes out of burette
				}
				else
				{
					//if buret is open, close it
					contextObject->closed = true;
					//TODO: Liquid stops going out of burette
				}
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if ((keyPoint->m_primitive == "Pour10Ml"&& procedure) || contextControl->m_primitive == "Pour10Ml")
		{
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
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
			if (contextControl->m_control == controlEnum)
			{
				if (contextObject->grabbed && appliedObject->grabbed && changingObject != NULL)
				{
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
			if (contextControl->m_control == controlEnum)
			{
				if (appliedObject->grabbed)
				{
					keyPoint->m_isSuccessTriggered = true;
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
				keyPoint->m_isSuccessTriggered = true;
				//TODO: Show amount of liquid in sign
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