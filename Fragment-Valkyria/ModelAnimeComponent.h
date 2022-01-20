#pragma once
/*****************************************************************//**
 * \file   ModelAnimeComponent.h
 * \brief  モデルのアニメーションの一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModelComponent.h"
 /**
  * \brief プロジェクト名
  */
namespace FragmentValkyria {
   /**
    * \brief モデル関係
    */
   namespace Model {
      /**
       * \class モデルのアニメーションの一括管理クラス
       * \brief アニメーションの管理を行う
       */
      class ModelAnimeComponent :public ModelComponent {
      public:
         /**
          * \brief コンストラクタ
          * \param owner オブジェクトの基底クラスの参照
          */
         ModelAnimeComponent(Object::ObjectBase& owner);
         /**
          * \brief デストラクタ
          */
         virtual ~ModelAnimeComponent() override;
         /**
          * \brief 初期化処理
          */
         void Init() override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Draw() override;

         /**
          * \brief アニメーションの変更
          * \param animeName アニメーション名
          * \param repeate アニメーションをループ再生するかのフラグ
          */
         void ChangeAnime(std::string_view animeName, bool repeate,double animeSpeed=1.0);
         /**
          * \brief アニメーションの総時間の取得
          * \return アニメーションの総時間
          */
         inline float GetPlayProgress() const { return _playTime / _totalTime; }
         /**
          * \brief アニメーションを繰り返した回数の取得
          * \return アニメーションを繰り返した回数
          */
         inline int repeatedCount() const { return _repeatedCount; }
         /**
          * \brief アニメーション時間を取得
          * \return アニメーション時間
          */
         inline float playTime() const { return _playTime; }
         /**
          * \brief 再生時間の掛率の取得
          * \return 再生時間の掛率
          */
         inline double timeRate() const { return _timeRate; }
         /**
          * \brief 再生時間の掛率の設定
          * \param 再生時間の掛率
          */
         inline void timeRate(double timeRate) { _timeRate = timeRate; };

      private:
         int _animIndex{ -1 };      //!< 現在のアニメ番号
         int _newAnimIndex{ -1 };   //!< 新しくアタッチするアニメ番号
         int _attachNum{ -1 };      //!< 現在のアタッチ番号
         int _nextAttachNum{ -1 };  //!< アニメーションを切り替える際の次のアタッチ番号
         bool _blending{ false };   //!< アニメーションブレンド中かの判定
         float _blendRate{ 0.f };   //!< アニメーションブレンド率
         float _playTime{ 0 };      //!< アニメーションの再生時間
         float _totalTime{ 0 };     //!< アニメーションの総再生時間
         double _timeRate{ 1.0 };   //!< 再生時間の掛率
         int _repeatedCount{ 0 };   //!< アニメーションを繰り返した回数
         bool _repeate{ false };    //!< アニメーションをループさせるかのフラグ
      };
   }
}
