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
         bool _updating{ false };   //!< 更新しているかのフラグ
         std::vector<std::unique_ptr<EffectBaseRoot>> _runEffects;      //!< オブジェクトの処理を回す用の動的配列
         std::vector<std::unique_ptr<EffectBaseRoot>> _pendingEffects;  //!< 保留中のオブジェクトを登録しておく動的配列

      };
}
}
