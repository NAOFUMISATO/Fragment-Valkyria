
/*****************************************************************//**
 * \file   ModeBoss.cpp
 * \brief  モードインゲーム
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeBoss.h"
#include "FallObjectCreator.h"
#include "ObjectServer.h"
#include "ObjectFactory.h"
#include "GameMain.h"
#include "LargeEnemyCreator.h"
#include "LargeEnemy.h"
#include "PlayerCreator.h"
#include "Player.h"
#include "BossStageCreator.h"
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
   loadJson.LoadEffects("player");
   loadJson.LoadTextures("ingame");
   auto& loadStage = _gameMain.loadStage();
   loadStage.LoadStageModels("Boss");
   ModeInGameBase::Init();
}

void ModeBoss::Enter() {

   auto& objFactory = GetObjFactory();
   objFactory.Register("LargeEnemy", std::make_unique<Create::LargeEnemyCreator>(_gameMain));
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>(_gameMain));
   objFactory.Register("Stage", std::make_unique<Create::BossStageCreator>(_gameMain));
   objFactory.Register("FallObject", std::make_unique<Create::FallObjectCreator>(_gameMain));
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>(_gameMain));

   auto player = objFactory.Create("Player");
   // アクターサーバーに登録※個別アクセス用
   auto& objServer = GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));

   auto stage = objFactory.Create("Stage");
   objServer.Add(std::move(stage));

   auto largeEnemy = objFactory.Create("LargeEnemy");
   objServer.Add(std::move(largeEnemy));

   ModeInGameBase::Enter();
}

void ModeBoss::Input(AppFrame::Input::InputManager& input) {
   if (input.GetMouse().RightClick()) {
      // 右クリックでタイトルへ遷移
      GetModeServer().GoToMode("Title", 'L');
   }
   //エフェクト仮描画
   if (input.GetKeyboard().ZClick()) {
      auto efcShot = std::make_unique<Effect::EffectPlayerShot>(_gameMain,"Shot");
      GetEfcServer().Add(std::move(efcShot));
   }
#ifdef _DEBUG
   _padLeftX = input.GetXJoypad().LeftStickX();
   _padLeftY = input.GetXJoypad().LeftStickY();
   _padRightX = input.GetXJoypad().RightStickX();
   _padRightY = input.GetXJoypad().RightStickY();
#endif
   ModeInGameBase::Input(input);
}

void ModeBoss::Update() {
#ifdef _DEBUG
   for (auto&& object : GetObjServer().runObjects()) {
       auto& objectBase = dynamic_cast<Object::ObjectBase&>(*object);
       if (objectBase.GetObjType() == Object::ObjectBase::ObjectType::Player) {
           auto& player = dynamic_cast<Player::Player&>(objectBase);
           _playerHp = player.hp();
           continue;
       }
       if (objectBase.GetObjType() == Object::ObjectBase::ObjectType::LargeEnemy) {
           auto& largeEnemy = dynamic_cast<Enemy::LargeEnemy&>(objectBase);

           _largeEnemyHp = largeEnemy.hp();
           continue;
       }
   }
#endif
   ModeInGameBase::Update();
}

void ModeBoss::Render() {
   ModeInGameBase::Render();
}
