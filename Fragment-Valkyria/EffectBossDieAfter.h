#pragma once
#include "EffectBase.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
      /**
       * \class ボスの死亡後エフェクトクラス
       * \brief ボスの死亡後エフェクトの発生
       */
      class EffectBossDieAfter :public EffectBase {
      public:
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key エフェクトハンドルを登録した文字列
          */
         EffectBossDieAfter(Game::GameMain& gameMain, std::string_view key);
         /**
          * \brief 初期化処理
          */
         void Init()override;
         /**
          * \brief 更新処理
          */
         void Update()override;
         /**
          * \brief エフェクトの種別を返す
          * \return 種別:ボスの死亡後
          */
         virtual EffectType GetEfcType() const override { return EffectType::BossDieAfter; }
      
      private:
         int _efcCnt{ 0 };
      };
   }
}
