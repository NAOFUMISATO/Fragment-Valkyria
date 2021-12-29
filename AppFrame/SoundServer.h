#pragma once
/*****************************************************************//**
 * \file   SoundServer.h
 * \brief  音源の一括管理クラス
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include <string_view>
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
      class SoundServer {
      public:
         /**
          * \brief コンストラクタ
          * \param appBase アプリケーションの基底クラスの参照
          */
         SoundServer(Game::GameBase& appBase) :_gameBase{ appBase } {};
         /**
          * \brief デストラクタ
          */
         virtual ~SoundServer() = default;
         /**
          * \brief 任意の文字列から音源を指定し、バックグラウンド再生を行う
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          */
         void Play(std::string_view key);
         /**
          * \brief 任意の文字列から音源を指定し、ループ再生を行う
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          */
         void PlayLoop(std::string_view key);
         /**
          * \brief 音量の変更を行う
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          * \param changeVolume 変更先の音量
          */
         void ChangeVolume(std::string_view key, int changeVolume);

      protected:
         /**
          * \brief 音量の変更を行った後、プレイタイプを指定し再生する
          * \param key ResourceServerで登録済みの音源を関連付けた任意の文字列
          * \param playType 再生の種別
          */
         void Play(std::string_view key, int playType);

      private:
         Game::GameBase& _gameBase;   //!< ゲームベースの参照
      };
   }
}