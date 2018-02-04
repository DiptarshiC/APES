/**
*@file name:double_linked_list.c
*
*@brief : A file defining all the functions in  double_linked_list.c
*
*@date: 26th January 2018
*
*@author: Diptarshi Chakraborty
*
*/

/**
*@Graphical_Description:
*
*		Here is a graphical representation of my doubly linked list. It grows from the head
*                                                          towards the tail
*
*
*			    ->| |->| |->| |->| |->| |->| |->| |->| |->| |->| |->| |->| |->| |->| |->next
*
*			prev<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-| |<-
*
*                              ^                                                                ^
*			     tail                                                              head
*
*
*/


#include<stdint.h>
#include<stdlib.h>
#include "double_linked_list.h" 





#define	get_list_container(ptr,type,member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );} )

/**
*@Function name:destroy
*
*@Description:destroy all the nodes in the linked list by freeing the memory
*
*@Verbose:In this function deletion operations starts at the 
*
*at the tail.Therefore in this function till the prev pointer
*
*is null, we move till the tail. Once the tail pointer is reached
*
*the destruction of the array begins by moving towards the head.
*
*The function then returns the pointer to the head.The function first
*
*checks whether the parameter provided is a null pointer and if it
*
*is a null pointer, the function returns a NULL pointer
*
*@Input parameters : A linked list data structure pointer
*
*@Returns:pointer to the head of the linked list
*
*/


struct node* destroy(struct node *info1)
{




	/*Below is a null pointer check that is common to all functions*/

	if(info1==NULL)

	{

		return NULL;

	}

	struct node *temp=(struct node*) malloc(sizeof(struct node));/**
                                                                      *A temporary pointer variable which helps
                                                                      *
                                                                      *to store a node tempoararily while look
                                                                      *
                                                                      *for the tail. Once the tail is found
                                                                      *
                                                                      *traversal towards the head begins
                                                                      */


	if(info1->prev==NULL)//tail found!!
	{
		while(info1->next!=NULL)//while we donot reach the head
		{
			temp=info1->next;//temporarily stores the current node.

			info1=info1->next;//we are moving to the next node

			free(temp);//we are freeing the memory from the current node
		}
	}

	else
	{
		while(info1->prev!=NULL)//checks whether the tail has been reached yet
		{
			info1=info1->prev;//keeps moving till it reaches the tail node

		}

		/*Now when pointer to the structure reaches the tail
		*
		*destruction  can begin
		*/

		 while(info1->next!=NULL)//while we donot reach the head
                {
                        temp=info1;//temporarily stores the current node.

                        info1=info1->next;//we are moving to the next node

                        free(temp);//we are freeing the memory from the current node
                }

		/*At last the function will free the memory of the head pointer and return the
		*
		*pointer to the head address.
		*/

		temp=info1;
		free(temp);
		return info1;

	}



}

/**
*@Function name:insert_at_end
*
*@Description:adds a node at the end of the linked list.
*
*creates  a head node if  it does not exist
*
*@Verbose : In this function it is assumed that the pointer
*
*being passed is a head pointer.If this pointer  is a NULL pointer
*
*then the function creates a node for the pointer and
*
*the pointer itself becomes a head pointer.however if the pointer
*
*is indeed the pointer of an already long doubly linked list,then
*
*it creates a node previous to the head where it is filled with
*
*info
*
*@Input parameters :The head node pointer and a data to add
*
*@Returns:pointer to the head of the linked list
*
*/

struct node*  insert_at_end(struct node *info1, uint32_t data1){

 	struct node *temp=(struct node*) malloc(sizeof(struct node));/**
                              					      *A temporary pointer variable which helps
                                                                      *
                                                                      *to store a node tempoararily while look
                                                                      *
                                                                      *for the tail. Once the tail is found
                                                                      *
                                                                      *traversal towards the head begins
                                                                      */



	 /*Below is a null pointer check that is common to all functions*/

        if(info1==NULL)

        {

                info1=(struct node*) malloc(sizeof(struct node));
	/*	struct info * cont;
		cont=get_list_container(info1,struct info,n1);*/
		info1->prev=NULL;
		info1->next=NULL;

        }

	else

	{
		struct node *new_node=(struct node*) malloc(sizeof(struct node));/*This is a new pointer  that
					  					   *that will be the new head node*/



		while(info1->prev!=NULL)//checks whether the tail has been reached yet
		{
			info1=info1->prev;//keeps moving till it reaches the tail node

		}

		//new_node->data=data1;
		new_node->next=info1;
		new_node->prev=NULL;
		info1=new_node;//assigning the new node as the head node



	}
		return info1;


}

/**
*@Function name:insert_at_beginning
*
*@Description:adds a node at the beginning of the linked list. 
*
*Adds  a head node if  it does not exist
*
*@Verbose : This function adds a pointer to the tail of the linked list.
*
*It is assumed that it shall accept the head of the pointer as an input.
*
*If the pointer entered is a NULL, then it shall create a node using that pointer
*
*and shall be both the head and the tail of the dll.
*
*@Input parameters :The head node pointer and a data to add
*
*@Returns:pointer to the head of the linked list
*
*/

struct node*  insert_at_beginning(struct node *info1, uint32_t data){

	struct node *temp=(struct node*) malloc(sizeof(struct node));/**
                              					      *A temporary pointer variable which helps
                                                                      *
                                                                      *to store a node tempoararily while look
                                                                      *
                                                                      *for the tail. Once the tail is found
                                                                      *
                                                                      *traversal towards the head begins
                                                                      */



	 /*Below is a null pointer check that is common to all functions*/

        if(info1==NULL)

        {

                info1=(struct node*) malloc(sizeof(struct node));
		//info1->data=data1;
		info1->prev=NULL;
		info1->next=NULL;

        }

	else

	{
		struct node *new_node=(struct node*) malloc(sizeof(struct node));/*This is a new pointer  that
					  					   *that will be the new head node*/




		//new_node->data=data1;
		new_node->next=NULL;
		new_node->prev=info1;
		info1=new_node;//assigning the new node as the head node



	}
		return info1;



}
/**
*@Function name:insert_at_position
*
*@Description:adds a node to a given position to the linked list
*
*@Input parameters :The base node pointer, data to add, and the relative position to add the data
*
*@Returns:pointer to the head of the linked list
*
*/

struct node* insert_at_position(struct node *base,uint32_t data, uint8_t pos);



/**
*@Function name:delete_from_end
*
*@Description:deletes a node to the end of the link list
*
*@Verbose: In this function, we delete the node at the tail of the
*
*linked list.Therefore, the parameter accepted is the head of the linked list.
*
*If the node pointer is a null pointer, then the function
*
*returns a null pointer anyways.
*
*@Input parameters :head node pointer
*
*@Returns:pointer to the head of the linked list
*
*/


struct node* delete_from_end(struct node *head){


	if(head==NULL)

        {

                return NULL;

        }

        struct node *temp=(struct node*) malloc(sizeof(struct node));/**
                                                                      *A temporary pointer variable which helps
                                                                      *
                                                                      *to store a node tempoararily while look
                                                                      *
                                                                      *for the tail. Once the tail is found
                                                                      *
                                                                      *traversal towards the head begins
                                                                      */

	while(head->prev!=NULL)//checks whether the tail has been reached yet
		{
			head=head->prev;//keeps moving till it reaches the tail node

		}

        temp=head;//assigns the head to a temporary node type pointer
        head=head->next;//assigns the new head to its next node
        free(temp);
        return head;




}
/**
*@Function name:delete_from_beginning
*
*@Description:deletes a node  at the beginning of the linked list
*
*@Verbose : In this function, the head node moves one position towards
*
*the tail and free the node memory from the current head node
*
*the tail and the head node gets deleted.If the pointer entered
*
*is a null pointer, then the function returns a null pointer
*
*@Input parameters :head node pointer
*
*@Returns:pointer to the head of the linked list
*
*/

struct node* delete_from_beginning(struct node *head){



	if(head==NULL)

	{

		return NULL;

	}

	struct node *temp=(struct node*) malloc(sizeof(struct node));/**
                                                                      *A temporary pointer variable which helps
                                                                      *
                                                                      *to store a node tempoararily while look
                                                                      *
                                                                      *for the tail. Once the tail is found
                                                                      *
                                                                      *traversal towards the head begins
                                                                      */


	temp=head;//assigns the head to a temporary node type pointer
	head=head->prev;//assigns the new head to its previous
	free(temp);
	return head;
}
/**
*@Function name:delete_from_position
*
*@Description:deletes a node  at a given position in the linked list
*
*@Input parameters :base node pointer and an index where to remove the data
*
*@Returns:pointer to the head of the linked list
*
*/


struct node* delete_from_position(struct node *base,uint8_t index){

		if(base==NULL)

        {

                return NULL;

        }

        struct node *temp=(struct node*) malloc(sizeof(struct node));/**
                                                                      *A temporary pointer variable which helps
                                                                      *
                                                                      *to store a node tempoararily while look
                                                                      *
                                                                      *for the node at the given index.
                                                                      *
                                                                      *
                                                                      */
	
	struct node *free1=(struct node*) malloc(sizeof(struct node));/**
                                                                      *A temporary pointer variable which helps
                                                                      *
                                                                      *us free a block of memory
                                                                      *
                                                                      *for the node at the given index.
                                                                      *
                                                                      *
                                                                      */

	for(uint8_t i=1;i<=index;i++)/**
				     *An for loop to reach the given 
				     *index
				     */
 
	{

		base=base->prev;
	}
		free1=base;
		free(free1);
		temp=base->next;
		base=base->prev;//moves one place towards the tail
		base->next=temp;

}
/**
*@Function name:size
*
*@Description:determines the number of links in your linked list
*
*@Verbose:In this function, it is assumed that the parameter being passed is the
*
*head pointer.The function therefore counts the number of times there
*
*can be movement till the previous node.Once the node.prev is NULL
*
*we can put a stop to incrementing the counter and the count is then returned
*
*@Input parameters:a node pointer
*
*@Returns:the size of the linked list in the number of nodes
*
*/


uint32_t  size(struct node *info1){


	uint32_t count=0;

	while(info1->prev!=NULL)
	{

		count++;
		info1=info1->prev;

	}

	return count;

}
