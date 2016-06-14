#include "contacts.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 	100

int main(int argc, char const *argv[])
{
	FILE *file;
	vertexPtr rear = NULL;
	char target1[100], target2[100];
	char* buf = (char*)malloc(sizeof(char) * MAX_BUFFER);
	for (int i = 0; i < argc; ++i)
	{
		char* arg = argv[i];
		if ((file = fopen(arg, "r")) == NULL)
		{
			printf("Cannot open file %s!\n", argv[0]);
		}
		else
		{
			contPtr info = (contPtr)malloc(sizeof(struct contacts));
			info->name = arg;
			if (rear == NULL)
			{
				rear = initVertex(info);
			}
			while(fgets(buf, MAX_BUFFER, file) != NULL)
			{
				rear = addVertexRelationOnName(rear, arg, buf);
			}			
		}

	}
	while(1)
	{
		printf("Please input two target name:\n");
		scanf("%s%s", target1, target2);
		vertexPtr target = getVertexBasedOnName(rear, target1);
		vertexPtr src = getVertexBasedOnName(rear, target2);
		bool e = DFS(src, target);
		if (e == FALSE)
		{
			printf("They don't have any relationship on the graph\n");
		}
	}
	/* code */
	return 0;
}