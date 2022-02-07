
/*****************************************************************//**
 * \file   LaserCreator.cpp
 * \brief  レーザー生成クラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "LaserCreator.h"
#include <algorithm>
#include "Laser.h"
#include "GameMain.h"
#include "ModelAnimeComponent.h"

using namespace FragmentValkyria;
using namespace FragmentValkyria::Create;

LaserCreator::LaserCreator(Game::GameMain& gameMain) : CreatorBase{ gameMain } {

}

std::unique_ptr<Object::ObjectBase> LaserCreator::Create()
{
	auto laser = std::make_unique<Enemy::Laser>(_gameMain);
	for (auto&& objects : _gameMain.objServer().runObjects()) {
		auto& objectBase = dynamic_cast<Object::ObjectBase&>(*objects);

		if (objectBase.GetObjType() != Object::ObjectBase::ObjectType::LargeEnemy) {
			continue;
		}
		auto handle = objectBase.modelAnimeComponent().modelHandle();
		auto laserFrame = objectBase.modelAnimeComponent().FindFrameChild("root", "gatling3");
		auto laserPos = MV1GetFramePosition(handle, laserFrame);
		auto startPos = laserPos;

		laser->position(AppFrame::Math::ToMath(startPos));

		break;
	}

	auto state = std::make_unique<AppFrame::State::StateServer>("Idle", std::make_shared<Enemy::Laser::StateIdle>(*laser));
	laser->stateServer(std::move(state));

	auto [laserX, laserY, laserZ] = laser->position().GetVec3();

	auto laserDistance = _gameMain.objServer().GetVecData("LaserDirectionPos") - Vector4(laserX, 0.0, laserZ);
	laserDistance.Normalized();
	auto endPos = laser->position() + laserDistance * 10000 + Vector4(0.0, laserY, 0.0);
	laser->end(endPos);

	return std::move(laser);
}
