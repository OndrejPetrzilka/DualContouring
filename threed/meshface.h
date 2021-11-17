//----------------------------------------------------------------------------
// ThreeD Triangle Mesh Face
//----------------------------------------------------------------------------

#ifndef _THREED_MESHFACE_H
#define _THREED_MESHFACE_H

#include <threed/color.h>
#include <threed/plane.h>

namespace ThreeD {


/**
 * MeshFace
 */
class MeshFace
{
public:
    /**
     * Constructor.
     *
     * Constructs a mesh face as a triangle bound by the three vertices
     * @p v0, @p v1 and @p v2, and having color @p c.
     */
    MeshFace(const Vector *v0, const Vector *v1, const Vector *v2)
    {
        _v[0] = v0;
        _v[1] = v1;
        _v[2] = v2;
        _planeNormal = Plane(*_v[2], *_v[1], *_v[0]).normal().normalized();
    }

    /** Return pointer to vertex n. */
    const Vector *vertexPtr(int n) const { return _v[n]; }

    /** Return actual value of vertex n. */
    const Vector &vertex(int n) const { return *_v[n]; }

    /** Return plane. *//*
    const Plane plane() const
    {
        return Plane(vertex(2), vertex(1), vertex(0));
    }*/

    /** Return normalized plane normal. */
    const Vector planeNormal() const
    {
        return _planeNormal;
    }

    /** Return minimum and maximum points in each dimension.
     */
    void getMinMax(Vector *vmin, Vector *vmax) const;

    /** Set vertex pointers. */
    void setVertexPointers(const Vector **vptrs)
    {
        _v[0] = vptrs[0];
        _v[1] = vptrs[1];
        _v[2] = vptrs[2];
    }

    /** Is Between
     */
    bool isBetween(const Vector &vmin, const Vector &vmax) const
    {
        return (vertex(0).isBetween(vmin, vmax) &&
                vertex(1).isBetween(vmin, vmax) &&
                vertex(2).isBetween(vmin, vmax));
    }

	//void draw()
	//{
	//	for (i = 0; i < 3; ++i) 
	//	{
	//		const Vector &vertex = _v[i][0];
	//		const Vector &normal = _v[i][1];
	//		/*if (normal == ThreeD::Vector())
	//			glNormal3f(planeNormal().x(), planeNormal().y(), -planeNormal().z());
	//		else
	//			glNormal3f(normal.x(), normal.y(), -normal.z());

	//		glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	//		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	//		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//		glMaterialf(GL_FRONT, GL_SHININESS, _shininess);
	//		glColor3f(_c.red(), _c.green(), _c.blue());

	//		glVertex3f(vertex.x(), vertex.y(), -vertex.z());*/
	//	}
	//}

	const Vector& getNormal(int i)
	{
		return _v[i][1] == Vector() ? _planeNormal : _v[i][1];
	}

public:

    /*
     * data
     */

    const Vector *_v[3];

    Color _c;
    Color _ambient;
    Color _diffuse;
    Color _specular;
    float _shininess;

    Vector _planeNormal;
};


} // namespace ThreeD
#endif // _THREED_MESHFACE_H
