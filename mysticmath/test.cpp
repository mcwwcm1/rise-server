#include <iostream>
#include "float3.h"
#include "quaternion.h"
#include "matrix4x4.h"
#include "mysticmath.h"

using namespace std;

int main()
{
	Matrix4x4 m1 = getTranslationMatrix(Float3(10, 0, 0));
	Matrix4x4 m2 = getRotationMatrix(Quaternion::fromEuler(Float3(0, 90, 0)));
	Matrix4x4 m3 = getScaleMatrix(Float3(5,5,5));

	cout << (Float3(0, 0, 1) * m2).str();

	return 0;

	Matrix4x4 m = m1 * m2;
	
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			cout << *m[x * 4 + y] << ";	";
		}
		cout << "\n";
	}
	return 0;
}