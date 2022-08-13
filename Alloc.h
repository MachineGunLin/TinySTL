#ifndef _ALLOC_H
#define _ALLOC_H

#include <cstdlib>

namespace TinySTL {
    /*
        空间配置器，以字节数为单位分配
        内部使用
    */
    class alloc {
    private:
        enum EAlign {
            ALIGN = 8  // 小型区块的上调边界
        };
        enum EMaxBytes {
            MAXBYTES = 128 // 小型区块的上限，超过的区块由 malloc 分配
        };
        enum ENFreeLists {
            NFREELISTS = (EMaxBytes::MaxBytes / EAlign::ALIGN)  // free-lists 的个数
        };
        enum ENObjs {
            NOBJS = 20      // 每次增加的节点数
        };          
    
    private:
        // free-lists 的节点构造
        union obj {
            union obj *next;
            char client[1];
        };

        static obj *free_list[ENFreeLists::NFREELISTS];
    
    private:
        static char *start_free;        // 内存池起始位置
        static char *end_free;          // 内存池结束位置
        static size_t heap_size;

        // 将 bytes 上调至 8 的倍数
        static size_t ROUND_UP(size_t bytes) {
            return ((bytes + EAlign::ALIGN - 1) & ~(EAlign::ALIGN - 1));
        }
        // 根据区块大小，决定使用第 n 号 free-list, n 从 0 开始计算
        static size_t FREELIST_INDEX(size_t bytes) {
            return (((bytes) + EAlign::ALIGN - 1) / EAlign::ALIGN - 1);
        }
        // 返回一个大小为 n 的对象，并可能加入大小为 n 的其他区块到 free-list
        static void *refill(size_t n);
        // 配置一大块空间，可容纳 nobjs 个大小为 size 的区块
        // 如果配置 nobjs 个区块有所不便，nobjs 可能会降低
        static char *chunk_alloc(size_t size, size_t& nobjs);
    
    public:
        static void *allocate(size_t bytes);
        static void deallocate(void *ptr, size_t bytes);
        static void *reallocate(void *ptr, size_t old_sz, size_t new_sz);

        char *alloc::start_free = 0;
        char *alloc::end_free = 0;
        size_t alloc::heap_size = 0;
        alloc::obj *alloc::free_list[alloc::ENFreeLists::NFREELISTS] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        }
    };
}

#endif