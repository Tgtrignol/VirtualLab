#include "HUD.h"
#include "Hydra.h"
#include "ProcedureManager.h"
#include "GameManager.h"
#include "Scene.h"
#include "Notes.h"
#include "ProcedureObject.h"

HUD::HUD() {

}

void HUD::draw() {

	std::string text = GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject == nullptr ? "" :
		GameManager::getInstance()->scene->procedureManager->righternSelectedProcedureObject->name;
	GameManager::getInstance()->scene->notes->drawNotes(text.data(), text.length(), 200, 300, 0);

	GameManager::getInstance()->scene->notes->drawNotes(buttonText.data(), buttonText.length(), 200, 300, 0);

}

void HUD::init() {

}