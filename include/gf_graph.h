#ifndef _GF_GRAPH_H
#define _GF_GRAPH_H

#include "schema.h"
#include "tuple.h"
#include "vertexid.h"
struct gf_vertex {
	/* Every vertex has a unique vertex id */
	vertexid_t id;

	/* Links for vertex set data structure in memory */
	struct gf_vertex *prev;
	struct gf_vertex *next;

	/* Vertex tuple */
	tuple_t tuple;
};

struct gf_edge {
	/* Every edge has two vertex ids */
	vertexid_t id1, id2;

	/* Links for edge set data structure in memory */
	struct gf_edge *prev;
	struct gf_edge *next;

	/* Edge tuple */
	tuple_t tuple;
};

struct gf_graph {
	struct gf_vertex *v;	/* Vertices */
	struct gf_edge *e;		/* Edges */
	struct gf_graph *next;
};

typedef struct gf_vertex *gf_vertex_t;
typedef struct gf_edge *gf_edge_t;
typedef struct gf_graph *gf_graph_t;
int get_vertexes_nums(gf_graph_t);
int get_edges_nums(gf_graph_t);


void gf_vertex_init(gf_vertex_t v);
void gf_vertex_set_id(gf_vertex_t v, vertexid_t id);

void gf_edge_init(gf_edge_t e);
void gf_edge_set_vertices(gf_edge_t e, vertexid_t id1, vertexid_t id2);

void gf_graph_init(gf_graph_t g);
void gf_graph_insert_vertex(gf_graph_t g, gf_vertex_t v);
void gf_graph_insert_edge(gf_graph_t g, gf_edge_t e);


gf_vertex_t gf_graph_find_vertex_by_id(gf_graph_t g, vertexid_t id);
gf_edge_t gf_graph_find_edge_by_ids(gf_graph_t g, vertexid_t id1, vertexid_t id2);
#endif // _GF_GRAPH_H

