#pragma once
/*****************************************************************//**
 * \file   LaserCreator.h
 * \brief  ���[�U�[�����Ǘ��N���X
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/

#include "CreatorBase.h"

namespace FragmentValkyria {

   namespace Create {

      class LaserCreator : public CreatorBase
      {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         LaserCreator(Game::GameMain& gameMain);
      private:
         /**
         * \brief ���[�U�[s�̈ʒu�A�A�j���[�V�����A��Ԃ̓o�^���s��
         * \param game �Q�[���{�̃N���X�̎Q��
         * \return ���[�U�[�̃C���X�^���X
         */
         virtual std::unique_ptr<Object::ObjectBase> Create() override;

      };
   }
}
