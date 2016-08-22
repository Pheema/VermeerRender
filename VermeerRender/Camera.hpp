#pragma once

#include <cmath>
#include "ray.hpp"
#include "vector3f.hpp"

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

        // レンズの焦点距離を設定する
        void
        SetLens(float focalLength) {
			m_hPerf = m_sensorHeight / focalLength;
		}

		// 与えられた座標に焦点をあわせる
		void
		FocusTo(const Vector3f& target)
		{ 
			m_focusDist = (target - pos).Length();
		}
        
        // 垂直画角から焦点距離を指定する
        void
        SetFOV(float fov)
        {
            m_hPerf = 2.0f * tan(0.5f * fov);
        }

        // カメラの方向を指定する
        void
        SetViewDir(const Vector3f& dir);
        
        // targetの方向に向かせる
        void
        LookAt(const Vector3f& target);

        // カメラ原点から画素(i, j)に向かう正規化されたベクトルを求める
        Ray
        PixelToRay(int i, int j, int imageWidth, int imageHeight) const;

        // 焦点距離を返す
        inline float
        GetFocalLength() const { return m_sensorHeight / m_hPerf; }

		// 有効口径を返す
		inline float
		GetApeture() const { return m_apeture; }

		inline void
		SetApeture(float apeture) { m_apeture = apeture; }

		// 有効口径をF値で指定する
		inline void
		SetFNumber(float fNumber) { m_apeture = GetFocalLength() / fNumber; }

        // 垂直画角を返す
        inline float
        GetFOV() { return 2.0f * atan(0.5f * m_sensorHeight / m_focusDist); }

		// レンズの面積を返す
		inline float
		GetLensArea() const { return M_PI * (0.5f * m_apeture) * (0.5f * m_apeture); }

        inline Vector3f 
        Right() const  { return m_right; }

        inline Vector3f
        Up() const { return m_up; }

        inline Vector3f
        Forward() const { return m_forward; }

    private:
        // カメラの各ローカル軸(正規化されている必要有り)
        Vector3f m_right;
        Vector3f m_up;
        Vector3f m_forward;
        
		float m_focusDist = 1.0;
		const float m_sensorHeight = 24e-3f;		// 標準センサーサイズ
		float m_hPerf = m_sensorHeight / 55e-3f;	// 初期レンズは55mm
		float m_apeture = 25e-3f;	// 有効口径(焦点距離(55mm)/F値(2.2))

        Vector3f m_worldUp = Vector3f::Up();
    };

    
}