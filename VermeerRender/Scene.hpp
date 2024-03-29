﻿#pragma once

#include <vector>
#include "camera.hpp"
#include "constant.hpp"
#include "geometricObject.hpp"
#include "mesh.h"
#include "texture2D.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
	class Accel;

	class Scene
    {
    public:
        Scene() {}

        void SetCamera(Camera& camera)
        {
            m_cameraPtr = &camera;
        }

		const Camera&
		GetCamera() const
		{
			return *m_cameraPtr;
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
		AddGeoObject(Mesh& mesh)
		{
			// TODO: privateにアクセスしているので直す
			for (auto& triangle : mesh.triangles)
			{
				m_geoObjectPtrs.push_back(&triangle);
			}
		}

        void
        AddGeoObject(GeometricObject& obj)
        {
			if (&(obj.GetMaterial()) == nullptr)
			{
				std::cout << "No material is assigned." << std::endl;
				abort();
			}
            m_geoObjectPtrs.push_back(&obj);
        }

		const std::vector<GeometricObject*>&
		GetGeoObjectPtrs() const
		{
			return m_geoObjectPtrs;
		}


        bool
        Intersect(const Ray& ray, HitInfo* hitInfo) const
        {
            bool isHit = false;
            for (auto& geoObjPtr : m_geoObjectPtrs)
            {
                HitInfo h;
                if (geoObjPtr->Intersect(ray, &h) &&
					(h.length < hitInfo->length))
                {
                    isHit = true;
                    *hitInfo = h;
                }
            }
			hitInfo->ray = ray;
            return isHit;
        }

		Accel* accelPtr;

    private:
        std::vector<GeometricObject*> m_geoObjectPtrs;
        Camera* m_cameraPtr;
        Color3f m_bgColor = Color3f::Zero();
		const Texture2D* m_bgTexture = nullptr;
    };
}