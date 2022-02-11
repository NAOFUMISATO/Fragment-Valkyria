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
         
         enum class SpriteState {
            Active,       //!< 生存
            Paused,       //!< 停止
            Dead          //!< 死亡
         };
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         SpriteBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief デフォルトデストラクタ
          */
         virtual ~SpriteBaseRoot()=default;
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
         virtual void Draw();

         /**
          * \brief 死亡しているかの判定
          * \return 死亡していればtrue、でなければfalseを返す
          */
         inline bool IsDead() { return (_sprState == SpriteState::Dead); }
         /**
          * \brief 生存しているかの判定
          * \return 生存していればtrue、でなければfalseを返す
          */
         inline bool IsActive() { return (_sprState == SpriteState::Active); }
         /**
          * \brief 状態を死亡状態に設定
          */
         inline void SetDead() { _sprState = SpriteState::Dead; }
         /**
          * \brief ゲームベースの参照の取得
          * \return ゲームベースの参照
          */
         inline Game::GameBase& gameBase() const { return _gameBase; }
         /**
          * \brief 位置の設定
          * \param objPosition 位置
          */
         inline void position(const Vector4& objPosition) { _position = objPosition; }
         /**
          * \brief 位置の取得
          * \return 位置
          */
         inline Vector4 position() const { return _position; }
         
         /**
          * \brief 状態一括管理クラスの設定
          * \param state 各状態クラスのインスタンス
          */
         void stateServer(std::unique_ptr<StateServer> state);
         /**
          * \brief 状態一括管理クラスの取得
          * \return 状態管理クラス
          */
         inline StateServer& stateServer() const { return *_stateServer; }
         /**
          * \brief スプライトサーバーの参照をゲームベースクラス経由で取得
          * \return スプライトサーバーの参照
          */
         SpriteServer& GetSprServer() const;
         /**
          * \brief jsonファイル管理クラスの参照をゲーム本体経由で取得
          * \return jsonファイル管理クラスの参照
          */
         AppFrame::Resource::LoadResourceJson& GetLoadJson() const;
         /**
          * \brief 画像簡易描画クラスの参照をゲームベース経由で取得
          * \return 画像簡易描画クラスの参照
          */
         Texture::TextureComponent& GetTexComponent() const;
         /**
          * \brief リソースサーバーの参照をゲームベース経由で取得
          * \return リソースサーバーの参照
          */
         Resource::ResourceServer& GetResServer() const;

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