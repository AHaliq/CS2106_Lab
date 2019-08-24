#include <stdio.h>
#include <stdlib.h>
// stdlib includes malloc() and free()

// define instructions
#define PREVIOUS -1
#define NEXT 1
#define DELETE 0
#define INSERTSUBNODE 2
#define COLLAPSE 3 // New instruction compared to ex2.

// Node declarations

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

void insertNodeNext(int, int, node *);
void insertNodePrevious(int, int, node *);
void deleteNode(int, node *);
void printList(node *);
void printSubNodes(node *);
void deleteList(node *);
void insertSubNode(int, int, int, node *);
void deleteAllSubNodes(node *);
void collapseSubNodes(int, node *); //TODO
node *getNode(int, node *);

void (*funcs[5])(int position, node *originNode);
void ptr_insertNodePrevious(int, node *);
void ptr_insertNodeNext(int, node *);
void ptr_insertSubNode(int, node *);
void ptr_collapseSubNodes(int, node *);

// Start of main
int main()
{
	int position;
	int instruction;
	//int subPosition;
	//int value;

	// Declaration of the origin Node
	node *originNode = (node *)malloc(sizeof(node));
	originNode->previousNode = originNode;
	originNode->nextNode = originNode;
	originNode->data = 0;
	originNode->subNodeHead = NULL;

	funcs[0] = ptr_insertNodePrevious;
	funcs[1] = deleteNode;
	funcs[2] = ptr_insertNodeNext;
	funcs[3] = ptr_insertSubNode;
	funcs[4] = ptr_collapseSubNodes;

	while (scanf("%i%i", &instruction, &position) == 2)
	{
		(*funcs[instruction + 1])(position, originNode);
	}
	printList(originNode);
	deleteList(originNode);

	printf("Circular List after delete\n");
	printList(originNode);

	free(originNode);

	return 0;
}

// Pointer functions

void ptr_insertNodePrevious(int position, node *originNode)
{
	int value;
	scanf("%i", &value);
	insertNodePrevious(position, value, originNode);
}
void ptr_insertNodeNext(int position, node *originNode)
{
	int value;
	scanf("%i", &value);
	insertNodeNext(position, value, originNode);
}
void ptr_insertSubNode(int position, node *originNode)
{
	int subPosition, value;
	scanf("%i%i", &subPosition, &value);
	insertSubNode(position, subPosition, value, originNode);
}
void ptr_collapseSubNodes(int position, node *originNode)
{
	collapseSubNodes(position, originNode);
}

//Function Implementations

void collapseSubNodes(int position, node *targetNode)
{
	node *cur = getNode(position, targetNode);
	int sum = 0;
	subNode *sn = cur->subNodeHead;
	while (sn != NULL)
	{
		sum += sn->data;
		sn = sn->nextSubNode;
	}
	cur->data += sum;
	deleteAllSubNodes(cur);
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
