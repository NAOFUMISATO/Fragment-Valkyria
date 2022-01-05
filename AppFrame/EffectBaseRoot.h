#pragma once
#include "Vector4.h"
#include <string_view>
#include <EffekseerForDXLib.h>
#include <tuple>
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
         inline bool IsDead() { return (_efcState == EffectState::Dead); }
         inline bool IsActive() { return (_efcState == EffectState::Active); }
         void SetDead() { _efcState = EffectState::Dead; }
         void SetEffectLoadHandle(std::string_view key);
         void PlayEffect();
         inline Math::Vector4 position() { return _position; }
         void SetPosition(Math::Vector4 pos);
         inline Math::Vector4 rotation() { return _rotation; }
         void SetRotation(Math::Vector4 rot);
         inline Math::Vector4 scale() { return _scale; }
         void SetScale(Math::Vector4 sca);
         double GetSpeed();
         void SetSpeed(double speed);
         std::tuple<int, int, int, int> GetEfcColor() { return _color; }
         void SetEfcColor(std::tuple<int, int, int, int> color);

      protected:
         Game::GameBase& _gameBase;
         EffectState _efcState{ EffectState::Active };
         int _loadHandle{ -1 };
         int _playHandle{ -1 };
         Math::Vector4 _position{ 0,0,0 };
         Math::Vector4 _rotation{ 0,0,0 };
         Math::Vector4 _scale{ 0,0,0 };
         std::tuple<int, int, int, int> _color{255,255,255,255};
      };
   }
} 