#include <vector>
#include <cmath>
#include <cstdlib>

#include "simple_mesh.hpp"
#include "loadobj.hpp"


#include "../vmlib/vec3.hpp"
#include "../vmlib/vec2.hpp"

float floorVertices[];
SimpleMeshData makeBowl();
SimpleMeshData makeCube();
SimpleMeshData makeCube(Vec3f vertices[], SimpleMeshData simpleMesh, Vec3f color, bool texture);
SimpleMeshData makeRoof();
SimpleMeshData makeCylinder(Vec3f topCentre, float radius, float height, Vec3f color, SimpleMeshData simpleMesh);
SimpleMeshData makePaw();
SimpleMeshData makeTriangularCone(Vec3f vertices[], SimpleMeshData simpleMesh, Vec3f color);
SimpleMeshData makeHouse();
SimpleMeshData makeBed();
SimpleMeshData makeBall();
Vec3f crossNormals(Vec3f aLeft, Vec3f aRight);