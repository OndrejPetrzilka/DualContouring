//----------------------------------------------------------------------------
// ThreeD Triangle Mesh Face
//----------------------------------------------------------------------------

#include <threed/meshface.h>

using namespace ThreeD;

//----------------------------------------------------------------------------

void MeshFace::getMinMax(Vector *vmin, Vector *vmax) const
{
    float vmin_x, vmax_x;
    float vmin_y, vmax_y;
    float vmin_z, vmax_z;
    vmin_x = vmax_x = vertex(0).x();
    vmin_y = vmax_y = vertex(0).y();
    vmin_z = vmax_z = vertex(0).z();

    for (int n = 1; n < 3; n++) {
        const Vector &v = vertex(n);
        if (v.x() < vmin_x)
            vmin_x = v.x();
        if (v.x() > vmax_x)
            vmax_x = v.x();
        if (v.y() < vmin_y)
            vmin_y = v.y();
        if (v.y() > vmax_y)
            vmax_y = v.y();
        if (v.z() < vmin_z)
            vmin_z = v.z();
        if (v.z() > vmax_z)
            vmax_z = v.z();
    }

    *vmin = Vector(vmin_x, vmin_y, vmin_z);
    *vmax = Vector(vmax_x, vmax_y, vmax_z);
}

//----------------------------------------------------------------------------

