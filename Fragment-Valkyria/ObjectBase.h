#pragma once
#pragma once
/*****************************************************************//**
 * \file   ObjectBase.h
 * \brief  オブジェクトの基底クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
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
   namespace Model {
      class ModelAnimeComponent;
   }
   namespace Camera {
      class CameraComponent;
   }
   /**
    * \brief オブジェクト関係
    */
   namespace Object {
      class ObjectServer;
      /**
       * \class オブジェクトの基底クラス
       * \brief 各オブジェクトはこのクラスを継承して定義する
       */
      class ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using StateServer = AppFrame::State::StateServer;
      public:
         /**
          * \brief オブジェクトの種別列挙
          */
         enum class ObjectType {
            Object = 0,   //!< オブジェクト
            Player,       //!< プレイヤー
            Enemy,        //!< 敵
            Stage         //!< ステージ
         };
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
         ObjectBase(Game::GameMain& gameMain);
         /**
          * \brief デストラクタ
          */
         virtual ~ObjectBase();
         /**
          * \brief 初期化処理
          */
         virtual void Init() {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(AppFrame::Input::InputManager& input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update() {};
         /**
          * \brief 描画処理
          */
         virtual void Draw() {};

         /**
          * \brief オブジェクトの種別を返す（純粋仮想関数化）
          * \return 派生先で定義
          */
         virtual ObjectType GetObjType() const = 0;
         /**
          * \brief オブジェクトの死亡判定を返す
          * \return オブジェクトの状態がDeadならばTrue、でなければfalseを返す
          */
         bool IsDead() { return (_objState == ObjectState::Dead); }
         /**
          * \brief オブジェクトの生存判定を返す
          * \return オブジェクトの状態がActiveならばTrue、でなければfalseを返す
          */
         bool IsActive() { return (_objState == ObjectState::Active); }
         /**
          * \brief ゲームの参照を返す
          * \return ゲームの参照
          */
         Game::GameMain& gameMain() { return _gameMain; }
         /**
          * \brief ワールド行列の設定
          */
         virtual void ComputeWorldTransform();
         /**
          * \brief ワールド行列の取得
          * \return ワールド行列
          */
         Matrix44& worldTransform() { return _worldTransform; }
         /**
          * \brief オブジェクトの向きの取得
          * \return オブジェクトの回転ベクトル
          */
         Vector4 GetForward() const {
            auto vec = Vector4{ 0,0,1 };
            Matrix44 matrix;
            matrix.RotateY(_rotation.GetY(), true);
            return vec * matrix;
         }

         /**
          * \brief オブジェクト位置の設定
          * \param pos オブジェクト位置
          */
         void position(const Vector4& objPosition) { _position = objPosition; }
         /**
          * \brief オブジェクト位置を取得
          * \return オブジェクト位置
          */
         Vector4 position() const { return _position; }
         /**
          * \brief オブジェクトの回転角の設定
          * \param rot オブジェクトの回転
          */
         void rotation(const Vector4& objRotation) { _rotation = objRotation; }
         /**
          * \brief オブジェクトの回転角を取得
          * \return オブジェクトの回転
          */
         Vector4 rotation() const { return _rotation; }
         /**
          * \brief オブジェクトの拡大率の設定
          * \param sca オブジェクトの拡大率
          */
         void scale(const Vector4& objScale) { _scale = objScale; }
         /**
          * \brief オブジェクトの拡大率の取得
          * \return オブジェクトの拡大率
          */
         Vector4 scale() const { return _scale; }
         /**
          * \brief 状態一括管理クラスの設定
          * \param state 各状態クラスのインスタンス
          */
         void stateServer(std::unique_ptr<StateServer> state);
         /**
          * \brief アニメーション一括管理クラスの設定
          * \param model アニメーション管理クラスのインスタンス
          */
         void modelAnimeComponent(std::unique_ptr<Model::ModelAnimeComponent> model);
         /**
          * \brief カメラ管理クラスの設定
          * \param camera カメラ管理クラスのインスタンス
          */
         void cameraComponent(std::shared_ptr<Camera::CameraComponent> camera);
         /**
          * \brief 状態一括管理クラスの取得
          * \return 状態管理クラス
          */
         StateServer& stateServer() { return *_stateServer; }
         /**
          * \brief アニメーション一括管理クラスの取得
          * \return アニメーション管理クラス
          */
         Model::ModelAnimeComponent& modelAnimeComponent() { return *_modelAnimeComponent; }
         /**
          * \brief カメラ管理クラスの取得
          * \return カメラ管理クラス
          */
         Camera::CameraComponent& cameraComponent() { return *_cameraComponent; }
         /**
          * \brief オブジェクト一括管理クラスの参照をゲーム本体を経由し取得
          * \return オブジェクト一括管理クラスの参照
          */
         ObjectServer& GetObjServer();

      protected:
         /**
          * \brief オブジェクトの状態を取得
          * \return オブジェクトの状態
          */
         ObjectState objState() const { return _objState; }
         /**
          * \brief オブジェクトの状態を設定
          * \param state オブジェクトの状態
          */
         void objState(ObjectState state) { _objState = state; }


         ObjectState _objState{ ObjectState::Active };                   //!< オブジェクトの種別変数
         std::unique_ptr<StateServer> _stateServer;                        //!< 状態の一括管理クラスのポインタ
         std::unique_ptr<Model::ModelAnimeComponent> _modelAnimeComponent; //!< モデルのアニメーション管理クラスのポインタ
         std::shared_ptr<Camera::CameraComponent> _cameraComponent;        //!< カメラ管理クラスのポインタ
         Game::GameMain& _gameMain;              //!< ゲーム本体クラスの参照
         Matrix44 _worldTransform{ Matrix44() }; //!< ワ－ルド行列
         Vector4 _position{ 0,0,0 };               //!< 位置
         Vector4 _rotation{ 0,0,0 };             //!< 回転
         Vector4 _scale{ 1,1,1 };                //!< 拡大率
      };

   }

}
