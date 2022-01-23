#include "ModeInGameBase.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "LoadStageFromJson.h"

using namespace FragmentValkyria::Mode;
using namespace FragmentValkyria;

ModeInGameBase::ModeInGameBase(Game::GameMain& gameMain) : ModeBase{gameMain} {
}

void ModeInGameBase::Init() {

}

void ModeInGameBase::Enter() {
   Update();
}

void ModeInGameBase::Input(AppFrame::Input::InputManager& input) {
   GetObjServer().Input(input);
   GetEfcServer().Input(input);
}

void ModeInGameBase::Update() {
   GetObjServer().Update();
   GetEfcServer().Update();
}

void ModeInGameBase::Render() {
   GetObjServer().Render();
   GetEfcServer().Render();
}

void ModeInGameBase::Exit() {
   GetObjServer().Clear();
   GetEfcServer().Clear();
   GetResServer().DeleteDuplicateModels();
   GetObjFactory().Clear();
   _gameMain.loadStage().ClearStageModels();
}

Create::ObjectFactory& ModeInGameBase::GetObjFactory() const {
   return _gameMain.objFactory();
}