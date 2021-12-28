
/*****************************************************************//**
 * \file   ModeInGame.cpp
 * \brief  ���[�h�C���Q�[��
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeInGame.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "PlayerCreator.h"
#include "StageCreator.h"
#include "ResourceLoadJson.h"
using namespace FragmentValkyria::Mode;

ModeInGame::ModeInGame(Game::GameMain& game) : ModeBase(game), _gameMain(game) {
}

void ModeInGame::Init() {
   auto& rj = GetResJson();
   rj.LoadModels("player");
   rj.LoadModels("skysphere");
   rj.LoadModels("ground");

}

void ModeInGame::Enter() {

   auto& of = objFactory();
   of.Register("Player", std::make_unique<Create::PlayerCreator>());
   of.Register("Stage", std::make_unique<Create::StageCreator>());

   auto player = of.Create("Player");
   // �A�N�^�[�T�[�o�[�ɓo�^���ʃA�N�Z�X�p
   auto& os = objServer();
   os.Register("Player", player->position());
   os.Add(std::move(player));

   auto stage = of.Create("Stage");
   os.Add(std::move(stage));

   Update();
}

void ModeInGame::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().RightClick()) {
      // �E�N���b�N�Ń^�C�g���֑J��
      GetModeServer().GoToMode("Title", 'L');
   }
   objServer().Input(input);
}

void ModeInGame::Update() {
   objServer().Update();
}

void ModeInGame::Render() {
   objServer().Render();
}

void ModeInGame::Exit() {
   // �A�N�^�[���폜
   objServer().Clear();
   // �f���v���P�[�g�������f�������폜
   GetResServer().DeleteDuplicateModels();
   // �N���G�C�^�[���폜
   objFactory().Clear();
}
