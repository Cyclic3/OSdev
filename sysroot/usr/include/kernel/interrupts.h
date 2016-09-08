#include <kernel/cpu.h>
void CallKernel(char* function_name,unsigned int n_args,void** args)
{
    //TODO: address translation
    //asm("push %0"::"r"((size_t)function_name));
    asm("push %0;int $0x80"::"r"(n_args));
    //asm("push %0"::"r"(args));
    asm("int $0x80");
}
void KernelCallIRQ()
{
    char* function_name;
    unsigned int n_args;
    size_t args;
    //asm volatile ("pop %0":"=r"(function_name));
    asm volatile ("pop %0":"=r"(n_args));
    //;pop %1;pop %0":"=r"(function_name),"=r"(n_args),"=r"(args));
    printf("%x",n_args);
    //;pop %1;pop %2","=X"(n_args),"=X"(args));
    //I know the code above is evil, but it is the best that can be done.
}
