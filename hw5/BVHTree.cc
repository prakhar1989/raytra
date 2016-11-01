#include "BVHTree.h"

BVHTree* BVHTree::make_bvhtree (
        const std::vector<BoundingBox *>::iterator begin,
        const std::vector<BoundingBox *>::iterator end,
        Axis axis
)
{
    if (std::distance(begin, end) == 0)
        return nullptr;

    if (std::distance(begin, end) == 1) {
        BVHTree* node = new BVHTree();
        node->bbox = *begin;
        return node;
    }

    auto mid = begin + std::distance(begin, end)/2;
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
    node->right = make_bvhtree(mid, end, nextDir);
    return node;
}

int BVHTree::get_depth()
{
    if (bbox == nullptr)
        return 0;

    std::cout << "id: " << bbox->id << ", " << bbox->center << std::endl;
    if (left == nullptr && right == nullptr)
        return 1;
    int ld = left == nullptr ? 0 : left->get_depth();
    int rd = right == nullptr ? 0 : right->get_depth();
    return 1 + std::max(ld, rd);
}

void BVHTree::compute_intersections (
        const Ray &ray,
        std::vector<int>& indices
) const
{
    if (bbox == nullptr || !bbox->does_intersect(ray))
        return;

    // on the leaf node
    if (bbox->get_surface_index() != -1)
       indices.push_back(bbox->get_surface_index());

    // recurse on left and right subtrees
    // to find more intersections
    if (left != nullptr)
        left->compute_intersections(ray, indices);

    if (right != nullptr)
        right->compute_intersections(ray, indices);
}

void BVHTree::print()
{
    std::cout << "( ";
    if (left != nullptr)
        left->print();
    std::cout << bbox->id;
    if (right != nullptr)
        right->print();
    std::cout << " )";
}

BVHTree::~BVHTree()
{
    delete left;
    delete right;
    delete bbox;
}