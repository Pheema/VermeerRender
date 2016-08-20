#pragma once

#include "assimp\scene.h"
#include "assimp\Importer.hpp"
#include "assimp\mesh.h"
#include "triangle.hpp"

#include <vector>


namespace VermeerRender
{
	class Mesh : public GeometricObject
	{
	public:

		Mesh(const char* filepath)
		{
			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(filepath, NULL);
			if (!scene) {
				printf("Unable to laod mesh: %s\n", importer.GetErrorString());
			}

			if (scene->HasMeshes())
			{
				for (int i = 0; i < scene->mNumMeshes; ++i)
				{
					const aiMesh* targetMeshPtr = scene->mMeshes[i];

					// ---- À•W‚Ì“Ç‚Ýž‚Ý ----
					if (targetMeshPtr->HasPositions())
					{
						vertices.resize(targetMeshPtr->mNumVertices);
						for (int j = 0; j < targetMeshPtr->mNumVertices; ++j)
						{
							vertices[j].pos = Vector3f(
								targetMeshPtr->mVertices[j].x,
								targetMeshPtr->mVertices[j].y,
								targetMeshPtr->mVertices[j].z
							);
						}
					}

					// ---- –@ü‚Ì“Ç‚Ýž‚Ý ----
					if (targetMeshPtr->HasNormals())
					{
						vertices.resize(targetMeshPtr->mNumVertices);
						for (int j = 0; j < targetMeshPtr->mNumVertices; ++j)
						{
							vertices[j].normal = Vector3f(
								targetMeshPtr->mNormals[j].x,
								targetMeshPtr->mNormals[j].y,
								targetMeshPtr->mNormals[j].z
							).Normalized();
						}
					}

					// ---- UV‚Ì“Ç‚Ýž‚Ý ----
					if (targetMeshPtr->HasTextureCoords(0))
					{
						vertices.resize(targetMeshPtr->mNumVertices);
						for (int j = 0; j < targetMeshPtr->mNumVertices; ++j)
						{
							vertices[j].uv = Vector3f(
								targetMeshPtr->mTextureCoords[0][j].x,
								targetMeshPtr->mTextureCoords[0][j].y,
								0.0f
							);
						}
					}
					
					// ---- ŽOŠpŒ`‚Ì“o˜^ ----
					if (targetMeshPtr->HasFaces())
					{
						triangles.resize(targetMeshPtr->mNumFaces);
						for (int j = 0; j < targetMeshPtr->mNumFaces; ++j)
						{
							triangles[j].vertexPtrs[0] = &vertices[targetMeshPtr->mFaces[j].mIndices[0]];
							triangles[j].vertexPtrs[1] = &vertices[targetMeshPtr->mFaces[j].mIndices[1]];
							triangles[j].vertexPtrs[2] = &vertices[targetMeshPtr->mFaces[j].mIndices[2]];
						}
					}
				}
			}
		}

		virtual void
		SetMaterial(Material& material) override
		{
			for (auto& triangle : triangles)
			{
				triangle.SetMaterial(material);
			}
		}

		virtual bool
		Intersect(const Ray& ray, HitInfo* const hitInfo)
		{
			bool isHit = false;
			for (auto& triangle : triangles)
			{
				HitInfo h;
				if (triangle.Intersect(ray, &h) &&
					(h.length < hitInfo->length))
				{
					isHit = true;
					*hitInfo = h;
				}
			}
			hitInfo->ray = ray;
			return isHit;
		}

		virtual void
		CalcBound() override
		{
			std::cerr << "This function is not implemented." << std::endl;
			abort();
		}

		virtual Vector3f
		SampleSurface(const HitInfo& hitInfo) override
		{
			std::cerr << "This function is not implemented." << std::endl;
			abort();
			return Vector3f(0);
		}

		virtual float
		SamplingArea(const HitInfo& hitInfo)
		{
			std::cerr << "This function is not implemented." << std::endl;
			abort();
			return 0.0f;
		}

	public:
		std::vector<Vertex> vertices;
		std::vector<Triangle> triangles;
	};
}