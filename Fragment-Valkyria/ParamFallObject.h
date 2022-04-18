#pragma once
/*****************************************************************//**
 * \file   ParamFallObject.h
 * \brief  �����I�u�W�F�N�g�̒l�Ǘ��N���X
 * 
 * \author NAOFUMISATO
 * \date   April 2022
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
   /**
    * \brief �l�Ǘ��֌W
    */
   namespace Param {
      /**
       * \class �����I�u�W�F�N�g�̒l�Ǘ��N���X
       * \brief �����I�u�W�F�N�g�̒l�̓ǂݍ��݁A�擾���s��
       */
      const class ParamFallObject :public AppFrame::Param::ParamBase {
      public:
         /**
          * \brief �����I�u�W�F�N�g�̒l�̓ǂݍ��݁A�擾���s��
          * \class �����I�u�W�F�N�g�̒l�Ǘ��N���X
          */
         ParamFallObject(Game::GameMain& gameMain, std::string_view key);
      };
   }
}
