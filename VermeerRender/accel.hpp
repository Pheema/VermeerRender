#pragma once

#include <queue>
#include <algorithm>
#include <vector>
#include "bvhNode.hpp"
#include "ray.hpp"
#include "hitInfo.hpp"
#include "scene.hpp"

namespace VermeerRender
{
	class Accel
	{
	public:
		Accel() {}

		void Build(const Scene& scene)
		{
			m_bvhNodes.clear();
			
			{
				BVHNode rootNode;
				rootNode.m_childObjPtrs.reserve(scene.GetGeoObjectPtrs().size());
				for (auto& objPtr : scene.GetGeoObjectPtrs())
				{
					objPtr->CalcBound();
					rootNode.bounds.Merge(objPtr->GetBounds());

					rootNode.m_childObjPtrs.emplace_back(objPtr);
				}
				m_bvhNodes.emplace_back(rootNode);
			}

			// 処理すべきノードインデックスのキュー
			std::queue<int> bvhNodeIndexQueue;
			bvhNodeIndexQueue.emplace(0);

			int index = 0;
			while (!bvhNodeIndexQueue.empty())
			{
				int nodeIndex = bvhNodeIndexQueue.front();
				bvhNodeIndexQueue.pop();

				if (m_bvhNodes[nodeIndex].m_childObjPtrs.size() < 2)
				{
					m_bvhNodes[nodeIndex].isLeaf = true;
					continue;
				}

				int widestAxis = m_bvhNodes[nodeIndex].bounds.GetWidestAxis();
				float center = m_bvhNodes[nodeIndex].bounds.GetCenter()[widestAxis];
#if 0
				// 座標の中心で2分割する
				auto iter0 = std::partition(
					m_bvhNodes[nodeIndex].m_childObjPtrs.begin(),
					m_bvhNodes[nodeIndex].m_childObjPtrs.end(),
					[center, widestAxis](GeometricObject* obj) { return (obj->GetBounds().GetCenter()[widestAxis] < center); }
				);
#else
				size_t halfSize = static_cast<int>(m_bvhNodes[nodeIndex].m_childObjPtrs.size()) / 2;
				std::nth_element(
					m_bvhNodes[nodeIndex].m_childObjPtrs.begin(),
					m_bvhNodes[nodeIndex].m_childObjPtrs.begin() + halfSize,
					m_bvhNodes[nodeIndex].m_childObjPtrs.end(),
					[center, widestAxis](GeometricObject* obj1, GeometricObject* obj2) { return (obj1->GetBounds().GetCenter()[widestAxis] < obj2->GetBounds().GetCenter()[widestAxis]); }
				);
				auto iter0 = m_bvhNodes[nodeIndex].m_childObjPtrs.begin() + halfSize;
#endif

				BVHNode childNode[2];
				
				if (iter0 == m_bvhNodes[nodeIndex].m_childObjPtrs.begin()) iter0++;
				if (iter0 == m_bvhNodes[nodeIndex].m_childObjPtrs.end()) iter0--;

				for (auto iter = m_bvhNodes[nodeIndex].m_childObjPtrs.begin();
					iter != iter0; ++iter)
				{
					childNode[0].bounds.Merge((*iter)->GetBounds());
					childNode[0].m_childObjPtrs.emplace_back(*iter);
				}

				for (auto iter = iter0;
					iter != m_bvhNodes[nodeIndex].m_childObjPtrs.end(); ++iter)
				{
					childNode[1].bounds.Merge((*iter)->GetBounds());
					childNode[1].m_childObjPtrs.emplace_back(*iter);
				}

				m_bvhNodes.emplace_back(childNode[0]);
				m_bvhNodes.emplace_back(childNode[1]);

				m_bvhNodes[nodeIndex].m_childNodeIndicies[0] = ++index;
				m_bvhNodes[nodeIndex].m_childNodeIndicies[1] = ++index;

				bvhNodeIndexQueue.emplace(m_bvhNodes[nodeIndex].m_childNodeIndicies[0]);
				bvhNodeIndexQueue.emplace(m_bvhNodes[nodeIndex].m_childNodeIndicies[1]);

				m_bvhNodes[nodeIndex].m_childObjPtrs.clear();
				m_bvhNodes[nodeIndex].m_childObjPtrs.shrink_to_fit();
			}
			std::cout << "[Done] BVH Construction" << std::endl;
		};

		bool
		Intersect(const Ray& ray, HitInfo* const hitInfo)
		{
			std::queue<int> bvhNodeIndexQueue;
			bvhNodeIndexQueue.emplace(0);
			
			// ---- BVHのトラバーサル ----
			bool isHit = false;
			HitInfo h;

			// 2つの子ノード又は子オブジェクトに対して
			while (!bvhNodeIndexQueue.empty())
			{
				// 葉ノードに対して
				int bvhNodeIndex = bvhNodeIndexQueue.front();
				bvhNodeIndexQueue.pop();

				if (!m_bvhNodes[bvhNodeIndex].Intersect(ray, &h)) continue;

				if (m_bvhNodes[bvhNodeIndex].isLeaf)
				{
					for (auto& childObjPtr : m_bvhNodes[bvhNodeIndex].m_childObjPtrs)
					{
						if (childObjPtr->Intersect(ray, &h) &&
							h.length < hitInfo->length)
						{
							isHit = true;
							*hitInfo = h;
						}
					}
					hitInfo->ray = ray;
				}
				else
				{
					// 枝ノードの場合
					for (int childNodeIndex : m_bvhNodes[bvhNodeIndex].m_childNodeIndicies)
					{
						// BVHの手前でrayが既にオブジェクトに当たっていたら
						// 以下のIntersect()はfalseとなる（枝刈り）
						// if (m_bvhNodes[childNodeIndex].Intersect(ray, &h))
						// {
							bvhNodeIndexQueue.emplace(childNodeIndex);
						// }
					}
				}
				
			}
			hitInfo->ray = ray;
			return isHit;
		};

	private:
		std::vector<BVHNode> m_bvhNodes;
	};
}