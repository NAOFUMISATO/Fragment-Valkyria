#pragma once
/*****************************************************************//**
 * \file   ModeInGameBase.h
 * \brief  �e�C���Q�[���̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   namespace Create {
      class ObjectFactory;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class ���[�h�C���Q�[���N���X
       * \brief �C���Q�[���̏�������
       */
      class ModeInGameBase : public ModeBase {
      public:
         /**
          * \brief �C���Q�[����ʂ̎擾
          */
         enum class InGameType {
            Boss   //!< ���[�h�{�X
         };
         /**
          * \brief �R���X�g���N�^
          * \param game �Q�[���{�̂̎Q��
          */
         ModeInGameBase(Game::GameMain& gameMain);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         ~ModeInGameBase() = default;
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
         /**
          * \brief �o������
          */
         void Exit() override;

         /**
          * \brief �C���Q�[����ʂ̎擾
          * \return �h����Œ�`
          */
         virtual InGameType GetInGameType() const = 0;
         /**
          * \brief �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[�����C���o�R�Ŏ擾
          * \return �I�u�W�F�N�g�����ꊇ�Ǘ��N���X�̎Q��
          */
         Create::ObjectFactory& GetObjFactory() const;
      };
   }
}