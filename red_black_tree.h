#ifndef TREEX_RED_BLACK_TREE_H
#define TREEX_RED_BLACK_TREE_H

#include "binary_tree.h"

namespace treex {

    template <typename T>
    class RedBlackTree;

    template <typename T>
    class RedBlackNode: public BaseNode<T, RedBlackNode> {
    public:
        typedef BaseNode<T, RedBlackNode> base_type;
        friend class RedBlackTree<T>;
        using base_type::base_type;
        bool isBlack() const;
    protected:
        bool isBlack_ = true;
    };

    template <typename T>
    class RedBlackTree: public BaseTree<T, RedBlackNode> {
    public:
        typedef RedBlackNode<T> node_type;
        typedef BaseTree<T, RedBlackNode> base_type;

        friend class RedBlackNode<T>;
        using base_type::base_type;

        void insert(node_type* node) override;
        void remove(node_type* node) override;
    protected:
        void insertBalance(node_type* node);
        void removeBalance(node_type* node);
    };
}

#include "details/red_black_tree_details.h"

#endif //TREEX_RED_BLACK_TREE_H
