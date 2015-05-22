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
				
				//contextObject->doControl("Rinse", ProcedureObject2);
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

				//contextObject->doControl("GrabRelease");
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

				//contextObject->doControl("FlushLiquid", ProcedureObject2);
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

				//contextObject->doControl("FillHalfway", ProcedureObject2);
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

				//contextObject->doControl("FillUntilMark", ProcedureObject2);
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

				//contextObject->doControl("Liquefy");
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

				//contextObject->doControl("Attach", ProcedureObject2);
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

				//contextObject->doControl("Swerve");
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

				//contextObject->doControl("Rotate");
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

				//contextObject->doControl("DetachFunnel");
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

				//contextObject->doControl("Cork", ProcedureObject2);
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

				//contextObject->doControl("Dump", ProcedureObject2);
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

				//contextObject->doControl("SuckLiquid", ProcedureObject2);
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

				//contextObject->doControl("TurnSideWay");
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

				//contextObject->doControl("PourWithLiquid", ProcedureObject2);
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

				//contextObject->doControl("Dry", ProcedureObject2);
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

				//contextObject->doControl("Turn45Degree");
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

				//contextObject->doControl("LowerLiquid", ProcedureObject2);
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

				//contextObject->doControl("OpenCloseTap");
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

				//contextObject->doControl("Pour10Ml", ProcedureObject2);
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

				//contextObject->doControl("Fill", ProcedureObject2);
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

				//contextObject->doControl("Titrate1Ml");
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

				//contextObject->doControl("ReadAmount");
			}
			else
			{
				keyPoint->m_isSuccessTriggered = false;
				//TODO: Show error sign
			}
		}
		break;
	}

	//TODO: Check for procedure completion.
}