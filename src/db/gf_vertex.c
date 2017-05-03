#include "gf_graph.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
void gf_vertex_init(gf_vertex_t v) {
	assert(v != NULL);
	memset(v, 0, sizeof(struct gf_vertex));
}
void gf_vertex_set_id(gf_vertex_t v, vertexid_t id) {
	assert(v != NULL);
	v->id = id;
}