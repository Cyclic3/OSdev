/*#ifndef KERNEL_CONVERSION_H
#define KERNEL_CONVERSION_H
char* hexdump(unsigned int v, char vvv[9]) 
{
	const char vv[] = "0123456789abcdef";
	vvv[0] = vv[(v/268435456)];
	v=(v%268435456);
	vvv[1] = vv[(v/16777216)];
	v=(v%16777216);
	vvv[2] = vv[(v/1048576)];
	v=(v%1048576);
	vvv[3] = vv[(v/65536)];
	v=(v%65536);
	vvv[4] = vv[(v/4096)];
	v=(v%4096);
	vvv[5] = vv[(v/256)];
	v=(v%256);
	vvv[6] = vv[v/16];
	v=(v%16);
	vvv[7] = vv[v];
	vvv[8] = '\0';return vvv;
}
#endif*/