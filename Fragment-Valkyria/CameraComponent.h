#pragma once
/*****************************************************************//**
 * \file   CameraComponent.h
 * \brief  カメラ管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "AppFrame.h"
#include <tuple>
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   /**
    * \brief カメラ関係
    */
   namespace Camera {
      /**
       * \class カメラ管理クラス
       * \brief インゲームで使用するカメラを管理する
       */
      class CameraComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
         using InputManager = AppFrame::Input::InputManager;
         using StateServer = AppFrame::State::StateServer;
      public:
         /**
          * \brief コンストラクタ
          */
         CameraComponent();
         /**
          * \brief 初期化処理
          */
         void Init();
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input);
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Draw();

         /**
          * \brief カメラの位置を設定する
          * \param position カメラの位置
          */
         void SetPosition(Vector4 position) { _position = position; }

         void SetPlyPos(Vector4 plyPos) { _plyPos = plyPos; }
         /**
          * \brief カメラの注視点を設定する
          * \param target 注視点になる座標
          * \param forward カメラの向き
          */
         inline void SetTarget(Vector4 target) {
             _target = target;
         }
         /**
          * \brief 注視点方向のベクトルの取得
          * \return 注視点方向の単位ベクトル
          */
         Vector4 GetForward() const {
            auto vec = _target - _position;
            vec.Normalized();
            return vec;
         }

         void stateServer(std::unique_ptr<StateServer> state) {
             _stateServer = std::move(state);
         }

      private:
         /**
          * \brief カメラのビュー行列の設定
          * \param cameraPosition カメラの位置
          * \param cameraTarget カメラの注視点
          * \param cameraUp カメラの上方向
          * \return カメラのビュー行列
          */
         Matrix44 GetCameraViewMatrix(Vector4& cameraPosition, Vector4& cameraTarget, Vector4& cameraUp);
         /**
          * \brief カメラの投影行列の設定
          * \param cameraNear カメラの描画限界(手前)
          * \param cameraFar カメラの描画限界(奥)
          * \param fov 視野角
          * \return カメラの投影行列
          */
         Matrix44 GetCameraProjectionMatrix(double cameraNear, double cameraFar, double fov);
         Vector4 _position{ 0, 0, 0 };        //!< 位置
         Vector4 _target{ 0, 0, 0 };          //!< 注視点
         Vector4 _up{ 0, 1 ,0 };              //!< 上方向
         Vector4 _forwardOfTarget{ 0, 0, 1}; //!< 注視点オブジェクトの向き
         Vector4 _plyToPos{ 0, 0, 0 };
         Vector4 _plyToTarget{ 0, 0, 0 };
         Vector4 _plyPos{ 0, 0, 0 };
         double _targetDistance{ 500 };       //!< 注視点オブジェクトとのZ座標の距離
         double _vertDistance{ 120 };         //!< 注視点オブジェクトとのY座標の距離
         std::tuple<double, double, double> _nearFarFov{   //!< カメラの描画限界(手前,奥)及び視野角のTuple型(透視変換に使用)
            std::make_tuple(2.0,10000.0,AppFrame::Math::Utility::DegreeToRadian(60.0)) };

         std::unique_ptr<StateServer> _stateServer;                        //!< 状態の一括管理クラスのポインタ

      public:
          /**
          * \class カメラの状態の基底クラス
          * \brief 各カメラの状態はこれを派生して定義する
          */
          class StateBase : public AppFrame::State::StateBaseRoot {
          public:
              /**
              * \brief コンストラクタ
              * \param owner カメラの参照
              */
              StateBase(CameraComponent& owner) : _owner{ owner } {};
              /**
               * \brief 描画処理
               */
              void Draw() override;

          protected:
              CameraComponent& _owner;   //!< カメラの参照
          };
          /**
          * \class 通常状態クラス
          * \brief 通常状態の処理を回す
          */
          class StateNormal : public StateBase
          {
          public:
              /**
               * \brief コンストラクタ
               * \param owner カメラの参照
               */
              StateNormal(CameraComponent& owner) : StateBase{ owner } {};
              /**
               * \brief 入口処理
               */
              void Enter() override;
              /**
               * \brief 入力処理
               * \param input 入力一括管理クラスの参照
               */
              void Input(InputManager& input) override;
              /**
               * \brief 更新処理
               */
              void Update() override;
          };
      };
   }
}