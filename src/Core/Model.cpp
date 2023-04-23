#include "Model.h"
#include "LineRenderer.h"
#include "TriangleRenderer.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

namespace MiniRenderer
{
	Model::Model(const std::string path)
	{
		LoadMesh(path);
	}

	void Model::DrawWireframe(Framebuffer& buffer, uint32_t meshIndex, uint32_t color)
	{
		if (meshes.size() < meshIndex + 1) return;

		int bufferWidth = buffer.GetFramebufferWidth();
		int bufferHeight = buffer.GetFramebufferHeight();
		printf("Number of Faces: %d\tNumber of Vertices: %d\n", meshes[meshIndex].nFaces, meshes[meshIndex].nVertices);

		Mat4 modelMatrix, projectionMatrix;
		
		for (uint32_t i = 0; i < meshes[meshIndex].nFaces / 3; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				Vec3f v0 = meshes[meshIndex].vertices[meshes[meshIndex].faces[i * 3 + j] - 1];
				Vec3f v1 = meshes[meshIndex].vertices[meshes[meshIndex].faces[i * 3 + (j + 1) % 3] - 1];

				modelMatrix.Identity();

				Scale(modelMatrix, Vec3f(1.5f, 2.5f, 1.5f));
				//Rotate(modelMatrix, ToRadians(-10.0f), Vec3f(0.0f, 0.0f, 1.0f));
				Rotate(modelMatrix, ToRadians(20.0f), Vec3f(1.0f, 0.0f, 0.0f));
				Rotate(modelMatrix, ToRadians(60.0f), Vec3f(0.0f, 1.0f, 0.0f));
				Translate(modelMatrix, Vec3f(0.0f, 0.0f, 10.0f));

				Perspective(projectionMatrix, 60.0f, (float)bufferWidth / (float)bufferHeight, 0.1f, 100.0f);
				//Orthographic(projectionMatrix, 10.0f, -10.0f, 10.0f, -10.0f, 0.01f, 20.0f);

				v0 = projectionMatrix * modelMatrix * v0;
				v1 = projectionMatrix * modelMatrix * v1;

				v0.x = (int)((v0.x + 1) * bufferWidth / 2);
				v0.y = (int)((v0.y + 1) * bufferHeight / 2);
				
				v1.x = (int)((v1.x + 1) * bufferWidth / 2);
				v1.y = (int)((v1.y + 1) * bufferHeight / 2);

				DrawLine(v0.x, v0.y, v1.x, v1.y, color, buffer);
			}
		}
	}

	void Model::Draw(Framebuffer& buffer, Camera& camera, uint32_t meshIndex, uint32_t color)
	{
		if (meshes.size() < meshIndex + 1) return;

		int bufferWidth = buffer.GetFramebufferWidth();
		int bufferHeight = buffer.GetFramebufferHeight();
		//printf("Number of Faces: %d\tNumber of Vertices: %d\n", meshes[meshIndex].nFaces, meshes[meshIndex].nVertices);

		Mat4 modelMatrix, projectionMatrix, viewMatrix = camera.GetViewMatrix();
		
		Vec3f lightDirection(0.2f, 0.3f, 1.0f);
		lightDirection.normalize();

		for (uint32_t i = 0; i < meshes[meshIndex].nFaces / 3; i++)
		{
			Vec2i triangle[3];
			for (uint32_t j = 0; j < 3; j++)
			{
				Vec3f v0 = meshes[meshIndex].vertices[meshes[meshIndex].faces[i * 3 + j] - 1];
				Vec3f v1 = meshes[meshIndex].vertices[meshes[meshIndex].faces[i * 3 + (j + 1) % 3] - 1];
				Vec3f v2 = meshes[meshIndex].vertices[meshes[meshIndex].faces[i * 3 + (j + 2) % 3] - 1];

				modelMatrix.Identity();
				projectionMatrix.Identity();

				Scale(modelMatrix, Vec3f(1.5f, 2.5f, 1.5f));
				//Rotate(modelMatrix, ToRadians(-10.0f), Vec3f(0.0f, 0.0f, 1.0f));
				Rotate(modelMatrix, ToRadians(20.0f), Vec3f(1.0f, 0.0f, 0.0f));
				Rotate(modelMatrix, ToRadians(45.0f), Vec3f(0.0f, 1.0f, 0.0f));

				Perspective(projectionMatrix, 60.0f, (float)bufferWidth / (float)bufferHeight, 0.1f, 100.0f);
				//Orthographic(projectionMatrix, 10.0f, -10.0f, 10.0f, -10.0f, 0.01f, 20.0f);

				v0 = modelMatrix * v0;
				v1 = modelMatrix * v1;
				v2 = modelMatrix * v2;

				// Get Normal
				Vec3f normal = Cross(v2 - v0, v1 - v0);
				normal.normalize();

				// Flat Shading
				float intensity = Dot(normal, lightDirection);

				v0 = projectionMatrix * viewMatrix * v0;
				v1 = projectionMatrix * viewMatrix * v1;
				v2 = projectionMatrix * viewMatrix * v2;

				v0.x = (int)((v0.x + 1) * bufferWidth / 2);
				v0.y = (int)((v0.y + 1) * bufferHeight / 2);
				triangle[0] = Vec2i(v0.x, v0.y);

				v1.x = (int)((v1.x + 1) * bufferWidth / 2);
				v1.y = (int)((v1.y + 1) * bufferHeight / 2);
				triangle[1] = Vec2i(v1.x, v1.y);

				v2.x = (int)((v2.x + 1) * bufferWidth / 2);
				v2.y = (int)((v2.y + 1) * bufferHeight / 2);
				triangle[2] = Vec2i(v2.x, v2.y);

				uint32_t red = ((color >> 16) & 0xFF) * intensity;
				uint32_t green = ((color >> 8) & 0xFF) * intensity;
				uint32_t blue = (color & 0xFF) * intensity;

				uint32_t col = (red << 16) + (green << 8) + blue;

				DrawTriangle(triangle, col, buffer);
			}
		}
	}

	void Model::LoadMesh(const std::string path)
	{
		// Get the model file type.
		size_t dot = path.find_last_of('.');
		if (dot == std::string::npos) 
			throw std::runtime_error("Model File Type undefined!\n");

		const std::string modelType = path.substr(dot + 1, path.length() - dot);

		if (Iequals(modelType, "obj"))
		{
			// Load Wavefront Model File.
			std::ifstream file;
			file.open(path, std::ifstream::in);
			if (file.fail()) throw std::runtime_error("Failed to open model file.\n");
			std::string line;

			// The number of meshes in this obj file.
			int meshCount = 0;
			int prevMeshCount = -1;
			while (!(file >> std::ws).eof())
			{
				std::getline(file, line);
				if (file.fail()) throw std::runtime_error("Failed read from model file.\n");
				std::istringstream iss(line.c_str());
				char trash;
				if (!line.compare(0, 2, "v "))
				{
					iss >> trash;
					if (meshCount != prevMeshCount)
					{
						// Add the Mesh and increase the mesh count.
						meshes.push_back(Mesh());
						meshCount++;
						prevMeshCount = meshCount;	// Store the last mesh count.
					}

					// Now put in the vertices value.
					meshes[meshCount - 1].nVertices++;
					Vec3f v;
					iss >> v.x;
					iss >> v.y;
					iss >> v.z;
					meshes[meshCount - 1].vertices.push_back(v);
				}
				else if (!line.compare(0, 2, "f "))
				{
					iss >> trash;
					int f, t, n;

					// We further increase the meshCount so if we go back to any v statement, we are writing to a new mesh and not the old one.
					if(prevMeshCount == meshCount)	// So we only increment it once.
						meshCount++;

					iss >> f;
					iss >> trash;
					if (trash != '/')
					{
						// This means this mesh does not contain any data for tangents and normals.
						// So we push all these 3 values as mesh faces.
						meshes[prevMeshCount - 1].nFaces += 3;
						meshes[prevMeshCount - 1].faces.push_back(f);
						meshes[prevMeshCount - 1].faces.push_back(int(trash - '0'));	// Trash value is the second face index.
						iss >> f;	// Third face index.
						meshes[prevMeshCount - 1].faces.push_back(f);
					}
					else
					{
						// f faces/tangent/normal
						iss >> t >> trash >> n;
						// For now we only care about the faces so just store that value.
						meshes[prevMeshCount - 1].nFaces++;
						meshes[prevMeshCount - 1].faces.push_back(f);
					}
				}
			}
		}
		else if (Iequals(modelType, "gltf"))
		{
			// Load GLTF Model File.
		}

	}
}