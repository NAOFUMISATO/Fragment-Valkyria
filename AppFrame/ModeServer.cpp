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
         // フェードイン、フェードアウトモードを登録する
         Register("FadeIn", std::make_shared<ModeFadeIn>());
         Register("FadeOut", std::make_shared<ModeFadeOut>());
         // 最初のモードを登録する
         Register(key, mode);
         // 最初のモードを挿入
         PushBack(key);
         // 最初のモードの上にフェードインを挿入
         PushBack("FadeIn");
      }

      void ModeServer::Register(std::string_view key, std::shared_ptr<ModeBaseRoot> mode) {
         // レジストリを走査し、指定のキーがあれば削除する
         if (_modeRegistry.contains(key.data())) {
            _modeRegistry.erase(key.data()); 
         }
         // 指定のモードをレジストリにキーとともに登録する
         _modeRegistry.emplace(key, mode);
         // 指定のモードの初期化処理を行う
         mode->Init();
      }

      void ModeServer::PushBack(std::string_view key) {
#ifndef _DEBUG
           // レジストリを走査し、指定のキーがなければ返す
         if (!_modeRegistry.contains(key.data())) {
            return;
         }
#else
         // レジストリを走査し、指定のキーがなければエラーメッセージを出力する
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
         // 指定のモードの入口処理を行う
         pushMode->Enter();
         // リストの末尾に指定のモードを追加する
         _modeList.push_back(pushMode);
      }

      void ModeServer::PopBack() {
#ifndef _DEBUG
         // リストに何もなければ返す
         if (_modeList.empty()) {
            return;
         }
#else
         // リストに何もなければエラーメッセージを出力する
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------リスト末尾のモードを削除しようとしましたが、リストが空でした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         // 末尾のモードの出口処理を行う
         _modeList.back()->Exit();
         // リストの末尾のモードを削除する
         _modeList.pop_back();
      }

      void ModeServer::GoToMode(std::string_view key, char fadeType) {
         // 指定のモードを現モードの真下に挿入
         InsertBelowBack(key.data());
         // フェードインを現モードの真下に挿入（結果的に指定のモードの真上に挿入される）
         FadeInsertBelowBack(fadeType);
         // フェードアウトを現モードの真上に挿入
         FadePushBack(fadeType);
      }

      void ModeServer::InsertBelowBack(std::string_view key) {
#ifndef _DEBUG
         // レジストリを走査し、指定のキーがなければ返す
         if (!_modeRegistry.contains(key.data())) {
            return;
         }
#else
         // レジストリを走査し、指定のキーがなければエラーメッセージを出力する
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
         // 指定のモードの入口処理を行う
         insertMode->Enter();
         // 指定のモードをリストの末尾分、後方に進んだ位置に挿入する
         _modeList.insert(std::prev(_modeList.end()), insertMode);
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetMode(std::string_view key) {
#ifndef _DEBUG
         //指定のキーがなければnullを返す
         if (!_modeRegistry.contains(key.data())) {
            return nullptr;
         }
#else
         //指定のキーがなければエラーメッセージを出力する
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
         //指定のキーのモードを返す
         return _modeRegistry[key.data()];
      }

      void ModeServer::Input(Input::InputManager& input) {
#ifndef _DEBUG
         // リストに何もなければ返す
         if (_modeList.empty()) {
            return;
         }
#else
         // リストに何もなければエラーメッセージを出力する
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------リスト末尾のモードの入力処理を実行しようとしましたが、リストが空でした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
         // FPSをミリ秒単位で計測するため始点
         _fpsCount = clock();
#endif
         //リストの末尾のモードのみ入力処理を回す
         _modeList.back()->Input(input);
      }

      void ModeServer::Update() {
#ifndef _DEBUG
         // リストに何もなければ返す
         if (_modeList.empty()) {
            return;
         }
#else
         // リストに何もなければエラーメッセージを出力する
         try {
            if (_modeList.empty()) {
               throw std::logic_error("----------リスト末尾のモードの更新処理を実行しようとしましたが、リストが空でした。----------\n");
            }
         }
         catch (std::logic_error& error) {
            OutputDebugString(error.what());
         }
#endif
         //リストの末尾のモードのみ更新処理を回す
         _modeList.back()->Update();
         
         // カウントがunsigned intの範囲を超えるなら0にする
         if (_frameCount >= CountMax) {
            _frameCount = 0;
         }
         _frameCount++;   //ゲームのフレームを進める
      }

      void ModeServer::Render() const {
         //リストの全てのモードの描画処理を回す
         for (auto&& mode : _modeList) {
            mode->Render();
         }
#ifdef _DEBUG
         // FPSをミリ秒単位で計測するため終点
         auto nowCount = clock();
         // FPSをデバッグ描画する
         DrawFormatString(DrawFPSPosX, DrawFPSPosY, 
            AppFrame::Math::Utility::GetColorCode(DrawFPSColor, DrawFPSColor, DrawFPSColor), "FPS : %d" , nowCount-_fpsCount);
#endif
      }

      void ModeServer::FadeInsertBelowBack(char fadeType) {
#ifndef _DEBUG
         // レジストリを走査し、指定のキー(フェード)がなければ返す
         if (!_modeRegistry.contains("FadeIn")) {
            return;
         }
#else
         // レジストリを走査し、指定のキー(フェード)がなければエラーメッセージを出力する
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
         // フェード時間の設定
         insertMode->fadeType(fadeType);
         // 指定のモード(フェード)の入口処理
         insertMode->Enter();
         // 指定のモード(フェード)をリストの末尾分、後方に進んだ位置に挿入する
         _modeList.insert(std::prev(_modeList.end()), insertMode);
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
         // フェード時間の設定
         pushMode->fadeType(fadeType);
         // 指定のモード(フェード)入口処理
         pushMode->Enter();
         // リストの末尾に指定のモード(フェード)を追加する
         _modeList.push_back(pushMode);
      }

      std::shared_ptr<ModeBaseRoot> ModeServer::GetNowMode() {
         // 末尾のモードを取得する
         return _modeList.back();
      }
   }
}