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

		ProcedureObject *contextObject;
		Control *contextControl;

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

		if (keyPoint->m_primitive == "Rinse")
		{
			if (contextControl->m_control == controlEnum)
			{
				keyPoint->m_isSuccessTriggered = true;
			}
			else
			{
				//TODO: Show error sign
			}
		}
		break;
	}

	//TODO: Check for procedure completion.
}