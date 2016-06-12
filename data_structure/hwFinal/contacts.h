#ifndef __CONTACTS_H__
#define __CONTACTS_H__

#ifndef bool
#define bool int
#endif // !bool

#ifndef TRUE
#define TRUE 1
#endif // !TRUE

#ifndef FALSE
#define FALSE 0
#endif // !FALSE

struct contacts
{
	char* name;
	char* call;
};

typedef struct contacts * contPtr;

struct graph_vertex
{
	int marked;
	struct graph_edge * arc_ptr;
	contPtr info;
	struct graph_vertex * nextvertex;
};

typedef struct graph_vertex * vertexPtr;

struct graph_edge
{
	struct graph_vertex * _vertexPtr;
	struct graph_edge * nextarc;
};

typedef struct graph_edge * edgePtr;

vertexPtr addVertexRelation(vertexPtr src, vertexPtr target);

vertexPtr addVertexRelationOnName(vertexPtr node, char* src, char* target);

vertexPtr initVertex(contPtr _info);

vertexPtr getVertexBasedOnName(vertexPtr src, char* _name);

bool BFS(vertexPtr src, vertexPtr target);

bool isVisitedMark(vertexPtr src);

vertexPtr clearVisitedMark(vertexPtr src);

bool DFS(vertexPtr src, vertexPtr target);

#endif // !__CONTACTS_H__

