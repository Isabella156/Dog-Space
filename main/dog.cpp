#include "dog.hpp"

// reference: learnopengl face culling, Face culling
float floorVertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,// Bottom-left
     0.5f,  0.5f, -0.5f,// top-right
     0.5f, -0.5f, -0.5f,// bottom-right      
     0.5f,  0.5f, -0.5f,// top-right
    -0.5f, -0.5f, -0.5f,// bottom-left
    -0.5f,  0.5f, -0.5f,// top-left
    // Front face
    -0.5f, -0.5f,  0.5f,// bottom-left
     0.5f, -0.5f,  0.5f, // bottom-right
     0.5f,  0.5f,  0.5f,// top-right
     0.5f,  0.5f,  0.5f,// top-right
    -0.5f,  0.5f,  0.5f,// top-left
    -0.5f, -0.5f,  0.5f, // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,// top-right
    -0.5f,  0.5f, -0.5f,// top-left
    -0.5f, -0.5f, -0.5f,// bottom-left
    -0.5f, -0.5f, -0.5f,// bottom-left
    -0.5f, -0.5f,  0.5f,// bottom-right
    -0.5f,  0.5f,  0.5f,// top-right
    // Right face
     0.5f,  0.5f,  0.5f,// top-left
     0.5f, -0.5f, -0.5f,// bottom-right
     0.5f,  0.5f, -0.5f,// top-right         
     0.5f, -0.5f, -0.5f,// bottom-right
     0.5f,  0.5f,  0.5f,// top-left
     0.5f, -0.5f,  0.5f,// bottom-left     
     // Bottom face
     -0.5f, -0.5f, -0.5f,// top-right
      0.5f, -0.5f, -0.5f,// top-left
      0.5f, -0.5f,  0.5f,// bottom-left
      0.5f, -0.5f,  0.5f,// bottom-left
     -0.5f, -0.5f,  0.5f,// bottom-right
     -0.5f, -0.5f, -0.5f, // top-right
     // Top face
     -0.5f,  0.5f, -0.5f,// top-left
      0.5f,  0.5f,  0.5f, // bottom-right
      0.5f,  0.5f, -0.5f ,// top-right     
      0.5f,  0.5f,  0.5f,// bottom-right
     -0.5f,  0.5f, -0.5f,// top-left
     -0.5f,  0.5f,  0.5f// bottom-left 
};

SimpleMeshData makeBowl() {
    std::vector<Vec3f> pos;
    std::vector<Vec3f> normals;
    Vec3f color = { 0.5f,0.5f,0.5f };
    float r1 = 0.6f;
    float prevX = r1 * std::cos(0.f);
    float prevZ = r1 * std::sin(0.f);
    Vec3f edge1, edge2, normal;
    for (int i = 0; i < 12; i++) {
        float const angle = (i + 1) / float(12) * 2.f * 3.1415926f;
        float x = r1 * std::cos(angle);
        float z = r1 * std::sin(angle);

        // bottom
        pos.emplace_back(Vec3f{ prevX, 0.f, prevZ });
        pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });
        pos.emplace_back(Vec3f{ x, 0.f, z });

        edge1 = Vec3f{ prevX, 0.f, prevZ };
        edge2 = Vec3f{ 0.f - x, 0.f, 0.f - z };
        normal = crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);

        pos.emplace_back(Vec3f{ prevX * 1.4f, 0.f, prevZ * 1.4f });
        pos.emplace_back(Vec3f{ x * 1.4f, 0.f, z * 1.4f });
        pos.emplace_back(Vec3f{ 0.f, 0.f, 0.f });

        edge1 = Vec3f{ 1.4f * (prevX - x), 0.f, 1.4f * (prevZ - z) };
        edge2 = Vec3f{ x * 1.4f, 0.f, z * 1.4f };
        normal = crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);


        // inside
        pos.emplace_back(Vec3f{ prevX, 0.f, prevZ });
        pos.emplace_back(Vec3f{ x, 0.f, z });
        pos.emplace_back(Vec3f{ prevX * 1.2f, 0.4f, prevZ * 1.2f });

        edge1 = Vec3f{ -prevX + x, 0.f, -prevZ + z };
        edge2 = Vec3f{ x - prevX * 1.2f, -0.4f, z - prevZ * 1.2f };
        normal = -crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);

        pos.emplace_back(Vec3f{ prevX * 1.2f, 0.4f, prevZ * 1.2f });
        pos.emplace_back(Vec3f{ x, 0.f, z });
        pos.emplace_back(Vec3f{ x * 1.2f, 0.4f, z * 1.2f });

        edge1 = Vec3f{ -prevX * 1.2f + x, -0.4f, -prevZ * 1.2f + z };
        edge2 = Vec3f{ -0.2f * x, -0.4f, -0.2f * z };
        normal = -crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);

        // top
        pos.emplace_back(Vec3f{ prevX * 1.2f, 0.4f, prevZ * 1.2f });
        pos.emplace_back(Vec3f{ x * 1.2f, 0.4f, z * 1.2f });
        pos.emplace_back(Vec3f{ x * 1.3f, 0.4f, z * 1.3f });

        edge1 = Vec3f{ 1.2f * (prevX - x), 0.f, 1.2f * (prevZ - z) };
        edge2 = Vec3f{ -0.1f * x, 0.f, -0.1f * z };
        normal = crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);

        pos.emplace_back(Vec3f{ prevX * 1.3f, 0.4f, prevZ * 1.3f });
        pos.emplace_back(Vec3f{ prevX * 1.2f, 0.4f, prevZ * 1.2f });
        pos.emplace_back(Vec3f{ x * 1.3f, 0.4f, z * 1.3f });

        edge1 = Vec3f{ 0.1f * prevX, 0.f, 0.1f * prevZ };
        edge2 = Vec3f{ prevX * 1.2F - x * 1.3f, 0.f, prevZ * 1.2f - z * 1.3f };
        normal = crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);

        // outside
        pos.emplace_back(Vec3f{ prevX * 1.3f, 0.4f, prevZ * 1.3f });
        pos.emplace_back(Vec3f{ x * 1.3f, 0.4f, z * 1.3f });
        pos.emplace_back(Vec3f{ x * 1.4f, 0.f, z * 1.4f });

        edge1 = Vec3f{ 1.3f * (prevX - x), 0.f, 1.3f * (prevZ - z) };
        edge2 = Vec3f{ -0.1f * x, 0.4f, -0.1f * z };
        normal = crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);

        pos.emplace_back(Vec3f{ prevX * 1.4f, 0.f, prevZ * 1.4f });
        pos.emplace_back(Vec3f{ prevX * 1.3f, 0.4f, prevZ * 1.3f });
        pos.emplace_back(Vec3f{ x * 1.4f, 0.f, z * 1.4f });

        edge1 = Vec3f{ -0.1f * prevX, -0.4f, -0.1f * prevZ };
        edge2 = Vec3f{ prevX * 1.3f - x * 1.4f, 0.4f, prevZ * 1.3f - z * 1.4f };
        normal = crossNormals(edge1, edge2);
        normals.emplace_back(normal);
        normals.emplace_back(normal);
        normals.emplace_back(normal);

        prevX = x;
        prevZ = z;
    }
    std::vector col(pos.size(), color);
    return SimpleMeshData{ std::move(pos), std::move(col), std::move(normals) };
}

SimpleMeshData makeCube() {
    std::vector<Vec3f> pos, nor;
    Vec3f color = { 1.0f, 0.5f, 0.31f };
    for (int i = 0; i < 6 * 6 * 3; i += 9) {
        pos.emplace_back(Vec3f{ floorVertices[i], floorVertices[i + 1], floorVertices[i + 2] });
        pos.emplace_back(Vec3f{ floorVertices[i + 3], floorVertices[i + 4], floorVertices[i + 5] });
        pos.emplace_back(Vec3f{ floorVertices[i + 6], floorVertices[i + 7], floorVertices[i + 8] });
        Vec3f edge1, edge2, normals;
        edge1 = Vec3f{ floorVertices[i] - floorVertices[i + 3], floorVertices[i + 1] - floorVertices[i + 4] , floorVertices[i + 2] - floorVertices[i + 5] };
        edge2 = Vec3f{ floorVertices[i + 3] - floorVertices[i + 6], floorVertices[i + 4] - floorVertices[i + 7] , floorVertices[i + 5] - floorVertices[i + 8] };
        normals = crossNormals(edge1, edge2);
        nor.emplace_back(normals);
        nor.emplace_back(normals);
        nor.emplace_back(normals);
    }
    std::vector col(pos.size(), color);
    std::vector<int> indices;
    return  SimpleMeshData{ std::move(pos),  std::move(col), std::move(nor) };
}

SimpleMeshData makeCube(Vec3f vertices[], SimpleMeshData simpleMesh, Vec3f color, bool texture) {
    // pos
    std::vector<Vec3f> pos, nor;
    std::vector<Vec2f> texcoords;
    // front
    pos.emplace_back(vertices[1]);
    pos.emplace_back(vertices[0]);
    pos.emplace_back(vertices[3]);
    pos.emplace_back(vertices[3]);
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[1]);
    if (texture) {
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
    }

    for (int i = 0; i < 6; i++) {
        nor.emplace_back(vertices[1] - vertices[5]);
    }

    // back
    pos.emplace_back(vertices[4]);
    pos.emplace_back(vertices[5]);
    pos.emplace_back(vertices[6]);
    pos.emplace_back(vertices[6]);
    pos.emplace_back(vertices[7]);
    pos.emplace_back(vertices[4]);
    if (texture) {
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
    }

    for (int i = 0; i < 6; i++) {
        nor.emplace_back(vertices[5] - vertices[1]);
    }
    // left
    pos.emplace_back(vertices[0]);
    pos.emplace_back(vertices[4]);
    pos.emplace_back(vertices[7]);
    pos.emplace_back(vertices[7]);
    pos.emplace_back(vertices[3]);
    pos.emplace_back(vertices[0]);
    if (texture) {
        texcoords.emplace_back(Vec2f{ 1.f, 1.f });
        texcoords.emplace_back(Vec2f{ 0.f, 1.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 1.f, 0.f });
        texcoords.emplace_back(Vec2f{ 1.f, 1.f });
    }

    for (int i = 0; i < 6; i++) {
        nor.emplace_back(vertices[0] - vertices[1]);
    }
    // right
    pos.emplace_back(vertices[5]);
    pos.emplace_back(vertices[1]);
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[6]);
    pos.emplace_back(vertices[5]);
    if (texture) {
        texcoords.emplace_back(Vec2f{ 1.f, 1.f });
        texcoords.emplace_back(Vec2f{ 0.f, 1.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 1.f, 0.f });
        texcoords.emplace_back(Vec2f{ 1.f, 1.f });
    }

    for (int i = 0; i < 6; i++) {
        nor.emplace_back(vertices[1] - vertices[0]);
    }
    // top
    pos.emplace_back(vertices[5]);
    pos.emplace_back(vertices[4]);
    pos.emplace_back(vertices[0]);
    pos.emplace_back(vertices[0]);
    pos.emplace_back(vertices[1]);
    pos.emplace_back(vertices[5]);
    if (texture) {
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
    }

    for (int i = 0; i < 6; i++) {
        nor.emplace_back(vertices[1] - vertices[2]);
    }
    // bottom
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[3]);
    pos.emplace_back(vertices[7]);
    pos.emplace_back(vertices[7]);
    pos.emplace_back(vertices[6]);
    pos.emplace_back(vertices[2]);
    if (texture) {
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
        texcoords.emplace_back(Vec2f{ 0.f, 0.f });
    }

    for (int i = 0; i < 6; i++) {
        nor.emplace_back(vertices[2] - vertices[1]);
    }
    std::vector<Vec3f> col(pos.size(), color);
    SimpleMeshData cube = SimpleMeshData{ std::move(pos),  std::move(col), std::move(nor), std::move(texcoords) };
    simpleMesh = concatenate(simpleMesh, cube);
    return simpleMesh;
}

SimpleMeshData makeRoof() {
    float roofWidth = 0.5f, zd = 0.05f, height = 0.5f, width = 0.1f;
    float x = width / sqrt(2.f), y = width / sqrt(2.f) + height;
    float sideRoofLength = 0.61f;
    // left roof
    Vec3f v1l = Vec3f{ -roofWidth / sqrt(2.f), 0.f, -zd };
    Vec3f v1zd1 = Vec3f{ 0.f, height, -zd };
    Vec3f v3zd1 = Vec3f{ -x, y, -zd };
    Vec3f v3l = Vec3f{ -(roofWidth + x) / sqrt(2.f), y - height, -zd };
    Vec3f v1lRoofb = Vec3f{ v1l.x, v1l.y, v1l.z - sideRoofLength };
    Vec3f v3lRoofb = Vec3f{ v3l.x, v3l.y, v3l.z - sideRoofLength };
    Vec3f v3Roofb = Vec3f{ -x, y, -zd - sideRoofLength };
    Vec3f v1Roofb = Vec3f{ 0.f, height, -zd - sideRoofLength };

    // right roof
    Vec3f v1r = Vec3f{ roofWidth / sqrt(2.f), 0.f, -zd };
    Vec3f v2r = Vec3f{ (roofWidth + x) / sqrt(2.f), y - height, -zd };
    Vec3f v2zd1 = Vec3f{ x, y, -zd };
    Vec3f v1rRoofb = Vec3f{ v1r.x, v1r.y, v1r.z - sideRoofLength };
    Vec3f v2rRoofb = Vec3f{ v2r.x, v2r.y, v2r.z - sideRoofLength };
    Vec3f v2zd1Roofb = Vec3f{ v2zd1.x, v2zd1.y, v2zd1.z - sideRoofLength };

    Vec3f leftVertices[] = { v3zd1, v1zd1, v1l, v3l, v3Roofb, v1Roofb, v1lRoofb, v3lRoofb };
    Vec3f rightVertices[] = { v1zd1, v2zd1, v2r, v1r, v1Roofb, v2zd1Roofb, v2rRoofb, v1rRoofb };
    std::vector<Vec3f> col, nor, pos;
    std::vector<Vec2f> texcoords;
    SimpleMeshData roof = SimpleMeshData{ std::move(pos),  std::move(col), std::move(nor), std::move(texcoords) };
    Vec3f color = { 0.25f, 0.315f, 0.43f };
    roof = makeCube(leftVertices, roof, color, true);
    roof = makeCube(rightVertices, roof, color, true);

    return roof;
}

SimpleMeshData makeCylinder(Vec3f topCentre, float radius, float height, Vec3f color, SimpleMeshData simpleMesh) {
    std::vector<Vec3f> pos, nor;

    int aSubdivs = 12;
    const float PI = 3.1415926f;
    float prevX = topCentre.x + cos(0.f) * radius;
    float prevZ = topCentre.z + sin(0.f) * radius;
    float topY, bottomY;
    topY = topCentre.y;
    bottomY = topCentre.y - height;
    Vec3f bottomCentre = Vec3f{ topCentre.x, bottomY, topCentre.z };
    Vec3f topNormal = Vec3f{ 0.f, 1.f, 0.f };
    Vec3f bottomNormal = Vec3f{ 0.f, -1.f, 0.f };

    for (std::size_t i = 0; i < aSubdivs; i++) {
        float const angle = (i + 1) / float(aSubdivs) * 2 * PI;
        float currentX = topCentre.x + cos(angle) * radius;
        float currentZ = topCentre.z + sin(angle) * radius;
        Vec3f topPrev = Vec3f{ prevX, topY, prevZ };
        Vec3f topCurrent = Vec3f{ currentX, topY,  currentZ };
        Vec3f bottomPrev = Vec3f{ prevX, bottomY, prevZ };
        Vec3f bottomCurrent = Vec3f{ currentX, bottomY,  currentZ };
        Vec3f edge1, edge2, normal;
        // top
        pos.emplace_back(topCentre);
        pos.emplace_back(topCurrent);
        pos.emplace_back(topPrev);

        nor.emplace_back(topNormal);
        nor.emplace_back(topNormal);
        nor.emplace_back(topNormal);
        // bottom
        pos.emplace_back(bottomCentre);
        pos.emplace_back(bottomPrev);
        pos.emplace_back(bottomCurrent);

        nor.emplace_back(bottomNormal);
        nor.emplace_back(bottomNormal);
        nor.emplace_back(bottomNormal);
        // side
        pos.emplace_back(bottomCurrent);
        pos.emplace_back(bottomPrev);
        pos.emplace_back(topPrev);

        edge1 = { bottomPrev.x - bottomCurrent.x, bottomPrev.y - bottomCurrent.y, bottomPrev.z - bottomCurrent.z };
        edge2 = { topPrev.x - bottomPrev.x, topPrev.y - bottomPrev.y, topPrev.z - bottomPrev.z };
        normal = crossNormals(edge1, edge2);
        nor.emplace_back(normal);
        nor.emplace_back(normal);
        nor.emplace_back(normal);

        pos.emplace_back(topPrev);
        pos.emplace_back(topCurrent);
        pos.emplace_back(bottomCurrent);

        edge1 = { topCurrent.x - topPrev.x, topCurrent.y - topPrev.y, topCurrent.z - topPrev.z };
        edge2 = { bottomCurrent.x - topCurrent.x, bottomCurrent.y - topCurrent.y, bottomCurrent.z - topCurrent.z };
        normal = crossNormals(edge1, edge2);
        nor.emplace_back(normal);
        nor.emplace_back(normal);
        nor.emplace_back(normal);


        prevX = currentX;
        prevZ = currentZ;
    }
    std::vector<Vec3f> col(pos.size(), color);
    SimpleMeshData cylinder = SimpleMeshData{ std::move(pos),  std::move(col), std::move(nor) };
    simpleMesh = concatenate(simpleMesh, cylinder);
    return simpleMesh;
}

SimpleMeshData makePaw() {
    Vec3f topColor = Vec3f{ 0.92f, 0.99f, 0.99f };
    Vec3f bottomColor = Vec3f{ 0.51f, 0.67f, 0.89f };
    float radius = 0.2f, sizeRadius = 0.16f, bottomRadius = 0.75f;
    float height = 0.08f;

    Vec3f middleCentre = Vec3f{ 0.f, 0.f, -1.f };
    Vec3f left1 = Vec3f{ middleCentre.x - 0.9f * radius, middleCentre.y, middleCentre.z + 0.9f * radius };
    Vec3f right1 = Vec3f{ middleCentre.x + 0.9f * radius, middleCentre.y, middleCentre.z + 0.9f * radius };

    Vec3f left2 = Vec3f{ left1.x - 1.7f * radius, left1.y, left1.z - radius };
    Vec3f right2 = Vec3f{ right1.x + 1.7f * radius, right1.y, right1.z - radius };

    Vec3f left3 = Vec3f{ middleCentre.x - 1.3f * radius, middleCentre.y, middleCentre.z - 2.f * radius };
    Vec3f right3 = Vec3f{ middleCentre.x + 1.3f * radius, middleCentre.y, middleCentre.z - 2.f * radius };

    Vec3f bottomCentre = Vec3f{ middleCentre.x, middleCentre.y - height, middleCentre.z - 0.1f };

    std::vector<Vec3f> col, nor, pos;
    SimpleMeshData simpleMesh = SimpleMeshData{ std::move(pos),  std::move(col), std::move(nor) };
    // paw middle
    simpleMesh = makeCylinder(middleCentre, radius, height, topColor, simpleMesh);
    // paw left
    simpleMesh = makeCylinder(left1, radius, height, topColor, simpleMesh);
    simpleMesh = makeCylinder(left2, sizeRadius, height, topColor, simpleMesh);
    simpleMesh = makeCylinder(left3, sizeRadius, height, topColor, simpleMesh);
    // paw right
    simpleMesh = makeCylinder(right1, radius, height, topColor, simpleMesh);
    simpleMesh = makeCylinder(right2, sizeRadius, height, topColor, simpleMesh);
    simpleMesh = makeCylinder(right3, sizeRadius, height, topColor, simpleMesh);
    // bottom
    simpleMesh = makeCylinder(bottomCentre, bottomRadius, height, bottomColor, simpleMesh);
    return simpleMesh;
}

SimpleMeshData makeTriangularCone(Vec3f vertices[], SimpleMeshData simpleMesh, Vec3f color) {
    std::vector<Vec3f> pos, nor;
    Vec3f edge1, edge2, normal;
    // front
    pos.emplace_back(vertices[0]);
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[1]);
    for (int i = 0; i < 3; i++) {
        nor.emplace_back(vertices[0] - vertices[3]);
    }
    // back
    pos.emplace_back(vertices[3]);
    pos.emplace_back(vertices[4]);
    pos.emplace_back(vertices[5]);
    for (int i = 0; i < 3; i++) {
        nor.emplace_back(vertices[3] - vertices[0]);
    }
    // left
    pos.emplace_back(vertices[3]);
    pos.emplace_back(vertices[5]);
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[0]);
    pos.emplace_back(vertices[3]);
    edge1 = Vec3f{ vertices[5].x - vertices[3].x, vertices[5].y - vertices[3].y, vertices[5].z - vertices[3].z };
    edge2 = Vec3f{ vertices[2].x - vertices[5].x, vertices[2].y - vertices[5].y, vertices[2].z - vertices[5].z };
    normal = crossNormals(edge1, edge2);
    for (int i = 0; i < 6; i++) {
        nor.emplace_back(normal);
    }
    // right
    pos.emplace_back(vertices[3]);
    pos.emplace_back(vertices[0]);
    pos.emplace_back(vertices[1]);
    pos.emplace_back(vertices[1]);
    pos.emplace_back(vertices[4]);
    pos.emplace_back(vertices[3]);
    edge1 = Vec3f{ vertices[0].x - vertices[3].x, vertices[0].y - vertices[3].y, vertices[0].z - vertices[3].z };
    edge2 = Vec3f{ vertices[1].x - vertices[0].x, vertices[1].y - vertices[0].y, vertices[1].z - vertices[0].z };
    normal = crossNormals(edge1, edge2);
    for (int i = 0; i < 6; i++) {
        nor.emplace_back(normal);
    }
    // bottom
    pos.emplace_back(vertices[1]);
    pos.emplace_back(vertices[2]);
    pos.emplace_back(vertices[5]);
    pos.emplace_back(vertices[5]);
    pos.emplace_back(vertices[4]);
    pos.emplace_back(vertices[1]);
    edge1 = Vec3f{ vertices[2].x - vertices[1].x, vertices[2].y - vertices[1].y, vertices[2].z - vertices[1].z };
    edge2 = Vec3f{ vertices[5].x - vertices[2].x, vertices[5].y - vertices[2].y, vertices[5].z - vertices[2].z };
    normal = crossNormals(edge1, edge2);
    for (int i = 0; i < 6; i++) {
        nor.emplace_back(normal);
    }
    std::vector col(pos.size(), color);
    SimpleMeshData triangularCone = SimpleMeshData{ std::move(pos), std::move(col), std::move(nor) };
    simpleMesh = concatenate(simpleMesh, triangularCone);
    return simpleMesh;
}

SimpleMeshData makeHouse() {
    std::vector<Vec3f> col, nor, pos;
    Vec3f columnColor = { 0.77f, 0.54f, 0.25f };
    Vec3f wallColor = { 0.90f, 0.73f, 0.45f };
    SimpleMeshData house = SimpleMeshData{ std::move(pos),  std::move(col), std::move(nor) };

    float width = 0.1f, height = 0.5f;
    float roofWidth = 0.5f, columnWidth = 0.08f, wallWidth = 0.15f;
    float houseHeight = 0.7f, wallHeight = 0.6f, topWallHeight = 0.2f;
    float houseLength = 0.5f, wallLength = 0.05f;
    float zd = 0.05f, xd = 0.05f;
    float x = width / sqrt(2.f), y = width / sqrt(2.f) + height;
    float sideWallLength = houseLength - 3 * zd;
    float topRoofLength = 2 * columnWidth + sideWallLength + 4 * zd;
    // additional information
    Vec3f v1l = Vec3f{ -roofWidth / sqrt(2.f), 0.f, -zd };
    Vec3f v1r = Vec3f{ roofWidth / sqrt(2.f), 0.f, -zd };
    // top roof
    Vec3f v1 = Vec3f{ 0.f, height, 0.f };
    Vec3f v2 = Vec3f{ x, y, 0 };
    Vec3f v3 = Vec3f{ -x, y, 0 };
    Vec3f v4 = Vec3f{ 0.f, width * sqrt(2.f) + height, 0.f };
    Vec3f v1b = Vec3f{ 0.f, height, -topRoofLength };
    Vec3f v2b = Vec3f{ x, y, -topRoofLength };
    Vec3f v3b = Vec3f{ -x, y, -topRoofLength };
    Vec3f v4b = Vec3f{ 0.f, width * sqrt(2.f) + height, -topRoofLength };
    Vec3f topRoof[] = { v4, v2, v1, v3, v4b, v2b, v1b, v3b };
    house = makeCube(topRoof, house, columnColor, false);

    // left front column
    Vec3f v1l2 = Vec3f{ v1l.x + xd / sqrt(2.f) , v1l.y + xd , -2 * zd };
    Vec3f v1ld1 = Vec3f{ v1l2.x, v1l2.y - houseHeight, v1l2.z };
    Vec3f v1rd1 = Vec3f{ v1l2.x + columnWidth, v1l2.y - houseHeight, v1l2.z };
    Vec3f v1l3 = Vec3f{ v1l2.x + columnWidth, v1l2.y, v1l2.z };
    Vec3f v1l2Columnb = Vec3f{ v1l2.x, v1l2.y, v1l2.z - columnWidth };
    Vec3f v1ld1Columnb = Vec3f{ v1ld1.x, v1ld1.y, v1ld1.z - columnWidth };
    Vec3f v1rd1Columnb = Vec3f{ v1rd1.x, v1rd1.y, v1rd1.z - columnWidth };
    Vec3f v1l3Columnb = Vec3f{ v1l3.x, v1l3.y, v1l3.z - columnWidth };
    Vec3f leftFrontColumn[] = { v1l2, v1l3, v1rd1, v1ld1, v1l2Columnb, v1l3Columnb, v1rd1Columnb, v1ld1Columnb };
    house = makeCube(leftFrontColumn, house, columnColor, false);

    // left back column
    Vec3f leftBackColumn0 = Vec3f{ v1l2.x, v1l2.y, v1l2.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn1 = Vec3f{ v1l3.x, v1l3.y, v1l3.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn2 = Vec3f{ v1rd1.x, v1rd1.y, v1rd1.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn3 = Vec3f{ v1ld1.x, v1ld1.y, v1ld1.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn4 = Vec3f{ v1l2Columnb.x, v1l2Columnb.y, v1l2Columnb.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn5 = Vec3f{ v1l3Columnb.x, v1l3Columnb.y, v1l3Columnb.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn6 = Vec3f{ v1rd1Columnb.x, v1rd1Columnb.y, v1rd1Columnb.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn7 = Vec3f{ v1ld1Columnb.x, v1ld1Columnb.y, v1ld1Columnb.z - columnWidth - sideWallLength };
    Vec3f leftBackColumn[] = { leftBackColumn0, leftBackColumn1, leftBackColumn2, leftBackColumn3, leftBackColumn4, leftBackColumn5, leftBackColumn6, leftBackColumn7 };
    house = makeCube(leftBackColumn, house, columnColor, false);

    // right front column
    Vec3f v1r2 = Vec3f{ v1r.x - xd / sqrt(2.f), v1r.y + xd, -2 * zd };
    Vec3f v1r3 = Vec3f{ v1r2.x - columnWidth, v1r2.y, v1r2.z };
    Vec3f v1ld2 = Vec3f{ v1r2.x - columnWidth, v1r2.y - houseHeight, v1r2.z };
    Vec3f v1rd2 = Vec3f{ v1r2.x, v1l2.y - houseHeight, v1l2.z };
    Vec3f v1r2Columnb = Vec3f{ v1r2.x, v1r2.y, v1r2.z - columnWidth };
    Vec3f v1r3Columnb = Vec3f{ v1r3.x, v1r3.y, v1r3.z - columnWidth };
    Vec3f v1ld2Columnb = Vec3f{ v1ld2.x, v1ld2.y, v1ld2.z - columnWidth };
    Vec3f v1rd2Columnb = Vec3f{ v1rd2.x, v1rd2.y, v1rd2.z - columnWidth };
    Vec3f rightFrontColumn[] = { v1r3, v1r2, v1rd2, v1ld2, v1r3Columnb, v1r2Columnb, v1rd2Columnb, v1ld2Columnb };
    house = makeCube(rightFrontColumn, house, columnColor, false);

    // right back column
    Vec3f rightBackColumn0 = Vec3f{ v1r3.x, v1r3.y, v1r3.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn1 = Vec3f{ v1r2.x, v1r2.y, v1r2.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn2 = Vec3f{ v1rd2.x, v1rd2.y, v1rd2.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn3 = Vec3f{ v1ld2.x, v1ld2.y, v1ld2.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn4 = Vec3f{ v1r3Columnb.x, v1r3Columnb.y, v1r3Columnb.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn5 = Vec3f{ v1r2Columnb.x, v1r2Columnb.y, v1r2Columnb.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn6 = Vec3f{ v1rd2Columnb.x, v1rd2Columnb.y, v1rd2Columnb.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn7 = Vec3f{ v1ld2Columnb.x, v1ld2Columnb.y, v1ld2Columnb.z - columnWidth - sideWallLength };
    Vec3f rightBackColumn[] = { rightBackColumn0, rightBackColumn1, rightBackColumn2, rightBackColumn3, rightBackColumn4, rightBackColumn5, rightBackColumn6, rightBackColumn7 };
    house = makeCube(rightBackColumn, house, columnColor, false);

    // front left wall
    Vec3f v1l4 = Vec3f{ v1l3.x + wallWidth, v1l3.y, v1l3.z };
    Vec3f v1rd3 = Vec3f{ v1l3.x + wallWidth, v1l3.y - wallHeight, v1l3.z };
    Vec3f v1ld3 = Vec3f{ v1l3.x, v1l3.y - wallHeight, v1l3.z };
    Vec3f v1l3Wallb = Vec3f{ v1l3.x, v1l3.y, v1l3.z - wallLength };
    Vec3f v1l4Wallb = Vec3f{ v1l4.x, v1l4.y, v1l4.z - wallLength };
    Vec3f v1rd3Wallb = Vec3f{ v1rd3.x, v1rd3.y, v1rd3.z - wallLength };
    Vec3f v1ld3Wallb = Vec3f{ v1ld3.x, v1ld3.y, v1ld3.z - wallLength };
    Vec3f frontLeftWall[] = { v1l3, v1l4, v1rd3, v1ld3, v1l3Wallb, v1l4Wallb, v1rd3Wallb, v1ld3Wallb };
    house = makeCube(frontLeftWall, house, wallColor, false);

    // front right wall
    Vec3f v1r4 = Vec3f{ v1r3.x - wallWidth, v1r3.y, v1r3.z };
    Vec3f v1rd4 = Vec3f{ v1r3.x, v1r3.y - wallHeight, v1r3.z };
    Vec3f v1ld4 = Vec3f{ v1r3.x - wallWidth, v1r3.y - wallHeight, v1r3.z };
    Vec3f v1r3Wallb = Vec3f{ v1r3.x, v1r3.y, v1r3.z - wallLength };
    Vec3f v1r4Wallb = Vec3f{ v1r4.x, v1r4.y, v1r4.z - wallLength };
    Vec3f v1rd4Wallb = Vec3f{ v1rd4.x, v1rd4.y, v1rd4.z - wallLength };
    Vec3f v1ld4Wallb = Vec3f{ v1ld4.x, v1ld4.y, v1ld4.z - wallLength };
    Vec3f frontRighttWall[] = { v1r4, v1r3, v1rd4, v1ld4, v1r4Wallb, v1r3Wallb, v1rd4Wallb, v1ld4Wallb };
    house = makeCube(frontRighttWall, house, wallColor, false);

    // front top wall
    Vec3f v1ld5 = Vec3f{ v1l4.x, v1l4.y - topWallHeight, v1l4.z };
    Vec3f v1rd5 = Vec3f{ v1r4.x, v1r4.y - topWallHeight, v1r4.z };
    Vec3f v1ld5Wallb = Vec3f{ v1ld5.x, v1ld5.y, v1ld5.z - wallLength };
    Vec3f v1rd5Wallb = Vec3f{ v1rd5.x, v1rd5.y, v1rd5.z - wallLength };
    Vec3f frontTopWall[] = { v1l4, v1r4, v1rd5, v1ld5, v1l4Wallb, v1r4Wallb, v1rd5Wallb, v1ld5Wallb };
    house = makeCube(frontTopWall, house, wallColor, false);

    // front roof wall
    Vec3f v1zd2 = Vec3f{ 0.f, height, -2 * zd };
    Vec3f v1l2Wallb = Vec3f{ v1l2.x, v1l2.y, v1l2.z - wallLength };
    Vec3f v1zd2Wallb = Vec3f{ v1zd2.x, v1zd2.y, v1zd2.z - wallLength };
    Vec3f v1r2Wallb = Vec3f{ v1r2.x, v1r2.y, v1r2.z - wallLength };
    Vec3f frontRoofWall[] = { v1zd2, v1r2, v1l2, v1zd2Wallb, v1r2Wallb, v1l2Wallb };
    house = makeTriangularCone(frontRoofWall, house, wallColor);

    // back roof wall
    float roofWallDiff = sideWallLength + 2 * columnWidth - wallLength;
    Vec3f backRoofWall0 = { v1zd2.x, v1zd2.y, v1zd2.z - roofWallDiff };
    Vec3f backRoofWall1 = { v1r2.x, v1r2.y, v1r2.z - roofWallDiff };
    Vec3f backRoofWall2 = { v1l2.x, v1l2.y, v1l2.z - roofWallDiff };
    Vec3f backRoofWall3 = { v1zd2Wallb.x, v1zd2Wallb.y, v1zd2Wallb.z - roofWallDiff };
    Vec3f backRoofWall4 = { v1r2Wallb.x, v1r2Wallb.y, v1r2Wallb.z - roofWallDiff };
    Vec3f backRoofWall5 = { v1l2Wallb.x, v1l2Wallb.y, v1l2Wallb.z - roofWallDiff };
    Vec3f backRoofWall[] = { backRoofWall0, backRoofWall1, backRoofWall2, backRoofWall3, backRoofWall4, backRoofWall5 };
    house = makeTriangularCone(backRoofWall, house, wallColor);

    // back wall
    Vec3f backWall0 = Vec3f{ v1l3.x, v1l3.y, v1l3.z - roofWallDiff };
    Vec3f backWall1 = Vec3f{ v1r3.x, v1r3.y, v1r3.z - roofWallDiff };
    Vec3f backWall2 = Vec3f{ v1rd4.x, v1rd4.y, v1rd4.z - roofWallDiff };
    Vec3f backWall3 = Vec3f{ v1ld3.x, v1ld3.y, v1ld3.z - roofWallDiff };
    Vec3f backWall4 = Vec3f{ backWall0.x, backWall0.y, backWall0.z - wallLength };
    Vec3f backWall5 = Vec3f{ backWall1.x, backWall1.y, backWall1.z - wallLength };
    Vec3f backWall6 = Vec3f{ backWall2.x, backWall2.y, backWall2.z - wallLength };
    Vec3f backWall7 = Vec3f{ backWall3.x, backWall3.y, backWall3.z - wallLength };
    Vec3f backWall[] = { backWall0, backWall1, backWall2, backWall3, backWall4, backWall5, backWall6, backWall7 };
    house = makeCube(backWall, house, wallColor, false);

    // left wall
    Vec3f leftWall0 = Vec3f{ v1l2.x, v1l2.y, v1l2.z - columnWidth };
    Vec3f leftWall1 = Vec3f{ v1l3.x, v1l3.y, v1l3.z - columnWidth };
    Vec3f leftWall2 = Vec3f{ v1ld3.x, v1ld3.y, v1ld3.z - columnWidth };
    Vec3f leftWall3 = Vec3f{ leftWall2.x - columnWidth, leftWall2.y, leftWall2.z };
    Vec3f leftWall4 = Vec3f{ leftWall0.x, leftWall0.y, leftWall0.z - sideWallLength };
    Vec3f leftWall5 = Vec3f{ leftWall1.x, leftWall1.y, leftWall1.z - sideWallLength };
    Vec3f leftWall6 = Vec3f{ leftWall2.x, leftWall2.y, leftWall2.z - sideWallLength };
    Vec3f leftWall7 = Vec3f{ leftWall3.x, leftWall3.y, leftWall3.z - sideWallLength };
    Vec3f leftWall[] = { leftWall0, leftWall1, leftWall2, leftWall3, leftWall4, leftWall5, leftWall6, leftWall7 };
    house = makeCube(leftWall, house, wallColor, false);
    // right wall
    Vec3f rightWall0 = Vec3f{ v1r3.x, v1r3.y, v1r3.z - columnWidth };
    Vec3f rightWall1 = Vec3f{ v1r2.x, v1r2.y, v1r2.z - columnWidth };
    Vec3f rightWall2 = Vec3f{ v1rd2.x, leftWall2.y, v1rd2.z - columnWidth };
    Vec3f rightWall3 = Vec3f{ rightWall2.x - columnWidth, rightWall2.y, rightWall2.z };
    Vec3f rightWall4 = Vec3f{ rightWall0.x, rightWall0.y, rightWall0.z - sideWallLength };
    Vec3f rightWall5 = Vec3f{ rightWall1.x, rightWall1.y, rightWall1.z - sideWallLength };
    Vec3f rightWall6 = Vec3f{ rightWall2.x, rightWall2.y, rightWall2.z - sideWallLength };
    Vec3f rightWall7 = Vec3f{ rightWall3.x, rightWall3.y, rightWall3.z - sideWallLength };
    Vec3f rightWall[] = { rightWall0, rightWall1, rightWall2, rightWall3, rightWall4, rightWall5, rightWall6, rightWall7 };
    house = makeCube(rightWall, house, wallColor, false);

    return house;
}

SimpleMeshData makeBed() {
    Vec3f bedColor = Vec3f{ 0.12f, 0.08f, 0.06f };
    SimpleMeshData bed = load_wavefront_obj("assets/Dogbed.obj", bedColor);
    return bed;
}

SimpleMeshData makeBall() {
    Vec3f ballColor = Vec3f{ 0.73f, 0.87f, 0.84f };
    SimpleMeshData ball = load_wavefront_obj("assets/Tennisball.obj", ballColor);
    return ball;
}

Vec3f crossNormals(Vec3f aLeft, Vec3f aRight) {
    glm::vec3 edge1 = { aLeft.x, aLeft.y, aLeft.z };
    glm::vec3 edge2 = { aRight.x, aRight.y, aRight.z };
    glm::vec3 normals = glm::normalize(glm::cross(edge1, edge2));
    return Vec3f{ normals.x, normals.y, normals.z };
}