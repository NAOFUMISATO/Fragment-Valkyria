#pragma once
/*****************************************************************//**
 * \file   MouseInput.h
 * \brief  �}�E�X�̓��͊Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   November 2021
 *********************************************************************/
#include <DxLib.h>
#include "Vector4.h"
 /**
  * \brief �A�v���P�[�V�����t���[��
  */
namespace AppFrame {
   /**
    * \brief ���͊֌W
    */
   namespace Input {
      /**
       * \class �}�E�X�̓��͊Ǘ��N���X
       * \brief DxLib::GetMouseInput,GetMousePoint�œ��͂��󂯎��A�Ǘ�����
       */
      class MouseInput {
      public:
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief ���{�^���N���b�N�����Ԃ�
          * \return �����ꂽ�Ȃ�TRUE��Ԃ�
          */
         bool LeftClick() { return _trg & MOUSE_INPUT_LEFT; }
         /**
          * \brief �E�{�^���N���b�N�����Ԃ�
          * \return �����ꂽ�Ȃ�TRUE��Ԃ�
          */
         bool RightClick() { return _trg & MOUSE_INPUT_RIGHT; }
         /**
          * \brief ���{�^�����������Ԃ�
          * \return ������Ă���Ȃ�TRUE��Ԃ�
          */
         bool LeftPress() { return _key & MOUSE_INPUT_LEFT; }
         /**
          * \brief �E�{�^�����������Ԃ�
          * \return ������Ă���Ȃ�TRUE��Ԃ�
          */
         bool RightPress() { return _key & MOUSE_INPUT_RIGHT; }
         /**
          * \brief �}�E�X�|�C���^�[�̍��W�̎擾
          * \return �}�E�X�|�C���^�[�̍��W
          */
         Math::Vector4 GetPosition() { return _position; }
         /**
          * \brief ���W�̕ω��ʂ̎擾
          * \return ���W�̕ω���
          */
         Math::Vector4 GetDelta() { return _delta; }

      private:
         int _key{ 0 };   //!< ��������
         int _trg{ 0 };   //!< �g���K����
         Math::Vector4 _position{ 0,0,0 };  //!< �}�E�X�|�C���^�[���W
         Math::Vector4 _delta{ 0,0,0 };     //!< �}�E�X�|�C���^�[���W�̕ω���
      };
   }
}