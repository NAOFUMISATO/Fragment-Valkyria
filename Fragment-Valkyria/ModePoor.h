#pragma once
#include "ModeInGameBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModePoor : public ModeInGameBase {
      public:
         ModePoor(Game::GameMain& gameMain);
         void Init()override;
         void Enter()override;
         void Input(AppFrame::Input::InputManager& input)override;
         void Update()override;
         void Render()override;

         /**
          * \brief �C���Q�[����ʂ̎擾
          * \return ���[�h�G����
          */
         virtual InGameType GetInGameType()const { return InGameType::Poor; }

      private:
         int _wave{ 0 };
      };
   }
}
