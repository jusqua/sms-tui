#include "helper.h"

int main(void)
{
  // setup cmd to UTF-8 for DOS based system
  #if defined (_WIN32) || defined (_WIN64)
    system("chcp 65001");
  #endif
  // read data if exists
  if (!read_data())
  {
    return 1;
  }
  // some variables
  int choice;
  void (*option[])(void) =
  {
    new_registry,
    update_registry,
    remove_registry,
    list_registries,
    sort_registries,
    remove_data,
    about_info
  };
  // start
  do
  {
    clean();
    title("Menu de Opções");
    puts
    (
      "[1] Cadastrar novo aluno\n"
      "[2] Atualizar aluno cadastrado\n"
      "[3] Remover aluno cadastrado\n"
      "[4] Listar alunos cadastrados\n"
      "[5] Ordenar alunos cadastrados\n"
      "[6] Deletar arquivo de backup\n"
      "[7] Informações\n\n"
      "[0] Sair\n"
    );
    // get user input
    choice = int_input();
    if (choice > 0 && choice <= 7)
    {
      option[choice - 1]();
    }
    else if (choice != 0)
    {
      warning("Comando inválido");
    }
  }
  while (choice);
  // free memory before exit
  free_memory();
  return 0;
}
