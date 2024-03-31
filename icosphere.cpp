#include "icosphere.h"
#include <thread>
#include <iostream>

Icosphere::Icosphere() {
	print_line("Icosphere::Icosphere");
	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	float a = 1.0f;
	float b = 1.0f / phi;

	// Add vertices
	this->addVertex(Vector3(0, b, -a));  // v0
	this->addVertex(Vector3(b, a, 0));   // v1
	this->addVertex(Vector3(-b, a, 0));  // v2
	this->addVertex(Vector3(0, b, a));   // v3
	this->addVertex(Vector3(0, -b, a));  // v4
	this->addVertex(Vector3(-a, 0, b));  // v5
	this->addVertex(Vector3(0, -b, -a)); // v6
	this->addVertex(Vector3(a, 0, -b));  // v7
	this->addVertex(Vector3(a, 0, b));   // v8
	this->addVertex(Vector3(-a, 0, -b)); // v9
	this->addVertex(Vector3(b, -a, 0));  // v10
	this->addVertex(Vector3(-b, -a, 0)); // v11

	// Add faces
	this->addFace(2, 1, 0);
	this->addFace(2, 3, 1);
	this->addFace(5, 4, 3);
	this->addFace(4, 8, 3);
	this->addFace(7, 6, 0);
	this->addFace(6, 9, 0);
	this->addFace(11, 10, 4);
	this->addFace(10, 11, 6);
	this->addFace(9, 5, 2);
	this->addFace(5, 9, 11);
	this->addFace(8, 7, 1);
	this->addFace(7, 8, 10);
	this->addFace(2, 5, 3);
	this->addFace(8, 1, 3);
	this->addFace(9, 2, 0);
	this->addFace(1, 7, 0);
	this->addFace(11, 9, 6);
	this->addFace(7, 10, 6);
	this->addFace(5, 11, 4);
	this->addFace(10, 8, 4);

	this->generateMesh();
}

Icosphere::~Icosphere() {
	print_line("Icosphere::~Icosphere");

}

void Icosphere::_process(double delta) {}

void Icosphere::_bind_methods()
{
	print_line("Icosphere::~_bind_methods");
	ClassDB::bind_method(D_METHOD("_ready"), &Icosphere::_ready);
}

// This function gets called when the node is spawned
void Icosphere::_ready()
{
	print_line("Icosphere::_ready");
	this->generateMesh();
}

void _process(double delta) {
	print_line("Icosphere::_process");
}

int Icosphere::addVertex(const Vector3& vertex) {
	vertices.push_back(vertex);
	// Since PackedVector3Array is zero-indexed, the new vertex's index is the size - 1.
	return vertices.size() - 1;
}

void Icosphere::addFace(int v1, int v2, int v3) {
	indices.push_back(v1);
	indices.push_back(v2);
	indices.push_back(v3);
}

void Icosphere::subdivide(int levels) {
	for (int level = 0; level < levels; ++level) {
		PackedInt32Array newIndices;
		midpointIndexCache.clear(); // Reset cache for each subdivision level

		for (int i = 0; i < indices.size(); i += 3) {
			int v1 = indices[i];
			int v2 = indices[i + 1];
			int v3 = indices[i + 2];

			// Get new vertices in the middle of each edge of the triangle
			int a = getOrCreateMidpointIndex(v1, v2);
			int b = getOrCreateMidpointIndex(v2, v3);
			int c = getOrCreateMidpointIndex(v3, v1);

			// Replace the current triangle with 4 new triangles
			newIndices.push_back(v1); newIndices.push_back(a); newIndices.push_back(c);
			newIndices.push_back(v2); newIndices.push_back(b); newIndices.push_back(a);
			newIndices.push_back(v3); newIndices.push_back(c); newIndices.push_back(b);
			newIndices.push_back(a); newIndices.push_back(b); newIndices.push_back(c);
		}

		// Use the new set of indices to replace the old one
		indices = newIndices;
	}
}

int Icosphere::getOrCreateMidpointIndex(int index1, int index2) {
	// Ensure the first index is always the smaller one to avoid duplicates
	auto key = std::make_pair(MIN(index1, index2), MAX(index1, index2));

	// Check if this midpoint has already been created
	auto cachedIndex = midpointIndexCache.find(key);
	if (cachedIndex != midpointIndexCache.end()) {
		return cachedIndex->second;
	}

	// Create a new midpoint vertex
	Vector3 midpoint = vertices[index1].lerp(vertices[index2], 0.5);
	midpoint.normalize();
	int midpointIndex = vertices.size();
	vertices.push_back(midpoint);

	// Add to cache
	midpointIndexCache[key] = midpointIndex;

	return midpointIndex;
}

void Icosphere::generateMesh() {
	print_line("Icosphere::generateMesh");
	Ref<ArrayMesh> mesh = memnew(ArrayMesh);
	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);

	arrays[Mesh::ARRAY_VERTEX] = vertices;

	if (!indices.is_empty()) {
		arrays[Mesh::ARRAY_INDEX] = indices;
	}

	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
	set_mesh(mesh);
}
