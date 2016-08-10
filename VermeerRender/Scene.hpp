#pragma once

#include <vector>
#include "Camera.hpp"
#include "GeometricObject.hpp"
#include "Vector3f.hpp"

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

        void
        AddGeoObject(GeometricObject& obj)
        {
            m_geoObjectPtrs.push_back(&obj);
        }

        bool
        Intersect(const Ray& ray, HitInfo* hitInfo)
        {
            bool isHit = false;
            for (auto& geoObjPtr : m_geoObjectPtrs)
            {
                HitInfo h;
                if (geoObjPtr->Intersect(ray, &h) && (h.length < hitInfo->length))
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
    };
}