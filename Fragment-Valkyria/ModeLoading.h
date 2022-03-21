#pragma once
/*****************************************************************//**
 * \file   ModeLoading.h
 * \brief  ���[�h�ǂݍ��ݒ��N���X
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�ǂݍ��ݒ��N���X
       * \brief �ǂݍ��ݒ���ʂ�\��������
       */
      class ModeLoading :public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ModeLoading(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Render()override;

      private:
         int _grHandle{ -1 };                //!< �摜�n���h��
         std::vector<int> _grHandles{ 0 };   //!< �摜�n���h�����i�[�������I�z��
      };
   }
}
