#ifndef _CONSTRUCT_H
#define _CONSTRUCT_H

#include <new>

#include "TypeTraits.h"

namespace TinySTL {

    template<class T1, class T2>
    inline void Construct(T1 *ptr1, const T2& value) {
        new(ptr1) T1(value);
    }

    template<class T>
    inline void destroy(T *ptr) {
        ptr->~T();
    }

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIteraotr last, _true_type) {}

    template<class ForwardIteraotr>
    inline void _destroy(ForwardIteraotr first, ForwardIteraotr last, _false_type) {
        for (; first != last; ++first) {
            destroy(&*first);
        }
    }

    template<class ForwardIteraotr>
    inline void destroy(ForwardIteraotr first, ForwardIteraotr last) {
        typedef typename _type_traits<ForwardIteraotr>::is_POD_type is_POD_type;
        _destroy(first, last, is_POD_type);
    }
}

# endif
