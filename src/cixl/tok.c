#include <stdlib.h>
#include <string.h>

#include "cixl/box.h"
#include "cixl/macro.h"
#include "cixl/tok.h"
#include "cixl/vec.h"

struct cx_tok_type *cx_tok_type_init(struct cx_tok_type *type) {
  type->eval = NULL;
  type->copy = NULL;
  type->deinit = NULL;
  return type;
}

struct cx_tok *cx_tok_init(struct cx_tok *tok,
			   struct cx_tok_type *type,
			   void *data,
			   int row, int col) {
  tok->type = type;
  tok->data = data;
  tok->row = row;
  tok->col = col;
  return tok;
}

struct cx_tok *cx_tok_deinit(struct cx_tok *tok) {
  if (tok->type->deinit) { tok->type->deinit(tok); }
  return tok;
}

void cx_tok_copy(struct cx_tok *dst, struct cx_tok *src) {
  dst->row = src->row;
  dst->col = src->col;
  dst->type = src->type;

  if (src->type->copy) {
    src->type->copy(dst, src);
  } else {
    dst->data = src->data;
  }
}
