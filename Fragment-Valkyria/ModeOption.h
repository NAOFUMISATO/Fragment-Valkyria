#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {

   namespace Mode {
      class ModeOption :public ModeBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         ModeOption(Game::GameMain& gameMain);

         void Init()override;
         void Enter()override;
         void Input(AppFrame::Input::InputManager& input)override;
         void Update()override;
         void Render()override;

      private:
         std::unique_ptr<AppFrame::State::StateServer> _stateServer;
         double _cameraSens{ 0 };
         double _aimSens{ 0 };
         int _deadZone{ 0 };
         std::pair<int, int> _cameraCusorPos{ 0,0 };
         std::pair<int, int> _aimCusorPos{ 0,0 };
         std::pair<int, int> _deadzoneCusorPos{ 0,0 };
         std::pair<int, int> _selectCusorPos{ 0,0 };
         std::tuple<int, int, int, int, int, int, int> _grHandles{
            -1,-1,-1,-1,-1,-1,-1
         };

      public:
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:

            StateBase(ModeOption& owner) : _owner{ owner } {};

            void Update()override;

            void Draw() override;

         protected:
            ModeOption& _owner;
         };

         class StateCameraSencivitySelect : public StateBase{
         public:

            StateCameraSencivitySelect(ModeOption& owner) :StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;
         };

         class StateAimSencivitySelect : public StateBase{
         public:

            StateAimSencivitySelect(ModeOption& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

         };

         class StateDeadZoneSelect : public StateBase{
         public:

            StateDeadZoneSelect(ModeOption& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

         };

         class StateReturnSelect : public StateBase {
         public:

            StateReturnSelect(ModeOption& owner) : StateBase{ owner } {};

            void Enter() override;

            void Input(InputManager& input) override;

         };
      };
   }
}