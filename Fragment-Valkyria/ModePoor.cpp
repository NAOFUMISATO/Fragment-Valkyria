
/*****************************************************************//**
 * \file   ModePoor.cpp
 * \brief  ���[�h�G����
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModePoor.h"
#include <stdexcept>
#include "BulletCreator.h"
#include "FallObjectCreator.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "Game.h"
#include "LargeEnemyCreator.h"
#include "LargeEnemy.h"
#include "LaserCreator.h"
#include "PlayerCreator.h"
#include "Player.h"
#include "PoorEnemyGatlingCreator.h"
#include "PoorEnemyMeleeCreator.h"
#include "PoorEnemyAlmightyCreator.h"
#include "GatlingCreator.h"
#include "ObjectBase.h"
#include "LoadStageFromJson.h"
#include "ModelAnimeComponent.h"
#include "ParamPoorEnemy.h"

using namespace FragmentValkyria::Mode;

ModePoor::ModePoor() {
   _param = std::make_unique<Param::ParamPoorEnemy>("poorenemy");
   _playerParam = std::make_unique<Param::ParamPlayer>("player");
}

void ModePoor::Enter() {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _playerParam->GetIntParam(paramName);
   };
   using Vector4 = AppFrame::Math::Vector4;

   
   auto& objFactory = Game::Game::GetObjFactory();
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>());
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>());
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>());
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>());
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>());
   objFactory.Register("PoorEnemyMelee", std::make_unique<Create::PoorEnemyMeleeCreator>());
   objFactory.Register("PoorEnemyAlmighty", std::make_unique<Create::PoorEnemyAlmightyCreator>());

   std::vector<std::string> spawnTableNames;
   for (int i = 1; _param->GetIntParam("max_wave") >= i; i++) {
      std::string tableName = "poorwave" + std::to_string(i);
      spawnTableNames.emplace_back(tableName);
   }
   objFactory.LoadSpawnTables("poor", spawnTableNames );

   objFactory.SetSpawnTable("poorwave1");

   auto player = objFactory.Create("Player");
   // �A�N�^�[�T�[�o�[�ɓo�^���ʃA�N�Z�X�p
   auto& objServer = Game::Game::GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));
   auto& soundComponent = Game::Game::GetSoundComponent();
   soundComponent.PlayLoop("PoorBattleBgm");
   _wave = 1;
   _playSound = true;
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.ingameTimer(0);
   gameInstance.playerStatus(_playerParam->GetDoubleParam("max_hp"),
      _IntParam("max_bullet"), _IntParam("max_portion"));
   ModeInGameBase::Enter();
}

void ModePoor::Input(AppFrame::Input::InputManager& input) {
#ifdef _DEBUG
   if (input.GetXJoypad().BackClick()) {
      auto& gameInstance = Game::Game::GetInstance();
      gameInstance.isPoorClear(true);
      auto& soundComponent = Game::Game::GetSoundComponent();
      soundComponent.Stop("PoorBattleBgm");
      auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
      modeServer.GoToMode("Loading",'S');
   }
#endif
   ModeInGameBase::Input(input);
}

void ModePoor::Update() {
   if (_playSound) {
      auto& soundComponent = Game::Game::GetSoundComponent();
      soundComponent.Play("PoorBattleStartVoice");
      _playSound = false;
   }
   ModeInGameBase::Update();
   WaveProcess();
}

void ModePoor::Render() {
   ModeInGameBase::Render();
}

void ModePoor::WaveProcess() {
   // �I�u�W�F�N�g�ꊇ�Ǘ��N���X���珈�����񂷗p�̓��I�z����擾����
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   // ���I�z��Ɉ�v����v�f�����邩������s��
   auto _IsActiveEnemy = std::any_of(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<Object::ObjectBase>& obj) {
         // ������Ԃ̎G���G�͂��邩
         return (obj->GetObjType() == Object::ObjectBase::ObjectType::PoorEnemy) && 
            obj->IsActive(); });
   // ������Ԃ̎G���G�����Ȃ���
   if (!_IsActiveEnemy) {
      // �ő�wave�ɒB�����Ȃ烂�[�h�J�ڂ��s��
      if (_wave >= _param->GetIntParam("max_wave")) {
         auto& soundComponent = Game::Game::GetSoundComponent();
         soundComponent.Play("PoorBattleEndVoice");
         auto& gameInstance = Game::Game::GetInstance();
         gameInstance.isPoorClear(true);
         auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
         modeServer.GoToMode("Loading", 'S');
         return;
      }
      // ����wave�̃X�|�[���e�[�u����ݒ肷��
      auto& objFactory = Game::Game::GetObjFactory();
      objFactory.SetSpawnTable("poorwave" + std::to_string(_wave + 1));
      // wave��i�߂�
      _wave++;
   }
}
