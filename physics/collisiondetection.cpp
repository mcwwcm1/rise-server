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