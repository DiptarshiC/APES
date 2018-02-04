/**
*@file name:double_linked_list.h
*
*@brief : A file declaring all the functions that are defined in  double_linked_list.c
*
*@date: 26th January 2018
*
*@author: Diptarshi Chakraborty
*
*
*/




#include<stdint.h>
#include<stdlib.h>



/**
*@description :the  structure node is
*
* a structure which
*
* contains the fields of prev and  next
*
* which are also pointers to structures
*/



	struct node{

	struct node  *prev;

	struct node  *next;

	};


/**
*@description :the typedef structure info is a type
*
* definition for a structure which 
*
* contains the fields of data and  node
*/


	 struct info{

	uint32_t data;

	struct node n1;

	};




/**
*@description :a pointer head which always points to the head
*
*of a double linked list keeps a track
*
*of the head and is incremented or decremented
*
*based on the operation.It is of the type info
*/


//struct info *head;



/**
*@description :a pointer head which always points to the tail
*
*of a double linked list keeps a track
*
*of the tail and is incremented or decremented
*
*based on the operation.It is of the type info
*/


//struct info *tail;


/**
*@Function name:destroy
*
*@Description:destroy all the nodes in the linked list by freeing the memory
*
*@Input parameters : A linked list data structure pointer
*
*@Returns:pointer to the head of the linked list
*
*/

struct node* destroy(struct node *info1);


/**
*@Function name:insert_at_beginning
*
*@Description:adds a node at the beginning of the linked list. Adds  a head node if  it does not exist
*
*@Input parameters :The head node pointer and a data to add
*
*@Returns:pointer to the head of the linked list
*
*/

struct node*  insert_at_beginning(struct node *info1, uint32_t data);

/**
*@Function name:insert_at_end
*
*@Description:adds a node at the beginning of the linked list. Adds  a head node if  it does not exist
*
*@Input parameters :The head node pointer and a data to add
*
*@Returns:pointer to the head of the linked list
*
*/

struct node*  insert_at_end(struct node *info1, uint32_t data);




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
*@Input parameters :head node pointer
*
*@Returns:pointer to the head of the linked list
*
*/


struct node* delete_from_end(struct node *head);




/**
*@Function name:delete_from_beginning
*
*@Description:deletes a node  at the beginning of the linked list
*
*@Input parameters :head node pointer
*
*@Returns:pointer to the head of the linked list
*
*/

struct node* delete_from_beginning(struct node *head);





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


struct node* delete_from_position(struct node *base,uint8_t index);



/**
*@Function name:size
*
*@Description:determines the number of links in your linked list
*
*@Input parameters:a node pointer
*
*@Returns:the size of the linked list in the number of nodes
*
*/

uint32_t  size(struct node* info1);
