
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
#include "GameMain.h"
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

ModeBoss::ModeBoss(Game::GameMain& gameMain) : ModeInGameBase{ gameMain } {
}

void ModeBoss::Enter() {
    using Vector4 = AppFrame::Math::Vector4;

   auto& objFactory = GetObjFactory();
   objFactory.Register("LargeEnemy", std::make_unique<Create::LargeEnemyCreator>(_gameMain));
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>(_gameMain));
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>(_gameMain));
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>(_gameMain));
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>(_gameMain));
   objFactory.Register("Laser", std::make_unique<Create::LaserCreator>(_gameMain));
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>(_gameMain));

   std::vector<std::string> spawnTableNames;
   for (int i = 1; MaxWave >= i; i++) {
      std::string tableName = "bosswave" + std::to_string(i);
      spawnTableNames.emplace_back(tableName);
   }
   objFactory.LoadSpawnTables("boss",  spawnTableNames );

   objFactory.SetSpawnTable("bosswave1");

   auto player = objFactory.Create("Player");
   // アクターサーバーに登録※個別アクセス用
   auto& objServer = GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));
   GetSoundComponent().Stop("PoorBattleBgm");
   ModeInGameBase::Enter();
   _red = 0.1f;
   _lighting->SetDifColor(_red, 0.1f, 0.1f);
   _lighting->SetAmbColor(_red, 0.1f, 0.1f);
   _cntInit = false;
   _lightOnCount = 0;
}

void ModeBoss::Input(AppFrame::Input::InputManager& input) {
   ModeInGameBase::Input(input);
}

void ModeBoss::Update() {
   if (!_cntInit) {
      _lightCnt = GetModeServer().frameCount();
      _cntInit = true;
      
   }
   LightStaging();
   ModeInGameBase::Update();
   _lighting->Update();
}

void ModeBoss::Render() {
   ModeInGameBase::Render();
#ifdef _DEBUG
   auto bossHp = GetObjServer().GetDoubleData("BossHP");
   DrawFormatString(0, 940, AppFrame::Math::Utility::GetColorCode(255, 255, 255),"ボスHP : %f", bossHp);
#endif
}

void ModeBoss::LightStaging() {
   auto frame = GetModeServer().frameCount() - _lightCnt;
   if (_lightOnCount < 4) {
      if (_plus) {
         if (frame == 1) {
            GetSoundComponent().Play("Buzzer");
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
      GetSoundComponent().Play("LightOn");
   }
}
