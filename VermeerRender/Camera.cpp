#include "camera.hpp"
#include "xorShift128.hpp"
#include <random>

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
		// Random
		static XorShift128 xor;
		std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);

        const float aspect = static_cast<float>(imageWidth) / imageHeight;
        const float u = (i + uniDist(xor)) / imageWidth - 0.5f;
        const float v = (j + uniDist(xor)) / imageHeight - 0.5f;
		// 0.5f -> uniDist(xor)
        Vector3f pointOnSensor =
			pos +
            m_right * u * m_hPerf * m_focusDist * aspect +
            -m_up * v * m_hPerf * m_focusDist +
            m_forward * m_focusDist;
        
		float r = sqrt(uniDist(xor));
		float theta = 2.0f * M_PI * uniDist(xor);
		Vector3f pointOnLens =
			pos +
			m_right * 0.5f * m_apeture * r * cos(theta) +
			-m_up * 0.5f * m_apeture * r * sin(theta);

		Vector3f rayDir = (pointOnSensor - pointOnLens).Normalized();
		
		float dot = Dot(rayDir, Forward());
		float weight = dot * dot * m_focusDist * m_focusDist / (pointOnSensor - pointOnLens).SqLength();

		Ray cameraRay = Ray(pointOnLens, rayDir, RayTypes::CAMERA);
		cameraRay.weight = weight;

        return cameraRay;
    }
}