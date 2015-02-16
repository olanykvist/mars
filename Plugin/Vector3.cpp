#include "Vector3.h"
#include <cmath>

namespace MARS
{
	Vector3::Vector3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{
	}

	float Vector3::distanceTo(const Vector3& vector) const
	{
		float dx = this->x - vector.x;
		float dy = this->y - vector.y;
		float dz = this->z - vector.z;

		return sqrtf((dx * dx) + (dy * dy) + (dz * dz));
	}
}