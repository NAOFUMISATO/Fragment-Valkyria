#pragma once
#include <memory>
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
      class LoadJson;
   }
   namespace State {
      class StateServer;
   }
   namespace Effect {
      class EffectServer;
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
         /**
          * \brief オブジェクトの状態列挙
          */
         enum class ObjectState {
            Active,       //!< 生存
            Paused,       //!< 停止
            Dead          //!< 死亡
         };
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

         /**
          * \brief オブジェクトの死亡判定を返す
          * \return オブジェクトの状態がDeadならばTrue、でなければfalseを返す
          */
         inline bool IsDead() { return (_objState == ObjectState::Dead); }
         /**
          * \brief オブジェクトの生存判定を返す
          * \return オブジェクトの状態がActiveならばTrue、でなければfalseを返す
          */
         inline bool IsActive() { return (_objState == ObjectState::Active); }
         /**
          * \brief オブジェクトを死亡状態に設定
          */
         inline void SetDead() { _objState = ObjectState::Dead; }
         /**
          * \brief ゲームの参照を返す
          * \return ゲームの参照
          */
         inline Game::GameBase& gameBase() const { return _gameBase; }
         /**
          * \brief ワールド行列の設定
          */
         virtual void ComputeWorldTransform();
         /**
          * \brief ワールド行列の取得
          * \return ワールド行列
          */
         inline Matrix44& worldTransform() { return _worldTransform; }
         /**
          * \brief オブジェクトの向きの取得
          * \return オブジェクトの回転ベクトル
          */
         Vector4 GetForward() const {
            auto vec = Vector4{ 0,0,1 };
            vec.Normalized();
            Matrix44 matrix;
            matrix.RotateY(_rotation.GetY(), true);
            return vec * matrix;
         }

         /**
          * \brief オブジェクト位置の設定
          * \param pos オブジェクト位置
          */
         inline void position(const Vector4& objPosition) { _position = objPosition; }
         /**
          * \brief オブジェクト位置を取得
          * \return オブジェクト位置
          */
         inline Vector4 position() const { return _position; }
         /**
          * \brief オブジェクトの回転角の設定
          * \param rot オブジェクトの回転
          */
         inline void rotation(const Vector4& objRotation) { _rotation = objRotation; }
         /**
          * \brief オブジェクトの回転角を取得
          * \return オブジェクトの回転
          */
         inline Vector4 rotation() const { return _rotation; }
         /**
          * \brief オブジェクトの拡大率の設定
          * \param sca オブジェクトの拡大率
          */
         inline void scale(const Vector4& objScale) { _scale = objScale; }
         /**
          * \brief オブジェクトの拡大率の取得
          * \return オブジェクトの拡大率
          */
         inline Vector4 scale() const { return _scale; }
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
          * \brief オブジェクト一括管理クラスの参照をゲーム本体を経由し取得
          * \return オブジェクト一括管理クラスの参照
          */
         ObjectServer& GetObjServer() const;
         /**
          * \brief エフェクト一括管理クラスの参照をゲーム本体経由で取得
          * \return エフェクト一括管理クラスの参照
          */
         Effect::EffectServer& GetEfcServer() const;
         /**
          * \brief jsonファイル管理クラスの参照をゲーム本体経由で取得
          * \return jsonファイル管理クラスの参照
          */
         AppFrame::Resource::LoadJson& GetLoadJson() const;

      protected:
         /**
          * \brief オブジェクトの状態を取得
          * \return オブジェクトの状態
          */
         inline ObjectState objState() const { return _objState; }
         /**
          * \brief オブジェクトの状態を設定
          * \param state オブジェクトの状態
          */
         inline void objState(ObjectState state) { _objState = state; }
         /**
          * \brief ビルボードの描画を行う
          * \param pos 描画位置
          * \param size 描画サイズ(横方向のサイズ、縦方向は縦横比により決定)
          * \param angle 描画角度
          * \param handle 画像ハンドル
          */
         void DrawBillBoard(Vector4 pos,double size,double angle,std::vector<int> handle,double deltatime);

         ObjectState _objState{ ObjectState::Active }; //!< オブジェクトの種別変数
         std::unique_ptr<StateServer> _stateServer;    //!< 状態の一括管理クラスのポインタ
         Game::GameBase& _gameBase;              //!< ゲーム本体クラスの参照
         Matrix44 _worldTransform{ Matrix44() }; //!< ワ－ルド行列
         Vector4 _position{ 0,0,0 };             //!< 位置
         Vector4 _rotation{ 0,0,0 };             //!< 回転
         Vector4 _scale{ 1,1,1 };                //!< 拡大率
         int _cnt{0};
      };
   }
}