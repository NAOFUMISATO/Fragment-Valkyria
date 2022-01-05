#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Effect {
      class EffectBase :public AppFrame::Effect::EffectBaseRoot {
      public:
         enum class EffectType {
            Effect=0,
            PlayerAttack
         };
         EffectBase(Game::GameMain& gameMain);
         virtual ~EffectBase() override ;

         void Init() override {};
         void Input(AppFrame::Input::InputManager& input) override {};
         void Update() override {};
         void Draw() override {};
         virtual EffectType GetEfcType() const = 0;

      protected:
         Game::GameMain& _gameMain;
      };
}
}
