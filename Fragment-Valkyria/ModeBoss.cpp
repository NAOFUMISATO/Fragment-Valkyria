
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

void ModeBoss::Init() {
   auto& loadJson = GetLoadJson();
   loadJson.LoadModels("ingame");
   loadJson.LoadEffects("ingame");
   loadJson.LoadTextures("ingame");
   loadJson.LoadSounds("ingame");
   auto& loadStage = _gameMain.loadStage();
   loadStage.LoadStageModels("Stage");
   ModeInGameBase::Init();
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

   Create::SpawnTable spawnTable {
       std::make_tuple(0      , "PoorEnemyGatling", Vector4(500.0, 500.0, 500.0), Vector4(0.0, 0.0, 0.0)),
       std::make_tuple(0      , "PoorEnemyGatling", Vector4(1.0, 500.0, 0.0), Vector4(0.0, 0.0, 0.0)),
       std::make_tuple(0,       "PoorEnemyGatling", Vector4(-500.0, 500.0, 500.0), Vector4(0.0, 0.0, 0.0)),

       std::make_tuple(60 * 10, "PoorEnemyGatling", Vector4(500.0, 500.0, -500.0), Vector4(0.0, 0.0, 0.0)),
       std::make_tuple(0,       "PoorEnemyGatling", Vector4(0.0, 500.0, -500.0), Vector4(0.0, 0.0, 0.0)),
       std::make_tuple(0,       "PoorEnemyGatling", Vector4(-500.0, 500.0, -500.0), Vector4(0.0, 0.0, 0.0)),
   };

   objFactory.SetSpawnTable(spawnTable);

   auto player = objFactory.Create("Player");
   // アクターサーバーに登録※個別アクセス用
   auto& objServer = GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));

   auto largeEnemy = objFactory.Create("LargeEnemy");
   objServer.Add(std::move(largeEnemy));

   _gameMain.ingameTimer(0);

   ModeInGameBase::Enter();
}

void ModeBoss::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().RightClick()) {
      // 右クリックでタイトルへ遷移
      GetModeServer().GoToMode("Title", 'L');
   }
   //-----------仮--------------
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
