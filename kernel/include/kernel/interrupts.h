#include <kernel/cpu.h>
typedef struct 
{
    char * function_name;
    unsigned int n_args;
    void ** args;;
}KernelCall;
void stackdump()
{
    //this function will screw everything up, but that won't matter
    size_t rsp,rbp = 0;
    asm("movq %%rsp,%0;movq %%rbp,%1"::"r"(rsp),"r"(rbp));
    dump(rsp,rbp);
}
void volatile CallKernel(KernelCall * v)
{
    asm(""::"a"(v));
    asm("int $0x80;");
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
    asm("mov %%rax,%0":"=b"(v1));
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
