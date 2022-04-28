
/*****************************************************************//**
 * \file   ModeTutorial.cpp
 * \brief  モードチュートリアル
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
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.loadresJson().LoadSounds("ingame");
   gameInstance.loadStage().LoadStageModels("Stage");
}

void ModeTutorial::Enter() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   using Vector4 = AppFrame::Math::Vector4;

   auto& gameInstance = Game::Game::GetInstance();
   auto& objFactory = gameInstance.objFactory();
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>());
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>());
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>());
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>());
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>());
   objFactory.Register("PoorEnemyMelee", std::make_unique<Create::PoorEnemyMeleeCreator>());
   objFactory.Register("PoorEnemyAlmighty", std::make_unique<Create::PoorEnemyAlmightyCreator>());

   std::vector<std::string> spawnTableNames;
   for (int i = 1; MaxWave >= i; i++) {
      std::string tableName = "tutorialwave" + std::to_string(i);
      spawnTableNames.emplace_back(tableName);
   }
   objFactory.LoadSpawnTables("tutorial", spawnTableNames);
   objFactory.SetSpawnTable("tutorialwave1");
   auto player = objFactory.Create("Player");
   auto& objServer = gameInstance.objServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));
   gameInstance.playerStatus(_param->GetBoolParam("max_hp"),
      _IntParam("max_bullet"), _IntParam("max_portion"));
   auto& soundComponent = gameInstance.soundComponent();
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
   auto& objFactory = Game::Game::GetInstance().objFactory();
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
   auto& runSprites = Game::Game::GetInstance().sprServer().runSprites();
   // 動的配列に一致する要素があるか判定を行う
   auto isActiveTips = std::any_of(runSprites.begin(), runSprites.end(),
      [](std::unique_ptr<Sprite::SpriteBase>& spr) {
         // 生存状態のTipsはあるか
         return (spr->GetSprType() == Sprite::SpriteBase::SpriteType::TutorialTips) && spr->IsActive(); });
   return isActiveTips;
}

void ModeTutorial::ClearJudge(std::string_view key) {
   auto& runSprites = Game::Game::GetInstance().sprServer().runSprites();
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
   auto& sprServer = Game::Game::GetInstance().sprServer();
   sprServer.Add(std::move(tips));
}

void ModeTutorial::FallObjectRespawn() {
   // オブジェクト一括管理クラスから処理を回す用の動的配列を取得する
   auto& gameInstance = Game::Game::GetInstance();
   auto& runObjects = gameInstance.objServer().runObjects();
   // 動的配列に一致する要素があるか判定を行う
   auto isActiveFallObject = std::any_of(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<Object::ObjectBase>& obj) {
         // 生存状態の落下オブジェクトはあるか
         return (obj->GetObjType() == Object::ObjectBase::ObjectType::FallObject) && obj->IsActive(); });
   if (!isActiveFallObject) {
      auto fallObject = gameInstance.objFactory().Create("FallObject");
      fallObject->position({ 0,0,0 });
      gameInstance.objServer().Add(std::move(fallObject));
   }
}
