#pragma once
/*****************************************************************//**
 * \file   CollisionServer.h
 * \brief  当たり判定処理を行うクラス
 * 
 * \author AHMD2000
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
#include "ParamCollision.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   //二重インクルード防止
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief 当たり判定関係
    */
   namespace Collision {
      /**
       * \class 当たり判定処理を行うクラス
       * \brief 当たり判定処理を行うクラスを管理する
       */
      class CollisionComponent {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief 当たり判定結果の列挙
          */
         enum class ReportId {
            None,                      //!< 当たっていない
            HitFromPlayer,             //!< プレイヤーと当たった
            HitFromObjectRange,        //!< オブジェクトを持ち上げられる範囲にいる
            HitFromIdleFallObject,     //!< 待機状態の落下するオブジェクトと当たった
            HitFromFallObject,         //!< 落下している落下するオブジェクトと当たった
            HitFromGatling,            //!< ガトリングと当たった
            HitFromLargeEnemy,         //!< ボスと当たった
            HitFromPoorEnemy,          //!< 雑魚敵と当たった
            HitFromBullet,             //!< 遠隔弱攻撃の弾と当たった
            HitFromLaser,              //!< レーザーと当たった
            OutStage                   //!< ステージの外にいる
         };
         /**
          * \brief 当たり判定結果を管理するクラス
          */
         class Report {
         public:
            /**
             * \brief 当たり判定結果の設定
             * \param id 当たり判定結果
             */
            void id(ReportId id) { _id = id; }
            /**
             * \brief 当たり判定結果の取得
             * \return 当たり判定結果
             */
            ReportId& id() { return _id; }

         private:
            ReportId _id{ ReportId::None };  //!< 当たり判定の結果
         };
         /**
          * \brief コンストラクタ
          * \param owner オブジェクトの参照
          */
         CollisionComponent(Object::ObjectBase& owner);
         /**
          * \brief オブジェクトを持ち上げられる範囲にプレイヤーがいるか確認
          */
         void PlayerFromObjectRange();
         /**
          * \brief 落下するオブジェクトのモデルと当たっているか判定
          * \param fall 落下中か
          */
         void PlayerFromFallObjectModel(bool fall);
         /**
          * \brief オブジェクトのモデルにガトリングが当たっているか確認
          */
         void GatlingFromObjectModel();
         /**
          * \brief プレイヤーがガトリングに当たっているか確認
          */
         void GatlingFromPlayer();
         /**
          * \brief ラージエネミーのモデルに落下するオブジェクトが当たったか確認
          */
         void LargeEnemyFromObjectModel();
         /**
          * \brief 遠隔弱攻撃の弾がラージエネミーに当たったか確認
          */
         void LargeEnemyFromBullet();
         /**
          * \brief レーザーが落下するオブジェクトに当たっているか確認
          */
         void FallObjectFromLaser();
         /**
          * \brief レーザーがプレイヤーと当たっているか確認
          */
         void PlayerFromLaser();
         /**
          * \brief プレイヤーがラージエネミーのモデルと当たっているか確認
          */
         void LargeEnemyFromPlayer();
         /**
          * \brief プレイヤーが雑魚敵のモデルと当たっているか確認
          */
         void PoorEnemyFromPlayer();
         /**
          * \brief ガトリング攻撃をしてくる雑魚敵が遠隔弱攻撃の弾と当たっているか確認
          */
         void BulletFromPoorEnemy();
         /**
          * \brief 落下するオブジェクトがガトリング攻撃をしてくる雑魚敵と当たったか確認
          */
         void PoorEnemyGatlingFromObjectModel();
         /**
          * \brief プレイヤーがノックバックしているか確認
          */
         void PlayerKnockBack();
         /**
          * \brief プレイヤーとステージが当たっているか確認
          * \param pos プレイヤーの位置
          * \param moved プレイヤーの移動量のベクトル
          * \return 移動後の位置
          */
         Vector4 PlayerCheckStage(const Vector4& pos, const Vector4& moved);
         /**
          * \brief ラージエネミーとステージが当たっているか確認
          * \param pos ラージエネミーの位置
          * \param moved ラージエネミーの移動量のベクトル
          * \return 移動後の位置
          */
         Vector4 LargeEnemyCheckStage(const Vector4& pos, const Vector4& moved);
         /**
          * \brief プアエネミーとステージが当たっているか確認
          * \param pos プアエネミーの位置
          * \param moved プアエネミーの移動量のベクトル
          * \return 移動後の位置
          */
         Vector4 PoorEnemyCheckStage(const Vector4& pos, const Vector4& moved);
         /**
          * \brief 指定位置の垂線とステージが当たっているかの確認
          * \param pos 垂線を出現させる位置(Y軸は考慮しない)
          * \return 指定位置の垂線とステージが当たっているか
          */
         bool IsLineFromStage(const Vector4& pos);
         /**
          * \brief ステージ外にいるか確認
          */
         void OutStage();
         /**
          * \brief 当たり判定結果を管理するポインタの設定
          * \param report 当たり判定結果を管理するクラス
          */
         void report(Report report) { *_report = report; }
         /**
          * \brief 当たり判定結果を管理するクラスの参照の取得
          * \return 当たり判定結果を管理するクラスの参照
          */
         Report& report() { return *_report; }
         /**
          * \brief 当たった位置の設定
          * \param pos 当たった位置
          */
         void hitPos(Vector4 pos) { _hitPos = pos; }
         /**
          * \brief 当たった位置の取得
          * \return 当たった位置
          */
         Vector4 hitPos() { return _hitPos; }
         /**
          * \brief ダメージ量の設定
          * \param damage ダメージ量
          */
         void damage(double damage) { _damage = damage; }
         /**
          * \brief ダメージ量の取得
          * \return ダメージ量
          */
         double damage() { return _damage; }
         /**
          * \brief ノックバックしているか設定
          * \param knockBack ノックバックしているか
          */
         void knockBack(bool knockBack) { _knockBack = knockBack; }
         /**
          * \brief ノックバックしているかの取得
          * \return ノックバックしているか
          */
         bool knockBack() { return _knockBack; }

      private:

         Object::ObjectBase& _owner;                     //!< オブジェクトの参照
         std::unique_ptr<Param::ParamCollision> _param;  //!< 当たり判定処理を行うクラスの値管理クラス
         std::unique_ptr<Report> _report;                //!< 当たり判定結果を管理するポインタ
         double _damage{ 0.0 };                          //!< ダメージ量
         bool _knockBack{ false };                       //!< プレイヤーがノックバックしているか
         Vector4 _hitPos;                                //!< 当たった位置

      };

   }
}
