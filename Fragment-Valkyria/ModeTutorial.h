#pragma once
#include "ModeInGameBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModeTutorial :public ModeInGameBase {
         using InputManager = AppFrame::Input::InputManager;
      public:
         ModeTutorial(Game::GameMain& gameMain);
         /**
         * \brief ����������
         */
         void Init() override;
         /**
          * \brief ��������
          */
         void Enter() override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Render() override;

      private:
         bool TipsAlive();
         void ClearJudge(std::string_view key);
         void TipsBorn(std::string_view key);
         void FallObjectRespawn();
         int _tutorialProgress{ 1 };
         bool _born{ true };
         bool _isAppear{ false };
      };
   }
}
