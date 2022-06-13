#pragma once
/*****************************************************************//**
 * \file   Laser.h
 * \brief  レーザーの処理を回すクラス
 * 
 * \author YUSUKEAHAMEDO
 * \date   January 2022
 *********************************************************************/
#include "ObjectBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief 敵関係
    */
   namespace Enemy {
      /**
       * \class レーザーの処理を回すクラス
       * \brief ボスが放つレーザーの処理を回す
       */
      class Laser : public Object::ObjectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          */
         Laser();
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Draw() override;
         /**
          * \brief オブジェクトの種類を返す
          * \return レーザー
          */
         ObjectType GetObjType() const override { return ObjectType::Laser; }
         /**
          * \brief レーザーの終点を設定
          * \param endPos 設定する終点座標
          */
         void end(Vector4 endPos) { _end = endPos; }
         /**
          * \brief レーザーの終点座標を取得
          * \return レーザーの終点座標
          */
         inline Vector4 end() { return _end; }

      private:
         Vector4 _end{ 0.0, 0.0, 0.0 };  //!< レーザー終点座標

      public:
         /**
         * \class レーザーの状態の基底クラス
         * \brief 各レーザーの状態はこれを派生して定義する
         */
         class StateBase : public AppFrame::State::StateBaseRoot {
         public:
            /**
             * \brief コンストラクタ
             * \param owner プレイヤーの参照
             */
            StateBase(Laser& owner) : _owner{ owner } {};
            /**
             * \brief 描画処理
             */
            virtual void Draw() override;

         protected:
            Laser& _owner;        //!< レーザーの参照
            int _stateCnt{ 0 };   //!< 各状態へ入った時のフレームカウント保存用
         };
         /**
          * \class 照射状態クラス
          * \brief 照射状態の処理を回す
          */
         class StateIrradiation : public StateBase {
         public:
            /**
             * \brief コンストラクタ
             * \param owner レーザーの参照
             */
            StateIrradiation(Laser& owner) : StateBase{ owner } {};
            /**
             * \brief 入口処理
             */
            void Enter() override;
            /**
             * \brief 更新処理
             */
            void Update() override;
         };
      };
   }
}
