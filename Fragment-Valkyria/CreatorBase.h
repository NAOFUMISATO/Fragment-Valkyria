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
      class Game;
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
         /**
          * \brief �R���X�g���N�^
          */
         CreatorBase() {};
         /**
          * \brief �f�t�H���g�f�X�g���N�^
          */
         virtual ~CreatorBase() = default;
         /**
          * \brief �h����ŃI�u�W�F�N�g�������s��
          * \return �h����Œ�`
          */
         virtual std::unique_ptr<Object::ObjectBase> Create() = 0;
         /**
          * \brief �Q�[���{�̃N���X����Json�ǂݍ��ݗp�N���X�̎Q�Ƃ̎擾
          * \return Json�ǂݍ��ݗp�N���X�̎Q��
          */
         inline AppFrame::Resource::LoadResourceJson& GetLoadJson() const;
      };
   }
}