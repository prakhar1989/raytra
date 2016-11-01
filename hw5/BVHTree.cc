#include "BVHTree.h"

BVHTree* BVHTree::make_bvhtree (
        std::vector<BoundingBox *>::iterator begin,
        std::vector<BoundingBox *>::iterator end,
        Axis axis
)
{
    if (end == begin) {
        BVHTree* node = new BVHTree();
        node->bbox = *begin;
        return node;
    }

    auto mid = begin + (end-begin)/2;
    BVHTree* node = new BVHTree();
    node->bbox = BoundingBox::combine(begin, end);

    // partition
    std::nth_element(begin, mid, end,
         [&axis](const BoundingBox* a, const BoundingBox* b) -> bool {
             return BoundingBox::box_compare_along_dir(a, b, axis);
         }
    );

    // recursively call on the other two halves
    Axis nextDir = get_next_direction(axis);
    node->left = make_bvhtree(begin, mid, nextDir);
    node->right = make_bvhtree(mid+1, end, nextDir);
    return node;
}

int BVHTree::get_depth()
{
    if (left == nullptr && right == nullptr)
        return 1;
    int ld = left == nullptr ? 0 : left->get_depth();
    int rd = right == nullptr ? 0 : right->get_depth();
    return 1 + std::max(ld, rd);
}
