#pragma once
#include "EffectBaseRoot.h"
#include <unordered_map>
#include <vector>
#include <memory>
namespace AppFrame {
   namespace Input {
      class InputManager;
   }
   namespace Effect {
      class EffectServer {
      public:
         void Add(std::unique_ptr<EffectBaseRoot> efc);
         
         void Input(Input::InputManager& input);
        
         void Update();
         
         void Render();
        
         void Clear();
         
         std::vector<std::unique_ptr<EffectBaseRoot>>& runObjects() { return _runEffects; };
      private:
         bool _updating{ false };   //!< �X�V���Ă��邩�̃t���O
         std::vector<std::unique_ptr<EffectBaseRoot>> _runEffects;      //!< �I�u�W�F�N�g�̏������񂷗p�̓��I�z��
         std::vector<std::unique_ptr<EffectBaseRoot>> _pendingEffects;  //!< �ۗ����̃I�u�W�F�N�g��o�^���Ă������I�z��

      };
}
}
