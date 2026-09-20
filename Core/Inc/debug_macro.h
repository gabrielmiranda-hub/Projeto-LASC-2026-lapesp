#include <stdio.h>

#define DEBUG_ENABLE 0




#if DEBUG_ENABLE
// Se ativado, substitui 'DEBUG_PRINT' por 'printf' repassando todos os argumentos
#define DEBUG_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
// Se desativado, a macro vira um bloco vazio e o compilador remove o código
#define DEBUG_PRINT(fmt, ...)                                                                                                                                  \
    do {                                                                                                                                                       \
    } while (0)
#endif
