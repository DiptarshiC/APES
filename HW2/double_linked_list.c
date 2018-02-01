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



#include<stdint.h>
#include<stdlib.h>
#include "double_linked_list.h" 







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

info* destroy(info *info1)
/*{
	if(info->node==NULL)




	while(info1->node.next!=NULL)
		{
			free(info->node);
			info->node=info->node.previous;
		}

}

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

info*  insert_at_beginning(info *info1, uint32_t data);


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

info*  insert_at_end(info *info1, uint32_t data1)
{
	info* new_info=(info*) malloc(sizeof(info));//This line allocates space for the new node.

	if(info1==NULL)
	{
		return NULL;
	}

	else{

	new_info->data=data1;//This line allocates data to the info structure

	new_info->node.next=info1;//Make the new info as head

	new_info->node.prev=NULL;//Make the previous as NULL

	info1->node.prev=new_info;
	}

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

info* insert_at_position(info *base,uint32_t data, uint8_t pos);



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


info* delete_from_end(info *head);




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

info* delete_from_beginning(info *head);





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


info* delete_from_position(info *base,uint8_t index);




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


uint8_t  size(info *node);


