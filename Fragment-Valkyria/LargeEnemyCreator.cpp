
/*****************************************************************//**
 * \file   LargeEnemyCreator.cpp
 * \brief  ラージエネミー生成
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "LargeEnemyCreator.h"
#include "Game.h"
#include "LargeEnemy.h"
#include "LargeEnemyHP.h"
#include "ModelAnimeComponent.h"
#include "ObjectBase.h"
#include "ObjectServer.h"
#include "SpriteServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> LargeEnemyCreator::Create() {
   using Vector4 = AppFrame::Math::Vector4;
   // ラージエネミーの生成
   auto largeEnemy = std::make_unique<Enemy::LargeEnemy>();
   // ラージエネミーのアニメーション一括管理クラスの生成
   auto model = std::make_unique<Model::ModelAnimeComponent>(*largeEnemy);
   // モデルの設定
   model->SetModel("LargeEnemy");
   // ラージエネミーのアニメーション一括管理クラスの設定
   largeEnemy->modelAnimeComponent(std::move(model));

   // ラージエネミーの状態一括管理クラスの生成
   auto state = std::make_unique<AppFrame::State::StateServer>("Fall",
      std::make_shared<Enemy::LargeEnemy::StateFall>(*largeEnemy));
   // ラージエネミーの状態の追加登録
   state->Register("Idle", std::make_shared<Enemy::LargeEnemy::StateIdle>(*largeEnemy));
   state->Register("FallObject", std::make_shared<Enemy::LargeEnemy::StateFallObject>(*largeEnemy));
   state->Register("Consecutive", std::make_shared<Enemy::LargeEnemy::StateConsecutiveFallObject>(*largeEnemy));
   state->Register("Gatling", std::make_shared<Enemy::LargeEnemy::StateGatling>(*largeEnemy));
   state->Register("Move", std::make_shared<Enemy::LargeEnemy::StateMove>(*largeEnemy));
   state->Register("Die", std::make_shared<Enemy::LargeEnemy::StateDie>(*largeEnemy));
   state->Register("Laser", std::make_shared<Enemy::LargeEnemy::StateLaser>(*largeEnemy));
   state->Register("FanGatling", std::make_shared<Enemy::LargeEnemy::StateFanGatling>(*largeEnemy));
   state->Register("Stun", std::make_shared<Enemy::LargeEnemy::StateStun>(*largeEnemy));
   // ラージエネミーの状態一括管理クラスの設定
   largeEnemy->stateServer(std::move(state));

   // オブジェクトサーバーの各オブジェクトを取得
   auto& runObjects = Game::Game::GetObjServer().runObjects();
   for (auto&& object : runObjects) {
      // プレイヤーでなければ処理をスキップして戻る
      if (object->GetObjType() != Object::ObjectBase::ObjectType::Player) {
         continue;
      }
      // ラージエネミーのカメラ管理クラスをプレイヤーのカメラ管理クラスに設定
      largeEnemy->cameraComponent(object->cameraComponent());
   }
   auto& sprServer = Game::Game::GetSprServer();
   sprServer.Add(std::make_unique<Enemy::LargeEnemyHP>());

   // ラージエネミーのインスタンスを返す
   return std::move(largeEnemy);
}
