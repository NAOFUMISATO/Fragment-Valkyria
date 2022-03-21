
/*****************************************************************//**
 * \file   LaserCreator.cpp
 * \brief  レーザー生成
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "LaserCreator.h"
#include <algorithm>
#include "Laser.h"
#include "GameMain.h"
#include "ModelAnimeComponent.h"
#include "ObjectServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

LaserCreator::LaserCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> LaserCreator::Create() {
   // レーザーの生成
   auto laser = std::make_unique<Enemy::Laser>(_gameMain);
   for (auto&& objects : _gameMain.objServer().runObjects()) {
      
      if (objects->GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
         continue;
      }
      auto handle = objects->modelAnimeComponent().modelHandle();
      auto laserFrame = objects->modelAnimeComponent().FindFrame("pasted__laser_collision");
      auto laserPos = MV1GetFramePosition(handle, laserFrame);
      auto startPos = laserPos;

      laser->position(AppFrame::Math::ToMath(startPos));

      break;
   }
   auto laegeEnemyPos = _gameMain.objServer().GetVecData("LargeEnemyPos");

   auto laserDirection = _gameMain.objServer().GetVecData("LaserDirectionPos") - laegeEnemyPos/*Vector4(laserX, 0.0, laserZ)*/;
   laserDirection.Normalized();
   auto endPos = laegeEnemyPos + laserDirection * 10000 + Vector4(0.0, laser->position().GetY(), 0.0);
   laser->end(endPos);
   
   auto state = std::make_unique<AppFrame::State::StateServer>("Irradiation", std::make_shared<Enemy::Laser::StateIrradiation>(*laser));
   laser->stateServer(std::move(state));

   return std::move(laser);
}
