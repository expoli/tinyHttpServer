//
// Created by tcy on 2023/3/23.
//

#include <cstdio>
#include <cstdlib>
void error_die(const char *str) {
  perror(str);
  exit(-1);
}
