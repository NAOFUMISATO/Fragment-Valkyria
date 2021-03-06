
/*****************************************************************//**
 * \file   CameraComponent.cpp
 * \brief  カメラ管理
 *
 * \author NAOFUMISATO, YUSUKEAHAMEDO
 * \date   December 2021
 *********************************************************************/
#include "CameraComponent.h"
#include "Game.h"

namespace {
   constexpr auto ScreenWidth = 1920;   //!< 横画面サイズ
   constexpr auto ScreenHeight = 1080;  //!< 縦画面サイズ
}

using namespace FragmentValkyria::Camera;

CameraComponent::CameraComponent() {
   _param = std::make_unique<Param::ParamCameraComponent>("cameracomponent");
}

void CameraComponent::Init() {
    // プレイヤーからのカメラの注視点へのベクトルを作成
    _firstPlyToTarget = _target - _plyPos;
    // プレイヤーからのカメラの位置へのベクトルを作成
    _firstPlyToPos = _position - _plyPos;
    // ベクトルを90度回転させるマトリクスの作成
    _anyAxisMatrix.RotateY(90.0, true);
    // カメラの位置から注視点へのベクトルの作成
    auto posToTarget = _target - _position;
    posToTarget.Normalized();
    _posToTarget = posToTarget * 300.0;
    // カメラの描画限界および、視野角の初期化
    _nearFarFov = { 100.0,10000.0,AppFrame::Math::Utility::DegreeToRadian(60.0) };
}

void CameraComponent::Input(AppFrame::Input::InputManager& input) {
    // 各状態の入力処理を回す
    _stateServer->Input(input);
}

void CameraComponent::Update() {
    // 各状態の更新処理を回す
    _stateServer->Update();

    // ビュー行列の設定
    auto cameraMatrix = GetCameraViewMatrix(_position, _target, _up);
    SetCameraViewMatrix(ToDX(cameraMatrix));

    // 投影行列の設定
    auto [cameraNear, cameraFar, fov] = _nearFarFov;
    auto projectionMatrix = GetCameraProjectionMatrix(cameraNear, cameraFar, fov);
    SetupCamera_ProjectionMatrix(ToDX(projectionMatrix));

    // 3Dサウンドのリスナー設定(カメラと同一)
    Set3DSoundListenerPosAndFrontPosAndUpVec(ToDX(_position), ToDX(_target), ToDX(_up));
}

AppFrame::Math::Matrix44 CameraComponent::GetCameraViewMatrix(AppFrame::Math::Vector4& cameraPosition, AppFrame::Math::Vector4& cameraTarget, AppFrame::Math::Vector4& cameraUp) {
   Matrix44 cameraMatrix = Matrix44();
   // カメラ視点の設定
   cameraMatrix.LookAt(cameraPosition, cameraTarget, cameraUp);

   return cameraMatrix;
}

AppFrame::Math::Matrix44 CameraComponent::GetCameraProjectionMatrix(double cameraNear, double cameraFar, double fov) {
   auto aspect = static_cast<float>(ScreenHeight) / static_cast<float>(ScreenWidth);
   Matrix44 projection_matrix = Matrix44();
   // 透視変換
   projection_matrix.Perspective(fov, aspect, cameraNear, cameraFar);

   return projection_matrix;
}

void CameraComponent::Rotate() {
    // プレイヤーの位置からの注視点へのベクトルを作成
    _plyToTarget = _firstPlyToTarget * _rotateMatrix;
    // ズーム率をサインの値から算出
    auto sinValue = std::sin(_zoomRateRadian);
    _zoomRate = _posToTarget * sinValue;
    // プレイヤーからカメラの位置へのベクトルを作成
    _plyToPos = (_firstPlyToPos + _zoomRate) * _rotateMatrix;
}

void CameraComponent::Placement() {
    // プレイヤーの位置からカメラの注視点を設定する
    _target = _plyPos + _plyToTarget;
    // プレイヤーの位置からカメラの位置を設定する
    _position = _plyPos + _plyToPos;
    _position.Add(0.0, _vibrationValue, 0.0);
}

void CameraComponent::Vibration() {
   /**
    * \brief int型の値を文字列で指定し、値管理クラスから取得する
    * \param paramName 値を指定する文字列
    * \return 文字列により指定された値
    */
   const auto _DoubleParam = [&](std::string paramName) {
      return _param->GetDoubleParam(paramName);
   };
   const auto DivideT = _DoubleParam("divide_t");
   // オイラー法の時間の分割数分for文を回す
   for (auto i = 0.0; i < DivideT; ++i) {
      // 時間の分割数で割った速度をYの位置に足していく
      _vibrationValue += _vibrationVelocity / DivideT;
      // 速度に時間の分割数で割ったYの位置から自然長を引いたものにばね定数を掛けたものを足していく
      _vibrationVelocity += (-_DoubleParam("spring_k") * (_vibrationValue - _DoubleParam("center_height"))) / DivideT;
   }
   // 速度を減衰させる
   _vibrationVelocity *= 0.9;
}

void CameraComponent::StateNormal::Input(InputManager& input) {
   // ゲーム内感度の取得
   auto& gameInstance = Game::Game::GetInstance();
   auto [cameraSens, aimSens, deadZone] = gameInstance.sensitivity();
   // デッドゾーンの範囲を設定するためにデフォルトのデットゾーンに掛ける値の設定
   auto deadZoneMulti = _owner._param->GetIntParam("dead_zone_divide_value") / deadZone;
   // 右スティックが上に傾いていたらカメラの上下の回転の角度を増やす
   // デッドゾーン以上でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
   if (input.GetXJoypad().RightStickY() >= deadZone && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti) {
      // カメラ感度を0.2倍したものを足す
      _owner._upDownAngle += cameraSens * 0.2;
      // 10度以上傾いていたら10度にする
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 2) {
      // カメラ感度を0.4倍したものを足す
      _owner._upDownAngle += cameraSens * 0.4;
      // 10度以上傾いていたら10度にする
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 3) {
      // カメラ感度を0.6倍したものを足す
      _owner._upDownAngle += cameraSens * 0.6;
      // 10度以上傾いていたら10度にする
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 4) {
      // カメラ感度を0.8倍したものを足す
      _owner._upDownAngle += cameraSens * 0.8;
      // 10度以上傾いていたら10度にする
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より大きかった場合
   else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 4) {
      // カメラ感度を足す
      _owner._upDownAngle += cameraSens;
      // 10度以上傾いていたら10度にする
      if (_owner._upDownAngle >= 10.0) {
         _owner._upDownAngle = 10.0;
      }
   }
   // 右スティックが下に傾いていたらカメラの上下の回転の角度を減らす
   // デッドゾーン以下でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
   if (input.GetXJoypad().RightStickY() <= -deadZone && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti) {
      // カメラ感度を0.2倍したものを引く
      _owner._upDownAngle -= cameraSens * 0.2;
      // -20度以上傾いていたら-20度にする
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 2) {
      // カメラ感度を0.4倍したものを引く
      _owner._upDownAngle -= cameraSens * 0.4;
      // -20度以上傾いていたら-20度にする
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 3) {
      // カメラ感度を0.6倍したものを引く
      _owner._upDownAngle -= cameraSens * 0.6;
      // -20度以上傾いていたら-20度にする
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 4) {
      // カメラ感度を0.8倍したものを引く
      _owner._upDownAngle -= cameraSens * 0.8;
      // -20度以上傾いていたら-20度にする
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より小さかった場合
   else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 4) {
      // カメラ感度を引く
      _owner._upDownAngle -= cameraSens;
      // -20度以上傾いていたら-20度にする
      if (_owner._upDownAngle <= -20.0) {
         _owner._upDownAngle = -20.0;
      }
   }
   // 右スティックが右に傾いていたらカメラの左右の回転の角度を減らす
   // デッドゾーン以上でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
   if (input.GetXJoypad().RightStickX() >= deadZone && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti) {
      // カメラ感度を0.2倍したものを引く
      _owner._sideAngle -= cameraSens * 0.2;
      // -360度以下になったら0度にする
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 2) {
      // カメラ感度を0.4倍したものを引く
      _owner._sideAngle -= cameraSens * 0.4;
      //-360度以下になったら0度にする
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 3) {
      // カメラ感度を0.6倍したものを引く
      _owner._sideAngle -= cameraSens * 0.6;
      // -360度以下になったら0度にする
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 4) {
      // カメラ感度を0.8倍したものを引く
      _owner._sideAngle -= cameraSens * 0.8;
      // -360度以下になったら0度にする
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より大きかった場合
   else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 4) {
      // カメラ感度を引く
      _owner._sideAngle -= cameraSens;
      // -360度以下になったら0度にする
      if (_owner._sideAngle <= -360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // 右スティックが左に傾いていたらカメラの左右の回転の角度を増やす
   // デッドゾーン以下でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
   if (input.GetXJoypad().RightStickX() <= -deadZone && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti) {
      // カメラ感度を0.2倍したものを足す
      _owner._sideAngle += cameraSens * 0.2;
      // 360度以上になったら0度にする
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 2) {
      // カメラ感度を0.4倍したものを足す
      _owner._sideAngle += cameraSens * 0.4;
      // 360度以上になったら0度にする
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 3) {
      // カメラ感度を0.6倍したものを足す
      _owner._sideAngle += cameraSens * 0.6;
      // 360度以上になったら0度にする
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 4) {
      // カメラ感度を0.8倍したものを足す
      _owner._sideAngle += cameraSens * 0.8;
      // 360度以上になったら0度にする
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より小さかった場合
   else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 4) {
      // カメラ感度を足す
      _owner._sideAngle += cameraSens;
      // 360度以上になったら0度にする
      if (_owner._sideAngle >= 360.0) {
         _owner._sideAngle = 0.0;
      }
   }
   // カメラの位置から注視点へのベクトル作成
   auto posToTarget = _owner._target - _owner._position;
   // カメラの位置から注視点へのベクトルを90度回転させる
   auto anyAxisVec = posToTarget * _owner._anyAxisMatrix;
   // 上下の回転のマトリクス作成
   Matrix44 rotateUpDown = Matrix44();
   rotateUpDown.RotateAnyVecQuaternion(anyAxisVec, _owner._upDownAngle, true);
   // 左右の回転のマトリクス作成
   Matrix44 rotateSide = Matrix44();
   rotateSide.RotateAnyVecQuaternion(Vector4(0.0, 1.0, 0.0), _owner._sideAngle, true);
   // 上下の回転と左右の回転を合わせたマトリクスを作成
   _owner._rotateMatrix = rotateSide * rotateUpDown;
}

void CameraComponent::StateNormal::Update() {
   // 回転処理
   _owner.Rotate();
   // 振動処理
   _owner.Vibration();
   // 移動処理
   _owner.Placement();
   // ズームする場合はズーム状態へ
   if (_owner._zoom) {
      _owner._stateServer->PushBack("ZoomIn");
   }
}

void CameraComponent::StateZoomIn::Enter() {
   // ズームの割合のサインの値を取るラジアンを0.0に
   _owner._zoomRateRadian = 0.0;
}

void CameraComponent::StateZoomIn::Update() {
    // ズームの割合のサインの値を取るラジアンを5ラジアン増やす
    _owner._zoomRateRadian += AppFrame::Math::RADIAN_1 * 5.0;
    // もし二分のパイラジアンより大きくなったら射撃準備状態へ
    if (_owner._zoomRateRadian >= AppFrame::Math::PI / 2.0) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("ShootReady");
    }
    // 回転処理
    _owner.Rotate();
    // 振動処理
    _owner.Vibration();
    // 移動処理
    _owner.Placement();
}

void CameraComponent::StateShootReady::Input(InputManager& input) {
   // 感度の取得
   auto& gameInstance = Game::Game::GetInstance();
    auto [cameraSens, aimSens, deadZone] = gameInstance.sensitivity();
    // デッドゾーンの範囲を設定するためにデフォルトのデットゾーンに掛ける値の設定
    auto deadZoneMulti = _owner._param->GetIntParam("dead_zone_divide_value") / deadZone;
    // 右スティックが上に傾いていたらカメラの上下の回転の角度を増やす
   // デッドゾーンより以上でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
    if (input.GetXJoypad().RightStickY() >= deadZone && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti) {
       // エイム感度を0.2倍したものを足す
       _owner._upDownAngle += aimSens * 0.2;
       // 10度以上傾いていたら10度にする
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 2) {
       // エイム感度を0.4倍したものを足す
       _owner._upDownAngle += aimSens * 0.4;
       // 10度以上傾いていたら10度にする
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 3) {
       // エイム感度を0.6倍したものを足す
       _owner._upDownAngle += aimSens * 0.6;
       // 10度以上傾いていたら10度にする
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() < deadZone * deadZoneMulti * 4) {
       // エイム感度を0.8倍したものを足す
       _owner._upDownAngle += aimSens * 0.8;
       // 10度以上傾いていたら10度にする
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より大きかった場合
    else if (input.GetXJoypad().RightStickY() >= deadZone * deadZoneMulti * 4) {
       // エイム感度を足す
       _owner._upDownAngle += aimSens;
       // 10度以上傾いていたら10度にする
       if (_owner._upDownAngle >= 10.0) {
          _owner._upDownAngle = 10.0;
       }
    }
    // 右スティックが下に傾いていたらカメラの上下の回転の角度を減らす
    // デッドゾーン以下でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
    if (input.GetXJoypad().RightStickY() <= -deadZone && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti) {
       // エイム感度を0.2倍したものを引く
       _owner._upDownAngle -= aimSens * 0.2;
       // -20度以上傾いていたら-20度にする
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 2) {
       // エイム感度を0.4倍したものを引く
       _owner._upDownAngle -= aimSens * 0.4;
       // -20度以上傾いていたら-20度にする
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 3) {
       // エイム感度を0.6倍したものを引く
       _owner._upDownAngle -= aimSens * 0.6;
       // -20度以上傾いていたら-20度にする
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickY() > -deadZone * deadZoneMulti * 4) {
       // エイム感度を0.8倍したものを引く
       _owner._upDownAngle -= aimSens * 0.8;
       // -20度以上傾いていたら-20度にする
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より小さかった場合
    else if (input.GetXJoypad().RightStickY() <= -deadZone * deadZoneMulti * 4) {
       // エイム感度を引く
       _owner._upDownAngle -= aimSens;
       // -20度以上傾いていたら-20度にする
       if (_owner._upDownAngle <= -20.0) {
          _owner._upDownAngle = -20.0;
       }
    }
    // 右スティックが右に傾いていたらカメラの左右の回転の角度を減らす
    // デッドゾーン以上でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
    if (input.GetXJoypad().RightStickX() >= deadZone && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti) {
       // エイム感度を0.2倍したものを引く
       _owner._sideAngle -= aimSens * 0.2;
       // -360度以下になったら0度にする
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 2) {
       // エイム感度を0.4倍したものを引く
       _owner._sideAngle -= aimSens * 0.4;
       //-360度以下になったら0度にする
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 3) {
       // エイム感度を0.6倍したものを引く
       _owner._sideAngle -= aimSens * 0.6;
       // -360度以下になったら0度にする
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() < deadZone * deadZoneMulti * 4) {
       // エイム感度を0.8倍したものを引く
       _owner._sideAngle -= aimSens * 0.8;
       // -360度以下になったら0度にする
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より大きかった場合
    else if (input.GetXJoypad().RightStickX() >= deadZone * deadZoneMulti * 4) {
       // エイム感度を引く
       _owner._sideAngle -= aimSens;
       // -360度以下になったら0度にする
       if (_owner._sideAngle <= -360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // 右スティックが左に傾いていたらカメラの左右の回転の角度を増やす
    // デッドゾーン以下でパッドのスティックの入力範囲を分割した範囲の最初の範囲内だった場合
    if (input.GetXJoypad().RightStickX() <= -deadZone && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti) {
       // エイム感度を0.2倍したものを足す
       _owner._sideAngle += aimSens * 0.2;
       // 360度以上になったら0度にする
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の二つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 2) {
       // エイム感度を0.4倍したものを足す
       _owner._sideAngle += aimSens * 0.4;
       // 360度以上になったら0度にする
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の三つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 2 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 3) {
       // エイム感度を0.6倍したものを足す
       _owner._sideAngle += aimSens * 0.6;
       // 360度以上になったら0度にする
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲内だった場合
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 3 && input.GetXJoypad().RightStickX() > -deadZone * deadZoneMulti * 4) {
       // エイム感度を0.8倍したものを足す
       _owner._sideAngle += aimSens * 0.8;
       // 360度以上になったら0度にする
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // パッドのスティックの入力範囲を分割した範囲の四つ目の範囲より小さかった場合
    else if (input.GetXJoypad().RightStickX() <= -deadZone * deadZoneMulti * 4) {
       // エイム感度を足す
       _owner._sideAngle += aimSens;
       // 360度以上になったら0度にする
       if (_owner._sideAngle >= 360.0) {
          _owner._sideAngle = 0.0;
       }
    }
    // カメラの位置から注視点へのベクトル作成
    auto posToTarget = _owner._target - _owner._position;
    // カメラの位置から注視点へのベクトルを90度回転させる
    auto anyAxisVec = posToTarget * _owner._anyAxisMatrix;
    // 上下の回転のマトリクス作成
    Matrix44 rotateUpDown = Matrix44();
    rotateUpDown.RotateAnyVec(anyAxisVec, _owner._upDownAngle, true);
    // 左右の回転のマトリクス作成
    Matrix44 rotateSide = Matrix44();
    rotateSide.RotateAnyVec(Vector4(0.0, 1.0, 0.0), _owner._sideAngle, true);
    // 上下の回転と左右の回転を合わせたマトリクスを作成
    _owner._rotateMatrix = rotateSide * rotateUpDown;
}

void CameraComponent::StateShootReady::Update() {
   // 回転処理
    _owner.Rotate();
    // 振動処理
    _owner.Vibration();
    // 移動処理
    _owner.Placement();
    // ズームしない場合ズームアウト状態へ
    if (!_owner._zoom) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("ZoomOut");
    }
}

void CameraComponent::StateZoomOut::Update() {
    // ズームの割合のサインの値を取るラジアンを5ラジアン減らす
    _owner._zoomRateRadian -= AppFrame::Math::RADIAN_1 * 5.0;
    // もし0ラジアンより小さくなったら通常状態へ
    if (_owner._zoomRateRadian <= 0.0) {
        _owner._stateServer->PopBack();
        _owner._stateServer->PushBack("Normal");
    }
    // 回転処理
    _owner.Rotate();
    // 振動処理
    _owner.Vibration();
    // 移動処理
    _owner.Placement();
}
