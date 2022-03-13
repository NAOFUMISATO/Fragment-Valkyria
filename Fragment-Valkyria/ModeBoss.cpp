
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

   objFactory.LoadSpawnTables("boss", { "bosswave1","bosswave2" ,"bosswave3" ,"bosswave4" });

   objFactory.SetSpawnTable("bosswave1");

   auto player = objFactory.Create("Player");
   // アクターサーバーに登録※個別アクセス用
   auto& objServer = GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));
   GetSoundComponent().Stop("PoorBattleBgm");
   ModeInGameBase::Enter();
   _red = 0.1f;
   _flag = false;
   _lightOnCount = 0;
}

void ModeBoss::Input(AppFrame::Input::InputManager& input) {
   //-----------仮--------------
   if (input.GetMouse().LeftClick()) {
      GetModeServer().GoToMode("ClearResult", 'S');
   }
   //-----------仮--------------
   ModeInGameBase::Input(input);
}

void ModeBoss::Update() {
   if (!_flag) {
      _lightCnt = GetModeServer().frameCount();
      _flag = true;
      
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
      _lighting->SetDifColor(0.6f, 0.6f, 0.7f);
      _lighting->SetAmbColor(0.5f, 0.5f, 0.5f);
   }
   if (frame == 160) {
      GetSoundComponent().Play("LightOn");
   }
}
