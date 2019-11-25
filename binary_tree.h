#ifndef TREEX_BINARY_TREE_H
#define TREEX_BINARY_TREE_H

#include <iostream>

namespace treex {

    template <typename T, template <typename> class ConcreteNode>
    class BaseTree;

    template <typename T, template <typename> class ConcreteNode>
    class BaseNode;

    template <typename T, template <typename> class ConcreteNode>
    std::ostream& operator<<(std::ostream& os, const BaseTree<T, ConcreteNode>& tree);

    template <typename T, template <typename> class ConcreteNode>
    inline bool validNode(BaseNode<T, ConcreteNode>* node){
        return node != BaseTree<T, ConcreteNode>::nil;
    }

    template <typename T, template <typename> class ConcreteNode>
    class BaseNode {
    public:
        friend class BaseTree<T, ConcreteNode>;

        BaseNode();
        explicit BaseNode(const T& key);
        virtual ~BaseNode();

        T& key();
        const T& key() const;
        const ConcreteNode<T>* parent() const;
        const ConcreteNode<T>* left() const;
        const ConcreteNode<T>* right() const;
        ConcreteNode<T>* min();
        ConcreteNode<T>* max();
        ConcreteNode<T>* predecessor();
        ConcreteNode<T>* successor();
        int height() const;
        ConcreteNode<T>* find(const T& key);
        void transplant(BaseTree<T, ConcreteNode>* tree, ConcreteNode<T>* node);
        ConcreteNode<T>* cast();
        bool isLeftChild() const;
        bool isRightChild() const;
    protected:
        void print(std::ostream& os, const std::string& prefix, bool isLeft);

        T key_;
        ConcreteNode<T>* left_;
        ConcreteNode<T>* right_;
        ConcreteNode<T>* parent_;
    };

    template <typename T, template <typename> class ConcreteNode>
    class BaseTree {
    public:
        friend class BaseNode<T, ConcreteNode>;

        BaseTree();
        virtual ~BaseTree();

        virtual void insert(ConcreteNode<T>* node);
        virtual void remove(ConcreteNode<T>* node);
        ConcreteNode<T>* find(const T& key);
        ConcreteNode<T>* root();

        friend std::ostream& operator<<<T, ConcreteNode>(std::ostream& os, const BaseTree<T, ConcreteNode>& tree);

        static ConcreteNode<T>* const nil;
    protected:
        void transplant(ConcreteNode<T>* dst, ConcreteNode<T>* src);
        void leftRotate(ConcreteNode<T>* node);
        void rightRotate(ConcreteNode<T>* node);

        ConcreteNode<T>* root_;
    };

}

#include "details/binary_tree_details.h"

#endif //TREEX_BINARY_TREE_H
