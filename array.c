#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

static const size_t ARRAY_RESIZE_FACTOR = 2;

struct array
array_init()
{
	struct array rv = {
		.m_len        = 0,
		.m_capacity   = 0,
		.m_data       = NULL,
	};

	return rv;
}

enum array_result
array_with_capacity(struct array *array, size_t cap)
{
	if (NULL == array) {
		return ARRAY_NULLPTR;
	}

	uint32_t *data = malloc(sizeof(uint32_t) * cap);
	if (NULL == data) {
		return ARRAY_EOM;
	}

	array->m_len      = 0;
	array->m_capacity = cap;

	/*
	 * NB: This will leak if someone passes an array that contains allocated
 	 * data. If feels out of place to free the passed in array's m_data, so we
 	 * have to use the honor system here -- I can't think of a better way that
 	 * doesn't overly complicate the rest of this constructor.
 	 */
	array->m_data     = data;
	return ARRAY_OK;
}

void
array_destroy(struct array *array)
{
	if (NULL != array) {
		free(array->m_data);
	}
}

size_t
array_len(struct array *array)
{
	if (NULL == array) {
		return 0;
	} else {
		return array->m_len;
	}
}

static enum array_result
array_resize(struct array *array)
{
	assert (NULL != array);

	uint32_t *new_data;
	uint32_t *old_data = array->m_data;
	size_t old_cap  = array->m_capacity;
	size_t new_cap  = 0 == old_cap ? 1 : old_cap * ARRAY_RESIZE_FACTOR;

	new_data = malloc(sizeof(uint32_t) * new_cap);
	if (NULL == new_data) {
		return ARRAY_EOM;
	}

	if (0 != array->m_len) {
		for (size_t i = 0; i < array->m_len; ++i) {
			*(new_data + i) = *(old_data + i);
		}
	}

	free(old_data);
	array->m_capacity = new_cap;
	array->m_data = new_data;
	return ARRAY_OK;
}

enum array_result
array_insert(struct array *array, uint32_t value)
{
	if (NULL == array) {
		return ARRAY_NULLPTR;
	}

	if (array->m_len >= array->m_capacity) {
		enum array_result rv = array_resize(array);
		if (ARRAY_OK != rv) {
			return rv;	
		}
	}

	*(array->m_data + array->m_len++) = value;
	return ARRAY_OK;
}

enum array_result
array_pop(struct array *array, uint32_t **back)
{
	if (NULL == array) {
		return ARRAY_NULLPTR;
	}

	if (0 == array->m_len) {
		return ARRAY_EMTPY;
	}

	*back = array->m_data + --array->m_len;
	return ARRAY_OK;
}



bool
array_contains(struct array *array, uint32_t needle)
{
	for (size_t i = 0; i < array->m_len; ++i) {
		if (needle == *(array->m_data + i)) {
			return true;
		}
	}

	return false;
}

enum array_result
array_get(struct array *array, size_t idx, uint32_t **ptr)
{
	if (NULL == array) {
		return ARRAY_NULLPTR;
	}

	if (!(idx < array->m_len)) {
		return ARRAY_OUT_OF_BOUNDS;
	}

	*ptr = array_get_unchecked(array, idx);
	return ARRAY_OK;
}

uint32_t *
array_get_unchecked(struct array *array, size_t idx)
{
	assert(NULL != array);
	return array->m_data + idx;
}

void
array_map(struct array *array, array_mapfn fn)
{
	if (NULL == fn) {
		/* TOOD: error? */
		return;
	}

	for (size_t i = 0; i < array->m_len; ++i) {
		fn((array->m_data + i));
	}
}

