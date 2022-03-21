
/*****************************************************************//**
 * \file   EffectBossCrash.cpp
 * \brief  ボスの破壊エフェクト
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBossCrash.h"
#include "GameMain.h" 
#include "ObjectServer.h"

namespace {
   auto paramMap = AppFrame::Resource::LoadParamJson::GetParamMap("largeenemy", { 
      "crasheffect_count" ,"crasheffect_distance" ,"crasheffect_frame" });
   const int CrashCount = paramMap["crasheffect_count"];
   const int CrashFrame = paramMap["crasheffect_frame"];
   const double CrashDistance = paramMap["crasheffect_distance"];
}

using namespace FragmentValkyria::Effect;

EffectBossCrash::EffectBossCrash(Game::GameMain& gameMain, std::string_view key) :EffectBase{ gameMain,key } {
   SetEffectLoadHandle(key);
}

void EffectBossCrash::Init() {
   _efcCnt = _gameMain.modeServer().frameCount();
   EffectBase::Update();
}

void EffectBossCrash::Update() {
   auto frame = _gameMain.modeServer().frameCount() - _efcCnt;
   if (frame % CrashFrame == 0) {
      for (int i = 0; i < CrashCount; i++) {
         auto largeEnemyPos = _gameMain.objServer().GetVecData("LargeEnemyPos");
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