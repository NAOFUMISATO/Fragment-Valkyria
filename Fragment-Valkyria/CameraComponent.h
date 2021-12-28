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
         /**
          * \brief カメラの注視点を設定する
          * \param target 注視点になる座標
          * \param forward カメラの向き
          */
         void SetTarget(Vector4 target, Vector4 forward = { 0,0,0 }) {
            _target = target, _forwardOfTarget = forward;
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
         Vector4 _forwardOfTarget{ 0, 0, 0 }; //!< 注視点オブジェクトの向き
         double _targetDistance{ 500 };       //!< 注視点オブジェクトとのZ座標の距離
         double _vertDistance{ 120 };         //!< 注視点オブジェクトとのY座標の距離
         std::tuple<double, double, double> _nearFarFov{   //!< カメラの描画限界(手前,奥)及び視野角のTuple型(透視変換に使用)
            std::make_tuple(10.0,10000.0,AppFrame::Math::Utility::DegreeToRadian(50.0)) };
      };
   }
}