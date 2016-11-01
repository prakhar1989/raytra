#include "BVHTree.h"

/**
 * Constructs a new Bounding-Volume hierarchy tree from
 * a vector of bounding boxes.
 *
 * @param begin - starting iterator of the bounding boxes vector
 * @param end - ending iterator of the bounding boxes vector
 * @param axis - the axis on which the vector has to partitioned
 *
 * @return A pointer to the newly constructed BVH-Tree.
 */
BVHTree* BVHTree::make_bvhtree (
        std::vector<BoundingBox *>::iterator begin,
        std::vector<BoundingBox *>::iterator end,
        const Axis axis
)
{
    /*
     * testing the base case
     */
    if (std::distance(begin, end) == 0)
        return nullptr;

    /*
     * in the case of a single bounding box, just
     * create a new node and return it
     */
    if (std::distance(begin, end) == 1) {
        BVHTree* node = new BVHTree();
        node->bbox = *begin;
        return node;
    }

    /*
     * for more than one bounding box,
     * we need to merge all of them into one
     */
    BVHTree* node = new BVHTree();
    node->bbox = BoundingBox::combine(begin, end);

    /*
     * partition based on middle element, and the current axis
     * NOTE: this partitions the boxes array in-place
     */
    auto mid = begin + std::distance(begin, end)/2;
    std::nth_element(begin, mid, end,
         [&axis](const BoundingBox* a, const BoundingBox* b) -> bool {
             return BoundingBox::box_compare_along_dir(a, b, axis);
         }
    );

    /*
     * recursively construct the left and right subtrees
     * and pass the next axis as the sort direction
     */
    Axis nextDir = get_next_direction(axis);
    node->left = make_bvhtree(begin, mid, nextDir);
    node->right = make_bvhtree(mid, end, nextDir);
    return node;
}

/**
 * @return the depth of the tree
 */
int BVHTree::get_depth() const
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

/**
 * Computes intersections of a ray with the bounding boxes
 * inside the tree. To return the surfaces that the
 * ray intersects with, it mutates the passed in indices vector
 *
 * @param ray - The ray with the intersections have to be computed
 * @param indices - the vector which contains the indices on the
 * completion of this function.
 */
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

/**
 * prints the bounding boxes of the tree
 */
void BVHTree::print() const
{
    std::cout << "< ";
    if (left != nullptr)
        left->print();
    std::cout << bbox->id;
    if (right != nullptr)
        right->print();
    std::cout << " >";
}

/**
 * clean up the tree
 */
BVHTree::~BVHTree()
{
    delete left;
    delete right;
    delete bbox;
}