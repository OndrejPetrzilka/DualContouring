
#include <threed/boundingbox.h>
#include <threed/color.h>
#include <threed/vector.h>
#include <threed/object.h>
#include <threed/matrix.h>
#include <threed/transform.h>
#include <threed/mesh.h>
#include <threed/meshface.h>
#include <threed/isosurface.h>

class ImplicitIsosurface : public ThreeD::Isosurface 
{
public:
	double(*densityFunction)(float, float, float);

    ImplicitIsosurface(ThreeD::Vector min, ThreeD::Vector max, double(*densityFunction)(float, float, float))
	{
		Isosurface::addBoundingBox(ThreeD::BoundingBox(min, max));
		Isosurface::setTransform(ThreeD::Transform());
		this->densityFunction = densityFunction;
	}

    inline double calcDensity(float xt, float yt, float zt);
    virtual void fDensity(float x0, float y0, float z0, float dz, int num_points, float *densities);
    virtual void fNormal(const ThreeD::Vector *point, ThreeD::Vector *normal);

    /*virtual const Material &fMaterial(
        const ThreeD::Vector *point, float density);*/
};
