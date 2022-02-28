#pragma once
/*****************************************************************//**
 * \file   ModeInGameBase.h
 * \brief  各インゲームの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "ModeBase.h"
#include "LightAndShadow.h"
#include "Stage.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   // 二重インクルード防止
   namespace Game {
      class GameMain;
   }
   namespace Create {
      class ObjectFactory;
   }
   /**
    * \brief モード関係
    */
   namespace Mode {
      /**
       * \class モードインゲームクラス
       * \brief インゲームの処理を回す
       */
      class ModeInGameBase : public ModeBase {
         using Vector4 = AppFrame::Math::Vector4;
      public:
         /**
          * \brief コンストラクタ
          * \param game ゲーム本体の参照
          */
         ModeInGameBase(Game::GameMain& gameMain);
         /**
          * \brief 入口処理
          */
         void Enter() override;
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input) override;
         /**
          * \brief 更新処理
          */
         void Update() override;
         /**
          * \brief 描画処理
          */
         void Render() override;
         /**
          * \brief 出口処理
          */
         void Exit() override;
         /**
          * \brief オブジェクト生成一括管理クラスの参照をゲームメイン経由で取得
          * \return オブジェクト生成一括管理クラスの参照
          */
         Create::ObjectFactory& GetObjFactory() const;
         /**
          * \brief ステージの参照の取得
          * \return ステージの参照
          */
         inline Stage::Stage& GetStage() { return *_stage; }
         void IndividualEffectClear();
      protected:
         std::unique_ptr<Stage::Stage> _stage;                 //!< ステージクラスのユニークポインタ
         std::unique_ptr<Lighting::LightAndShadow> _lighting;  //!< ライティングクラスのユニークポインタ
#ifdef _DEBUG
         /**
          * \brief デバッグ描画処理
          */
         void DebugDraw();
         short _padLeftX{ 0 };   //!< 左スティックX軸入力量
         short _padLeftY{ 0 };   //!< 左スティックY軸入力量
         short _padRightX{ 0 };  //!< 右スティックX軸入力量
         short _padRightY{ 0 };  //!< 右スティックY軸入力量
#endif
      };
   }
}