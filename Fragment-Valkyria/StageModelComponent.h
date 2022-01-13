#pragma once
#include "AppFrame.h"

namespace FragmentValkyria {
   namespace Stage {
      class Stage;
      class StageModelComponent {
         using Vector4 = AppFrame::Math::Vector4;
         using Matrix44 = AppFrame::Math::Matrix44;
      public:
         StageModelComponent(Stage& stage);
         ~StageModelComponent();

         void Init();

         void Update();

         void Draw();

         void SetModels(std::string_view key);

         inline void modelHandles(std::vector<int> handles) { _modelHandles = handles; }

         inline std::vector<int> modelHandles() { return _modelHandles; }

         void SetEmiColor(int index, float r, float g, float b);

      protected:
         void SetPosition(int handle, Vector4 position);

         void SetRotation(int handle, Vector4 rotation);

         void SetScale(int handle, Vector4 scale);

         void SetMatrix(int handle, Matrix44& world);

         Stage& _stage;
         std::vector<int> _modelHandles{ -1 };       //!< モデルハンドル
         bool _isLighting{ true };     //!< ライティングを行うかのフラグ
      };
   }
}
