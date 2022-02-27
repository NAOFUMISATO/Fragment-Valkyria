#pragma once
/*****************************************************************//**
 * \file   SpriteBaseRoot.h
 * \brief  スプライト基底クラスの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
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
      class ResourceServer;
      class LoadResourceJson;
   }
   namespace State {
      class StateServer;
   }
   namespace Texture {
      class TextureComponent;
   }
   namespace Sprite {
      //二重インクルード防止
      class SpriteServer;
      
      class SpriteBaseRoot {
         using Vector4 = Math::Vector4;
         using Matrix44 = Math::Matrix44;
         using StateServer = State::StateServer;
      public:
         
         
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         SpriteBaseRoot(Game::GameBase& gameBase);
         

         
         /**
          * \brief ゲームベースの参照の取得
          * \return ゲームベースの参照
          */
         inline Game::GameBase& gameBase() const { return _gameBase; }
         

      protected:
         /**
          * \brief オブジェクトの状態を取得
          * \return 状態
          */
         inline SpriteState objState() const { return _sprState; }
         /**
          * \brief オブジェクトの状態を設定
          * \param state 設定する状態
          */
         inline void objState(SpriteState state) { _sprState = state; }

         Game::GameBase& _gameBase;                     //!< ゲーム本体クラスの参照
         std::unique_ptr<StateServer> _stateServer;     //!< 状態の一括管理クラスのポインタ
         int _grHandle{ -1 };                           //!< 画像ハンドル
         int _cx{ 0 }, _cy{ 0 };                        //!< 画像の基準点
         int _alpha{ 255 };                             //!< 透明度
         double _scale{ 1.0 };                          //!< 拡大率
         double _angle{ 0.0 };                          //!< 角度
         bool _turnFlag{ false };                       //!< 反転判定
         std::vector<int> _grHandles;                   //!< 画像ハンドル配列
         Vector4 _position{ 0,0,0 };                    //!< 位置
         SpriteState _sprState{ SpriteState::Active };  //!< オブジェクトの状態
      };
   }
}