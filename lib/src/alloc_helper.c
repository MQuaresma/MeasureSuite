/**
 * Copyright 2023 University of Adelaide
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "alloc_helper.h"
#include "debug.h"
#include "error/error.h"
#include "struct_measuresuite.h"
#include <errno.h> // errno
#include <fcntl.h> // open
#include <stdint.h>
#include <stdlib.h>   // realloc
#include <string.h>   // strerror
#include <sys/mman.h> // mmap...
#include <unistd.h>   // close

int realloc_or_fail(struct measuresuite *ms, void **dest, size_t new_len) {

  DEBUG("Reallocing to new length of %lu bytes\n", new_len);

  *dest = realloc(*dest, new_len);

  if (*dest == NULL) {
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  return 0;
}

int map_rwx(struct measuresuite *ms, void **dest, size_t new_len) {
	int prot = PROT_READ | PROT_WRITE | PROT_EXEC;
	int flags = 
#ifdef __APPLE__
		MAP_JIT | MAP_ANONYMOUS |
#endif
		MAP_PRIVATE;

  int fd_zero = open("/dev/zero", O_RDWR);

  if (fd_zero == -1) {
		DEBUG("Failed opening /dev/zero");
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  /*
	 * Mapping doesn't work when swapping to /dev/zero in macOS when MS is executed
	 * as a dependency in TS.
	 * My guess is it has to do with the address range this is being mapped to, which
	 * Darwin deems as not valid.
	 * Thus we need to set the fd to -1, which prevents swapping
	 */
	*dest = mmap(NULL, new_len, prot, flags, -1, 0);
  // NOLINTNEXTLINE
  if (*dest == MAP_FAILED) {
		DEBUG("Failed to map memory\n");
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  if (close(fd_zero) == -1) { /*No longer needed*/
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  return 0;
}

int realloc_rwx_or_fail(struct measuresuite *ms, void **dest, size_t old_len,
                        size_t new_len) {
  // not using mremap here, because it may clash with errno (and USE_GNU)
  if (munmap(*dest, old_len)) {
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  return map_rwx(ms, *dest, new_len);
}

int unmap(struct measuresuite *ms, void *dest, size_t old_len) {
  if (dest == NULL) {
    return 0;
  }
  if (munmap(dest, old_len)) {
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  return 0;
}

int init_arithmetic_results(measuresuite_t ms, struct function_tuple *fct) {

  /** Arithmetic Res */
  /** +---------------------------------------------------------+ */
  /** |         out1 , out2, ... , out[#numArgOut]              | */
  /** +---------------------------------------------------------+ */
  /**     ^---   out1 * arg_width */

  /** Out1  each sizeof(u64)*/
  /** +---------------------------------------------------------+ */
  /** |         out1[0] , out1[1], ... , out1[#arg_width]       | */
  /** +---------------------------------------------------------+ */

  size_t size = ms->arg_width * ms->num_arg_out * sizeof(uint64_t);
  DEBUG("init_arithmetic_results, size: %lu\n", size);

  fct->arithmetic_results = malloc(size);

  if (fct->arithmetic_results == NULL) {
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  return 0;
}

int init_cycle_results(struct measuresuite *ms) {

  FOR_EACH_FUNCTION {

    fct->cycle_results = malloc(ms->num_batches * sizeof(uint64_t));

    if (fct->cycle_results == 0) {
      ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
      ms->additional_info = strerror(errno);
      return 1;
    }
  }

  return 0;
}

int init_json(struct measuresuite *ms) {

  const unsigned long char_per_long = 10;
  // at max, every function poduces one long per batch
  const unsigned long char_per_batch = char_per_long * ms->size_functions;

  // every batch produces char_per_batch' chars
  const unsigned long dynamic_length = char_per_batch * ms->num_batches;

  // the static stuff...
  const int len_other_stats = 1025;
  ms->json_len = len_other_stats + dynamic_length;

  ms->json = calloc(ms->json_len, sizeof(char));
  if (ms->json == NULL) {
    ms->errorno = E_INTERNAL_MEASURE__AI__ALLOC;
    ms->additional_info = strerror(errno);
    return 1;
  }

  return 0;
}
