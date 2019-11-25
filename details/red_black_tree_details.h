#ifndef TREEX_RED_BLACK_TREE_DETAILS_H
#define TREEX_RED_BLACK_TREE_DETAILS_H

namespace treex {

    template<typename T>
    bool RedBlackNode<T>::isBlack() const {
        return isBlack_;
    }

    template<typename T>
    void RedBlackTree<T>::insert(typename RedBlackTree<T>::node_type* node) {
        base_type::insert(node);
        insertBalance(node);
    }

    template<typename T>
    void RedBlackTree<T>::remove(typename RedBlackTree<T>::node_type* node) {
        node_type* fixNode = RedBlackTree<T>::nil;
        node_type* next = node;
        bool isBlack = next->isBlack_;

        if(!validNode(node->left_)){
            fixNode = node->right_;
            this->transplant(node->right_, node);
        } else if(!validNode(node->right_)){
            fixNode = node->left_;
            this->transplant(node->left_, node);
        } else {
            next = node->successor();
            fixNode = next->right_;
            isBlack = next->isBlack_;

            if(next->parent_ == node){
                fixNode->parent_ = next;
            } else {
                this->transplant(next->right_, next);
                next->right_ = node->right_;
                next->right_->parent_ = next;
            }

            next->left_ = node->left_;
            next->left_->parent_ = next;
            next->isBlack_ = node->isBlack_;
        }

        if(isBlack){
            removeBalance(fixNode);
        }

        node->left_ = node->right_ = nullptr;
        delete node;
    }

    template<typename T>
    void RedBlackTree<T>::insertBalance(typename RedBlackTree<T>::node_type* node){
        node->isBlack_ = false;

        while(!node->parent_->isBlack_){
            if(node->parent_->isLeftChild()){
                if(!node->parent_->parent_->right_->isBlack_){
                    node->parent_->parent_->isBlack_ = false;
                    node->parent_->isBlack_ = true;
                    node->parent_->parent_->right_->isBlack_ = true;
                    node = node->parent_->parent_;
                } else if (node->isRightChild()){
                    node = node->parent_;
                    this->leftRotate(node);
                } else {
                    node->parent_->parent_->isBlack_ = false;
                    node->parent_->isBlack_ = true;
                    this->rightRotate(node->parent_->parent_);
                }
            } else {
                if(!node->parent_->parent_->left_->isBlack_){
                    node->parent_->parent_->isBlack_ = false;
                    node->parent_->isBlack_ = true;
                    node->parent_->parent_->left_->isBlack_ = true;
                    node = node->parent_->parent_;
                } else if (node->isLeftChild()){
                    node = node->parent_;
                    this->rightRotate(node);
                } else {
                    node->parent_->parent_->isBlack_ = false;
                    node->parent_->isBlack_ = true;
                    this->leftRotate(node->parent_->parent_);
                }
            }
        }

        this->root_->isBlack_ = true;
    }

    template<typename T>
    void RedBlackTree<T>::removeBalance(typename RedBlackTree<T>::node_type* node) {
        //TODO
    }
}

#endif //TREEX_RED_BLACK_TREE_DETAILS_H
