/* Some helper functions made by me (NightSoulT12) */
#pragma once

/* Libraries */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Constants */
// Default input buffer size
#define BSIZE 500
// Default width size
#define WSIZE 60
// Default input arrow type
#define INPUT "> "

/* Types */
// stdbool lib bool type clone
typedef enum bool
{
  false,
  true = !false
}
bool;

/* Function headers */
long modby(long, long);
bool _isdigit(char);
int _strlen(char *);
long _abs(long);
void string_input(char *, short);
long int_input(void);
char char_input(void);
float float_input(void);
void pause(void);
void clean(void);
void warning(char *);
void title(char *);
