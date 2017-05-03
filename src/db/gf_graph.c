#include "gf_graph.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int 
get_vertexes_nums(gf_graph_t g) {
	gf_vertex_t p = g->v;
	int i;
	for (i = 0;; i++) {
		if (p != NULL) p = p->next;
		else return i;
	}
}

int
get_edges_nums(gf_graph_t g)  {
	gf_edge_t p = g->e;
	int i;
	for (i = 0;; i++) {
		if (p != NULL) p = p->next;
		else return i;
	}
}

gf_vertex_t 
gf_graph_find_vertex_by_id(gf_graph_t g, vertexid_t id) {
	gf_vertex_t v;

	assert(g != NULL);

	for (v = g->v; v != NULL; v = v->next)
		if (v->id == id)
			return v;

	return NULL;
}

gf_edge_t 
gf_graph_find_edge_by_ids(gf_graph_t g, vertexid_t id1, vertexid_t id2) {
	gf_edge_t e;

	assert(g != NULL);

	for (e = g->e; e != NULL; e = e->next)
		if (e->id1 == id1 && e->id2 == id2)
			return e;

	return NULL;
}

void 
gf_graph_init(gf_graph_t g) {
	assert(g != NULL);
	memset(g, 0, sizeof(struct gf_graph));
}

void 
gf_graph_insert_vertex(gf_graph_t g, gf_vertex_t v) {
	gf_vertex_t w;

	assert(g != NULL);
	assert(v != NULL);

	if (g->v == NULL) {
		/* Insert vertex into empty graph */
		g->v = v;
		return;
	}
	
	for (w = g->v; w->next != NULL; w = w->next) {
		if (w->id == v->id) {//insert into existed vertex
			attribute_t attr = v->tuple->s->attrlist;
			while (attr) {
				schema_attribute_insert(w->tuple->s, attr);
				attr = attr->next;
			}
		}
	}
	/* Insert at the end of the double linked list */
	w->next = v;
	v->prev = w;
}

void 
gf_graph_insert_edge(gf_graph_t g, gf_edge_t e) {
	gf_edge_t f;

	if (g->e == NULL) {
		/* Insert edge into empty graph edge set */
		g->e = e;
		return;
	}
	
	for (f = g->e; f->next != NULL; f = f->next) {
		if (f->id1 == e->id1&&f->id2 == e->id2) {//insert into existed edge
			attribute_t attr = e->tuple->s->attrlist;
			while (attr) {
				schema_attribute_insert(e->tuple->s, attr);
				attr = attr->next;
			}
		}
	}
	/* Insert at the end of the double linked list */
	f->next = e;
	e->prev = f;
}
