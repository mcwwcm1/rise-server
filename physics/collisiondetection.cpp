// File: collisiondetection.cpp
// Purpose: Implements collisiondetection.h

#include "collisiondetection.h"
#include <iostream>

Float3 support(Shape shape1, Shape shape2, Float3 direction)
{
	return (shape1.getFurthestPointInDirection(direction) + shape1.position) - (shape2.getFurthestPointInDirection(direction * -1) + shape2.position);
}

bool sameDirection(Float3 a, Float3 b)
{
	return dot(a, b) > 0;
}

CollisionResult checkCollision(Shape shape1, Shape shape2)
{
	CollisionResult r = CollisionResult();

	Simplex simplex = Simplex();
	Float3 s = support(shape1, shape2, (Float3(1, 0, 0)).normalized());
	simplex.pushFront(s);
	
	std::cout<<s.str()<<"\n";

	Float3 d = s * -1;

	while(true)
	{
		s = support(shape1, shape2, d);
		std::cout<< "S" << s.str()<<"\n";
		std::cout<< "D" << d.str()<<"\n";

		if(dot(s, d) <= 0)
			return r;
		
		simplex.pushFront(s);

		// Maybe make the origin a constant to avoid memory allocation?
		if(simplex.doSimplex(d))
		{
			r.collided = true;
			// Do EPA
			return r;
		}
	}
}

bool Simplex::line(Float3& direction)
{
	cout<<"LINE\n";

	Float3 a = points[0];
	Float3 b = points[1];
	Float3 AB = b - a;
	Float3 AO = a * -1;

	if(sameDirection(AB, AO))
		direction = tripleProduct(AB, AO, AB);
	else
	{
		pointCount = 1;
		direction = AO;
	}
	return false;
}

bool Simplex::triangle(Float3& direction)
{
	cout<<"TRIANGLE\n";
	Float3 a = points[0];
	Float3 b = points[1];
	Float3 c = points[2];

	Float3 AB = b - a;
	Float3 AC = c - a;
	Float3 AO = a * -1;
	
	Float3 ABC = cross(AB, AC);

	if(sameDirection(cross(ABC, AC), AO))
	{
		if(sameDirection(AC, AO))
		{
			points[1] = c;
			pointCount = 2;
			direction = tripleProduct(AC, AO, AC);
		}
		else
		{
			pointCount = 2;
			return line(direction);
		}
	}

	else
	{
		if(sameDirection(cross(AB, ABC), AO))
		{
			pointCount = 2;
			return line(direction);
		}
		else
		{
			if(sameDirection(ABC, AO))
				direction = ABC;
			
			else
			{
				points[3] = points[2];
				points[2] = points[1];
				points[1] = points[3];
				direction = ABC * -1;
			}
		}
	}

	return false;
}

bool Simplex::tetrahedron(Float3& direction)
{
	cout<<"TETRAHEDRON\n";
	Float3 a = points[0];
	Float3 b = points[1];
	Float3 c = points[2];
	Float3 d = points[3];

	Float3 AB = b - a;
	Float3 AC = c - a;
	Float3 AD = d - a;
	Float3 AO = a * -1;
	
	Float3 ABC = cross(AB, AC);
	Float3 ACD = cross(AC, AD);
	Float3 ADB = cross(AD, AB);

	if(sameDirection(ABC, AO))
	{
		pointCount = 3;
		return triangle(direction);
	}

	if(sameDirection(ACD, AO))
	{
		points[1] = points[2];
		points[2] = points[3];
		pointCount = 3;
		return triangle(direction);
	}

	if(sameDirection(ADB, AO))
	{
		points[2] = points[1];
		points[1] = points[3];
		pointCount = 3;
		return triangle(direction);
	}

	return true;
}

void handleCollision(Shape shape1, Shape shape2, Simplex simplex, CollisionResult& collissionResult)
{
	std::vector<Float3> polytope(simplex.begin(), simplex.end());
	std::vector<size_t>  faces = {
		0, 1, 2,
		0, 3, 1,
		0, 2, 3,
		1, 3, 2
	};

	auto [normals, minFace] = GetFaceNormals(polytope, faces);

	Float3 minNormal;
	float   minDistance = FLT_MAX;
	
	while (minDistance == FLT_MAX) {
		minNormal = normals[minFace].xyz();
		minDistance = normals[minFace].w;
 
		Float3 s = support(shape1, shape2, minNormal);
		float sDistance = dot(minNormal, s);
 
		if (abs(sDistance - minDistance) > 0.001f) {
			minDistance = FLT_MAX;

		std::vector<std::pair<size_t, size_t>> uniqueEdges;

		for (size_t i = 0; i < normals.size(); i++) {
			if (sameDirection(normals[i].xyz(), s)) {
				size_t f = i * 3;

				AddIfUniqueEdge(uniqueEdges, faces, f,     f + 1);
				AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
				AddIfUniqueEdge(uniqueEdges, faces, f + 2, f    );

				faces[f + 2] = faces.back(); faces.pop_back();
				faces[f + 1] = faces.back(); faces.pop_back();
				faces[f    ] = faces.back(); faces.pop_back();

				normals[i] = normals.back(); normals.pop_back();

				i--;
			}
		}

		std::vector<size_t> newFaces;
		for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) {
			newFaces.push_back(edgeIndex1);
			newFaces.push_back(edgeIndex2);
			newFaces.push_back(polytope.size());
		}
			
		polytope.push_back(s);

		auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);

					float oldMinDistance = FLT_MAX;
			for (size_t i = 0; i < normals.size(); i++) {
				if (normals[i].w < oldMinDistance) {
					oldMinDistance = normals[i].w;
					minFace = i;
				}
			}
 
			if (newNormals[newMinFace].w < oldMinDistance) {
				minFace = newMinFace + normals.size();
			}
 
			faces  .insert(faces  .end(), newFaces  .begin(), newFaces  .end());
			normals.insert(normals.end(), newNormals.begin(), newNormals.end());
		}
	}
 
	collissionResult.normal = minNormal;
	collissionResult.displacement = minDistance + 0.001f;
}

std::pair<std::vector<Float4>, size_t> GetFaceNormals(
	const std::vector<Float3>& polytope,
	const std::vector<size_t>&  faces)
{
	std::vector<Float4> normals;
	size_t minTriangle = 0;
	float  minDistance = FLT_MAX;

	for (size_t i = 0; i < faces.size(); i += 3) {
		Float3 a = polytope[faces[i    ]];
		Float3 b = polytope[faces[i + 1]];
		Float3 c = polytope[faces[i + 2]];

		Float3 normal = cross(b - a, c - a).normalized();
		float distance = dot(normal, a);

		if (distance < 0) {
			normal   *= -1;
			distance *= -1;
		}

		normals.emplace_back(normal, distance);

		if (distance < minDistance) {
			minTriangle = i / 3;
			minDistance = distance;
		}
	}

	return { normals, minTriangle };
}

void AddIfUniqueEdge(
	std::vector<std::pair<size_t, size_t>>& edges,
	const std::vector<size_t>& faces,
	size_t a,
	size_t b)
{
	auto reverse = std::find(
		edges.begin(),
		edges.end(),
		std::make_pair(faces[b], faces[a])
	);
 
	if (reverse != edges.end()) {
		edges.erase(reverse);
	}
 
	else {
		edges.emplace_back(faces[a], faces[b]);
	}
}

bool Simplex::doSimplex(Float3& direction)
{
	switch(pointCount)	{
		case 2: // Line
			return line(direction);
		case 3: // Triangle
			return triangle(direction);
		case 4: // Tetrahedron
			return tetrahedron(direction);
	}
	
	return false;
}

void Simplex::pushFront(Float3 point)
{
	points[3] = points[2];
	points[2] = points[1];
	points[1] = points[0];
	points[0] = point;
	
	pointCount = std::min(pointCount + 1, 4);
}