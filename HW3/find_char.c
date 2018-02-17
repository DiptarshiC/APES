#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>



        struct node{

        char car;
        uint32_t count;
        struct node *next;
        };


void main()

{


	char c;
	char newchar;
	char flag=0;

	FILE *ptr=fopen("textcopy.txt","a");
	FILE *fptr=fopen("text.txt","r");

	c=fgetc(fptr);
	struct node *headnode;
	headnode = (struct node*)malloc(sizeof(struct node));
	headnode->count=1;
	headnode->car=c;
	headnode->next=NULL;
	struct node *currentnode;
	currentnode = (struct node*)malloc(sizeof(struct node));

	while(c!=EOF)
	{
			c=fgetc(fptr);
			flag=0;
			currentnode=headnode;

			while(currentnode->next!=NULL)
			{
				if(c==(currentnode->car))
				{
					(currentnode->count)=(currentnode->count)+1;
					flag=1;
					break;
				}
				else
				{
			 		currentnode=currentnode->next;
				}
			}
				if(flag==1)
				{

					continue;
				}


				//I move forward 
				if(currentnode->next==NULL)
				{

					if(c==(currentnode->car))
                                	{
                                        	(currentnode->count)=(currentnode->count)+1;

						continue;

                                	}

					else
					{
                        			//create new node
                        			struct node *nextnode;
                        			nextnode = (struct node*)malloc(sizeof(struct node));
                        			nextnode->car=c;
                        			nextnode->count=1;
						nextnode->next=NULL;
                        			currentnode->next=nextnode;
						continue;
					}

                 		}




		fprintf(ptr,"%c",c);
		//c=fgetc(fptr);
	}

	fclose(fptr);
	fclose(ptr);

	/*Now to check the count*/

	printf("Charactrer \t Count\n");
	currentnode=headnode;

	while(currentnode->next!=NULL)
	{

		printf("%c \t          %d\n",currentnode->car,currentnode->count);
		currentnode=currentnode->next;

	}

}

