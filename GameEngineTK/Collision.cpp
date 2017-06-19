#include "Collision.h"

using namespace DirectX::SimpleMath;

//float VectorLengthSQ(const Vector3& v);

//float DistanceSQ3D(const Vector3& p1, const Vector3& p2);

/// <summary>
/// ���̋��̂����蔻��
/// </summary>
/// <param name="sphereA">��A</param>
/// <param name="sphereB">��B</param>
/// <returns>true: �������Ă��� false: �������Ă��Ȃ�</returns>
bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB)
{
	// ���S���W�Ԃ̍����v�Z
	Vector3 sub = sphereB.Center - sphereA.Center;
	// ���S���W�Ԃ̋������v�Z
	float distanceSquare = sub.x * sub.x + sub.y * sub.y + sub.z * sub.z;
	// ���a�̘a
	float radiusSquare = sphereA.Radius + sphereB.Radius;
	radiusSquare = radiusSquare * radiusSquare;

	// ���������a�̘a���傫����΁A�������Ă��Ȃ�
	if (distanceSquare > radiusSquare)
	{
		return false;
	}

	return true;
}