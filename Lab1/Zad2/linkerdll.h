#ifndef LINKERDLL_H_INCLUDED
#define LINKERDLL_H_INCLUDED

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef DLL_FLAG     // DLL_FLAG is a flag given in Makefile only during the compilation of dll version on main
int load_dll() {
    void* handle = dlopen("libblockutil.so", RTLD_LAZY);
    if (!handle) {
        printf("Error: dlopen procedure failed\n");
        return 0;
    }
    *(void**) (&create_table) = dlsym(handle, "create_table");
    *(void**) (&read_block) = dlsym(handle, "read_block");
    *(void**) (&get_bdata) = dlsym(handle, "get_bdata");
    *(void**) (&remove_block) = dlsym(handle, "remove_block");
    *(void**) (&free_btable) = dlsym(handle, "free_btable");
    if (dlerror() != NULL) {
        printf("UError during dll linking: %s \n", dlerror());
        return 0;
    }
    return 1;
}
#else   // "Empty" version of load_dll, when no dll is used in compilation
int load_dll() {
    return 1;
}
#endif // DLL_FLAG

#endif // LINKERDLL_H_INCLUDED
