
/*****************************************************************//**
 * \file   ModeTutorial.cpp
 * \brief  ���[�h�`���[�g���A��
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeTutorial.h"
#include "Game.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
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
#include "SpriteServer.h"
#include "TutorialTips.h"
#include "ParamPlayer.h"

namespace {
   constexpr auto MaxWave = 3;
}

using namespace FragmentValkyria::Mode;

ModeTutorial::ModeTutorial() {
   _param = std::make_unique<Param::ParamPlayer>("player");
}

void ModeTutorial::Init() {
   // �����̓ǂݍ���
   auto& loadresJson = Game::Game::GetLoadresJson();
   loadresJson.LoadSounds("ingame");
   // �X�e�[�W���\�[�X�̓ǂݍ���
   auto& loadStage = Game::Game::GetLoadStage();
   loadStage.LoadStageModels("Stage");
}

void ModeTutorial::Enter() {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   // �e�����Ǘ��N���X�̓o�^
   auto& objFactory = Game::Game::GetObjFactory();
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>());
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>());
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>());
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>());
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>());
   objFactory.Register("PoorEnemyMelee", std::make_unique<Create::PoorEnemyMeleeCreator>());
   objFactory.Register("PoorEnemyAlmighty", std::make_unique<Create::PoorEnemyAlmightyCreator>());
   // �X�|�[���e�[�u���̓ǂݍ���
   std::vector<std::string> spawnTableNames;
   for (int i = 1; MaxWave >= i; i++) {
      std::string tableName = "tutorialwave" + std::to_string(i);
      spawnTableNames.emplace_back(tableName);
   }
   objFactory.LoadSpawnTables("tutorial", spawnTableNames);
   // �ŏ��̃X�|�[���e�[�u����ݒ�
   objFactory.SetSpawnTable("tutorialwave1");
   // �v���C���[����
   auto player = objFactory.Create("Player");
   auto& objServer = Game::Game::GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));
   // �v���C���[�̌��݂̃X�e�[�^�X�𔽉f
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.playerStatus(_param->GetBoolParam("max_hp"),
      _IntParam("max_bullet"), _IntParam("max_portion"));
   // �����̍Đ�
   auto& soundComponent = Game::Game::GetSoundComponent();
   soundComponent.Stop("TitleBgm");
   soundComponent.PlayLoop("TutorialBgm");

   _born = true;

   ModeInGameBase::Enter();
}

void ModeTutorial::Input(InputManager& input) {
   switch (_tutorialProgress) {
   case 1:
      ClearJudge("MoveClear");
      break;
   case 2:
      ClearJudge("CameraClear");
      break;
   case 3:
      ClearJudge("WeakBulletClear");
      break;
   case 4:
      ClearJudge("ReloadClear");
      break;
   case 5:
      FallObjectRespawn();
      ClearJudge("ObjectShootClear");
      break;
   case 6:
      ClearJudge("HealClear");
      break;
   }
   ModeInGameBase::Input(input);
}

void ModeTutorial::Update() {
   auto& objFactory = Game::Game::GetObjFactory();
   if (!TipsAlive()) {
      switch (_tutorialProgress) {
      case 1:
         TipsBorn("MovePromotion");
         break;
      case 2:
         TipsBorn("CameraPromotion");
         break;
      case 3:
         TipsBorn("WeakBulletPromotion");
         objFactory.SetSpawnTable("tutorialwave2");
         break;
      case 4:
         TipsBorn("ReloadPromotion");
         break;
      case 5:
         TipsBorn("ObjectShootPromotion");
         objFactory.SetSpawnTable("tutorialwave3");
         break;
      case 6:
         TipsBorn("HealPromotion");
         break;
      }
   }
   ModeInGameBase::Update();
}

void ModeTutorial::Render() {
   ModeInGameBase::Render();
}

bool ModeTutorial::TipsAlive() {
   auto& runSprites = Game::Game::GetSprServer().runSprites();
   // ���I�z��Ɉ�v����v�f�����邩������s��
   auto isActiveTips = std::any_of(runSprites.begin(), runSprites.end(),
      [](std::unique_ptr<Sprite::SpriteBase>& spr) {
         // ������Ԃ�Tips�͂��邩
         return (spr->GetSprType() == Sprite::SpriteBase::SpriteType::TutorialTips) && spr->IsActive(); });
   return isActiveTips;
}

void ModeTutorial::ClearJudge(std::string_view key) {
   auto& runSprites = Game::Game::GetSprServer().runSprites();
   for (auto& sprite : runSprites) {
      if (sprite->GetSprType() == Sprite::SpriteBase::SpriteType::TutorialTips) {
         auto& tips = dynamic_cast<Tutorial::TutorialTips&>(*sprite);
         if (tips.IsTipsClear(key)) {
            _tutorialProgress++;
         }
      }
   }
}

void ModeTutorial::TipsBorn(std::string_view key){
   auto tips = std::make_unique<Tutorial::TutorialTips>(key);
   auto& sprServer = Game::Game::GetSprServer();
   sprServer.Add(std::move(tips));
}

void ModeTutorial::FallObjectRespawn() {
   // �I�u�W�F�N�g�ꊇ�Ǘ��N���X���珈�����񂷗p�̓��I�z����擾����
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   // ���I�z��Ɉ�v����v�f�����邩������s��
   auto isActiveFallObject = std::any_of(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<Object::ObjectBase>& obj) {
         // ������Ԃ̗����I�u�W�F�N�g�͂��邩
         return (obj->GetObjType() == Object::ObjectBase::ObjectType::FallObject) && obj->IsActive(); });
   if (!isActiveFallObject) {
      auto fallObject = Game::Game::GetObjFactory().Create("FallObject");
      fallObject->position({ 0,0,0 });
      auto& objServer = Game::Game::GetObjServer();
      objServer.Add(std::move(fallObject));
   }
}
