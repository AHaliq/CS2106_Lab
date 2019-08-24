#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2 // New instruction compared to ex1.

// Node and SubNode declarations ( Do not Edit )
typedef struct SUBNODE
{
	int data;
	struct SUBNODE *nextSubNode;
} subNode;

typedef struct NODE
{
	int data;
	struct NODE *previousNode;
	struct NODE *nextNode;
	subNode *subNodeHead;
} node;

// Function prototypes
void insertNodeNext(int, int, node *);		 // TODO
void insertNodePrevious(int, int, node *); //TODO
void deleteNode(int, node *);							 // TODO
void deleteList(node *);									 // TODO
void insertSubNode(int, int, int, node *); // TODO
void deleteAllSubNodes(node *);						 // Optional TODO: will be useful in ex3.
node *getNode(int, node *);

void printList(node *);			// Implemented for you
void printSubNodes(node *); // Implemented for you :Helper function for print list

// Start of main
int main()
{
	int position;
	int instruction;
	int subPosition;
	int value;

	// Declaration of the origin Node ( do not edit )
	node *originNode = (node *)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;

	// input and processing has been done for you
	while (scanf("%i%i", &instruction, &position) == 2)
	//scanf() returns an int that corresponds to the number of values scanned.
	{
		if (instruction == DELETE)
		{
			deleteNode(position, originNode);
		}
		else if (instruction == INSERTSUBNODE)
		{
			scanf("%i%i", &subPosition, &value);
			insertSubNode(position, subPosition, value, originNode);
		}
		else if (instruction == NEXT)
		{
			scanf("%i", &value);
			insertNodeNext(position, value, originNode);
		}
		else if (instruction == PREVIOUS)
		{
			scanf("%i", &value);
			insertNodePrevious(position, value, originNode);
		}
	}

	printList(originNode);
	deleteList(originNode);

	printf("Circular List after delete\n");
	printList(originNode);

	free(originNode);
	return 0;
}

//Function Implementations
void insertSubNode(int position, int subPosition, int value, node *originNode)
{
	node *cur = getNode(position, originNode);
	subNode *new = (subNode *)malloc(sizeof(subNode));
	new->data = value;
	new->nextSubNode = NULL;
	if (subPosition == 0)
	{
		subNode *pHead = cur->subNodeHead;
		cur->subNodeHead = new;
		new->nextSubNode = pHead;
	}
	else
	{
		int i = 0;
		subNode *sn = cur->subNodeHead;
		while (i++ < subPosition - 1)
		{
			if (sn == NULL)
				return; // invalid insertSubNode
			sn = sn->nextSubNode;
		}
		subNode *nxt = sn->nextSubNode;
		sn->nextSubNode = new;
		new->nextSubNode = nxt;
	}
}

void insertNodePrevious(int position, int value, node *originNode)
{
	node *cur = getNode(position, originNode);
	node *pre = cur->previousNode;
	node *new = (node *)malloc(sizeof(node));
	new->data = value;
	new->subNodeHead = NULL;
	cur->previousNode = new;
	pre->nextNode = new;
	new->previousNode = pre;
	new->nextNode = cur;
}

void insertNodeNext(int position, int value, node *originNode)
{
	node *cur = getNode(position, originNode);
	node *nxt = cur->nextNode;
	node *new = (node *)malloc(sizeof(node));
	new->data = value;
	new->subNodeHead = NULL;
	cur->nextNode = new;
	nxt->previousNode = new;
	new->nextNode = nxt;
	new->previousNode = cur;
}

void deleteNode(int position, node *originNode)
{
	node *cur = getNode(position, originNode);
	node *pre = cur->previousNode;
	node *nxt = cur->nextNode;
	pre->nextNode = nxt;
	nxt->previousNode = pre;
	cur->nextNode = NULL;
	cur->previousNode = NULL;
	deleteAllSubNodes(cur);
	free(cur);
}

void deleteAllSubNodes(node *targetNode)
{
	subNode *sn = targetNode->subNodeHead;
	while (sn != NULL)
	{
		subNode *nxt = sn->nextSubNode;
		sn->nextSubNode = NULL;
		free(sn);
		sn = nxt;
	}
	targetNode->subNodeHead = NULL;
}

void deleteList(node *originNode)
{
	while (originNode->nextNode != originNode)
	{
		deleteNode(1, originNode);
	}
	deleteAllSubNodes(originNode);
}

node *getNode(int position, node *originNode)
{
	int i = 0;
	node *n = originNode;
	while (i++ < position)
	{
		n = n->nextNode;
	}
	return n;
}

//Print list has been implemented for you.
// You may study the functions below to help you implement your own functions.
void printList(node *originNode)
{
	int count = 0;
	printf("Printing clockwise:\n");
	node *iterator = originNode->nextNode;
	printf("[Pos %i:%i]", 0, originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while (iterator != originNode)
	{
		count++;
		printf("[Pos %i:%i]", count, iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->nextNode;
	}
	printf("[Pos %i:%i]", 0, originNode->data);
	// printing subNodes
	printSubNodes(originNode);

	printf("\nPrinting counter-clockwise:\n");
	iterator = originNode->previousNode;
	printf("[Pos %i:%i]", 0, originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n   |\n   v\n");
	while (iterator != originNode)
	{
		printf("[Pos %i:%i]", count, iterator->data);
		// printing subNodes
		printSubNodes(iterator);
		printf("\n   |\n   v\n");
		iterator = iterator->previousNode;
		count--;
	}
	printf("[Pos %i:%i]", 0, originNode->data);
	// printing subNodes
	printSubNodes(originNode);
	printf("\n");

	return;
}

void printSubNodes(node *mainNode)
{
	int count = 0;
	subNode *iterator = mainNode->subNodeHead;
	while (iterator != NULL)
	{
		printf("->[subNode pos %i:%i]", count, iterator->data);
		iterator = iterator->nextSubNode;
		count++;
	}
}
