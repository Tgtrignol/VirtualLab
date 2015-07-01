#include "HUD.h"
#include "Hydra.h"
#include "ProcedureManager.h"
#include "ProcedureObject.h"
#include "GameManager.h"
#include "Scene.h"
#include "Notes.h"
#include "GaussianBlur.h"
#include "Control.h"
#include "TextRepresentation.h"

HUD::HUD() {

}

HUD::~HUD() {

}

void HUD::draw() {

	if (GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject != NULL) {
		for each (Control *control in GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject->controls)
		{
			//GameManager::getInstance()->scene->menu->drawText(control->m_primitive.data(), control->m_primitive.length(), 200, 300, 0);
		}
	}

	if (GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject != NULL) {
		for each (Control *control in GameManager::getInstance()->scene->procedureManager->lefternSelectedProcedureObject->controls)
		{
			//GameManager::getInstance()->scene->menu->drawNotes(control->m_primitive.data(), control->m_primitive.length(), 200, 300, 0);
		}
	}

	std::string text = GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject == nullptr ? "" :
		GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject->name;
	//GameManager::getInstance()->scene->menu->drawNotes(text.data(), text.length(), 200, 300, 0);

	//GameManager::getInstance()->scene->notes->drawNotes(buttonText.data(), buttonText.length(), 200, 300, 0);



}

void HUD::init() {

}