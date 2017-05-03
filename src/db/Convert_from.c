#include "include\gf_graph.h"
#include "include\graph.h"
#include "include\tuple.h"
#include <string.h>
#include <stdlib.h>
gf_graph_t convert_from(graph_t g) {
	gf_graph_t gf_g;
	gf_graph_init(gf_g);
	while (g) {
		vertex_t v = g->v;
		while (v) {
			gf_vertex_t newv;
			gf_vertex_init(newv);
			gf_vertex_set_id(newv, v->id);
			newv->tuple = v->tuple;
			gf_graph_insert_vertex(gf_g, newv);
			v = v->next;
		}
		edge_t e = g->e;
		while (e) {
			gf_edge_t newe;
			gf_edge_init(newe);
			gf_edge_set_vertices(newe, e->id1, e->id2);
			newe->tuple = e->tuple;
			gf_graph_insert_edge(gf_g, newe);
			e = e->next;
		}
		g = g->next;
	}
	return gf_g;
}