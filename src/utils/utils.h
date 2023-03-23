//
// Created by tcy on 2023/3/23.
//

#ifndef CURSORCPPWEB_UTILS_H
#define CURSORCPPWEB_UTILS_H

#define PRINTF(str) printf("[%s - %d]" #str "=%s\n", __func__, __LINE__, str);
void error_die(const char *str);

#endif // CURSORCPPWEB_UTILS_H
