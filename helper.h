/* Define types, constants and functions to main project */
#pragma once

// include some libraries and functions
#include "tools.h"

/* Constants */
// data base default name
#define DBFILE "file.db"
// number to integrity file check
#define DBHEX 0x12
// actual year (temporary use)
#define YEAR 2021
// maximum number of students
#define STUDENTS 20
// maximum number of lessons
#define LESSONS 36
// maximum number of grades
#define GRADES 4
// default characters name size
#define NAME 16

/* Types */
// data base file header
typedef struct dbheader
{
  short last_registry;
  short registries;
  short updated_registries;
  short sortby;
  short reverse;
}
dbheader;

// student linked structure
typedef struct student
{
  short registry;
  char name[NAME];
  short class;
  float grade[GRADES];
  float media;
  short absence;
  float frequency;
  struct student *next;
}
student;

/* Enumerations */
// for listing
enum listby
{
  NONE,
  ALL,
  APPROVED,
  BYABSENCE,
  BYGRADE,
  UPDATE,
  REMOVE
};

// for sorting
enum sortby
{
  S_REGISTRY = 1,
  S_CLASS,
  S_NAME,
  S_MEDIA,
  S_FREQUENCY
};

// varible declaration
extern dbheader info;
extern student *data_list;

/* Functions headers */
// user.c functions
void new_registry(void);
void update_registry(void);
void remove_registry(void);
void list_registries(void);
void sort_registries(void);
void remove_data(void);
void about_info(void);

// auxiliary.c functions
void free_memory(void);
void show_registry(student *, short);
void select_registries(short);
void save_data(void);
bool read_data(void);
void get_name(char *);
bool comparator(student *, student *, short);
bool alphabetical(char *, char *, bool);
student *step(student *, short);
student *sort(student *, short);
void sortdataby(short);
bool registry_check(void);
