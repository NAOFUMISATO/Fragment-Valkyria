
#include "ModePoor.h"
#include <stdexcept>
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
#include "ModelAnimeComponent.h"
#include "EffectPlayerShot.h" //仮

using namespace FragmentValkyria::Mode;

ModePoor::ModePoor(Game::GameMain& gameMain) : ModeInGameBase{ gameMain } {
}

void ModePoor::Init() {
   auto& loadJson = GetLoadJson();
   loadJson.LoadModels("ingame");
   loadJson.LoadEffects("ingame");
   loadJson.LoadTextures("ingame");
   loadJson.LoadSounds("ingame");
   _gameMain.loadStage().LoadStageModels("Stage");
}

void ModePoor::Enter() {
   using Vector4 = AppFrame::Math::Vector4;

   auto& objFactory = GetObjFactory();
   objFactory.Register("Player", std::make_unique<Create::PlayerCreator>(_gameMain));
   objFactory.Register("Gatling", std::make_unique<Create::GatlingCreator>(_gameMain));
   objFactory.Register("Bullet", std::make_unique<Create::BulletCreator>(_gameMain));
   objFactory.Register("PoorEnemyGatling", std::make_unique<Create::PoorEnemyGatlingCreator>(_gameMain));

   objFactory.LoadSpawnTables("poor", {"poorwave1","poorwave2" ,"poorwave3" ,"poorwave4" });

   objFactory.SetSpawnTable("poorwave1");

   auto player = objFactory.Create("Player");
   // アクターサーバーに登録※個別アクセス用
   auto& objServer = GetObjServer();
   objServer.RegistVector("PlayerPos", player->position());
   objServer.Add(std::move(player));

   _gameMain.ingameTimer(0);
   _wave = 1;
   ModeInGameBase::Enter();
}

void ModePoor::Input(AppFrame::Input::InputManager& input) {
   //-----------仮--------------
   if (input.GetXJoypad().BackClick()) {
      GetModeServer().GoToMode("Movie",'L');
   }
   if (input.GetMouse().LeftClick()) {
      GetModeServer().GoToMode("ClearResult", 'S');
   }
   if (input.GetKeyboard().ZClick()) {
      auto efcShot = std::make_unique<Effect::EffectPlayerShot>(_gameMain, "Shot");
      GetEfcServer().Add(std::move(efcShot));
   }
   //-----------仮--------------
   ModeInGameBase::Input(input);
}

void ModePoor::Update() {
   ModeInGameBase::Update();
   auto&& runObjects = GetObjServer().runObjects();

   auto isActiveEnemy = std::any_of(runObjects.begin(), runObjects.end(),
      [](std::unique_ptr<AppFrame::Object::ObjectBaseRoot>& obj) {
#ifndef _DEBUG
         auto& objectBase = dynamic_cast<FragmentValkyria::Object::ObjectBase&>(*obj);
#else
         try {
            auto& objectBase = dynamic_cast<Object::ObjectBase&>(*obj);
         }
         catch (std::bad_cast&) {
            OutputDebugString("----------ダウンキャスト失敗----------");
         }
         auto& objectBase = dynamic_cast<Object::ObjectBase&>(*obj);
#endif
         return (objectBase.GetObjType() == Object::ObjectBase::ObjectType::PoorEnemyGatling) && objectBase.IsActive(); });
   if (!isActiveEnemy) {
      switch (_wave){
      case 1:
         GetObjFactory().SetSpawnTable("poorwave2");
         _wave++;
         break;
      case 2:
         GetObjFactory().SetSpawnTable("poorwave3");
         _wave++;
         break;
      case 3:
         GetObjFactory().SetSpawnTable("poorwave4");
         _wave++;
         break;
      case 4:
         GetModeServer().GoToMode("Movie");
         break;
      }
   }
}

void ModePoor::Render() {
   ModeInGameBase::Render();
}