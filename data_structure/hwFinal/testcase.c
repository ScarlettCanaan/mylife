#include "contacts.h"
#include <malloc.h>

int main()
{
	contPtr info = (contPtr)malloc(sizeof(struct contacts));
	info->name = "1";
	vertexPtr rear = initVertex(info);
	rear = addVertexRelationOnName(rear, "1", "4");
	rear = addVertexRelationOnName(rear, "4", "2");
	rear = addVertexRelationOnName(rear, "2", "1");
	rear = addVertexRelationOnName(rear, "5", "1");
	rear = addVertexRelationOnName(rear, "2", "3");
	vertexPtr target = getVertexBasedOnName(rear, "4");
	vertexPtr src = getVertexBasedOnName(rear, "2");
	bool e = DFS(src, target);
	e = 0;
	e = BFS(src, target);
	return 0;
}