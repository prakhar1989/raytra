#ifndef HW_BVHTREE_H
#define HW_BVHTREE_H

#include <vector>
#include "bounding_box.h"
#include <algorithm>

class BVHTree {
public:
    static BVHTree* make_bvhtree (
        std::vector<BoundingBox*>::iterator begin,
        std::vector<BoundingBox*>::iterator end,
        Axis axis
    );

    virtual ~BVHTree();
    int get_depth();
    void compute_intersections (
        const Ray& ray,
        std::vector<int>& indices
    ) const;
    void print();
private:
    BoundingBox *bbox = nullptr;
    BVHTree *left = nullptr;
    BVHTree *right = nullptr;
};

#endif //HW_BVHTREE_H
