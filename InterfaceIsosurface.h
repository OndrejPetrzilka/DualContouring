#include <threed/boundingbox.h>
#include <threed/color.h>
#include <threed/vector.h>
#include <threed/object.h>
#include <threed/matrix.h>
#include <threed/transform.h>
#include <threed/mesh.h>
#include <threed/meshface.h>
#include <threed/isosurface.h>
#include "Face.h"
#include <vcclr.h>

using namespace System;
using namespace SharpDX;
using namespace ThreeD;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;

namespace DualContouring
{
	public interface class IIsosurface
	{
		void fDensity(Vector3% pos, float dz, int num_points, float *densities);
		void fNormal(Vector3% point, [Out] Vector3% normal);
	};

	class InterfaceIsosurface : public ThreeD::Isosurface 
	{
	private:
		gcroot<IIsosurface^> surface;

	public:
		InterfaceIsosurface(ThreeD::Vector min, ThreeD::Vector max, IIsosurface^ surface)
		{
			Isosurface::addBoundingBox(ThreeD::BoundingBox(min, max));
			Isosurface::setTransform(ThreeD::Transform());
			this->surface = surface;
		}

		virtual void fDensity(float x0, float y0, float z0, float dz, int num_points, float *densities)
		{
			surface->fDensity(Vector3(x0, y0, z0), dz, num_points, densities);
		}

		virtual void fNormal(const ThreeD::Vector *point, ThreeD::Vector *normal)
		{
			Vector3 n;
			surface->fNormal(Vector3(point->x(), point->y(), point->z()), n);
			*normal = ThreeD::Vector(n.X, n.Y, n.Z);
		}
	};
}