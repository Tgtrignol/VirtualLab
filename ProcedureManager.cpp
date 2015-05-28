#include "ProcedureManager.h"
#include "ControlEnum.h"
#include "DSLReader.h"
#include "KeyPoint.h"
#include "ProcedureInformation.h"
#include "ProcedureObject.h"
#include "Control.h"

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
		return;

	for each (KeyPoint *keyPoint in currentProcedureInformation->m_keyPoints)
	{
		if (keyPoint->m_isSuccessTriggered)
			continue;

		ProcedureObject *contextObject = 0;
		Control *contextControl = 0;

		for each (ProcedureObject *procedureObject in currentProcedureInformation->m_procedureObjects)
		{
			//TODO: Check if Hydra is near the object, then perform control

			bool isBreakCalled = false;
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

			if (isBreakCalled)
				break;
		}

		if (contextControl == 0 || contextObject == 0)
			return;
	
		if (keyPoint->m_primitive == "Rinse")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "GrabRelease")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "FlushLiquid")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "FillHalfway")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "FillUntilMark")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Liquefy")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Attach")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Swerve")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Rotate")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "DetachFunnel")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Cork")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Dump")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "SuckLiquid")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "TurnSideWay")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "PourWithLiquid")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Dry")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Turn45Degree")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "LowerLiquid")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "OpenCloseTap")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Pour10Ml")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Fill")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "Titrate1Ml")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		else if (keyPoint->m_primitive == "ReadAmount")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}

		//Performing Control
		if (keyPoint->m_isSuccessTriggered == true)
		{
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
			//TODO: Call control method
			//contextObject->doControl(keyPoint->m_primitive, appliedObject, changingObject);
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