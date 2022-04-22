
/*****************************************************************//**
 * \file   ClearScore.cpp
 * \brief  クリアスコアを表示する
 * 
 * \author NAOFUMISATO
 * \date   March 2022
 *********************************************************************/
#include "ClearScore.h"
#include "GameMain.h"
#include "ParamModeClear.h"

namespace {
   constexpr auto OneSecFrame = 60;
}

using namespace FragmentValkyria::Clear;

ClearScore::ClearScore() {
   _param = std::make_unique < Param::ParamModeClear >("clear");
}

void ClearScore::Init() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _IntParam = [&](std::string paramName) {
      return _param->GetIntParam(paramName);
   };
   _position = _param->GetVecParam("score_pos");
   auto handles = GetResServer().GetTextures("ClearScore");
   auto gameInstance = Game::GameMain::GetInstance();
   auto timer = static_cast<int>(gameInstance->ingameTimer());
   const auto CScoreSec = _IntParam("c_scoresec");
   const auto BScoreSec = _IntParam("b_scoresec");
   const auto AScoreSec = _IntParam("a_scoresec");
   const auto SScoreSec = _IntParam("s_scoresec");
   if (timer >= OneSecFrame * CScoreSec) {
      _grHandle = handles[3];
   }
   else if (timer >= OneSecFrame * BScoreSec && OneSecFrame * CScoreSec > timer) {
      _grHandle = handles[2];
   }
   else if (timer >= OneSecFrame * AScoreSec && OneSecFrame * BScoreSec > timer) {
      _grHandle = handles[1];
   }
   else if (timer >= OneSecFrame * SScoreSec && OneSecFrame * AScoreSec > timer) {
      _grHandle = handles[0];
   }
   else if (OneSecFrame * SScoreSec > timer) {
      _grHandle = handles[4];
   }
}

void ClearScore::Draw() {
   SpriteBase::Draw();
}
