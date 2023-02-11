#include "Model.h"
#include "LineRenderer.h"
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

		for (int i = 0; i < meshes[meshIndex].nFaces / 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				Vec3 v0 = meshes[meshIndex].vertices[meshes[meshIndex].faces[i * 3 + j] - 1];
				Vec3 v1 = meshes[meshIndex].vertices[meshes[meshIndex].faces[i * 3 + (j + 1) % 3] - 1];
				int x0 = (v0.x + 1.0f) * bufferWidth / 4.0f;
				int y0 = (v0.y + 1.0f) * bufferHeight / 4.0f;
				int x1 = (v1.x + 1.0f) * bufferWidth / 4.0f;
				int y1 = (v1.y + 1.0f) * bufferHeight / 4.0f;
				DrawLine(x0, y0, x1, y1, color, buffer);
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
					Vec3 v;
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