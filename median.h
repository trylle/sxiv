#ifndef MEDIAN_H
#define MEDIAN_H

#include <Imlib2.h>

#ifdef __cplusplus
#define EXTERN_C extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C
#define EXTERN_C_END
#endif

EXTERN_C
DATA32 get_median_color();
EXTERN_C_END

#endif /* MEDIAN_H */
