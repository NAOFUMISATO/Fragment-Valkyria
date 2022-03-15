#pragma once
/*****************************************************************//**
 * \file   EffectBase.h
 * \brief  エフェクトの基底クラス
 * 
 * \author NAOFUMISATO
 * \date   January 2022
 *********************************************************************/
#include "AppFrame.h"
/**
 * \brief プロジェクト名
 */
namespace FragmentValkyria {
   //二重インクルード防止
   namespace Game {
      class GameMain;
   }
   /**
    * \brief エフェクト関係
    */
   namespace Effect {
      /**
       * \class エフェクトの基底クラス
       * \brief 各エフェクトはこのクラスを派生して定義する
       */
      class EffectBase {
         using Vector4 = AppFrame::Math::Vector4;
         using Color = AppFrame::Data::Color;
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
          * \brief エフェクトの種別列挙
          */
         enum class EffectType {
            Effect,             //!< エフェクト
            GatlingBullet,      //!< ガトリング弾
            GatlingMuzzleFlash, //!< ガトリングの発火炎(マズルフラッシュ)
            ObjectFall,         //!< オブジェクト落下
            Heal,               //!< 回復
            WeakBullet,         //!< プレイヤー弱攻撃
            BossBeam,           //!< ボスビーム攻撃
            BossCharge,         //!< ボスビームチャージ
            BossStan,           //!< ボスのスタン
            BossDieAfter,       //!< ボスの死亡後
            BossFall,           //!< ボス落下
            ObjectUp,           //!< オブジェクト上昇
            PoorCrash,          //!< 雑魚敵の破壊
            BossCrash,          //!< ボスの破壊
            PreliminaryLight    //!< 予備動作
         };
         /**
          * \brief コンストラクタ
          * \param gameMain ゲーム本体クラスの参照
          * \param key ResourceServerに登録した任意の文字列のエフェクトキー
          */
         EffectBase(Game::GameMain& gameMain,std::string_view key);
         /**
          * \brief 初期化処理
          */
         virtual void Init() {};
         /**
          * \brief 入力処理
          * \param input 入力一括管理クラスの参照
          */
         virtual void Input(AppFrame::Input::InputManager& input) {};
         /**
          * \brief 更新処理
          */
         virtual void Update();
         /**
          * \brief 描画処理
          */
         virtual void Draw() {};
         /**
          * \brief エフェクトの種別を返す（純粋仮想関数化）
          * \return 派生先で定義
          */
         virtual EffectType GetEfcType() const = 0;
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
         inline Vector4 position() const { return _position; }
         /**
          * \brief エフェクトの位置の設定
          * \return エフェクトを設定する位置
          */
         void position(Vector4 pos) { _position = pos; }
         /**
          * \brief エフェクシアによる位置の設定
          */
         void SetPosition();
         /**
          * \brief エフェクトの回転角の取得
          * \return エフェクトの回転角
          */
         inline Vector4 rotation() const { return _rotation; }
         /**
          * \brief エフェクトの回転角の設定
          * \param rot エフェクトを設定する回転角
          */
         void rotation(Vector4 rot) { _rotation = rot; }
         /**
          * \brief エフェクシアによる回転角の設定
          */
         void SetRotation();
         /**
          * \brief エフェクトの拡大率の取得
          * \return エフェクトの拡大率
          */
         inline Vector4 scale() const { return _scale; }
         /**
          * \brief エフェクトの拡大率の設定
          * \param sca エフェクトを設定する拡大率
          */
         void scale(Vector4 sca) { _scale = sca; }
         /**
          * \brief エフェクシアによる拡大率の設定
          */
         void SetScale();
         /**
          * \brief エフェクトの再生速度の取得
          * \return エフェクトの再生速度
          */
         double speed()const { return _speed; }
         /**
          * \brief エフェクトの再生速度の設定
          * \param speed 設定する再生速度
          */
         void speed(double speed) { _speed = speed; }
         /**
          * \brief エフェクシアによる再生速度の設定
          */
         void SetSpeed();
         /**
          * \brief エフェクトの色の取得
          * \return エフェクトの色
          */
         inline AppFrame::Data::Color color() const { return _color; }
         /**
          * \brief エフェクトの色の設定
          * \param color 設定する色
          */
         void color(Color color) { _color = color; }
         /**
          * \brief エフェクシアによる色の設定
          */
         void SetEfcColor();

      protected:
         Game::GameMain& _gameMain;                    //!< ゲーム本体クラスの参照
         EffectState _efcState{ EffectState::Active }; //!< エフェクトの状態
         int _loadHandle{ -1 };                        //!< エフェクトを読み込むハンドル
         int _playHandle{ -1 };                        //!< エフェクトを再生するハンドル
         double _speed{ 10.0 };                        //!< 再生速度
         Vector4 _position{ 0,0,0 };                   //!< 位置
         Vector4 _rotation{ 0,0,0 };                   //!< 回転角
         Vector4 _scale{ 1.0,1.0,1.0 };                //!< 拡大率
         Color _color{ Color() };                      //!< 色(RGBA)
      };
}
}
