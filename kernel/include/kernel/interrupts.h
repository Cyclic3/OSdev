#include <kernel/cpu.h>
typedef struct 
{
    char * function_name;
    unsigned int n_args;
    void ** args;;
}KernelCall;
void PushAll()
{
    asm("push %eax");
    asm("push %ecx");
    asm("push %edx");
    asm("push %ebx");
    //asm("push %esp");
    //    asm("push %ebp");
    //    asm("push %esi");
    //    asm("push %edi");
}
void PopAll()
{
    //    asm("pop %edi");
    //asm("pop %esi");
    //    asm("pop %ebp");
    //    asm("pop %esp");
    asm("pop %ebx");
    asm("pop %edx");
    asm("pop %ecx");
    asm("pop %eax");
}
void stackdump()
{
    //this function will screw everything up, but that won't matter
    int esp,ebp;
    asm("mov %%esp,%0;mov %%ebp,%1"::"r"(esp),"r"(ebp));
    dump(esp,ebp);
}
size_t get_eip();
asm(
    "get_eip:"
    "pop %eax;"
    "push %eax;"
    "push %eax;"
    "ret"
    );
void volatile CallKernel(KernelCall * v)
{
    asm(""::"a"(v));
    asm("int $0x80;");
    PopAll();
    return;
}
//void volatile KernelCallIRQ();
void volatile KernelCallIRQ();
asm(
  "KernelCallIRQ:;"
      "call KernelCallIRQ_2;"
    "ret;"
  ";"
    );
void LOL() {puts("LOL");}
void volatile KernelCallIRQ_2()
{
    
    size_t v1;
    asm("mov %%eax,%0":"=b"(v1));
    KernelCall * v = (KernelCall *) v1;
    unsigned char inline check(char * name,unsigned int n_args)
    {
        return (!strcmp(v->function_name,name))&&(v->n_args==n_args);
    }
    if (check("puts",1))
    {
        puts((v->args));
    }
    else{puts("Bad kernel call");}
    puts("ret");
    return;
}
