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
   namespace Lighting {
      class LightAndShadow;
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
          * \brief インゲーム種別の取得
          */
         enum class InGameType {
            Boss   //!< モードボス
         };

         /**
          * \brief コンストラクタ
          * \param game ゲーム本体の参照
          */
         ModeInGameBase(Game::GameMain& gameMain);
         /**
          * \brief デフォルトデストラクタ
          */
         ~ModeInGameBase() = default;
         /**
          * \brief 初期化処理
          */
         void Init() override;
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
          * \brief インゲーム種別の取得
          * \return 派生先で定義
          */
         virtual InGameType GetInGameType() const = 0;
         /**
          * \brief オブジェクト生成一括管理クラスの参照をゲームメイン経由で取得
          * \return オブジェクト生成一括管理クラスの参照
          */
         Create::ObjectFactory& GetObjFactory() const;

      protected:
         std::unique_ptr<Lighting::LightAndShadow> _lighting;
#ifdef _DEBUG
         void DebugDraw();
         short _padLeftX{ 0 };
         short _padLeftY{ 0 };
         short _padRightX{ 0 };
         short _padRightY{ 0 };
         double _largeEnemyHp{ 0 };
         double _playerHp{ 0 };
#endif
      };
   }
}