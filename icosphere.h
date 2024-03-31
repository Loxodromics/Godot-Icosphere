#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include <core/math/vector3.h>
#include <core/templates/vector.h>
#include <scene/3d/mesh_instance_3d.h>
#include <map>

class Icosphere : public MeshInstance3D {
	GDCLASS(Icosphere, MeshInstance3D);

public:
	Icosphere();
	~Icosphere();
	Icosphere(const Icosphere &other);

	void setRadius(float p_radius);
	float getRadius() const;

	void setSubdivisions(int p_subdivisions);
	int getSubdivisions() const;

	PackedVector3Array getVertices() const;
	PackedInt32Array getIndices() const;

protected:
	static void _bind_methods();
	void _notification(int p_what);
	void _ready();

private:
	PackedVector3Array vertices;
	PackedInt32Array indices;
	std::map<std::pair<int, int>, int> midpointIndexCache; // Cache to store midpoints

	float radius{ 1.0f };
	int subdivisions{ 4 };

	void generateMesh();
	void initializeBaseIcosahedron();
	void subdivide(int levels);
	void update();

	int addVertex(const Vector3 &vertex);
	void addFace(int v1, int v2, int v3);
	int getOrCreateMidpointIndex(int index1, int index2);
};

#endif // ICOSPHERE_H
