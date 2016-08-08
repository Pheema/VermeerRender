#pragma once

#include <cmath>
#include "Ray.hpp"
#include "Vector3f.hpp"

namespace VermeerRender
{
    class Camera
    {
    public:
        Vector3f pos;
        
        Camera() {}
        Camera(const Vector3f& pos, const Vector3f& dir) : pos(pos)
        {
            SetViewDir(dir);
        }

        // 焦点距離を直接指定する
        void
        SetFocalLength(float focalLength) { m_focalLength = focalLength; }
        
        // 垂直画角から焦点距離を指定する
        void
        SetFOV(float fov)
        {
            m_focalLength = 0.5f * m_sensorHeight / tan(0.5f * fov);
        }

        // カメラの方向を指定する
        void
        SetViewDir(const Vector3f& dir);
        
        // targetの方向に向かせる
        void
        LookAt(const Vector3f& target);

        // カメラ原点から画素(i, j)に向かう正規化されたベクトルを求める
        Ray
        PixelToRay(int i, int j, int imageWidth, int imageHeight);

        // 焦点距離を返す
        float
        GetFocalLength() { return m_focalLength; }

        // 垂直画角を返す
        float
        GetFOV() { return 2.0f * atan(0.5f * m_sensorHeight / m_focalLength); }

        inline Vector3f 
        Right() { return m_right; }

        inline Vector3f
        Up() { return m_up; }

        inline Vector3f
        Forward() { m_forward; }

    private:
        // カメラの各ローカル軸(正規化されている必要有り)
        Vector3f m_right;
        Vector3f m_up;
        Vector3f m_forward;
        
        float m_focalLength = 55e-3f;   // 55mm標準レンズ
        float m_sensorHeight = 24e-3f;  // フルサイズセンサー

        Vector3f m_worldUp = Vector3f::Up();
    };

    
}