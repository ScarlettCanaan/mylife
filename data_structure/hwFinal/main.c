#include "contacts.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 	100

int main(int argc, char const *argv[])
{
	FILE *file;
	int j = 0;
	vertexPtr rear = NULL;
	char target1[100], target2[100];
	char* buf = (char*)malloc(sizeof(char) * MAX_BUFFER);
	for (int i = 1; i < argc; ++i)
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

			while ((j = fgets(buf, MAX_BUFFER, file)) != NULL)
			{
				for (j = 0; buf[j] != '\n'; ++j);
				buf[j] = '\0';
				rear = addVertexRelationOnName(rear, arg, buf);
				//free(buf);
				buf = (char*)malloc(sizeof(char) * MAX_BUFFER);
			}
		}
	}
	while (1)
	{
		printf("Please input two target name:\n");
		scanf("%s%s", target1, target2);
		vertexPtr target = getVertexBasedOnName(rear, target2);
		if (target == NULL) 
		{
			printf("Don't has %s on the graph!\n", target2);
			break;
		}
		vertexPtr src = getVertexBasedOnName(rear, target1);
		if (src== NULL)
		{
			printf("Don't has %s on the graph!\n", target1);
			break;
		}
		bool e = DFS(src, target);
		if (e == FALSE)
		{
			printf("They don't have any relationship on the graph\n");
		}
		else
		{
			printf("\n");
		}
	}
	/* code */
	system("PAUSE");
	return 0;
}
