#pragma once

#include <crtdefs.h>

namespace engiX
{
    class Object
    {
    public:
        Object();
        ~Object();

        virtual const wchar_t* Typename() const = 0;

        static void* operator new(size_t sz) { return Alloc(sz); }
        static void* operator new[](size_t sz) { return Alloc(sz); }
        static void operator delete(void* pObj) { Free(pObj); }
        static void operator delete[](void* pObj, size_t sz) { Free(pObj); }
        static void FreeMemoryPool();
        static void DumpAliveObjects();
        static size_t AliveObjectsCount();
        static size_t AliveObjectsMemoryUsage();

    private:
        static void* Alloc(size_t sz);
        static void Free(void* pObj);
    };
}