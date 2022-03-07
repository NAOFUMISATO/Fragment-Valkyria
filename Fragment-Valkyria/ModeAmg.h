#pragma once
#include "ModeBase.h"

namespace FragmentValkyria {
   namespace Mode {
      class ModeAmg :public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̂̎Q��
          */
         ModeAmg(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ��������
          */
         void Enter()override;
         /**
          * \brief �X�V����
          */
         void Update() override;
         /**
          * \brief �`�揈��
          */
         void Render() override;

      private:
         int _grHandle{ -1 };
         int _fadeCnt{ -1 };
         bool _cntInit{ false }; 
      };
   }
}
