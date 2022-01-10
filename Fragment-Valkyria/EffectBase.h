#pragma once
/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  �G�t�F�N�g�̊��N���X
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief �v���W�F�N�g��
 */
namespace FragmentValkyria {
   namespace Game {
      class GameMain;
   }
   namespace Effect {
      class EffectBase :public AppFrame::Effect::EffectBaseRoot {
      public:
         enum class EffectType {
            Effect=0,
            PlayerShot
         };
         EffectBase(Game::GameMain& gameMain);
         virtual ~EffectBase() override ;

         void Init(std::string_view key) override ;
         void Input(AppFrame::Input::InputManager& input) override {};
         void Update() override {};
         void Draw() override ;
         virtual EffectType GetEfcType() const = 0;

      protected:
         Game::GameMain& _gameMain;
      };
}
}
