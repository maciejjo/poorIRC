#include "usertree.h"


User *newUser(pid_t userPID, char nick[])
{
	User *newElement;
	newElement = calloc(1, sizeof(User *));

	newElement->userPID=userPID;
	strcpy(newElement->nickname,nick);
	newElement->left=NULL;
	newElement->right=NULL;
	newElement->parent=NULL;

	return newElement;
}
//-------------------------------------------------------------------------------

void addUser(User **root,pid_t userPID, char nickname[])
{
	User *parent,*current,*newElement;
	newElement = newUser(userPID,nickname);

	parent=NULL;
	current = *root;

	while(current != NULL)
	{
		parent = current;
		if(userPID<(current->userPID))
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}
	newElement->parent=parent;
	if(parent==NULL)
	{
		*root=newElement;
	}
	else if(userPID<(parent->userPID))
	{
		parent->left=newElement;
	}
	else
	{
		parent->right=newElement;
	}
}
//-------------------------------------------------------------------------------
void showInOrder(User *root)
{
	User *current;
	current=root;
	if(current!=NULL)
	{
		showInOrder(current->left);
		printf("%d %s\n",current->userPID,current->nickname);
		showInOrder(current->right);
	}
}
//-------------------------------------------------------------------------------
void showPreOrder(User *root)
{
	User *current;
	current=root;
	if(current!=NULL)
	{
		printf("%d %s\n",current->userPID,current->nickname);
		showPreOrder(current->left);
		showPreOrder(current->right);
	}
}
//-------------------------------------------------------------------------------
void showPostOrder(User *root)
{
	User *current;
	current=root;
	if(current!=NULL)
	{
		showPostOrder(current->left);
		showPostOrder(current->right);
		printf("%d %d %s\n",current->userPID,current->nickname);
	}
}
//-------------------------------------------------------------------------------
User* minimumFrom(User *leaf)
{
	while(leaf->left!=NULL)
	{
		leaf=leaf->left;
	}
	return leaf;
}
//-------------------------------------------------------------------------------
User* findNextInOrder(User *leaf)
{
	User *current;
	if(leaf->right!=NULL)
	{
		return minimumFrom(leaf->right);
	}

	current=leaf->parent;
	while((current!=NULL)&&(leaf==current->right))
	{
		leaf=current;
		current=current->parent;
	}
	return current;
}
//-------------------------------------------------------------------------------
User* findUserByPID(User **root, pid_t userPID)
{

	User *current;
	current =* root;

	while((current!=NULL)&&
			(userPID != (current->userPID)))
	{
		if(userPID < (current->userPID))
		{
			current = current->left;
		}
		else
		{
			current = current->right;
		}
	}

	return current;
}
//-------------------------------------------------------------------------------
int deleteUser(User **rootd,pid_t userPID)
{
	User *element,*toDelete,*temp;
	element=*rootd;

	//szuka w drzewie wezla do usuniecia
	while(element!=NULL)
	{
		if(userPID==(element->userPID))
		{
			break;
		}
		else if(userPID<(element->userPID))
		{
			element=element->left;
		}
		else
		{
			element=element->right;
		}
	}

	if(element==NULL) return 0;
	if((element->left==NULL)||(element->right==NULL)) //sprawdza czy ma synow
	{
		toDelete=element;           //jak jeden lub wcale
	}
	else
	{
		toDelete=findNextInOrder(element); //jak ma dwoch to szuka takiego, ktory go zastapi usuwany element
	}                                      //czyli NAJMNIEJSZEGO Z PRAWEGO PODRZEWA
	//----
	if(toDelete->left!=NULL)    //zapamietuje lewy lub prawy nastepnik
	{
		temp=toDelete->left;
	}
	else
	{
		temp=toDelete->right;
	}
	//----
	if(temp!=NULL) temp->parent=toDelete->parent; //ustawia poprzednika w lewym lub prawym (poszukanym wyzej)
	//----
	if(toDelete->parent==NULL)
	{
		(*rootd)=temp;      //usuwanie korzenia

	}
	else if(toDelete==(toDelete->parent)->left)     //z jednym dzieckiem #1 - lewy
	{
		(toDelete->parent)->left=temp;

	}
	else
	{
		(toDelete->parent)->right=temp;          //z jednym dzieckiem #2 - prawy

	}
	//----
	if(toDelete!=element)
	{
		element->userPID = toDelete->userPID;
		strcpy(element->nickname,toDelete->nickname);

	}
	free(toDelete);
	return 1;
}
