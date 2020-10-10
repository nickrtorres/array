#ifndef ARRAY_H_
#define ARRAY_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef void (*array_mapfn) (uint32_t *);

enum array_result {
	ARRAY_OK,
	ARRAY_EMTPY,
	ARRAY_EOM,
	ARRAY_NULLPTR,
	ARRAY_OUT_OF_BOUNDS,
};

struct array {
	size_t m_len;
	size_t m_capacity;
	uint32_t *m_data;
};

struct array array_init();
enum array_result array_with_capacity(struct array *array, size_t cap);
void array_destroy(struct array *array);

size_t array_len(struct array *array);

enum array_result array_insert(struct array *array, uint32_t value);
enum array_result array_pop(struct array *array, uint32_t **back);

bool array_contains(struct array *array, uint32_t needle);
enum array_result array_get(struct array *array, size_t idx, uint32_t **ptr);
uint32_t *array_get_unchecked(struct array *array, size_t idx);;

enum array_result array_map(struct array *array, array_mapfn fn);

#endif /* ARRAY_H_ */
