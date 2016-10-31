#ifndef HW5_BOUNDING_BOX_H
#define HW5_BOUNDING_BOX_H


class BoundingBox {
public:
    BoundingBox(float x_min, float x_max,
                float y_min, float y_max,
                float z_min, float z_max);
    float xmin, xmax, ymin, ymax, zmax, zmin;
    point center;
    int surface_index;
};


#endif //HW5_BOUNDING_BOX_H
