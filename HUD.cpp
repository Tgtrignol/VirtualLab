#include "HUD.h"
#include "Hydra.h"
#include "ProcedureManager.h"
#include "ProcedureObject.h"
#include "GameManager.h"
#include "Scene.h"
#include "Notes.h"
#include "GaussianBlur.h"
#include "Control.h"

HUD::HUD() {

	postProcessing = new GaussianBlur(true);
	postProcessing->init();

}

HUD::~HUD() {

	delete postProcessing;

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

	GameManager::getInstance()->scene->notes->drawList(RightControlnames, 480, 320, 0);

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

	GameManager::getInstance()->scene->notes->drawList(LeftControlnames, 240, 320, 0);

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
	GameManager::getInstance()->scene->notes->drawNotes(text.data(), text.length(), 380, 330, 0, 0.0f, 0.0f, 0.0f);

	//GameManager::getInstance()->scene->notes->drawNotes(buttonText.data(), buttonText.length(), 200, 300, 0);



}

void HUD::init() {

}