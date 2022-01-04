#pragma once
#include "Vector4.h"
namespace AppFrame{
   namespace Game {
      class GameBase;
   }
   namespace Input {
      class InputManager;
   }
   namespace Effect {
      class EffectBaseRoot {
      public:
         enum class EffectState {
            Active,
            Paused,
            Dead
         };
         EffectBaseRoot(Game::GameBase& gameBase);
         virtual ~EffectBaseRoot();

         virtual void Init() {};
         virtual void Input(Input::InputManager& input) {};
         virtual void Update() {};
         virtual void Draw() {};
         bool IsDead() { return (_efcState == EffectState::Dead); }
         bool IsActive() { return (_efcState == EffectState::Active); }
         void SetDead() { _efcState = EffectState::Dead; }

      protected:
         Game::GameBase& _gameBase;
         EffectState _efcState{ EffectState::Active };
         int _efcHandle{ -1 };
         Math::Vector4 _position{ 0,0,0 };
         Math::Vector4 _rotation{ 0,0,0 };


      };
}
}
