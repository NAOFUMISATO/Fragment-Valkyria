
/*****************************************************************//**
 * \file   LaserCreator.cpp
 * \brief  レーザー生成
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "LaserCreator.h"
#include <algorithm>
#include "Laser.h"
#include "Game.h"
#include "ModelAnimeComponent.h"
#include "ObjectServer.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

std::unique_ptr<Object::ObjectBase> LaserCreator::Create() {
   // レーザーの生成
   auto laser = std::make_unique<Enemy::Laser>();
   auto& objServer = Game::Game::GetObjServer();
   auto& runObjects = objServer.runObjects();
   for (auto&& object : runObjects) {
      if (object->GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
         continue;
      }
      auto handle = object->modelAnimeComponent().modelHandle();
      auto laserFrame = object->modelAnimeComponent().FindFrame("pasted__laser_collision");
      auto laserPos = MV1GetFramePosition(handle, laserFrame);
      auto startPos = laserPos;

      laser->position(AppFrame::Math::ToMath(startPos));

      break;
   }
   auto laegeEnemyPos = objServer.GetVecData("LargeEnemyPos");
   auto laserDirection = objServer.GetVecData("LaserDirectionPos") - laegeEnemyPos;
   laserDirection.Normalized();
   auto endPos = laegeEnemyPos + laserDirection * 10000 + Vector4(0.0, laser->position().GetY(), 0.0);
   laser->end(endPos);
   
   auto state = std::make_unique<AppFrame::State::StateServer>("Irradiation",
      std::make_shared<Enemy::Laser::StateIrradiation>(*laser));
   laser->stateServer(std::move(state));

   return std::move(laser);
}
