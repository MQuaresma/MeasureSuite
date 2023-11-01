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

#include "helper.h"
#include <measuresuite.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static const char symbol[] = {"add_two_numbers"};
static const int arg_width = 1;
static const int arg_num_in = 2;
static const int arg_num_out = 1;

static int test_load_asm_ok() {

  char add_two_asm[] = {"mov rax, [rsi]\n"
                        "add rax, [rdx]\n"
                        "mov [rdi], rax\n"
                        "ret\n"};

  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;
  ms_assert_ok(ms_load_data(ms, ASM, (uint8_t *)add_two_asm,
                            strlen(add_two_asm), symbol, &id));
  ms_assert(id == 0);

  // should not error out if the symbol is NULL
  ms_assert_ok(ms_load_data(ms, ASM, (uint8_t *)add_two_asm,
                            strlen(add_two_asm), NULL, &id));
  ms_assert(id == 0); // should not have changed

  ms_assert_ok(ms_terminate(ms));

  return 0;
}

static int test_load_many_asm_ok() {

  char add_two_asm[] = {"mov rax, [rsi]\n"
                        "add rax, [rdx]\n"
                        "mov [rdi], rax\n"
                        "ret\n"};

  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;
  ms_assert_ok(ms_load_data(ms, ASM, (uint8_t *)add_two_asm,
                            strlen(add_two_asm), symbol, &id));
  ms_assert(id == 0);

  id = -1;
  ms_assert_ok(ms_load_data(ms, ASM, (uint8_t *)add_two_asm,
                            strlen(add_two_asm), NULL, &id));
  ms_assert(id == 1);

  id = -1;
  ms_assert_ok(ms_load_data(ms, ASM, (uint8_t *)add_two_asm,
                            strlen(add_two_asm), NULL, &id));
  ms_assert(id == 2);

  id = -1;
  ms_assert_ok(ms_load_data(ms, ASM, (uint8_t *)add_two_asm,
                            strlen(add_two_asm), NULL, &id));
  ms_assert(id == 3);

  ms_assert_ok(ms_measure(ms, 1, 1));

  assert_string_in_json(ms, "{\"stats\":{\"numFunctions\":4,\"runtime\":");

  ms_assert_ok(ms_terminate(ms));

  return 0;
}

static int test_load_bin_ok() {
  const uint8_t code[] = {
      0x48, 0x8b, 0x06, // mov rax, [rsi]
      0x48, 0x03, 0x02, // add rax, [rdx]
      0x48, 0x89, 0x07, // mov [rdi], rax
      0xc3,             // ret
  };

  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;
  ms_assert_ok(ms_load_data(ms, BIN, code, sizeof(code), symbol, &id));
  ms_assert(id == 0);

  // should not error out if the symbol is NULL
  ms_assert_ok(ms_load_data(ms, BIN, code, sizeof(code), NULL, &id));
  ms_assert(id == 0); // should not have changed

  ms_assert_ok(ms_terminate(ms));

  return 0;
}


static int test_load_shared_object_fail() {

  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;
  // should fail.
  ms_assert(ms_load_data(ms, SHARED_OBJECT, (uint8_t *)NULL, 0, symbol, &id) ==
            1);
  ms_assert(id == -1);

  ms_assert_ok(ms_terminate(ms));

  return 0;
}

int main() {
  SIGILL_SETUP();
#if USE_ASSEMBLYLINE
  int res = 0;
  res |= test_load_asm_ok();
  res |= test_load_many_asm_ok();
  res |= test_load_bin_ok();
  res |= test_load_shared_object_fail();
  return res;
#else
  return SKIP;
#endif
}
