#include "HUD.h"
#include "Hydra.h"
#include "ProcedureManager.h"
#include "ProcedureObject.h"
#include "GameManager.h"
#include "Scene.h"
#include "GaussianBlur.h"
#include "Control.h"
#include "TextRepresentation.h"

HUD::HUD() {
	textrepresentation = new TextRepresentation();
}

HUD::~HUD() {

}

void HUD::draw() {

	std::vector<std::string> LeftControlnames;
	std::vector<std::string> RightControlnames;

	if (GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject != NULL) {
		
		for each (Control* control in GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject->controls)
		{
			RightControlnames.push_back(ControlEnumToString(control->m_control) + "= " + control->m_primitive);
		}
	}
	else if (GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject == NULL)
	{
		if(GameManager::getInstance()->scene->procedureManager->getRightGrabbedObject() != NULL)
		{
			for each (Control *control in GameManager::getInstance()->scene->procedureManager->getRightGrabbedObject()->controls)
			{
				RightControlnames.push_back(ControlEnumToString(control->m_control) + "= " + control->m_primitive);
			}

		}
	}

	textrepresentation->drawList(RightControlnames, 480, 320, 0);

	if (GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject != NULL) {
		for each (Control *control in GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject->controls)
		{
			LeftControlnames.push_back(ControlEnumToString(control->m_control) + "= " + control->m_primitive);

		}
	}
	else if (GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject == NULL)
	{
		if (GameManager::getInstance()->scene->procedureManager->getLeftGrabbedObject() != NULL)
		{
			for each (Control *control in GameManager::getInstance()->scene->procedureManager->getLeftGrabbedObject()->controls)
			{
				LeftControlnames.push_back(ControlEnumToString(control->m_control) + "= " + control->m_primitive);
			}
		}
	}

	textrepresentation->drawList(LeftControlnames, 240, 320, 0);

	std::string text = "";
	if (GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject != nullptr)
	{
		text = GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject->name;
	}
	if (GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject != nullptr)
	{
		text = GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject->name;
	}

	//Show name of pointed object
	textrepresentation->drawText(text.data(), text.length(), 380, 330, 0, 0.0f, 0.0f, 0.0f);

	//GameManager::getInstance()->scene->notes->drawNotes(buttonText.data(), buttonText.length(), 200, 300, 0);

	if (errorText != "")
		textrepresentation->drawText(errorText.data(), errorText.length(), 320, 360, 0);

}

void HUD::init() {

}