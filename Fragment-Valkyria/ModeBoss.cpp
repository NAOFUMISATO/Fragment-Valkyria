
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
#include "EffectPlayerShot.h" //仮

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

   auto largeEnemy = objFactory.Create("LargeEnemy");
   objServer.Add(std::move(largeEnemy));

   ModeInGameBase::Enter();
}

void ModeBoss::Input(AppFrame::Input::InputManager& input) {
   //-----------仮--------------
   if (input.GetMouse().RightClick()) {
      GetModeServer().GoToMode("Title", 'L');
   }
   if (input.GetMouse().LeftClick()) {
      GetModeServer().GoToMode("ClearResult", 'S');
   }
   if (input.GetKeyboard().ZClick()) {
      auto efcShot = std::make_unique<Effect::EffectPlayerShot>(_gameMain,"Shot");
      GetEfcServer().Add(std::move(efcShot));
   }
   //-----------仮--------------
   ModeInGameBase::Input(input);
}

void ModeBoss::Update() {
   ModeInGameBase::Update();
}

void ModeBoss::Render() {
   ModeInGameBase::Render();
#ifdef _DEBUG
   auto bossHp= GetObjServer().GetDoubleData("BossHP");
   DrawFormatString(0, 940, AppFrame::Math::Utility::GetColorCode(255, 255, 255),"ボスHP : %f", bossHp);
#endif
}
