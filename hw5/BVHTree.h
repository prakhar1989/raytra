#ifndef HW_BVHTREE_H
#define HW_BVHTREE_H

#include <vector>
#include <algorithm>
#include "bounding_box.h"

class BVHTree {
public:
    static BVHTree* make_bvhtree (
        std::vector<BoundingBox*>::iterator begin,
        std::vector<BoundingBox*>::iterator end,
        const Axis axis
    );

    virtual ~BVHTree();
    void compute_intersections (
        const Ray& ray,
        std::vector<int>& indices
    ) const;

    /* useful for debugging */
    void print() const;
    int get_depth() const;

private:
    BoundingBox *bbox = nullptr;
    BVHTree *left = nullptr;
    BVHTree *right = nullptr;
};

#endif //HW_BVHTREE_H
