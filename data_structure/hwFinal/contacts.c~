#include "contacts.h"
#include "queue.h"
#include <string.h>
#include <malloc.h>
#include <stdio.h>

vertexPtr addVertexRelation(vertexPtr src, vertexPtr target)
{
	if (src == NULL)
	{
		return target;
	}
	vertexPtr vptr = src;
	edgePtr ePtr = src->arc_ptr;
	for (; vptr->nextvertex != src && !strcmp(vptr->info->name, target->info->name); vptr = vptr->nextvertex);
	if (!strcmp(vptr->info->name, target->info->name))
	{
		if (ePtr == NULL)
		{
			ePtr = (edgePtr)malloc(sizeof(struct graph_edge));
			ePtr->_vertexPtr = target;
			ePtr->nextarc = NULL;
			src->arc_ptr = ePtr;
			return src;
		}
		else
		{
			for (; ePtr->nextarc != NULL && !strcmp(ePtr->_vertexPtr->info->name, target->info->name); ePtr = ePtr->nextarc);
			if (!strcmp(ePtr->_vertexPtr->info->name, target->info->name))
			{
				return src;
			}
			else
			{
				edgePtr temp = (edgePtr)malloc(sizeof(struct graph_edge));
				temp->_vertexPtr = target;
				temp->nextarc = NULL;
				ePtr->nextarc = temp;
				return src;
			}
		}
	}
	else
	{
		vptr->nextvertex = target;
		vertexPtr targetRear = target;
		for (; targetRear->nextvertex != vptr; targetRear = targetRear->nextvertex);
		targetRear->nextvertex = src;
		if (ePtr == NULL)
		{
			ePtr = (edgePtr)malloc(sizeof(struct graph_edge));
			ePtr->_vertexPtr = target;
			ePtr->nextarc = NULL;
			src->arc_ptr = ePtr;
			return src;
		}
		else
		{
			for (; ePtr->nextarc != NULL && !strcmp(ePtr->_vertexPtr->info->name, target->info->name); ePtr = ePtr->nextarc);
			if (!strcmp(ePtr->_vertexPtr->info->name, target->info->name))
			{
				return src;
			}
			else
			{
				edgePtr temp = (edgePtr)malloc(sizeof(struct graph_edge));
				temp->_vertexPtr = target;
				temp->nextarc = NULL;
				ePtr->nextarc = temp;
				return src;
			}
		}
	}
}

vertexPtr addVertexRelationOnName(vertexPtr node, char* src, char* target)
{
	vertexPtr ptr = node;
	vertexPtr srcVertex = NULL, targetVertex = NULL;
	if (ptr == NULL)
	{
		contPtr temptr = (contPtr)malloc(sizeof(struct contacts));
		temptr->name = src;
		node = initVertex(temptr);
		ptr = node;
	}
	for (; ptr->nextvertex != node; ptr = ptr->nextvertex)
	{
		if (!strcmp(ptr->info->name, src))
		{
			srcVertex = ptr;
		}
		if (!strcmp(ptr->info->name, target))
		{
			targetVertex = ptr;
		}
	}
	if (!strcmp(ptr->info->name, src))
	{
		srcVertex = ptr;
	}
	if (!strcmp(ptr->info->name, target))
	{
		targetVertex = ptr;
	}
	if (srcVertex == NULL)
	{
		srcVertex = (vertexPtr)malloc(sizeof(struct graph_vertex));
		srcVertex->info = (contPtr)malloc(sizeof(struct contacts));
		srcVertex->info->name = src;
		srcVertex->marked = 0;
		srcVertex->nextvertex = ptr->nextvertex;
		srcVertex->arc_ptr = NULL;
		ptr->nextvertex = srcVertex;
	}
	edgePtr ePtr = srcVertex->arc_ptr;
	if (ePtr == NULL)
	{
		ePtr = (edgePtr)malloc(sizeof(struct graph_edge));
		ePtr->nextarc = NULL;
		ePtr->_vertexPtr = NULL;
		srcVertex->arc_ptr = ePtr;
	}
	for (; ePtr->nextarc != NULL; ePtr = ePtr->nextarc)
	{
		if (!strcmp(ePtr->_vertexPtr->info->name, target))
		{
			return node;
		}
	}
	edgePtr temp = (edgePtr)malloc(sizeof(struct graph_edge));
	temp->nextarc = NULL;
	if (targetVertex == NULL)
	{
		targetVertex = (vertexPtr)malloc(sizeof(struct graph_vertex));
		targetVertex->info = (contPtr)malloc(sizeof(struct contacts));
		targetVertex->info->name = target;
		targetVertex->arc_ptr = NULL;
		targetVertex->marked = 0;
		targetVertex->nextvertex = ptr->nextvertex;
		ptr->nextvertex = targetVertex;
	}
	if (ePtr->_vertexPtr == NULL)
	{
		ePtr->_vertexPtr = targetVertex;
	}
	else
	{
		temp->_vertexPtr = targetVertex;
		ePtr->nextarc = temp;
	}
	return node;
}

vertexPtr initVertex(contPtr _info)
{
	vertexPtr ptr = (vertexPtr)malloc(sizeof(struct graph_vertex));
	ptr->info = _info;
	ptr->arc_ptr = NULL;
	ptr->nextvertex = ptr;
	ptr->marked = 0;
	return ptr;
}

vertexPtr getVertexBasedOnName(vertexPtr src, char* _name)
{
	vertexPtr temp = src;
	for (; temp->nextvertex != src; temp = temp->nextvertex)
	{
		if (!strcmp(temp->info->name, _name))
		{
			return temp;
		}
	}
	return temp->nextvertex;
}

bool BFS(vertexPtr src, vertexPtr target)
{
	vertexPtr ptr = clearVisitedMark(src);
	ptr->marked = TRUE;
	create();
	enq(src);
	while (!isQueueEmpty())
	{
		ptr = frontelement();
		deq();
		edgePtr ePtr = ptr->arc_ptr;
		for (; ePtr != NULL; ePtr = ePtr->nextarc)
		{
			if (!strcmp(ePtr->_vertexPtr->info->name, target->info->name))
			{
				while (!isQueueEmpty())
				{
					printf("%s ", frontelement()->info->name);
					deq();
				}
				return TRUE;
			}
			if (!isVisitedMark(ePtr->_vertexPtr))
			{
				ePtr->_vertexPtr->marked = TRUE;
				enq(ePtr->_vertexPtr);
			}
		}
	}
	return FALSE;
}

bool DFS(vertexPtr src, vertexPtr target)
{
	static count = 0;
	vertexPtr ptr = clearVisitedMark(src);
	ptr->marked = TRUE;
	enq(src);
	++count;
	if (ptr->arc_ptr != NULL)
	{
		edgePtr watch;
		for (watch = ptr->arc_ptr; watch != NULL; watch = watch->nextarc)
		{
			vertexPtr watchVertex = watch->_vertexPtr;
			if (!strcmp(watchVertex->info->name, target->info->name))
			{
				printf("%s : ", watchVertex->info->name);
				printf("%s", frontelement()->info->name);
				deq();
				--count;
				return TRUE;
			}
			if (!isVisitedMark(watchVertex))
			{
				if (DFS(watchVertex, target))
				{
					printf("->%s", frontelement()->info->name);
					deq();
					--count;
					if (count == 0)
					{
						printf("->%s", target->info->name);
					}
					return TRUE;
				}
			}
		}
	}
	else if (!strcmp(src->info->name, target->info->name))
	{
		printf("%s", frontelement()->info->name);
		deq();
		--count;
		return TRUE;
	}
	else
	{
		deq();
		--count;
		return FALSE;
	}
	deq();
	--count;
	return FALSE;
}

vertexPtr clearVisitedMark(vertexPtr src)
{
	vertexPtr temp = src;
	for (; temp->nextvertex != src; temp = temp->nextvertex)
	{
		temp->marked = FALSE;
	}
	return src;
}

bool isVisitedMark(vertexPtr src)
{
	return src->marked;
}