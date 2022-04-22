#pragma once
/*****************************************************************//**
 * \file   EffectBossCrash.h
 * \brief  ボスの破壊エフェクトクラス
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "EffectBase.h"
#include "ParamLargeEnemy.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
      /**
       * \class ボスの破壊エフェクトクラス
       * \brief ボスの破壊エフェクトの描画を行う
       */
      class EffectBossCrash :public EffectBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectBossCrash( std::string_view key);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief エフェクトの種類を返す
          * \return 種別:ボスの破壊
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossCrash; }

      private:
         std::unique_ptr<Param::ParamLargeEnemy> _param; //!< ボスクラスの値管理クラス
         int _efcCnt{ 0 };                               //!< フレームカウント保存用
         std::vector<Vector4> _bornPositions;            //!< エフェクト生成位置の動的配列
      };
   }
}
