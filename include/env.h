#ifndef ENV_H
#define ENV_H

typedef int bool;   //define bool
#define true 1
#define false 0
#define max(a, b) (a > b ? a : b)
#define STR_EQUAL(a, b) (!strcmp(a, b))
#define STRCASE_EQUAL(a, b) (!strcasecmp(a, b))
#define STR_NOT_EQUAL(a, b) (strcmp(a, b))
#define STRCASE_NOT_EQUAL(a, b) (strcasecmp(a, b))

#endif 
