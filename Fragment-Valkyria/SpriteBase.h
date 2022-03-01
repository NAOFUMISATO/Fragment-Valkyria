#pragma once
/*****************************************************************//**
 * \file   SpriteBase.h
 * \brief  スプライトの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   February 2022
 *********************************************************************/
#include "AppFrame.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief スプライト関係
    */
   namespace Sprite {
      // 二重インクルード防止
      class SpriteServer;
      /**
       * \class スプライトの基底クラス
       * \brief 各スプライトはこのクラスを派生して定義する
       */
      class SpriteBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using StateServer = AppFrame::State::StateServer;
      public:
         /**
          * \brief スプライトの状態列挙
          */
         enum class SpriteState {
            Active,       //!< 生存
            Paused,       //!< 停止
            Dead          //!< 死亡
         };
         /**
          * \brief スプライトの種別列挙
          */
         enum class SpriteType {
            Sprite = 0,      //!< スプライト
            PlayerHP,        //!< プレイヤーHP
            LargeEnemyHP,    //!< ボスHP
            RemainingBullet, //!< 残り弱攻撃弾数
            RemainingPortion,//!< 残り回復アイテム数
            Reticle          //!< レティクル
         };
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         SpriteBase(Game::GameMain& gameMain);
         /**
          * \brief デフォルトデストラクタ
          */
         virtual ~SpriteBase() = default;
         /**
          * \brief 初期化処理
          */
         virtual void Init() {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(AppFrame::Input::InputManager & input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update() {};
         /**
          * \brief 描画処理
          */
         virtual void Draw();
         /**
          * \brief スプライト種別の取得
          * \return 派生先で定義
          */
         virtual SpriteType GetSprType() const = 0;
         /**
          * \brief ゲーム本体クラスの参照を取得
          * \return ゲーム本体クラスの参照
          */
         inline Game::GameMain& gameMain() const { return _gameMain; }
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
          * \brief スプライトサーバーの参照をゲーム本体クラス経由で取得
          * \return スプライトサーバーの参照
          */
         SpriteServer& GetSprServer() const;
         /**
          * \brief jsonファイル管理クラスの参照をゲーム本体クラス経由で取得
          * \return jsonファイル管理クラスの参照
          */
         AppFrame::Resource::LoadResourceJson& GetLoadJson() const;
         /**
          * \brief 画像簡易描画クラスの参照をゲーム本体クラス経由で取得
          * \return 画像簡易描画クラスの参照
          */
         AppFrame::Texture::TextureComponent& GetTexComponent() const;
         /**
          * \brief リソースサーバーの参照をゲーム本体クラス経由で取得
          * \return リソースサーバーの参照
          */
         AppFrame::Resource::ResourceServer& GetResServer() const;

      protected:
         /**
         * \brief スプライトの状態を取得
         * \return スプライトの状態
         */
         inline SpriteState sprState() const { return _sprState; }
         /**
          * \brief スプライトの状態を設定
          * \param state 設定するスプライトの状態
          */
         inline void sprState(SpriteState state) { _sprState = state; }

         Game::GameMain& _gameMain;                     //!< ゲーム本体クラスの参照
         int _grHandle{ -1 };                           //!< 画像ハンドル
         int _cx{ 0 };                                  //!< 画像のX基準点
         int _cy{ 0 };                                  //!< 画像のY基準点
         int _alpha{ 255 };                             //!< 透明度
         bool _transFlag{ true };                       //!< 背景透過フラグ
         bool _turnFlag{ false };                       //!< 反転判定
         double _scale{ 1.0 };                          //!< 拡大率
         double _angle{ 0.0 };                          //!< 角度
         Vector4 _position{ 0,0,0 };                    //!< 位置
         std::vector<int> _grHandles;                   //!< 画像ハンドル配列
         SpriteState _sprState{ SpriteState::Active };  //!< オブジェクトの状態
         std::unique_ptr<StateServer> _stateServer;     //!< 状態の一括管理クラスのポインタ
      };
   }
}
