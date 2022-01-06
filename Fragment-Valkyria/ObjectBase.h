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
      class ObjectBase :public AppFrame::Object::ObjectBaseRoot{
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
          * \brief コンストラクタ
          * \param gameMain ゲーム本体の参照
          */
         ObjectBase(Game::GameMain& gameMain);
         /**
          * \brief デストラクタ
          */
         ~ObjectBase()override;
         /**
          * \brief 初期化処理
          */
         void Init() override {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         void Input(AppFrame::Input::InputManager& input)override {};
         /**
          * \brief 更新処理
          */
         void Update()override {};
         /**
          * \brief 描画処理
          */
         void Draw() override {};

         /**
          * \brief オブジェクトの種別を返す（純粋仮想関数化）
          * \return 派生先で定義
          */
         virtual ObjectType GetObjType() const = 0;
         
         Game::GameMain& gameMain() const { return _gameMain; }
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
          * \brief アニメーション一括管理クラスの取得
          * \return アニメーション管理クラス
          */
         Model::ModelAnimeComponent& modelAnimeComponent() const { return *_modelAnimeComponent; }
         /**
          * \brief カメラ管理クラスの取得
          * \return カメラ管理クラス
          */
         Camera::CameraComponent& cameraComponent() const { return *_cameraComponent; }

      protected:
         Game::GameMain& _gameMain;              //!< ゲーム本体クラスの参照
         std::unique_ptr<Model::ModelAnimeComponent> _modelAnimeComponent; //!< モデルのアニメーション管理クラスのポインタ
         std::shared_ptr<Camera::CameraComponent> _cameraComponent;        //!< カメラ管理クラスのポインタ
      };
   }
}
