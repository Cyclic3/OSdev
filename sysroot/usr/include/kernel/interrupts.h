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
    puts("CALL");
    //printf("%x\n",v);
    //TODO: address translation
    //asm("push %0"::"r"((size_t)function_name));
    PushAll();
    asm(""::"d"(v));
    //

    //asm("push %0"::"r"(args))
    //asm("push 0x69686968");
    //stackdump();
    //size_t eip = get_eip();
    //printf("faulty instruction @ %x\n",eip);
    //asm("push %0"::"r"(eip));
    asm("int $0x80;");//pop %ax");
    //eip = get_eip();
    //printf("faulty instruction @ %x\n",eip);
    //dump(eip,eip+0x40);
    //stackdump();
    
    PopAll();
    //asm("pop %eax");
    return;
    //It is crashing between the return and the next instruction. This is probably due to a cirrupted stack pointer, and it failing top pop off the pointer
    //asm("ljmp kernel_ret");
    //PopAll();
}
//void volatile KernelCallIRQ();
void volatile KernelCallIRQ();
asm(
  "KernelCallIRQ:;"
    //    "pushal;"
    //  "cld;"
      "call KernelCallIRQ_2;"
    //  "call LOL;"
    //    "popal;"
    "ret;"
  ";"
    );
void LOL() {puts("LOL");}
void volatile KernelCallIRQ_2()
{
    
    size_t v1;
    asm("mov %%edx,%0":"=a"(v1));
    KernelCall * v = (KernelCall *) v1;
    unsigned char inline check(char * name,unsigned int n_args)
    {
        return (!strcmp(v->function_name,name))&&(v->n_args==n_args);
    }
    //puts(v->function_name);
    printf("%s, %i\n",v->function_name,v->n_args);
    if(check("puts",1))
    {
        puts((v->args));
    }
    else{puts("Bad kernel call");}
    puts("ret");
    return;
}
