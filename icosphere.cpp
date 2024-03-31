#include "icosphere.h"
#include <thread>
#include <iostream>

Icosphere::Icosphere() {
	print_line("Icosphere::Icosphere");

	this->initializeBaseIcosahedron();
	this->subdivide(this->subdivisions);
	this->generateMesh();
}

Icosphere::~Icosphere() {
	print_line("Icosphere::~Icosphere");
	vertices.clear();
	indices.clear();
}

void Icosphere::setRadius(float p_radius) {
	radius = p_radius;
	this->update();
}

float Icosphere::getRadius() const {
	return radius;
}

void Icosphere::setSubdivisions(int p_subdivisions) {
	subdivisions = MAX(0, p_subdivisions); // Ensure subdivisions is never negative
	this->update();
}

int Icosphere::getSubdivisions() const {
	return subdivisions;
}

PackedVector3Array Icosphere::getVertices() const {
	return this->vertices;
}

PackedInt32Array Icosphere::getIndices() const {
	return this->indices;
}

void Icosphere::_bind_methods()
{
	print_line("Icosphere::~_bind_methods");

	ClassDB::bind_method(D_METHOD("setRadius", "radius"), &Icosphere::setRadius);
	ClassDB::bind_method(D_METHOD("getRadius"), &Icosphere::getRadius);

	ClassDB::bind_method(D_METHOD("setSubdivisions", "subdivisions"), &Icosphere::setSubdivisions);
	ClassDB::bind_method(D_METHOD("getSubdivisions"), &Icosphere::getSubdivisions);

	// Optionally, add property information for better editor integration
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "radius", PROPERTY_HINT_RANGE, "0.01,100,0.01"), "setRadius", "getRadius");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "subdivisions", PROPERTY_HINT_RANGE, "0,10,1"), "setSubdivisions", "getSubdivisions");
}

// This function gets any event calls from the engine
void Icosphere::_notification(int p_what) {

	// print_line(vformat("notification: what: %d", p_what));
	switch (p_what) {

		case NOTIFICATION_READY: {
			_ready();
		} break;
		default:
			break;
	}
}

// This function gets called when the node is spawned
void Icosphere::_ready()
{
	print_line("Icosphere::_ready");
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

void Icosphere::generateMesh() {
	print_line("Icosphere::generateMesh");
	Ref<ArrayMesh> mesh = memnew(ArrayMesh);
	mesh.instantiate();
	Array arrays;
	arrays.resize(Mesh::ARRAY_MAX);

	// Set vertices
	arrays[Mesh::ARRAY_VERTEX] = vertices;

	// Calculate and set normals and UVs
	PackedVector3Array normals;
	PackedVector2Array uvs; // For UV coordinates
	PackedColorArray colors; // For vertex colors
	for (int i = 0; i < vertices.size(); i++) {
		Vector3 normal = vertices[i].normalized();
		normals.push_back(normal);

		// Debug output for each normal
		// String normalStr = vformat("Normal %d: (%f, %f, %f)", i, normal.x, normal.y, normal.z);
		// print_line(normalStr);

		// Calculate UV coordinates based on vertex position
		float u = atan2(normal.x, normal.z) / (2.0 * Math_PI) + 0.5;
		float v = acos(normal.y) / Math_PI; // Adjusted to flip the texture
		uvs.push_back(Vector2(u, v)); // Use Vector2 for UV coordinates

		// Assign a color based on the vertex position or normal
		Color color = Color(normal.x * 0.5 + 0.5, normal.y * 0.5 + 0.5, normal.z * 0.5 + 0.5);
		colors.push_back(color);
	}
	arrays[Mesh::ARRAY_NORMAL] = normals;
	arrays[Mesh::ARRAY_TEX_UV] = uvs;
	arrays[Mesh::ARRAY_COLOR] = colors; // Add the color array to the mesh

	// Set indices for faces
	if (!indices.is_empty()) {
		arrays[Mesh::ARRAY_INDEX] = indices;
	}

	mesh->add_surface_from_arrays(Mesh::PRIMITIVE_TRIANGLES, arrays);
	set_mesh(mesh);
}

void Icosphere::initializeBaseIcosahedron() {
	vertices.clear();
	indices.clear();

	float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
	float a = 1.0f;
	float b = 1.0f / phi;

	// Add vertices
	this->addVertex(Vector3(0, b, -a).normalized() * this->radius);  // v0
	this->addVertex(Vector3(b, a, 0).normalized() * this->radius);   // v1
	this->addVertex(Vector3(-b, a, 0).normalized() * this->radius);  // v2
	this->addVertex(Vector3(0, b, a).normalized() * this->radius);   // v3
	this->addVertex(Vector3(0, -b, a).normalized() * this->radius);  // v4
	this->addVertex(Vector3(-a, 0, b).normalized() * this->radius);  // v5
	this->addVertex(Vector3(0, -b, -a).normalized() * this->radius); // v6
	this->addVertex(Vector3(a, 0, -b).normalized() * this->radius);  // v7
	this->addVertex(Vector3(a, 0, b).normalized() * this->radius);   // v8
	this->addVertex(Vector3(-a, 0, -b).normalized() * this->radius); // v9
	this->addVertex(Vector3(b, -a, 0).normalized() * this->radius);  // v10
	this->addVertex(Vector3(-b, -a, 0).normalized() * this->radius); // v11

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
}

void Icosphere::update() {
	// Reset to the base icosahedron before applying subdivisions
	this->initializeBaseIcosahedron();

	// Apply subdivisions
	this->subdivide(this->subdivisions);

	// Generate the new mesh
	this->generateMesh();
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
	midpoint = midpoint.normalized() * this->radius;
	int midpointIndex = vertices.size();
	vertices.push_back(midpoint);

	// Add to cache
	midpointIndexCache[key] = midpointIndex;

	return midpointIndex;
}

int Icosphere::addVertex(const Vector3& vertex) {
	vertices.push_back(vertex);
	// Since PackedVector3Array is zero-indexed, the new vertex's index is the size - 1.
	return vertices.size() - 1;
}

void Icosphere::addFace(int v1, int v2, int v3) {
	indices.push_back(v3);
	indices.push_back(v2);
	indices.push_back(v1);
}