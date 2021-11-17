
#include "ImplicitIsosurface.h"

//----------------------------------------------------------------------------

double ImplicitIsosurface::calcDensity(float xt, float yt, float zt)
{
    return densityFunction(xt, yt, zt);
}

//----------------------------------------------------------------------------

void ImplicitIsosurface::fDensity(
    float x0, float y0, float z0,
    float dz, int num_points, float *densities)
{
    for (int i = 0; i < num_points; ++i) {
        float xt, yt, zt;
        _globalTransInv.transform(x0, y0, z0, &xt, &yt, &zt);

        densities[i] = calcDensity(xt, yt, zt);

        z0 += dz;
    }
}

//----------------------------------------------------------------------------

void ImplicitIsosurface::fNormal(
    const ThreeD::Vector *point, ThreeD::Vector *normal)
{
    float xt, yt, zt;
    _globalTransInv.transform(
        point->x(), point->y(), point->z(),
        &xt, &yt, &zt);

    double d = calcDensity(xt, yt, zt);
    float nx = calcDensity(xt + 0.001f, yt, zt) - d;
    float ny = calcDensity(xt, yt + 0.001f, zt) - d;
    float nz = calcDensity(xt, yt, zt + 0.001f) - d;

    _globalTransInv.transformNormal(nx, ny, nz, &nx, &ny, &nz);
    *normal = ThreeD::Vector(nx, ny, nz).normalized();
}

//----------------------------------------------------------------------------

//const ThreeD::Isosurface::Material &ImplicitIsosurface::fMaterial(
//    const ThreeD::Vector *point, float density)
//{
//    return _mat;
//}
