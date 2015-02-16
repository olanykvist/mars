#ifndef MARS_VECTOR3_H
#define MARS_VECTOR3_H

namespace MARS
{
	class Vector3
	{
	public:
		Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		float distanceTo(const Vector3& vector) const;
		float x;
		float y;
		float z;
	};
};

#endif