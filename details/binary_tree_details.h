#ifndef TREEX_BINARY_TREE_DETAILS_H
#define TREEX_BINARY_TREE_DETAILS_H

#include <queue>
#include <iomanip>

namespace treex {

    template <typename T, template <typename> class ConcreteNode>
    BaseNode<T, ConcreteNode>::BaseNode(): BaseNode(T()){}

    template <typename T, template <typename> class ConcreteNode>
    BaseNode<T, ConcreteNode>::BaseNode(const T& key):
        left_(BaseTree<T, ConcreteNode>::nil),
        right_(BaseTree<T, ConcreteNode>::nil),
        parent_(BaseTree<T, ConcreteNode>::nil),
        key_(key){}

    template <typename T, template <typename> class ConcreteNode>
    BaseNode<T, ConcreteNode>::~BaseNode(){
        if(validNode(left_))
            delete left_;
        if(validNode(right_))
            delete right_;
    }

    template <typename T, template <typename> class ConcreteNode>
    T& BaseNode<T, ConcreteNode>::key(){
        return key_;
    }

    template <typename T, template <typename> class ConcreteNode>
    const T& BaseNode<T, ConcreteNode>::key() const {
        return key_;
    }

    template <typename T, template <typename> class ConcreteNode>
    const ConcreteNode<T>* BaseNode<T, ConcreteNode>::parent() const {
        return parent_;
    }

    template <typename T, template <typename> class ConcreteNode>
    const ConcreteNode<T>* BaseNode<T, ConcreteNode>::left() const {
        return left_;
    }

    template <typename T, template <typename> class ConcreteNode>
    const ConcreteNode<T>* BaseNode<T, ConcreteNode>::right() const {
        return right_;
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseNode<T, ConcreteNode>::min(){
        auto node = this;
        while(validNode(node->left_))
            node = node->left_;
        return node->cast();
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseNode<T, ConcreteNode>::max(){
        auto node = this;
        while(validNode(node->right_))
            node = node->right_;
        return node->cast();
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseNode<T, ConcreteNode>::predecessor(){
        auto node = this;
        if(validNode(node->left_))
            return node->left_->max();
        while(node->isLeftChild())
            node = node->parent_;
        return node->parent_->cast();
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseNode<T, ConcreteNode>::successor(){
        auto node = this;
        if(validNode(node->right_))
            return node->right_->min();
        while(node->isRightChild())
            node = node->parent_;
        return node->parent_->cast();
    }

    template <typename T, template <typename> class ConcreteNode>
    int BaseNode<T, ConcreteNode>::height() const {
        if(!validNode(this))
            return 0;
        return std::max(left_->height(), right_->height()) + 1;
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseNode<T, ConcreteNode>::find(const T& key){
        auto node = this;
        while(validNode(node)){
            if(key == node->key_){
                break;
            } else if(key < node->key_){
                node = node->left_;
            } else {
                node = node->right_;
            }
        }
        return node->cast();
    }

    template <typename T, template <typename> class ConcreteNode>
    void BaseNode<T, ConcreteNode>::transplant(BaseTree<T, ConcreteNode>* tree, ConcreteNode<T>* node){

        if(!validNode(parent_))
            tree->root_ = node;
        else if(isLeftChild())
            parent_->left_ = node;
        else
            parent_->right_ = node;

        if(validNode(node))
            node->parent_ = parent_;
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseNode<T, ConcreteNode>::cast(){
        return static_cast<ConcreteNode<T>*>(this);
    }

    template <typename T, template <typename> class ConcreteNode>
    bool BaseNode<T, ConcreteNode>::isLeftChild() const {
        return parent_->left_ == this;
    }

    template <typename T, template <typename> class ConcreteNode>
    bool BaseNode<T, ConcreteNode>::isRightChild() const {
        return parent_->right_ == this;
    }

    template <typename T, template <typename> class ConcreteNode>
    void BaseNode<T, ConcreteNode>::print(std::ostream& os, const std::string& prefix, bool isLeft){
        os << prefix << (isLeft ? "├──" : "└──" ) << key_ << "\n";
        const auto childPrefix = prefix + (isLeft ? "│   " : "    ");

        if(validNode(left_))
            left_->print(os, childPrefix, true);

        if(validNode(right_))
            right_->print(os, childPrefix, false);
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* const BaseTree<T, ConcreteNode>::nil = new ConcreteNode<T>();

    template <typename T, template <typename> class ConcreteNode>
    BaseTree<T, ConcreteNode>::BaseTree(): root_(BaseTree<T, ConcreteNode>::nil){}

    template <typename T, template <typename> class ConcreteNode>
    BaseTree<T, ConcreteNode>::~BaseTree(){
        delete root_;
    }

    template <typename T, template <typename> class ConcreteNode>
    void BaseTree<T, ConcreteNode>::insert(ConcreteNode<T>* node){
        auto current = root_;
        auto parent = BaseTree<T, ConcreteNode>::nil;

        while(validNode(current)){
            parent = current;

            if(node->key_ < current->key_)
                current = current->left_;
            else
                current = current->right_;
        }

        node->parent_ = parent;

        if(!validNode(parent))
            root_ = node;
        else if(node->key_ < parent->key_)
            parent->left_ = node;
        else
            parent->right_ = node;

    }

    template <typename T, template <typename> class ConcreteNode>
    void BaseTree<T, ConcreteNode>::remove(ConcreteNode<T>* node){

        if(!validNode(node->left_))
            transplant(node->right_, node);
        else if(!validNode(node->right_))
            transplant(node->left_, node);
        else {
            auto next = node->successor();

            if(next->parent_ != node){
                transplant(next->right_, next);
                next->right_ = node->right_;
                next->right_->parent_ = next;
            }

            transplant(next, node);
            next->left_ = node->left_;
            next->left_->parent_ = next;
        }

        node->left_ = node->right_ = nullptr;
        delete node;
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseTree<T, ConcreteNode>::find(const T& key){
        return root_->find(key);
    }

    template <typename T, template <typename> class ConcreteNode>
    void BaseTree<T, ConcreteNode>::transplant(ConcreteNode<T>* dst, ConcreteNode<T>* src){
        src->transplant(this, dst);
    }

    template <typename T, template <typename> class ConcreteNode>
    ConcreteNode<T>* BaseTree<T, ConcreteNode>::root(){
        return root_;
    }

    template <typename T, template <typename> class ConcreteNode>
    void BaseTree<T, ConcreteNode>::leftRotate(ConcreteNode<T>* node){
        auto right = node->right_;
        node->right_ = right->left_;

        if(validNode(right->left_))
            right->left_->parent_ = node->right_;

        right->parent_ = node->parent_;

        if(!validNode(right->parent_))
            root_ = right;
        else if(right->isLeftChild())
            right->parent_->left_ = right;
        else
            right->parent_->right_ = right;

        right->left_ = node;
        node->parent_ = right;
    }

    template <typename T, template <typename> class ConcreteNode>
    void BaseTree<T, ConcreteNode>::rightRotate(ConcreteNode<T>* node){
        auto left = node->left_;
        node->left_ = left->right_;

        if(validNode(left->right_))
            left->right_->parent_ = node->left_;

        left->parent_ = node->parent_;

        if(!validNode(left->parent_))
            root_ = left;
        else if(left->isLeftChild())
            left->parent_->left_ = left;
        else
            left->parent_->right_ = left;

        left->right_ = node;
        node->parent_ = left;
    }

    template <typename T, template <typename> class ConcreteNode>
    std::ostream& operator<<(std::ostream& os, const BaseTree<T, ConcreteNode>& tree){
        if(validNode(tree.root_))
            tree.root_->print(os, "", false);
        return os;
    }

}

#endif //TREEX_BINARY_TREE_DETAILS_H
