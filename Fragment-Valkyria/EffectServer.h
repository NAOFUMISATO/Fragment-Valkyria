#pragma once
/*****************************************************************//**
 * \file   EffectServer.h
 * \brief  �G�t�F�N�g�ꊇ�Ǘ��N���X
 *
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "EffectBase.h"
#include <unordered_map>
#include <vector>
#include <memory>
 /**
  * \brief �v���W�F�N�g��
  */
namespace FragmentValkyria {
   /**
    * \brief �G�t�F�N�g�֌W
    */
   namespace Effect {
      /**
       * \class �G�t�F�N�g�ꊇ�Ǘ��N���X
       * \brief EffectBaseRoot���p������Effect�N���X���ꊇ�Ǘ�����
       */
      class EffectServer {
      public:
         /**
          * \brief �G�t�F�N�g�𓮓I�z��ɒǉ�����
          * \param efc �ǉ�����G�t�F�N�g
          */
         void Add(std::unique_ptr<EffectBase> efc);
         /**
          * \brief ���I�z��ɒǉ������G�t�F�N�g�̓��͏�������
          * \param input ���͈ꊇ�Ǘ��N���X�̎Q��
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief ���I�z��ɒǉ������G�t�F�N�g�̍X�V��������
          */
         void Update();
         /**
          * \brief ���I�z��ɒǉ������G�t�F�N�g�̕`�揈������
          */
         void Render();
         /**
         * \brief �S�Ă̓��I�z��̑S�v�f���폜����
         */
         void Clear();
         /**
          * \brief �G�t�F�N�g�̏������񂷓��I�z����擾����
          * return �G�t�F�N�g�̏������񂷓��I�z��
          */
         std::vector<std::unique_ptr<EffectBase>>& runEffects() { return _runEffects; }

      private:
         bool _updating{ false };                                   //!< �X�V���Ă��邩�̃t���O
         std::vector<std::unique_ptr<EffectBase>> _runEffects;      //!< �G�t�F�N�g�̏������񂷗p�̓��I�z��
         std::vector<std::unique_ptr<EffectBase>> _pendingEffects;  //!< �ۗ����̃G�t�F�N�g��o�^���Ă������I�z��

      };
   }
}
