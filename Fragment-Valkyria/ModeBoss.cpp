
/*****************************************************************//**
 * \file   ModeBoss.cpp
 * \brief  モードボス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeBoss.h"
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
#include "StageCreator.h"
#include "GatlingCreator.h"
#include "ObjectBase.h"
#include "LoadStageFromJson.h"

namespace {
   constexpr auto MaxWave = 4;
   constexpr auto LightOnMaxCount = 4;
   constexpr auto DefaultDifRed = 0.6f;
   constexpr auto DefaultDifGreen = 0.6f;
   constexpr auto DefaultDifBlue = 0.7f;
   constexpr auto DefaultAmbRed = 0.5f;
   constexpr auto DefaultAmbGreen = 0.5f;
   constexpr auto DefaultAmbBlue = 0.5f;
   constexpr auto PlaySoundFrame = 169;
}

using namespace FragmentValkyria::Mode;

ModeBoss::ModeBoss() {
}

void ModeBoss::Enter() {
   using Vector4 = AppFrame::Math::Vector4;
   
   auto& gameInstance = Game::Game::GetInstance();

   auto& objFactory = gameInstance.objFactory();
   // 各生成管理クラスを登録
   objFactory.Register("LargeEnemy", std::make_unique<Create::LargeEnemyCreator>());
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>());
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>());
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>());
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>());
   objFactory.Register("Laser", std::make_unique<Create::LaserCreator>());
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>());

   std::vector<std::string> spawnTableNames;
   // 各スポーンテーブルを登録
   for (int i = 1; MaxWave >= i; i++) {
      std::string tableName = "bosswave" + std::to_string(i);
      spawnTableNames.emplace_back(tableName);
   }
   objFactory.LoadSpawnTables("boss",  spawnTableNames );
   // 最初のスポーンテーブルを設定する
   objFactory.SetSpawnTable("bosswave1");
   // プレイヤー生成
   auto player = objFactory.Create("Player");
   // オブジェクトサーバーにプレイヤー位置を登録
   auto& objServer = gameInstance.objServer();
   objServer.RegistVector("PlayerPos", player->position());
   // オブジェクトサーバーにプレイヤーを登録
   objServer.Add(std::move(player));

   gameInstance.soundComponent().Stop("PoorBattleBgm");

   ModeInGameBase::Enter();
   // ライティング処理に使用する変数を初期化
   _lightOnCount = 0;
   _red = 0.1f;
   _cntInit = false;
   _lighting->SetDifColor(_red, 0.1f, 0.1f);
   _lighting->SetAmbColor(_red, 0.1f, 0.1f);
}

void ModeBoss::Input(AppFrame::Input::InputManager& input) {
   ModeInGameBase::Input(input);
}

void ModeBoss::Update() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   if (!_cntInit) {
      _lightCnt = modeServer.frameCount();
      _cntInit = true;
      
   }
   LightStaging();
   ModeInGameBase::Update();
   _lighting->Update();
}

void ModeBoss::Render() {
   ModeInGameBase::Render();
#ifdef _DEBUG
   auto& objServer = Game::Game::GetInstance().objServer();
   auto bossHp = objServer.GetDoubleData("LargeEnemyHP");
   DrawFormatString(0, 940, AppFrame::Math::Utility::GetColorCode(255, 255, 255),
      "ボスHP : %f", bossHp);
#endif
}

void ModeBoss::LightStaging() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto& soundComponent = Game::Game::GetInstance().soundComponent();
   auto frame = modeServer.frameCount() - _lightCnt;
   if (_lightOnCount < 4) {
      if (_plus) {
         if (frame == 1) {
            soundComponent.Play("Buzzer");
         }
         _red += 0.02f;
         if (_red > 0.5f) {
            _plus = false;
         }
      }
      if (!_plus) {
         _red -= 0.02f;
         if (_red < 0.1f) {
            _plus = true;
            _lightOnCount++;
         }
      }
      _lighting->SetDifColor(_red, 0.1f, 0.1f);
      _lighting->SetAmbColor(_red, 0.1f, 0.1f);
   }
   else{
      _lighting->SetDifColor(DefaultDifRed, DefaultDifGreen, DefaultDifBlue);
      _lighting->SetAmbColor(DefaultAmbRed, DefaultAmbGreen, DefaultAmbBlue);
   }
   if (frame == PlaySoundFrame) {
      soundComponent.Play("LightOn");
   }
}
