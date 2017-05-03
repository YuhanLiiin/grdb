#include "gf_graph.h"
#include "graph.h"
#include "tuple.h"
#include <string.h>
#include <stdlib.h>
int
cal_similarity(tuple_t p, tuple_t q) {
	attribute_t ap = p->s->attrlist, sp = ap, aq = q->s->attrlist, sq = aq;
	int count = 0;
	while (sp) {
		while (sq) {
			if (sp->name == sq->name&&sp->bt == sq->bt) count++;
			sq = sq->next;
		}
		sp = sp->next;
	}
}

schema_t
get_common(tuple_t p, tuple_t q) {
	attribute_t ap = p->s->attrlist, sp = ap, aq = q->s->attrlist, sq = aq;
	schema_t new_schema = NULL;
	schema_init(&new_schema);
		while (sp) {
			while (sq) {
				if (sp->name == sq->name&&sp->bt == sq->bt) {
					schema_attribute_insert(new_schema, sp);
				}
				sq = sq->next;
			}
			sp = sp->next;
		}
}

bool
all_vertexes_selected(gf_vertex_t* v, int len) {
	int i;
	for (i = 0; i < len; i++) {
		if (v[i] != NULL) return false;
	}
	return true;
}

bool
all_edges_selected(gf_edge_t *e, int len) {
	int i;
	for (i = 0; i < len; i++) {
		if (e[i] != NULL) return false;
	}
	return true;
}


bool
contain_schema(tuple_t t1, tuple_t t2) {
	int len = 0;
	attribute_t p =t2->s->attrlist;
	for (;; len++) {
		if (p == NULL) break;
		else p = p->next;
	}
	if (cal_similarity(t1, t2) != len) return false;
	else return true;
}

bool 
same_schema(tuple_t t1, tuple_t t2) {
	return contain_schema(t1, t2) && contain_schema(t2, t1);
}

bool
is_neighbour(int id1, int id2, gf_graph_t g) {
	if (id1 == id2) return false;
	else if (gf_graph_find_edge_by_ids(g, id1, id2) || gf_graph_find_edge_by_ids(g, id1, id2)) return true;
	else return false;

}

int
has_neighbour(int id, graph_t g, gf_graph_t gfg) {
	vertex_t v = g->v;
	while (v != NULL) {
		if (is_neighbour(id, v->id, g)) return v->id;
	}
	return -1;
}



void 
select_vertex_schema(gf_vertex_t* pv,schema_t s) {
	gf_vertex_t v=*pv;
	if (v->tuple->s == NULL) return;
	schema_t sv = v->tuple->s;
	attribute_t attr = s->attrlist;
	while (attr) {
		schema_attribute_remove(sv, attr);
	}
	if (sv) *pv = NULL;
}

void
select_edge_schema(gf_edge_t* pe, schema_t s) {
	gf_edge_t e = *pe;
	if (e->tuple->s == NULL) return;
	schema_t se = e->tuple->s;
	attribute_t attr = s->attrlist;
	while (attr) {
		schema_attribute_remove(se, attr);
	}
	if (se) *pe = NULL;
}
graph_t
convert_to(gf_graph_t g) {
	graph_t head = NULL,prevg=head;
	graph_init(head);
	int n_edges = get_edges_nums(g), n_vertexes = get_vertexes_nums(g);
	gf_edge_t* edges;
	gf_vertex_t* vertexes;
	int i;
	gf_edge_t e = g->e;
	for (i = 0; i < n_edges; i++) {
		edges[i] = e;
		e = e->next;
	}
	gf_vertex_t v = g->v;
	for (i = 0; i < n_vertexes; i++) {
		vertexes[i] = v;
		v = v->next;
	}
	while (!all_vertexes_selected(vertexes,n_vertexes)) {
		int i,ini_idx;
		vertex_t ini_v = NULL;
		vertex_init(ini_v);
		graph_t curg = NULL;
		graph_init(curg);
		for (i = 0; i < n_vertexes; i++) {
			if (vertexes[i] != NULL) {
				vertex_set_id(ini_v, vertexes[i]->id);
				ini_idx = i;
				break;
			}
		}
		schema_t se, sv;
		schema_init(&se);
		schema_init(&sv);
		int maxc = 0, idx = -1;
		for (i = 0; i < n_vertexes; i++) {
			if (vertexes[i] != NULL&&is_neighbour(ini_v->id,vertexes[i]->id,g->e)) {
				int temp = cal_similarity(ini_v->tuple, vertexes[i]->tuple);
				if (temp > maxc) {
					maxc = temp;
					idx = i;
				}

			}
		}

		if (maxc == 0) {//no neighbour has common schema
			ini_v->tuple = vertexes[ini_idx]->tuple;
			graph_insert_vertex(curg, ini_v);
			curg->sv = ini_v->tuple->s;
			prevg->next = curg;
			prevg = prevg->next;
			continue;
		}
		else {
			schema_t sv,se;//schema of vertex and edge
			schema_init(&sv);
			schema_init(&se);
			sv = get_common(vertexes[ini_idx]->tuple, vertexes[idx]->tuple);
			ini_v->tuple->s = sv;
			vertex_t neigh;
			vertex_init(neigh);
			vertex_set_id(neigh, vertexes[idx]->id);
			neigh->tuple->s = sv;
			curg->sv = sv;
			graph_insert_vertex(curg, ini_v);//insert 2 vertexes
			graph_insert_vertex(curg, neigh);
			select_vertex_schema(&vertexes[in_idx], sv);
			select_vertex_schema(&vertexes[idx], sv);
			edge_t e1,e2;//insert edges
			edge_init(e1);
			edge_init(e2);
			gf_edge_t temp = gf_graph_find_edge_by_ids(g, ini_v->id, neigh->id);
			if (temp) {
				edge_set_vertices(e1,ini_v->id, neigh->id);
				se = vertexes[idx]->tuple->s;
				e1->tuple->s = se;
				curg->se = se;
				graph_insert_edge(curg, e1);
			}
			temp = gf_graph_find_edge_by_ids(g, neigh->id, ini_v->id);
			if (temp) {
				edge_set_vertices(e2, ini_v->id, neigh->id);
				se = vertexes[idx]->tuple->s;
				e2->tuple->s = se;
				curg->se = se;
				graph_insert_edge(curg, e2);
			}
			while (1) {
				int flag = 0;
				for (int i = 0; i < n_vertexes; i++) {
					int find_id = has_neighbour(vertexes[i]->id, curg, g);
					if (find_id != -1) {
						if (contain_schema(gf_graph_find_edge_by_ids(g, temp, vertexes[i]->id)->tuple, curg->v->tuple)) {
							flag = 1;//find another vertex
							vertex_t newv;//insert vertex
							vertex_init(newv);
							vertex_set_id(newv, vertexes[i]->id);
							graph_insert_vertex(curg,newv);
							select_vertex_schema(&vertexes[i], sv);
							edge_t e1, e2;//insert edges
							edge_init(e1);
							edge_init(e2);
							gf_edge_t temp = gf_graph_find_edge_by_ids(g, find_id, vertexes[i]->id);
							if (temp) {
								edge_set_vertices(e1, find_id, vertexes[i]->id);
								e1->tuple->s = se;
								graph_insert_edge(curg, e1);
								int j;
								for (j = 0; j < n_edges; j++) {
									if (edges[j] == temp) {
										select_vertex_schema(&edges[j], sv);
									}
								}
								
							}
							temp = gf_graph_find_edge_by_ids(g, vertexes[i]->id, find_id);
							if (temp) {
								edge_set_vertices(e2, vertexes[i]->id, find_id);
								e2->tuple->s = se;
								graph_insert_edge(curg, e2);
								int j;
								for (j = 0; j < n_edges; j++) {
									if (edges[j] == temp) {
										select_vertex_schema(&edges[j], sv);
									}
								}
							}
						}
					}
				}
				if (flag == 0) break;
			}
			prevg->next = curg;
			prevg = prevg->next;
			//start search edge
			while (!all_edges_selected(edges, n_edges)) {
				edge_t ini_e = NULL;
				edge_init(ini_e);
				graph_t curg = NULL;
				graph_init(curg);
				int i;
				for (i = 0; i < n_edges; i++) {
					if (edges[i] != NULL) {
						edge_set_vertices(ini_e, edges[i]->id1, edges[i]->id2);
						ini_e->tuple = edges[i]->tuple;
						edges[i] = NULL;
						graph_insert_edge(curg,ini_e);
						vertex_t v1, v2;
						vertex_init(v1);
						vertex_init(v2);
						vertex_set_id(v1, ini_e->id1);
						vertex_set_id(v2, ini_e->id2);
						graph_insert_vertex(curg, v1);
						graph_insert_vertex(curg, v2);
						break;
					}
				}
				for (i = 0; i < n_edges; i++) {
					if (edges[i] != NULL&&same_schema(ini_e->tuple, edges[i]->tuple)) {
						edge_t newe = NULL;
						edge_init(newe);
						edge_set_vertices(newe, edges[i]->id1, edges[i]->id2);
						graph_insert_edge(curg, newe);
						vertex_t v1, v2;
						vertex_init(v1);
						vertex_init(v2);
						vertex_set_id(v1, newe->id1);
						vertex_set_id(v2, newe->id2);
						graph_insert_vertex(curg, v1);
						graph_insert_vertex(curg, v2);
					}
				}
				prevg->next = curg;
				prevg = prevg->next;
			}
			return head->next;

		}
	}
	 
} 