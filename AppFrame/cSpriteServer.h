#pragma once
/*****************************************************************//**
 * \file   SpriteServer.h
 * \brief  �X�v���C�g���ꊇ�Ǘ�����N���X
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBaseRoot.h"
#include "Vector4.h"
#include <vector>
#include <unordered_map>
/**
 * \brief �A�v���P�[�V�����t���[��
 */
namespace AppFrame {
   /**
    * \brief �X�v���C�g�֌W
    */
   namespace Sprite {
      /**
       * \class �X�v���C�g���ꊇ�Ǘ�����N���X
       * \brief �X�v���C�g��o�^���A�ꊇ�Ǘ�����
       */
      class SpriteServer {
         using Vector4 = Math::Vector4;
      public:
         /**
          * \brief �X�v���C�g�̓o�^
          * \param sprite �o�^����X�v���C�g
          */
         void Add(std::unique_ptr<SpriteBaseRoot> sprite);
         /**
          * \brief ���͏���
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief �X�V����
          */
         void Update();
         /**
          * \brief �`�揈��
          */
         void Render();
         /**
          * \brief �X�v���C�g�̉��
          */
         void Clear();
         /**
          * \brief �X�v���C�g��o�^���Ă��铮�I�z��̎擾
          */
         std::vector<std::unique_ptr<SpriteBaseRoot>>& runSprites() { return _runSprites; };


      private:
         bool _updating{ false };                                      //!< �X�V���Ă��邩�̃t���O
         std::vector<std::unique_ptr<SpriteBaseRoot>> _runSprites;     //!< �X�v���C�g��o�^���铮�I�z��
         std::vector<std::unique_ptr<SpriteBaseRoot>> _pendingSprites; //!< �X�v���C�g�̓o�^��ۗ����铮�I�z��
      };
   }
}
