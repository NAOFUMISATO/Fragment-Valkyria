#pragma once
/*****************************************************************//**
 * \file   SpriteServer.h
 * \brief  �X�v���C�g���ꊇ�Ǘ�����N���X
 *
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "SpriteBase.h"
#include "Vector4.h"
#include <vector>
#include <unordered_map>
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief �X�v���C�g�֌W
    */
   namespace Sprite {
      /**
       * \class �X�v���C�g���ꊇ�Ǘ�����N���X
       * \brief �X�v���C�g��o�^���A�ꊇ�Ǘ�����
       */
      class SpriteServer {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief �X�v���C�g�̓o�^
          * \param sprite �o�^����X�v���C�g
          */
         void Add(std::unique_ptr<SpriteBase> sprite);
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
         std::vector<std::unique_ptr<SpriteBase>>& runSprites() { return _runSprites; };

      private:
         bool _updating{ false };                                      //!< �X�V���Ă��邩�̃t���O
         std::vector<std::unique_ptr<SpriteBase>> _runSprites;     //!< �X�v���C�g��o�^���铮�I�z��
         std::vector<std::unique_ptr<SpriteBase>> _pendingSprites; //!< �X�v���C�g�̓o�^��ۗ����铮�I�z��
      };
   }
}
