#define _POSIX_C_SOURCE 200112
#define uint8 uint8_t
#define uint32 uint32_t
/*2:*/
#line 45 "./critbit.w"

#include <stdint.h> 
#include <string.h> 
#include <stdlib.h> 

#include <sys/types.h> 
#include <errno.h> 

typedef struct{
void*child[2];
uint32 byte;
uint8 otherbits;
}critbit0_node;

typedef struct{
void*root;
}critbit0_tree;

/*:2*//*3:*/
#line 69 "./critbit.w"

int
critbit0_ncontains(critbit0_tree*t,const char*u,size_t ulen){
const uint8*ubytes= (void*)u;
uint8*p= t->root;

/*4:*/
#line 90 "./critbit.w"

if(!p)return 0;

/*:4*/
#line 75 "./critbit.w"

/*5:*/
#line 114 "./critbit.w"

while(1&(intptr_t)p){
critbit0_node*q= (void*)(p-1);
/*6:*/
#line 140 "./critbit.w"

uint8 c= 0;
if(q->byte<ulen)c= ubytes[q->byte];
const int direction= (1+(q->otherbits|c))>>8;

/*:6*/
#line 117 "./critbit.w"

p= q->child[direction];
}

/*:5*/
#line 76 "./critbit.w"

/*7:*/
#line 156 "./critbit.w"

return 0==strncmp(u,(const char*)p,ulen);

/*:7*/
#line 77 "./critbit.w"

}

int
critbit0_contains(critbit0_tree*t,const char*u){
return critbit0_ncontains(t,u,strlen(u)+1);
}

/*:3*//*8:*/
#line 171 "./critbit.w"

int critbit0_insert(critbit0_tree*t,const char*u)
{
const uint8*const ubytes= (void*)u;
const size_t ulen= strlen(u);
uint8*p= t->root;

/*9:*/
#line 195 "./critbit.w"

if(!p){
char*x;
int a= posix_memalign((void**)&x,sizeof(void*),ulen+1);
if(a)return 0;
memcpy(x,u,ulen+1);
t->root= x;
return 2;
}

/*:9*/
#line 178 "./critbit.w"

/*5:*/
#line 114 "./critbit.w"

while(1&(intptr_t)p){
critbit0_node*q= (void*)(p-1);
/*6:*/
#line 140 "./critbit.w"

uint8 c= 0;
if(q->byte<ulen)c= ubytes[q->byte];
const int direction= (1+(q->otherbits|c))>>8;

/*:6*/
#line 117 "./critbit.w"

p= q->child[direction];
}

/*:5*/
#line 179 "./critbit.w"

/*10:*/
#line 207 "./critbit.w"

/*11:*/
#line 222 "./critbit.w"

uint32 newbyte;
uint32 newotherbits;

for(newbyte= 0;newbyte<ulen;++newbyte){
if(p[newbyte]!=ubytes[newbyte]){
newotherbits= p[newbyte]^ubytes[newbyte];
goto different_byte_found;
}
}

if(p[newbyte]!=0){
newotherbits= p[newbyte];
goto different_byte_found;
}
return 1;

different_byte_found:

/*:11*/
#line 208 "./critbit.w"

/*12:*/
#line 271 "./critbit.w"

while(newotherbits&(newotherbits-1))newotherbits&= newotherbits-1;
newotherbits^= 255;
uint8 c= p[newbyte];
int newdirection= (1+(newotherbits|c))>>8;

/*:12*/
#line 209 "./critbit.w"


/*:10*/
#line 180 "./critbit.w"

/*13:*/
#line 279 "./critbit.w"

/*14:*/
#line 290 "./critbit.w"

critbit0_node*newnode;
if(posix_memalign((void**)&newnode,sizeof(void*),sizeof(critbit0_node)))return 0;

char*x;
if(posix_memalign((void**)&x,sizeof(void*),ulen+1)){
free(newnode);
return 0;
}
memcpy(x,ubytes,ulen+1);

newnode->byte= newbyte;
newnode->otherbits= newotherbits;
newnode->child[1-newdirection]= x;

/*:14*/
#line 280 "./critbit.w"

/*15:*/
#line 344 "./critbit.w"

void**wherep= &t->root;
for(;;){
uint8*p= *wherep;
if(!(1&(intptr_t)p))break;
critbit0_node*q= (void*)(p-1);
if(q->byte> newbyte)break;
if(q->byte==newbyte&&q->otherbits> newotherbits)break;
uint8 c= 0;
if(q->byte<ulen)c= ubytes[q->byte];
const int direction= (1+(q->otherbits|c))>>8;
wherep= q->child+direction;
}

newnode->child[newdirection]= *wherep;
*wherep= (void*)(1+(char*)newnode);

/*:15*/
#line 281 "./critbit.w"


/*:13*/
#line 181 "./critbit.w"


return 2;
}

/*:8*//*16:*/
#line 367 "./critbit.w"

int critbit0_delete(critbit0_tree*t,const char*u){
const uint8*ubytes= (void*)u;
const size_t ulen= strlen(u);
uint8*p= t->root;
void**wherep= &t->root;
void**whereq= 0;
critbit0_node*q= 0;
int direction= 0;

/*17:*/
#line 390 "./critbit.w"

if(!p)return 0;

/*:17*/
#line 377 "./critbit.w"

/*18:*/
#line 423 "./critbit.w"

while(1&(intptr_t)p){
whereq= wherep;
q= (void*)(p-1);
uint8 c= 0;
if(q->byte<ulen)c= ubytes[q->byte];
direction= (1+(q->otherbits|c))>>8;
wherep= q->child+direction;
p= *wherep;
}

/*:18*/
#line 378 "./critbit.w"

/*19:*/
#line 441 "./critbit.w"

if(0!=strcmp(u,(const char*)p))return 0;
free(p);

/*:19*/
#line 379 "./critbit.w"

/*20:*/
#line 455 "./critbit.w"

if(!whereq){
t->root= 0;
return 1;
}

*whereq= q->child[1-direction];
free(q);

/*:20*/
#line 380 "./critbit.w"


return 1;
}

/*:16*//*21:*/
#line 472 "./critbit.w"

static void
traverse(void*top){
/*22:*/
#line 490 "./critbit.w"

uint8*p= top;

if(1&(intptr_t)p){
critbit0_node*q= (void*)(p-1);
traverse(q->child[0]);
traverse(q->child[1]);
free(q);
}else{
free(p);
}

/*:22*/
#line 475 "./critbit.w"

}

void critbit0_clear(critbit0_tree*t)
{
if(t->root)traverse(t->root);
t->root= NULL;
}

/*:21*//*23:*/
#line 518 "./critbit.w"

static int
allprefixed_traverse(uint8*top,
int(*handle)(const char*,void*),void*arg){
/*26:*/
#line 578 "./critbit.w"

if(1&(intptr_t)top){
critbit0_node*q= (void*)(top-1);
for(int direction= 0;direction<2;++direction)
switch(allprefixed_traverse(q->child[direction],handle,arg)){
case 1:break;
case 0:return 0;
default:return-1;
}
return 1;
}

/*:26*/
#line 522 "./critbit.w"

/*27:*/
#line 595 "./critbit.w"

return handle((const char*)top,arg);/*:27*/
#line 523 "./critbit.w"

}

int
critbit0_allprefixed(critbit0_tree*t,const char*prefix,
int(*handle)(const char*,void*),void*arg){
const uint8*ubytes= (void*)prefix;
const size_t ulen= strlen(prefix);
uint8*p= t->root;
uint8*top= p;

if(!p)return 1;
/*24:*/
#line 549 "./critbit.w"

while(1&(intptr_t)p){
critbit0_node*q= (void*)(p-1);
uint8 c= 0;
if(q->byte<ulen)c= ubytes[q->byte];
const int direction= (1+(q->otherbits|c))>>8;
p= q->child[direction];
if(q->byte<ulen)top= p;
}

/*:24*/
#line 535 "./critbit.w"

/*25:*/
#line 565 "./critbit.w"

for(size_t i= 0;i<ulen;++i){
if(p[i]!=ubytes[i])return 1;
}

/*:25*/
#line 536 "./critbit.w"


return allprefixed_traverse(top,handle,arg);
}

/*:23*/
