
class BoxIsosurface : public ThreeD::Isosurface {
public:
    BoxIsosurface(const ThreeD::Vector &size);

    inline double calcDensity(float xt, float yt, float zt);

    virtual void fDensity(
        float x0, float y0, float z0,
        float dz, int num_points, float *densities);

    virtual void fNormal(
        const ThreeD::Vector *point, ThreeD::Vector *normal);

    /*
     * data
     */

    ThreeD::Vector _center;
    ThreeD::Vector _length2;
};
