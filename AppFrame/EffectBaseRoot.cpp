#include "EffectBaseRoot.h"
#include "GameBase.h"
namespace AppFrame {
   namespace Effect {
      EffectBaseRoot::EffectBaseRoot(Game::GameBase& gameBase) :_gameBase{gameBase} {
      }

      EffectBaseRoot::~EffectBaseRoot() {};
   }
}
