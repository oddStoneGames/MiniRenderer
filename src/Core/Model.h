#pragma once

#include "Maths/Maths.h"
#include "Framebuffer.h"
#include <vector>
#include <string>

namespace MiniRenderer
{
	/// @brief Has all the vertex data from the model file.
	struct Mesh
	{
		std::vector<Vec3f> vertices;	// Vertices
		uint32_t nVertices;	// Number of Vertices
		std::vector<unsigned int> faces;	// Faces
		uint32_t nFaces;	// Number of Faces

		Mesh() : vertices(), nVertices(0), faces(), nFaces(0) {}
		Mesh(std::vector<Vec3f> verts, uint32_t nVerts, std::vector<unsigned int> f, uint32_t nF) : vertices(verts), nVertices(nVerts), faces(f), nFaces(nF) {}
	};

	/// @brief Has all Mesh, texture & material data.
	class Model
	{
	public:
		/// @brief Loads the Mesh with the values in path
		Model(const std::string path);
		~Model() {}
	public:
		std::vector<Mesh> meshes;
		/// @brief Draws the given Mesh as lines with the desired color to the given buffer.
		void DrawWireframe(Framebuffer& buffer, uint32_t meshIndex = 0, uint32_t color = 0xFFFF00);

		/// @brief Draws the given Mesh as triangles with the desired color to the given buffer.
		void Draw(Framebuffer& buffer, uint32_t meshIndex = 0, uint32_t color = 0xFFFF00);
	private:
		/// @brief Loads the Mesh with the values in path
		void LoadMesh(const std::string path);
	};

	/// @brief Returns if the two strings are equal(case insensitive)
	static bool Iequals(const std::string& a, const std::string& b)
	{
		return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b)
		{
			return tolower(a) == tolower(b);
		});
	}
}