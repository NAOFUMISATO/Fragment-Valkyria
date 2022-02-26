#pragma once
/*****************************************************************//**
 * \file   SoundComponent.h
 * \brief  音源の一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <string_view>
#include "Vector4.h"
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   //二重インクルード防止
   namespace Game {
      class GameBase;
   }
   /**
    * \brief 音源関係
    */
   namespace Sound {
      /**
       * \class 音源の一括管理クラス
       * \brief 音源の再生、音量変更を行う
       */
      class SoundComponent {
      public:
         /**
          * \brief コンストラクタ
          * \param gameBase アプリケーションの基底クラスの参照
          */
         SoundComponent(Game::GameBase& gameBase) :_gameBase{ gameBase } {};
         /**
          * \brief デストラクタ
          */
         virtual ~SoundComponent() = default;
         /**
          * \brief 任意の文字列から音源を指定し、バックグラウンド再生を行う
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          * \param pos 3Dサウンドの場合に再生する位置
          */
         void Play(std::string_view key, Math::Vector4 pos = {0,0,0});
         /**
          * \brief 任意の文字列から音源を指定し、ループ再生を行う
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          * \param pos 3Dサウンドの場合に再生する位置
          */
         void PlayLoop(std::string_view key, Math::Vector4 pos = { 0,0,0 });
         /**
          * \brief 音量の変更を行う
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          * \param changeVolume 変更先の音量
          */
         void ChangeVolume(std::string_view key, int changeVolume);
         /**
          * \brief 音源を停止する
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          */
         void Stop(std::string_view key);

      protected:
         /**
          * \brief 音量の変更を行った後、プレイタイプを指定し再生する
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          * \param playType 再生の種別
          * \param pos 3Dサウンドの場合に再生する位置
          */
         void Play(std::string_view key, int playType, Math::Vector4 pos);

      private:
         Game::GameBase& _gameBase;   //!< ゲームベースの参照
      };
   }
}