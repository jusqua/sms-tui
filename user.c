#include "helper.h"

/* User functions */
// setup a new student registry
void new_registry(void)
{
  // check vacancies disponibility
  if (info.registries == STUDENTS)
  {
    warning("Limite de vagas alcançado");
    return;
  }
  // start
  clean();
  title("Cadastrar novo aluno");

  // setup memory for a new registry
  student *newone = (student*) calloc(1, sizeof(student));
  if (newone == NULL)
  {
    warning("Memória insulficiente");
    return;
  }

  // setup new registry
  info.registries++;
  newone->next = NULL;
  newone->registry = ++info.last_registry;
  newone->absence = -1;
  newone->media = -1;

  printf("Matrícula [%i%03i]\n\n", YEAR, info.last_registry);
  // get student name
  puts("Nome do aluno");
  puts("* Não utilize acentos");
  get_name(newone->name);

  // get student class
  puts("Código da turma");
  newone->class = _abs(int_input());

  // empty list check
  if (data_list == NULL)
  {
    data_list = newone;
  }
  else
  {
    student *temp = data_list;
    while (temp->next != NULL)
    {
      temp = temp->next;
    }
    temp->next = newone;
  }

  // end
  puts("Matrícula efetuada com sucesso!");
  save_data();
  sortdataby(info.sortby);
  // repeat process confimation, if have vacancies
  if (info.registries != STUDENTS)
  {
    char conf;
    // get confirmation
    puts("\nCadastrar um novo? [S]im/[n]ão");
    do
    {
      conf = char_input();
    }
    while(conf != 'S' && conf != 's' && conf != 'N' && conf != 'n');
    // if YES
    if (conf == 'S' || conf == 's')
    {
      new_registry();
    }
  }
  else
  {
    pause();
  }
}

// update a student registry
void update_registry(void)
{
  // check if have one registry at least
  if (registry_check())
  {
    return;
  }

  // setup variables
  student *temp;
  long choice;
  short search;

  // check if have outdated registries
  while (info.registries != info.updated_registries)
  {
    do
    {
      // start
      clean();
      title("Atualizar aluno cadastrado");
      select_registries(UPDATE);
      puts("* Selecione a matrícula ou [0] para voltar");
      choice = int_input();
      // end if choice is 0
      if (!choice)
      {
        return;
      }
      else if (choice > YEAR * 1000)
      {
        // setup search value
        search = modby(choice, YEAR * 1000);
      }
      else
      {
        search = 0;
      }
      temp = data_list;
      // serching registry
      while (temp->registry != search && temp->next != NULL)
      {
        temp = temp->next;
      }

      // check registry existence
      if (temp->registry != search)
      {
        warning("Matrícula não encontrada");
      }
      // check if registry is updated
      else if (temp->absence != -1)
      {
        warning("Matrícula já atualizada");
        search = 0;
      }
    }
    while (temp->registry != search);

    // get absence
    printf("Faltas [0 a %i]\n", LESSONS);
    do
    {
      temp->absence = int_input();
    }
    while (temp->absence < 0 || temp->absence > LESSONS);
    // set frequency
    temp->frequency = 100.0 - (((float)temp->absence * 100.0) / (float)LESSONS);

    // get grades
    temp->media = 0;
    for (short i = 0; i < GRADES; i++)
    {
      printf("Nota %i [0 a 10]\n", i + 1);
      do
      {
        temp->grade[i] = float_input();
      }
      while (temp->grade[i] < 0.0 || temp->grade[i] > 10.0);
      temp->media += temp->grade[i];
    }
    // set media
    temp->media /= (float)GRADES;

    // end
    puts("Matrícula atualizada com sucesso!");
    info.updated_registries++;
    save_data();
    sortdataby(info.sortby);
    pause();
  }
  // if the while breaks
  warning("Cadastros já atualizados");
  return;
}

// remove a student registry
void remove_registry(void)
{
  // check if have one registry at least
  if (registry_check())
  {
    return;
  }

  // variables
  student *actual;
  student *last;
  long choice;
  short search;
  char conf;

  // check if have registry yet
  while (data_list != NULL)
  {
    do
    {
      clean();
      title("Remover aluno cadastrado");
      select_registries(REMOVE);
      puts("* Selecione a matrícula ou [0] para voltar");
      choice = int_input();
      // Se escolha == 0 (Voltar)
      if (!choice)
      {
        return;
      }
      // setup search values
      search = modby(choice, YEAR * 1000);
      actual = data_list;
      last = NULL;
      // searching registry
      while (actual->registry != search && actual->next != NULL)
      {
        last = actual;
        actual = actual->next;
      }

      // check registry existence
      if (actual->registry != search)
      {
        warning("Matrícula não encontrada");
      }
      // if exist confirm remove
      else
      {
        puts("Você tem certeza? [S]im/[n]ão");
        do
        {
          conf = char_input();
        }
        while (conf != 'S' && conf != 's' && conf != 'N' && conf != 'n');
        // if do not have sure
        if (conf == 'N' || conf == 'n')
        {
          search = 0;
        }
      }
    }
    while (actual->registry != search);

    // configuring info struct
    if (actual->absence != -1)
    {
      info.updated_registries--;
    }
    info.registries--;
    // Remoção do usuário
    if (actual == data_list)
    {
      data_list = actual->next;
      free(actual);
    }
    else
    {
      last->next = actual->next;
      free(actual);
    }

    // end
    puts("Matrícula removida com sucesso!");
    save_data();
    pause();
  }
  // if while breaks
  warning("Nenhum cadastro existente");
}

// list registries of students
void list_registries(void)
{
  // check if have one registry at least
  if (registry_check())
  {
    return;
  }

  // variables
  short choice = NONE;
  char *text[] =
  {
    "Escolha uma opção de listagem",
    "Todos os cadastrados",
    "Aprovados",
    "Reprovados por frequência",
    "Reprovados por média"
  };

  // start
  do
  {
    clean();
    title("Listar alunos cadastrados");
    if (choice > BYGRADE || choice < NONE)
    {
      choice = NONE;
      puts("\n* Opção inválida\n");
    }
    select_registries(choice);
    printf("* %s\n", text[choice]);
    // Menu
    printf
    (
      "[%i] %s\n[%i] %s\n[%i] %s\n[%i] %s\n\n[0] Voltar\n",
      ALL, text[ALL],
      APPROVED, text[APPROVED],
      BYABSENCE, text[BYABSENCE],
      BYGRADE, text[BYGRADE]
    );
    // get choice
    choice = int_input();
  }
  while (choice);
}

// sort registries data
void sort_registries(void)
{
  // check if have one registry at least
  if (registry_check())
  {
    return;
  }

  // variables
  short choice = info.sortby;
  info.reverse = !info.reverse;
  char *sorting[] =
  {
    "crescente",
    "decrescente"
  };
  char *text[] =
  {
    "",
    "matrícula",
    "turma",
    "nome",
    "média",
    "frequência"
  };

  // start
  do
  {
    clean();
    title("Ordenar alunos cadastrados");
    if (choice > S_FREQUENCY || choice < S_REGISTRY)
    {
      choice = info.sortby;
    }
    else if (choice == info.sortby)
    {
      info.reverse = !info.reverse;
    }
    else
    {
    	info.reverse = false;
    }
    sortdataby(choice);
    info.sortby = choice;
    select_registries(ALL);
    printf("* por %s %s\n", text[choice], sorting[info.reverse]);

    // Menu
    printf
    (
      "[%i] %s\n[%i] %s\n[%i] %s\n[%i] %s\n[%i] %s\n\n[0] Voltar\n",
      S_REGISTRY, text[S_REGISTRY],
      S_CLASS, text[S_CLASS],
      S_NAME, text[S_NAME],
      S_MEDIA, text[S_MEDIA],
      S_FREQUENCY, text[S_FREQUENCY]
    );
    // get choice
    choice = int_input();
  }
  while (choice);
  save_data();
}

// remove database if exists
void remove_data(void)
{
  FILE* file = fopen(DBFILE, "r");
  short choice;
  // if not exist
  if (file == NULL)
  {
    warning("Nenhum backup encontrado");
    return;
  }
  fclose(file);

  // start
  clean();
  title("Deletar arquivo de backup");

  // get user sure
  puts("\nVocê tem certeza disso? [S]im/[n]ão");
  choice = char_input();
  // remove if sure
  if (choice == 'S' || choice == 's')
  {
    remove(DBFILE);
    warning("Arquivo de backup removido");
  }
}

// info and about
void about_info(void)
{
  clean();
  title("Informações do cabeçalho");
  printf("Último cadastro realizado   : %i%03i\n", YEAR, info.last_registry);
  printf("Número de alunos cadastrados: %hd\n", info.registries);
  printf("Número de alunos atualizados: %hd\n\n", info.updated_registries);
  title("Informações do desenvolvedor");
  puts
  (
    "Nome    : Ádrian Gama\n"
    "Nickname: jusqua\n"
    "Git Repo: https://gitlab.com/jusqua"
  );
  pause();
}
