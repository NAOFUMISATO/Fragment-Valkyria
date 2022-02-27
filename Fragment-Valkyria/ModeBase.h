#pragma once
/*****************************************************************//**
 * \file   ModeBase.h
 * \brief  �e���[�h�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   // ��d�C���N���[�h�h�~
   namespace Game {
      class GameMain;
   }
   namespace Object {
      class ObjectServer;
   }
   namespace Sprite {
      class SpriteServer;
   }
   /**
    * \brief ���[�h�֌W
    */
   namespace Mode {
      /**
       * \class �e���[�h�̊��N���X
       * \brief �e���[�h�͂��̃N���X��h�����Ē�`����
       */
      class ModeBase : public AppFrame::Mode::ModeBaseRoot {
      public:
         /**
          * \brief �R���X�g���N�^
          * \param gameMain �Q�[���{�̃N���X�̎Q��
          */
         ModeBase(Game::GameMain& gameMain);
         /**
          * \brief �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return �I�u�W�F�N�g�ꊇ�Ǘ��N���X�̎Q��
          */
         Object::ObjectServer& GetObjServer() const;
         /**
          * \brief �X�v���C�g�ꊇ�Ǘ��N���X�̎Q�Ƃ��Q�[���x�[�X�o�R�Ŏ擾
          * \return �X�v���C�g�ꊇ�Ǘ��N���X�̎Q��
          */
         Sprite::SpriteServer& GetSprServer() const;

      protected:
         Game::GameMain& _gameMain; //!< �Q�[���N���X�̎Q��
      };
   }
}
