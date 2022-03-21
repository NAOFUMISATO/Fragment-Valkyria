#pragma once
/*****************************************************************//**
 * \file   ModeServer.cpp
 * \brief  モードの一括管理
 *
 * \author NAOFUMISATO
 * \date   December 2021
 *********************************************************************/
#include "ModeServer.h"
#ifdef _DEBUG
#include <stdexcept>
#include <windows.h>
#include <time.h>
#include <DxLib.h>
#include "Utility.h"
#endif
#include "ModeBaseRoot.h"
#include "ModeFadeIn.h"
#include "ModeFadeOut.h"

namespace {
   constexpr auto CountMax = 4294967295;   //!< _frameCountの型unsigned intの範囲
#ifdef _DEBUG
   constexpr auto DrawFPSPosX = 0;         //!< デバッグ描画するFPSのX座標
   constexpr auto DrawFPSPosY = 0;         //!< デバッグ描画するFPSのY座標
   constexpr auto DrawFPSColor = 255;      //!< デバッグ描画するFPSの色
#endif
}
 /**
  * \brief アプリケーションフレーム
  */
namespace AppFrame {
   /**
    * \brief モード関係
    */
   namespace Mode {
      ModeServer::ModeServer(std::string_view key, std::shared_ptr<ModeBaseRoot> mode) {
         Register("FadeIn", std::make_shared<ModeFadeIn>(mode->gameBase()));
         Register("FadeOut", std::make_shared<ModeFadeOut>(mode->gameBase()));
         Register(key, mode);
         PushBack(key);           // 最初のシーンをプッシュバック
         PushBack("FadeIn");      // 最初のシーンの上にフェードインをプッシュバック
      }

      void ModeServer::Register(std::string_view key, std::shared_ptr<ModeBaseRoot> mode) {
         if (_modeRegistry.contains(key.data())) {
            _modeRegistry.erase(key.data());       // レジストリを走査し、指定のキーがあれば削除する
         }
         _modeRegistry.emplace(key, mode);  // 指定のモードをレジストリにキーとともに登録する
         mode->Init();                   // 指定のモードの初期化処理を行う
      }

      void ModeServer::PushBack(std::string_view key) {
#ifndef _DEBUG
         if (!_modeRegistry.contains(key.data())) {
            return;  // レジストリを走査し、指定のキーがなければ返す
         }
#else
         try {
            if (!_modeRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]がモードレジストリに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto pushMode = _modeRegistry[key.data()];
         pushMode->Enter();          // 指定のモードの入口処理を行う
         _modeList.push_back(pushMode); // リストの末尾に指定のモードを追加する
      }

      void ModeServer::PopBack() {
#ifndef _DEBUG
         if (_modeList.empty()) {
            return;  // リストに何もなければ返す
         }
#else
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------リスト末尾のモードを削除しようとしましたが、リストが空でした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _modeList.back()->Exit();  // 末尾のモードの出口処理を行う
         _modeList.pop_back();      // リストの末尾のモードを削除する
      }

      void ModeServer::GoToMode(std::string_view key, char fadeType) {
         InsertBelowBack(key.data());      // 指定のモードを現モードの真下に挿入
         FadeInsertBelowBack(fadeType);    // フェードインを現モードの真下に挿入（結果的に指定のモードの真上に挿入される）
         FadePushBack(fadeType);           // フェードアウトを現モードの真上に挿入
      }

      void ModeServer::InsertBelowBack(std::string_view key) {
#ifndef _DEBUG
         if (!_modeRegistry.contains(key.data())) {
            return;  // レジストリを走査し、指定のキーがなければ返す
         }
#else
         try {
            if (!_modeRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]がモードレジストリに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto insertMode = _modeRegistry[key.data()];
         insertMode->Enter();   // 指定のモードの入口処理を行う
         _modeList.insert(std::prev(_modeList.end()), insertMode);   // 指定のモードをリストの末尾分、後方に進んだ位置に挿入する
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetMode(std::string_view key) {
#ifndef _DEBUG
         if (!_modeRegistry.contains(key.data())) {
            return nullptr;   //指定のキーがなければnullを返す
         }
#else
         try {
            if (!_modeRegistry.contains(key.data())) {
               std::string message = key.data();
               throw std::logic_error("----------キー[" + message + "]がモードレジストリに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         return _modeRegistry[key.data()];   //指定のキーのモードを返す
      }

      void ModeServer::Input(Input::InputManager& input) {
#ifndef _DEBUG
         if (_modeList.empty()) {
            return;  // リストに何もなければ返す
         }
#else
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------リスト末尾のモードの入力処理を実行しようとしましたが、リストが空でした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }

         _fpsCount = clock();
#endif
         _modeList.back()->Input(input);   //リストの末尾のモードのみ入力処理を回す
      }

      void ModeServer::Update() {
#ifndef _DEBUG
         if (_modeList.empty()) {
            return;  // リストに何もなければ返す
         }
#else
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------リスト末尾のモードの更新処理を実行しようとしましたが、リストが空でした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         _modeList.back()->Update();   //リストの末尾のモードのみ更新処理を回す
         
         // カウントがunsigned intの範囲を超えるなら0にする
         if (_frameCount >= CountMax) {
            _frameCount = 0;
         }
         _frameCount++;   //ゲームのフレームを進める
      }

      void ModeServer::Render() const {
         for (auto&& mode : _modeList) {
            mode->Render();   //リストの全てのモードの描画処理を回す
         }
#ifdef _DEBUG
         auto nowCount = clock();
         DrawFormatString(DrawFPSPosX, DrawFPSPosY, 
            AppFrame::Math::Utility::GetColorCode(DrawFPSColor, DrawFPSColor, DrawFPSColor), "FPS : %d" , nowCount-_fpsCount);
#endif
      }

      void ModeServer::FadeInsertBelowBack(char fadeType) {
#ifndef _DEBUG
         if (!_modeRegistry.contains("FadeIn")) {
            return;   // レジストリを走査し、指定のキー(フェード)がなければ返す
         }
#else
         try {
            if (!_modeRegistry.contains("FadeIn")) {
               std::string message = "FadeIn";
               throw std::logic_error("----------キー[" + message + "]がモードレジストリに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto insertMode = _modeRegistry["FadeIn"];
         insertMode->fadeType(fadeType);     // フェード時間の設定
         insertMode->Enter();                // 指定のモード(フェード)の入口処理
         _modeList.insert(std::prev(_modeList.end()), insertMode);  // 指定のモード(フェード)をリストの末尾分、後方に進んだ位置に挿入する
      }

      void ModeServer::FadePushBack(char fadeType) {
#ifndef _DEBUG
         if (!_modeRegistry.contains("FadeOut")) {
            return;   // レジストリを走査し、指定のキー(フェード)がなければ返す
         }
#else
         try {
            if (!_modeRegistry.contains("FadeOut")) {
               std::string message = "FadeOut";
               throw std::logic_error("----------キー[" + message + "]がモードレジストリに存在しませんでした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         auto pushMode = _modeRegistry["FadeOut"];
         pushMode->fadeType(fadeType);     // フェード時間の設定
         pushMode->Enter();                // 指定のモード(フェード)入口処理
         _modeList.push_back(pushMode);    // リストの末尾に指定のモード(フェード)を追加する
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetNowMode() {
         return _modeList.back();
      }
   }
}