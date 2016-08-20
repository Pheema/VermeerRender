#include "camera.hpp"

namespace VermeerRender
{
    void
    Camera::SetViewDir(const Vector3f& dir)
    {
        m_forward = dir.Normalized();
        m_right = Cross(m_forward, m_worldUp);
        m_up = Cross(m_right, m_forward);
    }

    void
    Camera::LookAt(const Vector3f& target)
    {
        SetViewDir(target - pos);
    }

    Ray
    Camera::PixelToRay(int i, int j, int imageWidth, int imageHeight) const
    {
        const float aspect = static_cast<float>(imageWidth) / imageHeight;
        const float u = (i + 0.5f) / imageWidth - 0.5f;
        const float v = (j + 0.5f) / imageHeight - 0.5f;
        
        Vector3f rayDir =
            m_right * u * m_sensorHeight * aspect +
            -m_up * v * m_sensorHeight +
            m_forward * m_focalLength;
        rayDir.Normalize();

        return Ray(pos, rayDir, RayTypes::CAMERA);
    }
}