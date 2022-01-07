#pragma once
/*****************************************************************//**
 * \file   EffectBaseRoot.h
 * \brief  エフェクト基底クラスの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "Vector4.h"
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
          * \brief 初期化処理
          */
         virtual void Init() {};
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
         void PlayEffect();
         bool IsPlaying();
         void StopEffect();
         inline Math::Vector4 position() { return _position; }
         void SetPosition(Math::Vector4 pos);
         inline Math::Vector4 rotation() { return _rotation; }
         void SetRotation(Math::Vector4 rot);
         inline Math::Vector4 scale() { return _scale; }
         void SetScale(Math::Vector4 sca);
         double GetSpeed();
         void SetSpeed(double speed);
         std::tuple<int, int, int, int> GetEfcColor() { return _color; }
         void SetEfcColor(std::tuple<int, int, int, int> color);

      protected:
         Game::GameBase& _gameBase;
         EffectState _efcState{ EffectState::Active };
         int _loadHandle{ -1 };
         int _playHandle{ -1 };
         Math::Vector4 _position{ 0,0,0 };
         Math::Vector4 _rotation{ 0,0,0 };
         Math::Vector4 _scale{ 0,0,0 };
         std::tuple<int, int, int, int> _color{255,255,255,255};
      };
   }
} 