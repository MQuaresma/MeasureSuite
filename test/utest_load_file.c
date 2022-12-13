/**
 * Copyright 2022 University of Adelaide
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

#include "helper.h"
#include <assert.h>
#include <measuresuite.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char symbol[] = {"add_two_numbers"};
const char file_asm[] = {"./test_data/add_two_numbers.asm"};
const char file_bin[] = {"./test_data/add_two_numbers.bin"};
const char file_elf[] = {"./test_data/add_two_numbers.o"};
const char file_shared_object[] = {"./test_data/add_two_numbers.so"};

static int test_load_asm_ok() {

  const int arg_width = 1;
  const int arg_num_in = 2;
  const int arg_num_out = 1;
  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;

  assert((ms_load_file(ms, ASM, file_asm, symbol, &id)) == 0);
  assert(id == 0);

  // should not error out if the symbol is NULL
  assert((ms_load_file(ms, ASM, file_asm, NULL, &id)) == 0);
  assert(id == 0); // should not have changed

  assert(ms_terminate(ms) == 0);

  return 0;
}

static int test_load_bin_ok() {

  const int arg_width = 1;
  const int arg_num_in = 2;
  const int arg_num_out = 1;
  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;
  assert(ms_load_file(ms, BIN, file_bin, symbol, &id) == 0);
  assert(id == 0);

  // should not error out if the symbol is NULL
  assert(ms_load_file(ms, BIN, file_bin, NULL, &id) == 0);
  assert(id == 0); // should not have changed

  assert(ms_terminate(ms) == 0);

  return 0;
}

static int test_load_elf_ok() {
  const int arg_width = 1;
  const int arg_num_in = 2;
  const int arg_num_out = 1;
  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;
  assert(ms_load_file(ms, ELF, file_elf, symbol, &id) == 0);
  assert(id == 0);

  // should not error out if the symbol is NULL
  assert(ms_load_file(ms, ELF, file_elf, NULL, &id) == 0);
  assert(id == 0); // should not have changed

  assert(ms_terminate(ms) == 0);

  return 0;
}

static int test_load_shared_object_ok() {
  const int arg_width = 1;
  const int arg_num_in = 2;
  const int arg_num_out = 1;
  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;

  assert(ms_load_file(ms, SHARED_OBJECT, file_shared_object, symbol, &id) == 0);
  assert(id == 0);

  assert(ms_terminate(ms) == 0);

  return 0;
}

int main() {
  int res = 0;
  res |= test_load_asm_ok();
  res |= test_load_bin_ok();
  res |= test_load_elf_ok();
  res |= test_load_shared_object_ok();
  return res;
}
