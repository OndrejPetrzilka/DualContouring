#pragma once

#include "threed\isomesher_dc.h"
#include "threed\mesh.h"
#include <math.h>

using namespace SharpDX;
using namespace ThreeD;

namespace DualContouring
{
	public ref struct Face
	{
	public:
		Face(const Vector& v0, const Vector& v1, const Vector& v2, const Vector& n0, const Vector& n1, const Vector& n2)
			:v0(ToVector3(v0)), v1(ToVector3(v1)), v2(ToVector3(v2)), n0(ToVector3(n0)), n1(ToVector3(n1)), n2(ToVector3(n2))
		{
		}

		Vector3 v0;
		Vector3 v1;
		Vector3 v2;
		Vector3 n0;
		Vector3 n1;
		Vector3 n2;

	private:
		static Vector3 ToVector3(const Vector& v)
		{
			return Vector3(v.x(), v.y(), v.z());
		}
	};
}