#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "cixl/cx.h"
#include "cixl/error.h"
#include "cixl/scope.h"
#include "cixl/types/fimp.h"
#include "cixl/types/func.h"
#include "cixl/types/sym.h"

struct cx_sym *cx_sym_init(struct cx_sym *sym, const char *id, uint64_t tag) {
  sym->id = strdup(id);
  sym->tag = tag;
  return sym;
}

struct cx_sym *cx_sym_deinit(struct cx_sym *sym) {
  free(sym->id);
  return sym;
}

enum cx_cmp cx_cmp_sym(const void *x, const void *y) {
  const struct cx_sym *xs = x, *ys = y;
  if (xs->tag == ys->tag) { return CX_CMP_EQ; }
  return (strcmp(xs->id, ys->id) < 0) ? CX_CMP_LT : CX_CMP_GT;
}

static bool sym_imp(struct cx_scope *scope) {
  struct cx_box s = *cx_test(cx_pop(scope, false));
  cx_box_init(cx_push(scope),
	      scope->cx->sym_type)->as_sym = cx_sym(scope->cx, s.as_ptr);
  cx_box_deinit(&s);
  return true;
}

static bool str_imp(struct cx_scope *scope) {
  struct cx_sym s = cx_test(cx_pop(scope, false))->as_sym;
  cx_box_init(cx_push(scope), scope->cx->str_type)->as_ptr = strdup(s.id);
  return true;
}

static bool equid_imp(struct cx_box *x, struct cx_box *y) {
  return x->as_sym.tag == y->as_sym.tag;
}

static void fprint_imp(struct cx_box *v, FILE *out) {
  fprintf(out, "`%s", v->as_sym.id);
}

struct cx_type *cx_init_sym_type(struct cx *cx) {
  struct cx_type *t = cx_add_type(cx, "Sym", cx->any_type);
  t->equid = equid_imp;
  t->fprint = fprint_imp;

  cx_add_func(cx, "sym", cx_arg(cx->str_type))->ptr = sym_imp;
  cx_add_func(cx, "str", cx_arg(t))->ptr = str_imp;
  
  return t;
}
