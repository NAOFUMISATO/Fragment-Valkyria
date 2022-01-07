#pragma once
/*****************************************************************//**
 * \file   CreatorBase.h
 * \brief  �I�u�W�F�N�g�����Ǘ��̊��N���X
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <memory>
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
      class ObjectBase;
   }
   /**
    * \brief �I�u�W�F�N�g�����֌W
    */
   namespace Create {
      /**
       * \class �I�u�W�F�N�g�����Ǘ��̊��N���X
       * \brief �e�����Ǘ��N���X�͂��̃N���X���p�����Ē�`����
       */
      class CreatorBase {
      public:
          CreatorBase(Game::GameMain& gameMain);
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~CreatorBase() = default;
         /**
          * \brief �h����ŃI�u�W�F�N�g�������s��
          * \param game �Q�[���{�̃N���X�̎Q��
          * \return �h����Œ�`
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() = 0;

         inline AppFrame::Resource::LoadJson& GetLoadJson() const;
      
      protected:
          Game::GameMain& _gameMain;
      
      };
   }
}