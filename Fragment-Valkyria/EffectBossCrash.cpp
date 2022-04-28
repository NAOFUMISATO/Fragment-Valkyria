
/*****************************************************************//**
 * \file   EffectBossCrash.cpp
 * \brief  �{�X�̔j��G�t�F�N�g
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossCrash.h"
#include "Game.h"
#include "ObjectServer.h"

using namespace FragmentValkyria::Effect;

EffectBossCrash::EffectBossCrash(std::string_view key) :EffectBase{key } {
   _param = std::make_unique<Param::ParamLargeEnemy>("largeenemy");
   SetEffectLoadHandle(key);
}

void EffectBossCrash::Init() {
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   _efcCnt = modeServer.frameCount();
   EffectBase::Update();
}

void EffectBossCrash::Update() {
   /**
    * \brief int�^�̒l�𕶎���Ŏw�肵�A�l�Ǘ��N���X����擾����
    * \param paramName �l���w�肷�镶����
    * \return ������ɂ��w�肳�ꂽ�l
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   auto& modeServer = AppFrame::Mode::ModeServer::GetInstance();
   auto frame = modeServer.frameCount() - _efcCnt;
   if (frame % _IntParam("crasheffect_frame") == 0) {
      for (int i = 0; i < _IntParam("crasheffect_count"); i++) {
         auto& gameInstance = Game::Game::GetInstance();
         auto largeEnemyPos = gameInstance.objServer().GetVecData("LargeEnemyPos");
         const auto CrashDistance = _param->GetDoubleParam("crasheffect_distance");
         auto randomX = AppFrame::Math::Utility::GetRandom(-CrashDistance, CrashDistance);
         auto randomY = AppFrame::Math::Utility::GetRandom(0.0, CrashDistance);
         auto randomZ = AppFrame::Math::Utility::GetRandom(-CrashDistance, CrashDistance);
         auto [x, y, z] = largeEnemyPos.GetVec3();
         _position = Vector4(x + randomX, y + randomY, z + randomZ);
         PlayEffect();
         EffectBase::Update();
      }
   }
}