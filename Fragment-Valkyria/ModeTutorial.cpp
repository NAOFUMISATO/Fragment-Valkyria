
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
   // 音源の読み込み
   auto& loadresJson = Game::Game::GetLoadresJson();
   loadresJson.LoadSounds("ingame");
   // ステージリソースの読み込み
   auto& loadStage = Game::Game::GetLoadStage();
   loadStage.LoadStageModels("Stage");
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
   // 各生成管理クラスの登録
   auto& objFactory = Game::Game::GetObjFactory();
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>());
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>());
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>());
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>());
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>());
   objFactory.Register("PoorEnemyMelee", std::make_unique<Create::PoorEnemyMeleeCreator>());
   objFactory.Register("PoorEnemyAlmighty", std::make_unique<Create::PoorEnemyAlmightyCreator>());
   // スポーンテーブルの読み込み
   std::vector<std::string> spawnTableNames;
   for (int i = 1; MaxWave >= i; i++) {
      std::string tableName = "tutorialwave" + std::to_string(i);
      spawnTableNames.emplace_back(tableName);
   }
   objFactory.LoadSpawnTables("tutorial", spawnTableNames);
   // 最初のスポーンテーブルを設定
   objFactory.SetSpawnTable("tutorialwave1");
   // プレイヤー生成
   auto player = objFactory.Create("Player");
   auto& objServer = Game::Game::GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));
   // プレイヤーの現在のステータスを最大に反映
   auto& gameInstance = Game::Game::GetInstance();
   gameInstance.playerStatus(_param->GetDoubleParam("max_hp"),
      _IntParam("max_bullet"), _IntParam("max_portion"));
   // 音源の再生
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
   // 動的配列に一致する要素があるか判定を行う
   auto isActiveTips = std::any_of(runSprites.begin(), runSprites.end(),
      [](std::unique_ptr<Sprite::SpriteBase>& spr) {
         // 生存状態のTipsはあるか
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
   // オブジェクト一括管理クラスから処理を回す用の動的配列を取得する
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   // 動的配列に一致する要素があるか判定を行う
   auto isActiveFallObject = std::any_of(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<Object::ObjectBase>& obj) {
         // 生存状態の落下オブジェクトはあるか
         return (obj->GetObjType() == Object::ObjectBase::ObjectType::FallObject) && obj->IsActive(); });
   if (!isActiveFallObject) {
      auto fallObject = Game::Game::GetObjFactory().Create("FallObject");
      fallObject->position({ 0,0,0 });
      auto& objServer = Game::Game::GetObjServer();
      objServer.Add(std::move(fallObject));
   }
}
