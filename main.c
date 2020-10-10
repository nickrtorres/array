#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "array.h"

static void double_value(uint32_t *value)
{
	assert(0 != value);
	*value = *value * 2;
}


int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "%s: bad arg count %d: %s <test|bench>\n", argv[0], argc, argv[0]);
		exit(1);
	}

	if (0 == strcmp(argv[1], "test")) {
		assert(ARRAY_NULLPTR == array_insert(NULL, 42));

		struct array array = array_init();
		assert(ARRAY_OK == array_insert(&array, 42));
		assert(ARRAY_OK == array_insert(&array, 128));
		assert(ARRAY_OK == array_insert(&array, 500));

		assert(array_contains(&array, 42));
		assert(array_contains(&array, 128));
		assert(array_contains(&array, 500));
		assert(!array_contains(&array, 300));

		assert(ARRAY_NULLPTR == array_map(&array, NULL));
		assert(ARRAY_OK == array_map(&array, &double_value));
		assert(array_contains(&array, 84));
		assert(array_contains(&array, 256));
		assert(array_contains(&array, 1000));
		assert(!array_contains(&array, 42));
		assert(!array_contains(&array, 128));
		assert(!array_contains(&array, 500));


		uint32_t *ptr = 0;
		assert(ARRAY_OK == array_get(&array, 0, &ptr));
		assert(84 == *ptr);
		assert(ARRAY_OK == array_get(&array, 1, &ptr));
		assert(256 == *ptr);
		assert(ARRAY_OK == array_get(&array, 2, &ptr));
		assert(1000 == *ptr);
		assert(ARRAY_OUT_OF_BOUNDS == array_get(&array, UINT32_MAX, NULL));

		uint32_t *back = 0;
		assert(ARRAY_OK == array_pop(&array, &back));
		assert(1000 == *back);
		assert(ARRAY_OK == array_pop(&array, &back));
		assert(256 == *back);
		assert(ARRAY_OK == array_pop(&array, &back));
		assert(84 == *back);
		assert(ARRAY_EMTPY == array_pop(&array, &back));

		array_destroy(&array);
	} else if (0 == strcmp(argv[1], "bench")) {
		puts("resize v. prealloc");
		{
			struct timeval stop, start;
			gettimeofday(&start, NULL);

			struct array array = array_init();
			for (uint32_t i = 0; i < UINT32_MAX; ++i) {
				array_insert(&array, i);
			}

			gettimeofday(&stop, NULL);
			printf("\twithout alloc: %lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
			array_destroy(&array);
		}
		{
			struct timeval stop, start;
			gettimeofday(&start, NULL);

			struct array array;
			assert(ARRAY_OK == array_with_capacity(&array, UINT32_MAX));
			for (uint32_t i = 0; i < UINT32_MAX; ++i) {
				array_insert(&array, i);
			}

			gettimeofday(&stop, NULL);
			printf("\twithout alloc: %lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
			array_destroy(&array);
		}

		puts("bounds checking");
		{
			struct timeval stop, start;

			struct array array;
			assert(ARRAY_OK == array_with_capacity(&array, UINT32_MAX));
			for (uint32_t i = 0; i < UINT32_MAX; ++i) {
				array_insert(&array, i);
			}

			gettimeofday(&start, NULL);

			uint32_t *ptr;
			for (uint32_t i = 0; i < UINT32_MAX; ++i) {
				array_get(&array, i, &ptr);
			}

			gettimeofday(&stop, NULL);
			printf("\twith bounds-checking: %lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
			array_destroy(&array);
		}
		{
			struct timeval stop, start;

			struct array array;
			assert(ARRAY_OK == array_with_capacity(&array, UINT32_MAX));
			for (uint32_t i = 0; i < UINT32_MAX; ++i) {
				array_insert(&array, i);
			}

			gettimeofday(&start, NULL);

			uint32_t *ptr;
			for (uint32_t i = 0; i < UINT32_MAX; ++i) {
				ptr = array_get_unchecked(&array, i);
			}

			gettimeofday(&stop, NULL);
			printf("\twithout bounds-checking: %lu\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
			array_destroy(&array);
		}
	} else {
		fprintf(stderr, "%s: bad option %s: %s <test|bench>\n", argv[0], argv[1], argv[0]);
	}

	return 0;
}
