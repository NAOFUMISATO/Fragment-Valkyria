#pragma once
/*****************************************************************//**
 * \file   StageModelComponent.h
 * \brief  ステージ描画を一括管理するクラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief ステージ名
    */
   namespace Stage {
      /**
       * \class ステージ描画を一括管理するクラス
       * \brief ステージ情報の取得及び描画を一括で行う
       */
      class StageModelComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
      public:
         /**
          * \brief コンストラクタ
          * \param owner オブジェクト基底クラスの参照
          */
         StageModelComponent(Object::ObjectBase& owner);
         /**
          * \brief デフォルトデストラクタ
          */
         ~StageModelComponent()=default;
         /**
          * \brief 初期化処理
          */
         void Init();
         /**
          * \brief 更新処理
          */
         void Update();
         /**
          * \brief 描画処理
          */
         void Draw();
         /**
          * \brief ステージ情報とハンドルを動的配列に登録する
          * \param key 連想配列に関連付けた任意の文字列
          */
         void SetModels(std::string_view key);
         /**
          * \brief モデルハンドルの動的配列への設定
          * \param handles 設定する画像ハンドルの動的配列
          */
         inline void modelHandles(std::vector<int> handles) { _modelHandles = handles; }
         /**
          * \brief モデルハンドルの動的配列の取得
          * \return モデルハンドルの動的配列
          */
         inline std::vector<int> modelHandles() { return _modelHandles; }
         /**
          * \brief モデルの自己発光色の設定
          * \param index 設定するモデルのマテリアル番号
          * \param r 赤
          * \param g 緑
          * \param b 青
          */
         void SetEmiColor(int index, float r, float g, float b);
         /**
          * \brief ライティングフラグをfalseにする
          */
         inline void LightingOFF() { _isLighting = false; }
         /**
          * \brief ピクセルライティングフラグをtrueにする
          */
         inline void PixelLightingON() { _isPixelLighting = true; }
         /**
          * \brief ステージパーツのモデルハンドルの動的配列から指定のコリジョンフレームがあるモデルハンドルとコリジョン番号を返す
          * \param collName コリジョンフレーム名
          * \return モデルハンドルとコリジョン番号
          */
         std::pair<int, int> GetHandleAndCollNum(std::string_view collName);

      protected:
         /**
          * \brief 指定のハンドルの座標設定
          * \param handle ハンドル
          * \param position 座標
          */
         void SetPosition(int handle, Vector4 position);
         /**
          * \brief 指定のハンドルの回転角設定
          * \param handle ハンドル
          * \param rotation 回転角
          */
         void SetRotation(int handle, Vector4 rotation);
         /**
          * \brief 指定のハンドルの拡大率設定
          * \param handle ハンドル
          * \param scale 拡大率
          */
         void SetScale(int handle, Vector4 scale);
         /**
          * \brief 指定のハンドルのワールド行列の設定
          * \param handle ハンドル
          * \param world ワールド行列
          */
         void SetMatrix(int handle, Matrix44& world);

         Object::ObjectBase& _owner;                 //!< オブジェクトベースの参照
         std::vector<int> _modelHandles{ -1 };       //!< モデルハンドル
         bool _isLighting{ true };     //!< ライティングを行うかのフラグ
         bool _isPixelLighting{ false };
      };
   }
}
