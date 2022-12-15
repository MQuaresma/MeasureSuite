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

static int test_load_elf_ok() {
  // clang-format off
  const uint8_t elf_file_data[] = {
0x7f,0x45,0x4c,0x46,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // .ELF............
0x01,0x00,0x3e,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ..>.............
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ........@.......
0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x05,0x00,0x02,0x00, // ....@.....@.....
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x07,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x01,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // !...............
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x11,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0x01,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x03,0x00,0x00,0x00, // `...............
0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x19,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x02,0x00,0x00,0x00,0x00,0x00,0x00, // ........,0x.......
0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // 4...............
0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x48,0x8b,0x06,0x48,0x03,0x02,0x48,0x89,0x07,0xc3,0x00,0x00,0x00,0x00,0x00,0x00, // H..H..H.........
0x00,0x2e,0x74,0x65,0x78,0x74,0x00,0x2e,0x73,0x68,0x73,0x74,0x72,0x74,0x61,0x62, // ..text..shstrtab
0x00,0x2e,0x73,0x79,0x6d,0x74,0x61,0x62,0x00,0x2e,0x73,0x74,0x72,0x74,0x61,0x62, // ..symtab..strtab
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x04,0x00,0xf1,0xff, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x10,0x00,0x01,0x00, // ........$.......
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ................
0x00,0x74,0x65,0x73,0x74,0x2f,0x74,0x65,0x73,0x74,0x5f,0x64,0x61,0x74,0x61,0x2f, // .test/test_data/
0x61,0x64,0x64,0x5f,0x74,0x77,0x6f,0x5f,0x6e,0x75,0x6d,0x62,0x65,0x72,0x73,0x2e, // add_two_numbers.
0x61,0x73,0x6d,0x00,0x61,0x64,0x64,0x5f,0x74,0x77,0x6f,0x5f,0x6e,0x75,0x6d,0x62, // asm.add_two_numb
0x65,0x72,0x73,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, // ers.............
  };
  // clang-format on

  measuresuite_t ms = NULL;

  ms_initialize(&ms, arg_width, arg_num_in, arg_num_out);

  int id = -1;
  ms_assert_ok(
      ms_load_data(ms, ELF, elf_file_data, sizeof(elf_file_data), symbol, &id));
  ms_assert(id == 0);

  // should not error out if the symbol is NULL
  ms_assert_ok(
      ms_load_data(ms, ELF, elf_file_data, sizeof(elf_file_data), NULL, &id));
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
  int res = 0;
  res |= test_load_asm_ok();
  res |= test_load_bin_ok();
  res |= test_load_shared_object_fail();
  res |= test_load_elf_ok();
  return res;
}
