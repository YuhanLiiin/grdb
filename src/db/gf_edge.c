#include "gf_graph.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
void gf_edge_init(gf_edge_t e) {
	assert(e != NULL);
	memset(e, 0, sizeof(struct gf_edge));
}
void gf_edge_set_vertices(gf_edge_t e, vertexid_t id1, vertexid_t id2) {
	assert(e != NULL);

	e->id1 = id1;
	e->id2 = id2;
}