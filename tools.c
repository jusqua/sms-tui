#include "tools.h"

// module of a int
long modby(long nom, long dnom)
{
  return (nom - (nom / dnom) * dnom);
}

// ctype lib isdigit function clone
bool _isdigit(char ch)
{
  // check ascii code
  return (ch > 47 && ch < 58) || ch == '-' ? true : false;
}

// string lib strlen function clone
int _strlen(char *string)
{
  int length = 0;
  while (string[length++] != '\0');
  return --length;
}

// abs function clone
long _abs(long number)
{
  return number < 0 ? -number : number;
}

void string_input(char *string, short size)
{
  // count
  short c;
  // input buffer
  char buffer[BSIZE];
  // user input signal
  printf("%s", INPUT);
  // get input
  fgets(buffer, BSIZE, stdin);
  // tranfer to pointer
  for (c = 0; c < size && buffer[c] != '\n'; c++)
  {
    string[c] = buffer[c];
  }
  string[c] = '\0';
}

long int_input(void)
{
  // count
  short c;
  // input buffer
  char buffer[BSIZE];
  // do while is a digit
  do
  {
    // user input signal
    printf("%s", INPUT);
    // get input
    fgets(buffer, BSIZE, stdin);
    // verify process
    for (c = 0; buffer[c] != '\n'; c++)
    {
      // if not a digit
      if (!_isdigit(buffer[c]))
      {
        // end
        break;
      }
    }
  }
  while(buffer[c] != '\n' || c == 0);
  // return
  return atol(buffer);
}

char char_input(void)
{
  // input buffer
  char buffer[BSIZE];
  // do while until the second character is not a newline
  do
  {
    // user input signal
    printf("%s", INPUT);
    // get input
    fgets(buffer, BSIZE, stdin);
  }
  while(buffer[1] != '\n');
  // return
  return buffer[0];
}

float float_input(void)
{
  // count
  short c = 0;
  // input buffer
  char buffer[BSIZE];
  // do while is a digit or dot
  do
  {
    // user input signal
    printf("%s", INPUT);
    // get input
    fgets(buffer, BSIZE, stdin);
    // verify process
    for (c = 0; buffer[c] != '\n'; c++)
    {
      // if is not a digit or dot
      if (!_isdigit(buffer[c]) && buffer[c] != '.')
      {
        // end
        break;
      }
    }
  }
  while(buffer[c] != '\n' || c == 0);
  // return
  return atof(buffer);
}

// "Pauses" the program
void pause(void)
{
  // input buffer
  char buffer[BSIZE];
  // show what user need to do
  puts("Aperte [ENTER] para continuar...");
  // get input
  fgets(buffer, BSIZE, stdin);
}

// "Clear" terminal screen
void clean(void)
{
  // Get system
  #if defined (_WIN32) || defined (_WIN64) // Windows
    system("cls");
  #else // Linux or Mac
    system("clear");
  #endif
}

// Show a formated "error" mensage
void warning(char *text)
{
  short text_lenth = _strlen(text);
  short spaces = (((WSIZE - text_lenth) - 8) / 2);
  printf("\n[!] ");
  for (short c = 0; c < spaces; c++)
  {
    printf(" ");
  }
  printf("%s", text);
  for (short c = 0; c < spaces; c++)
  {
    printf(" ");
  }
  if (text_lenth % 2)
  {
    printf(" ");
  }
  printf(" [!]\n\n");
  pause();
}

// Show a formated title
void title(char *text)
{
  short text_lenth = _strlen(text);
  short spaces = ((WSIZE - text_lenth) / 2);
  for (short c = 0; c < spaces; c++)
  {
    printf("=");
  }
  printf("%s", text);
  for (short c = 0; c < spaces; c++)
  {
    printf("=");
  }
  if (text_lenth % 2)
  {
    printf("=");
  }
  puts("\n");
}
