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
      class ResourceServer;
      class LoadResourceJson;
   }
   namespace State {
      class StateServer;
   }
   namespace Texture {
      class SimpleTextureComponent;
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

         inline bool IsDead() { return (_sprState == SpriteState::Dead); }
         
         inline bool IsActive() { return (_sprState == SpriteState::Active); }
         
         inline void SetDead() { _sprState = SpriteState::Dead; }
         
         inline Game::GameBase& gameBase() const { return _gameBase; }

         
         inline void position(const Vector4& objPosition) { _position = objPosition; }
         
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
         Texture::SimpleTextureComponent& GetSimpTexComponent() const;
         Resource::ResourceServer& GetResServer() const;

      protected:
        
         inline SpriteState objState() const { return _sprState; }
         
         inline void objState(SpriteState state) { _sprState = state; }

         SpriteState _sprState{ SpriteState::Active }; 
         std::unique_ptr<StateServer> _stateServer;    //!< 状態の一括管理クラスのポインタ
         Game::GameBase& _gameBase;   //!< ゲーム本体クラスの参照
         Vector4 _position{ 0,0,0 };  //!< 位置
         std::vector<int> _grHandles;
         int _grHandle{ -1 };
         int _cx{ 0 }, _cy{ 0 };
         int _alpha{ 255 };
         double _scale{ 1.0 };
         double _angle{ 0.0 };
         bool _turnFlag{ false };
      };
   }
}