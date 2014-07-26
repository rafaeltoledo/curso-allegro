/*
 * Aplicativo de agenda
 * 2014 - Rafael Toledo
 */
#include <stdio.h>

// Não funciona para definir variáveis globais
//const int TAMANHO_AGENDA = 10;
#define TAMANHO_AGENDA 10

struct contato
{
    char nome[100];
    char telefone[100];
};

struct contato contatos[TAMANHO_AGENDA];
int tamanho = 0;

int exibe_menu()
{
    int opcao;
    puts("\tAGENDA:");
    puts("\n");
    puts(" 1. Adicionar Contato");
    puts(" 2. Listar Contatos");
    puts(" 3. Sair");
    puts("\n");
    printf("Opcao: ");
    scanf("%i", &opcao);

    return opcao;
}

void inserir_contato()
{
    struct contato meu_contato;

    printf("Nome: ");
    scanf("%s", meu_contato.nome);
    printf("Telefone: ");
    scanf("%s", meu_contato.telefone);

    contatos[tamanho] = meu_contato;
    tamanho++;
}

void listar_contatos()
{
    int indice;
    for (indice = 0; indice < tamanho; indice++)
    {
        printf("Nome: %s\n", contatos[indice].nome);
        printf("Telefone: %s\n\n",
                            contatos[indice].telefone);
    }
}

int main()
{
    int opcao;

    opcao = exibe_menu();
    while (opcao != 3)
    {
        if (opcao == 1)
        {
            // Inserir contato
            if (tamanho == TAMANHO_AGENDA)
            {
                puts("Agenda cheia!");
            }
            else
            {
                inserir_contato();
            }
        }
        else if (opcao == 2)
        {
            // Listar os contatos
            listar_contatos();
        }
        opcao = exibe_menu();
    }

    return 0;
}
