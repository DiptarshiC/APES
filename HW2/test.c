#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
#include "double_linked_list.h"


	typedef enum{
	success,
	fail
	}result;


static int count[24];

void test_destroy()
{

struct info dipto;
struct info *dip;
dip=&dipto;
dipto.data=32;
dipto.n1=NULL;

if(destroy(&dipto)==&(dipto))
	{

		count[0]=0;
	}
else
	{

		count[0]=1;
	}

if(dip->data==32)
	{

		count[1]=0;
	}
else
	{
		count[1]=1;
	}
if(dip->n1==NULL)
	{

		count[2]=0;
	}
else
	{
        
		count[2]=1;
	}



}

void test_insert_at_end()
{

struct info dipto;
struct info *dip;
dip=&dipto;
dipto.data=32;
dipto.n1= (struct node*) malloc(sizeof(struct node*));
dip->n1->prev=NULL;

struct info josh;
struct info *j;
j=&josh;
j->data=400;
josh.n1= (struct node*) malloc(sizeof(struct node*));
dip->n1->next=j->n1;
j->n1->prev=dip->n1;
struct info *store_result=(struct info*) malloc(sizeof(struct info*));
struct info *s_r;
s_r=store_result;
s_r->n1=(struct node*) malloc(sizeof(struct node*));

//s_r=insert_at_end(&josh,320);
if(josh.data==400)
	{

		count[3]=0;
	}
else
	{

		count[3]=1;
	}

if(s_r->data==400)
	{

		count[4]=0;
	}
else
	{
		count[4]=1;
	}
/*
if(dip->n1==NULL)
	{

		count[2]=0;
	}
else
	{
        
		count[2]=1;
	}


*/
}
void test_count()
{

	struct info *A=(struct info*) malloc(sizeof(struct info*));
	A->n1=(struct node*) malloc(sizeof(struct node*));
	struct info *B=(struct info*) malloc(sizeof(struct info*));
	B->n1=(struct node*) malloc(sizeof(struct node*));
	struct info *C=(struct info*) malloc(sizeof(struct info*));
	C->n1=(struct node*) malloc(sizeof(struct node*));
	A->n1->next=NULL;
	A->n1->prev=B->n1;
	B->n1->prev=C->n1;
	B->n1->next=A->n1;
	C->n1->next=B->n1;
	C->n1->prev=NULL;

	uint32_t count1=size(A);

	if(count1==3)
	{
		count[5]=0;
	}
	else
	{	count[5]=1; }
	



}

void main()

{
	/*
	struct info dipto;
	dipto.data=32;*/
	test_destroy();
	test_insert_at_end();
	test_count();
	for(int i=0;i<6;i++)
	{
		if(count[i]==0)
		{

			printf("SUCCESS\n");
		}

		else
		{
			printf("FAILURE\n");
		}

	}


}
