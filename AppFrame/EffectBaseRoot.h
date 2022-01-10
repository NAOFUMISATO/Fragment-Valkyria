#pragma once
/*****************************************************************//**
 * \file   EffectBaseRoot.h
 * \brief  エフェクト基底クラスの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Vector4.h"
#include "Color.h"
#include <string_view>
#include <EffekseerForDXLib.h>
#include <tuple>
/**
 * \brief アプリケーションフレーム
 */
namespace AppFrame{
   // 二重インクルード防止
   namespace Game {
      class GameBase;
   }
   namespace Input {
      class InputManager;
   }
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
     /**
      * \class エフェクト基底クラスの基底クラス
      * \brief ゲーム本体側でこのクラスを継承してエフェクト基底クラスを定義する
      */
      class EffectBaseRoot {
      public:
         /**
          * \brief エフェクトの状態列挙
          */
         enum class EffectState {
            Active,     //!< 生存
            Paused,     //!< 停止
            Dead        //!< 死亡
         };
         /**
          * \brief コンストラクタ
          * \param gameBase ゲームベースの参照
          */
         EffectBaseRoot(Game::GameBase& gameBase);
         /**
          * \brief デストラクタ
          */
         virtual ~EffectBaseRoot();
         /**
          * \brief 初期化及び_loadHandleにエフェクトハンドルを登録する
          * \param key ResourceServerに登録済みの任意の文字列
          */
         virtual void Init(std::string_view key);
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(Input::InputManager& input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update() {};
         /**
          * \brief 描画処理
          */
         virtual void Draw() {};
         /**
          * \brief 死亡判定を返す
          * \return 死亡状態ならtrue,でないならfalse
          */
         inline bool IsDead() { return (_efcState == EffectState::Dead); }
         /**
          * \brief 生存判定を返す
          * \return 生存状態ならtrue,でないならfalse
          */
         inline bool IsActive() { return (_efcState == EffectState::Active); }
         /**
          * \brief 状態を死亡に設定
          */
         void SetDead() { _efcState = EffectState::Dead; }
         /**
          * \brief エフェクトハンドルをResourceServerに登録されているエフェクトハンドルに設定する
          * \param key ResourceServerのエフェクトハンドルに関連付けている任意の文字列
          */
         void SetEffectLoadHandle(std::string_view key);
         /**
          * \brief エフェクトを再生する
          */
         void PlayEffect();
         /**
          * \brief エフェクトが再生中かの判定
          * \return 再生中ならtrue,でなければfalse
          */
         bool IsPlaying();
         /**
          * \brief エフェクトを停止する
          */
         void StopEffect();
         /**
          * \brief エフェクトの位置の取得
          * \return エフェクトの位置
          */
         inline Math::Vector4 position() { return _position; }
         /**
          * \brief エフェクトの位置の設定
          * \param pos エフェクトを設定する位置
          */
         void SetPosition(Math::Vector4 pos);
         /**
          * \brief エフェクトの回転角の取得
          * \return エフェクトの回転角
          */
         inline Math::Vector4 rotation() { return _rotation; }
         /**
          * \brief エフェクトの回転角の設定
          * \param rot エフェクトを設定する回転角
          */
         void SetRotation(Math::Vector4 rot);
         /**
          * \brief エフェクトの拡大率の取得
          * \return エフェクトの拡大率
          */
         inline Math::Vector4 scale() { return _scale; }
         /**
          * \brief エフェクトの拡大率の設定
          * \param sca エフェクトを設定する拡大率
          */
         void SetScale(Math::Vector4 sca);
         /**
          * \brief エフェクトの再生速度の取得
          * \return エフェクトの再生速度
          */
         double GetSpeed();
         /**
          * \brief エフェクトの再生速度の設定
          * \param speed エフェクトを設定する再生速度
          */
         void SetSpeed(double speed);
         /**
          * \brief エフェクトの色の取得
          * \return エフェクトの色
          */
         inline Color::Color color() { return _color; }
         /**
          * \brief エフェクトの色(RGBA)を設定
          * \param color エフェクトを設定する色(RGBA)
          */
         void SetEfcColor(Color::Color _color );

      protected:
         Game::GameBase& _gameBase;                    //!< ゲームベースの参照
         EffectState _efcState{ EffectState::Active }; //!< エフェクトの状態
         int _loadHandle{ -1 };                        //!< エフェクトを読み込むハンドル
         int _playHandle{ -1 };                        //!< エフェクトを再生するハンドル
         double _speed{ 10.0 };                        //!< 再生速度
         Math::Vector4 _position{ 0,0,0 };             //!< 位置
         Math::Vector4 _rotation{ 0,0,0 };             //!< 回転角
         Math::Vector4 _scale{ 1.0,1.0,1.0 };          //!< 拡大率
         Color::Color _color{ Color::Color() };        //!< 色(RGBA)
      };
   }
} 