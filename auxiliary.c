#include "helper.h"

dbheader info = {0, 0, 0, S_REGISTRY, false};
student *data_list = NULL;

/* Program functions */
// free alocated memory
void free_memory(void)
{
  student *temp;
  while (data_list != NULL)
  {
    temp = data_list;
    data_list = temp->next;
    free(temp);
  }
}

// show selected registry
void show_registry(student *actual, short stage)
{
  // show base
  printf("%i%03i     %-5i   %-15s   ", YEAR, actual->registry, actual->class, actual->name);
  if (stage < UPDATE)
  {
    // if no update show "blank"
    if (actual->absence == -1)
    {
      printf("  ----      ----");
    }
    else
    {
      // show grade media
      if (stage < BYABSENCE || stage == BYGRADE)
      {
        printf("  %-4.1f   ", actual->media);
      }
      // show frequency
      if (stage < BYGRADE)
      {
        printf("   %-3.0f%%", actual->frequency);
      }
    }
  }
  // new line
  puts("");
}

// auxiliary function for list, remove and update
void select_registries(short choice)
{
  student *actual = data_list;
  // NONE
  if (choice == NONE)
  {
    return;
  }
  
  // header
  printf("Matrícula | Turma |       Nome       ");
  if (choice == ALL || choice == APPROVED)
  {
    printf("| Média | Frequência");
    puts("\n---------------------------------------------------------");
  }
  else if (choice == BYABSENCE)
  {
    printf("| Frequência");
    puts("\n-------------------------------------------------");
  }
  else if (choice == BYGRADE)
  {
    printf("| Média");
    puts("\n---------------------------------------------");
  }
  else
  {
    puts("\n-------------------------------------");
  }
  
  // print registries
  while (actual != NULL)
  {
    // show registry by stage
    switch (choice)
    {
      case ALL:
        show_registry(actual, choice);
        break;
      case APPROVED:
        if (actual->media >= 7.0 && actual->frequency >= 60.0)
        {
          show_registry(actual, choice);
        }
        break;
      case BYABSENCE:
        if (actual->frequency < 60.0 && actual->absence != -1)
        {
          show_registry(actual, choice);
        }
        break;
      case BYGRADE:
        if (actual->media < 7.0 && actual->absence != -1)
        {
          show_registry(actual, choice);
        }
        break;
      case UPDATE:
        if (actual->absence == -1)
        {
          show_registry(actual, choice);
        }
        break;
      case REMOVE:
        show_registry(actual, choice);
        break;
    }
    // next student
    actual = actual->next;
  }
  puts("");
}

// save database
void save_data(void)
{
  // setup
  student *data_base = data_list;
  short hex = DBHEX;
  // open data base
  FILE *file = fopen(DBFILE, "wb");
  // check disk space
  if(file == NULL)
  {
    warning("Espaço em disco inexistente");
    return;
  }
  // start
  // save hex integrity
  fwrite(&hex, sizeof(short), 1, file);
  // save header
  fwrite(&info, sizeof(dbheader), 1, file);
  // save data list
  if (data_base != NULL)
  {
    fwrite(data_base, sizeof(student), 1, file);
    while (data_base->next != NULL)
    {
      data_base = data_base->next;
      fwrite(data_base, sizeof(student), 1, file);
    }
  }
  fclose(file);
}

// read data base file
bool read_data(void)
{
  student *last = NULL;
  student *actual = NULL;
  short hex;
  // opening file
  FILE *file = fopen(DBFILE, "rb");
  // check file existence
  if(file == NULL)
  {
    return true;
  }

  // start
  // check file integrity
  fread(&hex, sizeof(short), 1, file);
  if (hex != DBHEX)
  {
    fclose(file);
    remove(DBFILE);
    return true;
  }
  // read header
  fread(&info, sizeof(dbheader), 1, file);
  // verify if have registries
  if (!info.registries)
  {
    fclose(file);
    return true;
  }

  // get head data
  last = (student*)calloc(1, sizeof(student));
  if (last == NULL)
  {
    warning("Memória insulficiente");
    return false;
  }
  fread(last, sizeof(student), 1, file);
  data_list = last;

  // get tail data
  for (short i = 1; i < info.registries; i++)
  {
    actual = (student*)calloc(1, sizeof(student));
    if (actual == NULL)
    {
      warning("Memória insulficiente");
      return false;
    }
    fread(actual, sizeof(student), 1, file);
    // passthrough
    last->next = actual;
    actual->next = NULL;
    actual = actual->next;
    last = last->next;
  }
  fclose(file);
  return true;
}

// get student name and format
void get_name(char *name)
{
  short i;
  while (true)
  {
    string_input(name, NAME);
    if (_strlen(name) != 0)
    {
      for (i = 0; name[i] != '\0'; i++)
      {
        // get ascii code from a to z and A to Z
        if ((name[i] < 64 || name[i] > 91) && (name[i] < 96 || name[i] > 123))
        {
          break;
        }
        name[i] = toupper(name[i]);
      }
    }
    if (name[i] == '\0')
    {
      break;
    }
  }
}

// compare values for bubble sort
bool comparator(student *p, student *q, short sortby)
{
  switch (sortby)
  {
    case S_REGISTRY:
      return info.reverse ? q->registry > p->registry : q->registry < p->registry;
    case S_CLASS:
      return info.reverse ? q->class > p->class : q->class < p->class;
    case S_NAME:
      return alphabetical(q->name, p->name, info.reverse);
    case S_MEDIA:
      return info.reverse ? q->media > p->media : q->media < p->media;
    case S_FREQUENCY:
      return info.reverse ? q->frequency > p->frequency : q->frequency < p->frequency;
    default:
      return false;
  }
}

// compare alphabetically a text
bool alphabetical(char *text1, char *text2, bool reverse)
{
  short len1 = _strlen(text1);
  short len2 = _strlen(text2);
  short len = len1 < len2 ? len1 : len2;
  for (short i = 0; i < len; i++)
  {
    if (text1[i] > text2[i])
    {
      return reverse ? true : false;
    }
    else if (text1[i] < text2[i])
    {
      return reverse ? false : true;
    }
  }
  return false;
}

// swap values for bubble sort
student *step(student *data, short sortby)
{
  student *actual = data;
  student *posterior = actual->next;
  student *_return = posterior;
  while (posterior != NULL && comparator(data, data->next, sortby))
  {
    actual = posterior;
    posterior =  posterior->next;
  }
  actual->next = data;
  data->next = posterior;
  return _return;
}

// setup values recursively for bubble sort
student *sort(student *data, short sortby)
{
  if (data == NULL)
  {
    return NULL;
  }
  data->next = sort(data->next, sortby);
  if (data->next != NULL && comparator(data, data->next, sortby))
  {
    data = step(data, sortby);
  }
  return data;
}

// bubble sort linked student list
void sortdataby(short sortby)
{
  for (short i = 0; i < info.registries; i++)
  {
    data_list = sort(data_list, sortby);
  }
}

// temporary
bool registry_check(void)
{
  if (!info.registries)
  {
    warning("Nenhum cadastro efetuado");
    return true;
  }
  return false;
}
