#include "BVHTree.h"


BVHTree* BVHTree::make_bvhtree (
        std::vector<BoundingBox *>::iterator begin,
        std::vector<BoundingBox *>::iterator end,
        SortDirection dir
)
{
    if (end - begin <= 0)
        return nullptr;

    if (end - begin == 1) {
        BVHTree* node = new BVHTree();
        node->bbox = *begin;
        return node;
    }

    auto mid = begin + (end-begin)/2;
    BVHTree* node = new BVHTree();

    std::nth_element(begin, mid, end,
         [&dir](const BoundingBox* a, const BoundingBox* b) -> bool {
             return BoundingBox::box_compare_along_dir(a, b, dir);
         }
    );

    // recursively call on the other two halves
    SortDirection nextDir = get_next_direction(dir);
    node->left = make_bvhtree(begin, mid, nextDir);
    node->right = make_bvhtree(mid+1, end, nextDir);
    return node;
}
