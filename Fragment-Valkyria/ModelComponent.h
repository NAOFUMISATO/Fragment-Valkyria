#pragma once
/*****************************************************************//**
 * \file   ModelComponent.h
 * \brief  モデルの管理クラス
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
   namespace Object {
      class ObjectBase;
   }
   /**
    * \brief モデル関係
    */
   namespace Model {
      /**
       * \class モデルの一括管理クラス
       * \brief モデル情報の管理を行う
       */
      class ModelComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
      public:
         /**
          * \brief コンストラクタ
          * \param owner オブジェクトの基底クラスの参照
          */
         ModelComponent(Object::ObjectBase& owner);
         /**
          * \brief デストラクタ
          */
         virtual ~ModelComponent();
         /**
          * \brief 初期化処理
          */
         virtual void Init();
         /**
          * \brief 更新処理
          */
         virtual void Update();
         /**
          * \brief 描画処理
          */
         virtual void Draw();
         /**
          * \brief モデルの設定
          * \param key ResourceServerに登録済みの任意の文字列
          * \param no 同一モデルの通し番号
          * \return 登録する通し番号
          */
         virtual int SetModel(std::string_view key, int no = 0);
         /**
          * \brief 座標の設定
          * \param position 座標
          */
         virtual void SetPosition(Vector4 position);
         /**
          * \brief 回転角の設定
          * \param rotation 回転角度
          */
         virtual void SetRotation(Vector4 rotation);
         /**
          * \brief 拡大率の設定
          * \param scale 拡大率
          */
         virtual void SetScale(Vector4 scale);
         /**
          * \brief ワールド行列の設定
          * \param world ワールド行列
          */
         virtual void SetMatrix(Matrix44& world);
         /**
          * \brief モデルハンドルの設定
          * \param handle モデルハンドル
          */
         inline void modelHandle(int handle) { _modelHandle = handle; }
         /**
          * \brief モデルハンドルの取得
          * \return モデルハンドル
          */
         inline int modelHandle() { return _modelHandle; }
         /**
          * \brief モデルの拡散反射光色の設定
          * \param index 設定するモデルのマテリアル番号
          * \param red 赤
          * \param green 緑
          * \param blue 青
          */
         void SetDifColor(int index, float red, float green, float blue);
         /**
          * \brief モデルの鏡面反射光色の設定
          * \param index 設定するモデルのマテリアル番号
          * \param red 赤
          * \param green 緑
          * \param blue 青
          */
         void SetSpcColor(int index, float red, float green, float blue);
         /**
          * \brief モデルの自己発光色の設定
          * \param index 設定するモデルのマテリアル番号
          * \param red 赤
          * \param green 緑
          * \param blue 青
          */
         void SetEmiColor(int index, float red, float green, float blue);
         /**
          * \brief モデルの環境光色の設定
          * \param index 設定するモデルのマテリアル番号
          * \param red 赤
          * \param green 緑
          * \param blue 青
          */
         void SetAmbColor(int index, float red, float green, float blue);
         /**
          * \brief モデルの鏡面反射光の強さの設定
          * \param index 設定するモデルのマテリアル番号
          * \param power 設定する強さ
          */
         void SetSpcPower(int index, float power);
         /**
          * \brief モデルのブレンドモードを加算モードに設定
          * \param index 設定するモデルのマテリアル番号
          */
         void SetBlendModeAdd(int index);
         /**
          * \brief モデルのブレンドモード設定を解除する
          * \param index 解除するモデルのマテリアル番号
          */
         void SetBlendModeReset(int index);
         /**
          * \brief モデルのブレンドモードのパラメーターを設定する
          * \param index 設定するモデルのマテリアル番号
          * \param param 設定するパラメーター
          */
         void SetBlendParam(int index,int param);
         /**
          * \brief ライティングフラグをfalseにする
          * 
          */
         inline void LightingOFF() { _isLighting = false; }
         /**
          * \brief ピクセルライティングをtrueにする
          */
         inline void PixelLightingON() { _isPixelLighting = true; }
         /**
          * \brief モデルから指定名のフレームを取得する
          * \param frameName フレーム名
          * \return フレーム番号
          */
         int FindFrame(std::string_view frameName);
         /**
          * \brief モデルの指定名のフレームから指定名の子フレームを取得する
          * \param frameName フレーム名
          * \param childName 子フレーム名
          * \return フレーム番号
          */
         int FindFrameChild(std::string_view frameName, std::string_view childName);
         /**
          * \brief モデルから指定名のフレームの位置を取得する
          * \param frameName フレーム名
          * \return 指定名のフレームの位置
          */
         Vector4 GetFramePosion(std::string_view frameName);
         /**
          * \brief モデルの指定名のフレームから指定名の子フレームの位置を取得する
          * \param frameName フレーム名
          * \param childName 子フレーム名
          * \return 指定名の子フレームの位置
          */
         Vector4 GetFrameChildPosion(std::string_view frameName, std::string_view childName);

      protected:
         Object::ObjectBase& _owner;   //!< オブジェクトの基底クラスの参照
         std::string _key;             //!< モデル及びアニメーションで使用するキー
         int _modelHandle{ -1 };       //!< モデルハンドル
         bool _isLighting{ true };     //!< ライティングを行うかのフラグ
         bool _isPixelLighting{false}; //!< ピクセル単位でライティングを行うかのフラグ
      };
   }
}