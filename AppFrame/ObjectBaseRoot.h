#pragma once
#include <memory>
#include <vector>
#include "Vector4.h"
#include "Matrix44.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   // 二重インクルード防止
   namespace Game {
      class GameBase;
   }
   namespace Input {
      class InputManager;
   }
   namespace Resource {
      class LoadResourceJson;
   }
   namespace State {
      class StateServer;
   }
   namespace Effect {
      class EffectServer;
   }
   namespace Texture {
      class TextureComponent;
   }
   namespace Sound {
      class SoundComponent;
   }
   /**
    * \brief オブジェクト関係
    */
   namespace Object {
      //二重インクルード防止
      class ObjectServer;
      /**
       * \class オブジェクトの基底クラス
       * \brief 各オブジェクトはこのクラスを継承して定義する
       */
      class ObjectBaseRoot {
         using Vector4 = Math::Vector4;
         using Matrix44 = Math::Matrix44;
         using StateServer = State::StateServer;
      public:
         ;
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         ObjectBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief デストラクタ
          */
         virtual ~ObjectBaseRoot();
         /**
          * \brief 初期化処理
          */
         virtual void Init() {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update() {};
         /**
          * \brief 描画処理
          */
         virtual void Draw() {};

        

         

      protected:
         

         
      };
   }
}