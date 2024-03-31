
#ifndef ICOSPHERE_H
#define ICOSPHERE_H

#include <scene/3d/mesh_instance_3d.h>
#include <core/math/vector3.h>
#include <core/templates/vector.h>
#include <map>

class Icosphere : public MeshInstance3D {
	GDCLASS(Icosphere, MeshInstance3D);
public:
	Icosphere();
	~Icosphere();
	Icosphere(const Icosphere& other);

	void _ready();
	void _process(double delta);

	PackedVector3Array getVertices() const;
	PackedInt32Array getIndices() const;
	int addVertex(const Vector3& vertex);
	void addFace(int v1, int v2, int v3);

	void subdivide(int levels);

protected:
	static void _bind_methods();

private:
	PackedVector3Array vertices;
	PackedInt32Array indices;

	int getOrCreateMidpointIndex(int index1, int index2);
	std::map<std::pair<int, int>, int> midpointIndexCache; // Cache to store midpoints
	void generateMesh();
};


#endif //ICOSPHERE_H