#pragma once

#include <vector>
#include "camera.hpp"
#include "geometricObject.hpp"
#include "texture2D.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
    class Scene
    {
    public:
        Scene() {}

        void
        SetCamera(Camera& camera)
        {
            m_cameraPtr = &camera;
        }

        void
        SetBGColor(const Color3f& color)
        {
            m_bgColor = color;
        }

		const Color3f&
		GetBGColor(const Vector3f& dir) const
		{
			if (m_bgTexture != nullptr)
			{
				return m_bgTexture->GetPixel(dir);
			}
			return m_bgColor;
		}

		void
		SetBGTexture(const Texture2D& bgTex)
		{
			m_bgTexture = &bgTex;
		}

        void
        AddGeoObject(GeometricObject& obj)
        {
			if (&(obj.GetMaterial()) == nullptr)
			{
				std::cout << "No material is assigned." << std::endl;
				exit(1);
			}
            m_geoObjectPtrs.push_back(&obj);
        }

        bool
        Intersect(const Ray& ray, HitInfo* hitInfo) const
        {
            bool isHit = false;
            for (auto& geoObjPtr : m_geoObjectPtrs)
            {
                HitInfo h;
                if (geoObjPtr->Intersect(ray, &h) &&
					(h.length > FLT_EPSILON) &&
					(h.length < hitInfo->length)
					)
                {
                    isHit = true;
                    *hitInfo = h;
                }
            }
            return isHit;
        }

    private:
        std::vector<GeometricObject*> m_geoObjectPtrs;
        Camera* m_cameraPtr;
        Color3f m_bgColor = Color3f::Zero();
		const Texture2D* m_bgTexture = nullptr;
    };
}