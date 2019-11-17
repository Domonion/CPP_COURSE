//
// Created by domonion on 25.02.19.
//

#ifndef LINKED_PTR_HPP
#define LINKED_PTR_HPP

#include <algorithm>

namespace smart_ptr {

    namespace details {

        class list_node {
        public:
            list_node *l, *r;

            list_node() : l(nullptr), r(nullptr) {}

        public:
            void add(list_node *node) {
                node->l = this;
                node->r = r;
                if (r) r->l = node;
                r = node;
            }

            void remove() {
                if (l) l->r = r;
                if (r) r->l = l;
                l = r = nullptr;
            }

            void swap(list_node &node) {
                list_node *a = (l ? l : r);
                list_node *b = (node.l ? node.l : node.r);
                remove();
                node.remove();
                if (a) a->add(&node);
                if (b) b->add(this);
            }
        };
    }

    using namespace details;

    template<typename T>
    struct linked_ptr {
        template<typename U>
        friend
        struct linked_ptr;

    public:

        linked_ptr() : node(), ptr(nullptr) {}

        explicit linked_ptr(T *_ptr) : node(), ptr(_ptr) {}

        linked_ptr(linked_ptr<T> const &lp) : node(), ptr(lp.get()) {
            lp.node.add(&this->node);
        }

        template<typename U>
        explicit linked_ptr(U *_ptr) : node(), ptr(_ptr) {}

        template<typename U>
        linked_ptr(linked_ptr<U> const &lp) : node(), ptr(lp.get()) {
            lp.node.add(&this->node);
        }

        ~linked_ptr() {
            clear();
        }

        linked_ptr<T> &operator=(linked_ptr<T> const &lp) {
            linked_ptr<T> tmp(lp);
            swap(tmp);
            return *this;
        }

        template<typename U>
        linked_ptr &operator=(linked_ptr<U> const &lp) {
            linked_ptr tmp(lp);
            swap(tmp);
            return *this;
        }

        void reset(T *_ptr = nullptr) {
            clear();
            ptr = _ptr;
        }

        template<typename U>
        void reset(U *_ptr = nullptr) {
            clear();
            ptr = _ptr;
        }

        void swap(linked_ptr &lp) {
            node.swap(lp.node);
            std::swap(ptr, lp.ptr);
        }

        T *get() const {
            return ptr;
        }

        bool unique() const {
            return !node.l && !node.r && ptr;
        }

        T &operator*() const {
            return *get();
        }

        T *operator->() const {
            return get();
        }

        operator bool() const {
            return get();
        }

    private:

        void clear() {
            if (unique() && ptr) {
                delete ptr;
            }
            ptr = nullptr;
            node.remove();
        }

        mutable list_node node;
        T *ptr;

    };


    template<typename T, typename U>
    bool operator<(linked_ptr<T> const &a, linked_ptr<U> const &b) {
        return a.get() < b.get();
    }

    template<typename T, typename U>
    bool operator>(linked_ptr<T> const &a, linked_ptr<U> const &b) {
        return b < a;
    }

    template<typename T, typename U>
    bool operator==(linked_ptr<T> const &a, linked_ptr<U> const &b) {
        return a.get() == b.get();
    }

    template<typename T, typename U>
    bool operator!=(linked_ptr<T> const &a, linked_ptr<U> const &b) {
        return !(a == b);
    }

    template<typename T, typename U>
    bool operator<=(linked_ptr<T> const &a, linked_ptr<U> const &b) {
        return !(a > b);
    }

    template<typename T, typename U>
    bool operator>=(linked_ptr<T> const &a, linked_ptr<U> const &b) {
        return !(a < b);
    }

    template<typename T>
    void swap(linked_ptr<T> &a, linked_ptr<T> &b) {
        a.swap(b);
    }
}
#endif //LINKED_PTR_HPP
