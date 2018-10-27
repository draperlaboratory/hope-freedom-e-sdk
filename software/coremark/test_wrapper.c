/*
 * Copyright © 2017-2018 The Charles Stark Draper Laboratory, Inc. and/or Dover Microsystems, Inc.
 * All rights reserved. 
 *
 * Use and disclosure subject to the following license. 
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "test_status.h"
#include "test.h"

// include malloc wrappers
#include "mem.h"

#define SIFIVE_TEST_CTRL_ADDR  0x100000

enum {
        SIFIVE_TEST_FAIL = 0x3333,
        SIFIVE_TEST_PASS = 0x5555,
};

static volatile uint32_t *test;

/*
 * Hello world sanity test to check we can execute code i.e. main and 
 *     call printf
 */
int test_main(void)
  {
    test_positive(); // identify test as positive (will complete)

    coremark_main();
    
    test_pass();
    test_done();

    test = (uint32_t *)(void *)SIFIVE_TEST_CTRL_ADDR;
    *test = SIFIVE_TEST_PASS;
    while (1) {
        asm volatile("");
    }

  }

