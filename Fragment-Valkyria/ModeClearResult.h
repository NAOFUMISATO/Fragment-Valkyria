#pragma once
/*****************************************************************//**
 * \file   ModeClearResult.h
 * \brief  ���[�h�N���A��ʃN���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
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
       * \class ���[�h�N���A��ʃN���X
       * \brief �N���A��ʋy�уN���A�^�C���̕\�����s��
       */
      class ModeClearResult : public ModeBase {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q�Ƃ��擾
          */
         ModeClearResult(Game::GameMain& gameMain);
         /**
          * \brief ����������
          */
         void Init()override;
         /**
          * \brief ��������
          */
         void Enter()override;
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief �X�V����
          */
         void Update()override;
         /**
          * \brief �`�揈��
          */
         void Render()override;
         /**
          * \brief �o������
          */
         void Exit()override;

      private:
         bool _born{ true };  //!< �N���A�^�C���N���X�̃C���X�^���X�����t���O
         int _bgHandle{ -1 }; //!< �w�i�摜�n���h��
      };
   }
}
