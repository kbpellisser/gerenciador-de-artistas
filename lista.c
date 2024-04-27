//Antes de compilar o programa, execute no terminal o comando ---> chcp 65001 ("chcp 65001" serve para definir a página de código UTF-8 Isso é necessário para que caracteres especiais, como acentos, sejam exibidos corretamente no console do Windows, caso a biblioteca locale.h não esteja sendo incluído corretamente.)
//Se o terminal Visual Studio Code compilar o código inteiro ---> abra o (https://replit.com/), crie um ambiente main.c , um arquivo chamado artistas.txt e dê start.

#include <stdio.h> //Biblioteca padrão de entrada e saída
#include <string.h>  //Biblioteca para manipulação de strings, também conhecidos como array de chars
#include <stdlib.h> //Biblioteca onde estão localizadas as funções referentes às operações nas quais os mecanismos operam em função da entrada e da saída padrão, bem como em arquivos também.
#include <unistd.h> //Biblioteca padrão em sistemas operacionais Unix e Unix-like, como Linux e macOS. Ela fornece acesso a uma série de funções de sistema relacionadas à interação com o sistema operacional.
#include <locale.h> //Biblioteca que define a localidade para o padrão, que é a página de código ANSI padrão do usuário obtida do sistema operacional.

// Define o máximo de uma linha do arquivo
#define MAX_LINHA 500

// Definição da estrutura = Artista

typedef struct Artista
{
    char nome[100];
    char generoMusical[200];
    char localNascimento[800];
    char albuns[800];

} Artista;

// Definição da estrutura = ListaArtistas

typedef struct ListaArtistas
{
    Artista *artistas;
    int numElementos;
} ListaArtistas;

// Função de comparação para ordenar = artistas por nome

int comparar_Artistas(const void *a, const void *b)
{
    return strcmp(((Artista *)a)->nome, ((Artista *)b)->nome);
}

// Função para inicializar = lista de artistas

void inicializar_Lista(ListaArtistas *lista)
{
    lista->artistas = NULL;
    lista->numElementos = 0;
}

// Função para adicionar = artista à lista

void adicionar_Artista(ListaArtistas *lista, const Artista *artista)
{
    lista->numElementos++;
    lista->artistas = (Artista *)realloc(lista->artistas, lista->numElementos * sizeof(Artista));
    if (lista->artistas == NULL)
    {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    lista->artistas[lista->numElementos - 1] = *artista;
}

// Função para imprimir todos contidos na lista

void imprimir_Lista(ListaArtistas *lista)
{
    for (int i = 0; i < lista->numElementos; i++)
    {
        printf("Nome: %s\n", lista->artistas[i].nome);
        printf("Gênero Musical: %s\n", lista->artistas[i].generoMusical);
        printf("Local de Nascimento: %s\n", lista->artistas[i].localNascimento);
        printf("Álbuns:\n%s\n", lista->artistas[i].albuns);
        printf("==================\n");
    }
}

// Função para liberar a memória ocupada pela lista de artistas
void Liberar_Lista(ListaArtistas *lista)
{
    free(lista->artistas);
    lista->artistas = NULL;
    lista->numElementos = 0;
}

// Função de busca binária por nome de artista
int busca_Binaria_PorNome(ListaArtistas *lista, char *nome)
{
    int esquerda = 0;
    int direita = lista->numElementos - 1;

    while (esquerda <= direita)
    {
        int meio = esquerda + (direita - esquerda) / 2;
        int cmp = strcmp(lista->artistas[meio].nome, nome);

        if (cmp == 0)
        {
            return meio; // Retornará quando o artista for encontrado
        }
        else if (cmp < 0)
        {
            esquerda = meio + 1; // Procura à direita do meio
        }
        else
        {
            direita = meio - 1; // Procura à esquerda do meio
        }
    }

    return -1; // Retornará quando o artista não for encontrado
}

// Função para inserir um artista de forma ordenada na lista e atualizar o arquivo
void inserir_Artista_Ordenado(ListaArtistas *lista, const Artista *artista, const char *arquivo)
{
    int posicaoInsercao = 0;

    while (posicaoInsercao < lista->numElementos &&
           strcmp(artista->nome, lista->artistas[posicaoInsercao].nome) > 0)
    {
        posicaoInsercao++;
    }

    lista->numElementos++;
    lista->artistas = (Artista *)realloc(lista->artistas, lista->numElementos * sizeof(Artista));

    if (lista->artistas == NULL)
    {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    for (int i = lista->numElementos - 1; i > posicaoInsercao; i--)
    {
        lista->artistas[i] = lista->artistas[i - 1];
    }

    lista->artistas[posicaoInsercao] = *artista;

    FILE *albumArquivo = fopen(arquivo, "w");
    if (albumArquivo == NULL)
    {
        printf("Erro ao abrir arquivo para escrita.\n");
        exit(1);
    }

    for (int i = 0; i < lista->numElementos; i++)
    {
        fprintf(albumArquivo, "%s\n%s\n%s\n%s\n==========\n",
                lista->artistas[i].nome, lista->artistas[i].generoMusical,
                lista->artistas[i].localNascimento, lista->artistas[i].albuns);
    }

    fclose(albumArquivo);
}

void remover_Artista(ListaArtistas *lista, int posicao)
{
    if (posicao < 0 || posicao >= lista->numElementos)
    {
        printf("Posição inválida.\n");
        return;
    }

    FILE *arquivoTemporario = fopen("temp.txt", "w");
    if (arquivoTemporario == NULL)
    {
        printf("Erro ao criar arquivo temporário.\n");
        exit(1);
    }

    FILE *arquivo = fopen("artistas.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo para remoção de artista.\n");
        fclose(arquivoTemporario);
        exit(1);
    }

    int linha = 0;
    char linhaTexto[MAX_LINHA];
    Artista artista;

    while (fgets(linhaTexto, MAX_LINHA, arquivo))
    {
        linhaTexto[strcspn(linhaTexto, "\n")] = '\0';

        if (strcmp(linhaTexto, "==========") == 0)
        {
            if (linha >= 4)
            {
                if (linha / 4 != posicao)
                {
                    fprintf(arquivoTemporario, "%s\n%s\n%s\n%s\n==========\n",
                            artista.nome, artista.generoMusical, artista.localNascimento, artista.albuns);
                }
            }
            linha = 0;
        }
        else
        {
            switch (linha)
            {
            case 0:
                strcpy(artista.nome, linhaTexto);
                break;
            case 1:
                strcpy(artista.generoMusical, linhaTexto);
                break;
            case 2:
                strcpy(artista.localNascimento, linhaTexto);
                break;
            case 3:
                strcpy(artista.albuns, linhaTexto);
                break;
            }
            linha++;
        }
    }

    fclose(arquivo);
    fclose(arquivoTemporario);

    remove("artistas.txt");
    rename("temp.txt", "artistas.txt");

    for (int i = posicao; i < lista->numElementos - 1; i++)
    {
        lista->artistas[i] = lista->artistas[i + 1];
    }

    lista->numElementos--;
    lista->artistas = (Artista *)realloc(lista->artistas, lista->numElementos * sizeof(Artista));
    if (lista->numElementos == 0)
    {
        free(lista->artistas);
        lista->artistas = NULL;
    }
}

void editar_Artista(ListaArtistas *lista, int posicao, const Artista *novoArtista)
{
    if (posicao < 0 || posicao >= lista->numElementos)
    {
        printf("Posição inválida.\n");
        return;
    }

    FILE *arquivoTemporario = fopen("temp.txt", "w");
    if (arquivoTemporario == NULL)
    {
        printf("Erro ao criar arquivo temporário.\n");
        exit(1);
    }

    FILE *arquivo = fopen("artistas.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo para edição de artista.\n");
        fclose(arquivoTemporario);
        exit(1);
    }

    int linha = 0;
    char linhaTexto[MAX_LINHA];
    Artista artista;

    while (fgets(linhaTexto, MAX_LINHA, arquivo))
    {
        linhaTexto[strcspn(linhaTexto, "\n")] = '\0';

        if (strcmp(linhaTexto, "==========") == 0)
        {
            if (linha >= 4)
            {
                if (linha / 4 != posicao)
                {
                    fprintf(arquivoTemporario, "%s\n%s\n%s\n%s\n==========\n",
                            artista.nome, artista.generoMusical, artista.localNascimento, artista.albuns);
                }
                else
                {
                    fprintf(arquivoTemporario, "%s\n%s\n%s\n%s\n==========\n",
                            novoArtista->nome, novoArtista->generoMusical,
                            novoArtista->localNascimento, novoArtista->albuns);
                }
            }
            linha = 0;
        }
        else
        {
            switch (linha)
            {
            case 0:
                strcpy(artista.nome, linhaTexto);
                break;
            case 1:
                strcpy(artista.generoMusical, linhaTexto);
                break;
            case 2:
                strcpy(artista.localNascimento, linhaTexto);
                break;
            case 3:
                strcpy(artista.albuns, linhaTexto);
                break;
            }
            linha++;
        }
    }

    fclose(arquivo);
    fclose(arquivoTemporario);

    remove("artistas.txt");
    rename("temp.txt", "artistas.txt");

    lista->artistas[posicao] = *novoArtista;
}

int busca_Sequencial_PorAlbum(ListaArtistas *lista, char *album)
{
    for (int i = 0; i < lista->numElementos; i++)
    {
        if (strstr(lista->artistas[i].albuns, album) != NULL)
        {
            return i;
        }
    }
    return -1;
}

int main()
{

    printf("Álbuns Artisticos: \n");

    ListaArtistas lista;
    inicializar_Lista(&lista);

    FILE *arquivo = fopen("artistas.txt", "r");

    if (arquivo == NULL)
    {
        printf("---Não foi possível abrir o arquivo!---\n");
        exit(1);
    }

    char linhaTexto[MAX_LINHA];
    Artista artista;
    int linha = 0;

    while (fgets(linhaTexto, MAX_LINHA, arquivo))
    {
        linhaTexto[strcspn(linhaTexto, "\n")] = '\0';

        if (strcmp(linhaTexto, "==========") == 0)
        {
            if (linha >= 4)
            {
                adicionar_Artista(&lista, &artista);
            }
            linha = 0;
        }
        else
        {
            switch (linha)
            {
            case 0:
                strcpy(artista.nome, linhaTexto);
                break;
            case 1:
                strcpy(artista.generoMusical, linhaTexto);
                break;
            case 2:
                strcpy(artista.localNascimento, linhaTexto);
                break;
            case 3:
                strcpy(artista.albuns, linhaTexto);
                break;
            default:
                strcat(artista.albuns, "\n");
                strcat(artista.albuns, linhaTexto);
                break;
            }
            linha++;
        }
    }
    fclose(arquivo);

    printf("---- Informações dos Artistas ----\n");

    qsort(lista.artistas, lista.numElementos, sizeof(Artista), comparar_Artistas);

    imprimir_Lista(&lista);

    int opcao;
    char nomeBusca[100];
    char albumBusca[100];
    int resultadoBusca;
    char novoNomeArtista[100];
    char novoGeneroMusical[200];
    char novoLocalNascimento[800];
    int posicaoBusca;

    do
    {
        printf("\nMenu:\n");
        printf("1 - Buscar artista por nome\n");
        printf("2 - Buscar artista por álbum\n");
        printf("3 - Adicionar artista\n");
        printf("4 - Editar artista\n");
        printf("5 - Remover artista\n");
        printf("6 - Sair\n");
        printf("\nEscolha uma das opções acima e digite um número: ");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("\n\nDigite o nome do artista que você deseja encontrar: ");
            getchar();
            fgets(nomeBusca, sizeof(nomeBusca), stdin);
            nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

            posicaoBusca = busca_Binaria_PorNome(&lista, nomeBusca);

            if (posicaoBusca != -1)
            {
                printf("\n----Artista encontrado:----\n\n");
                printf("Nome: %s\n", lista.artistas[posicaoBusca].nome);
                printf("Gênero Musical: %s\n", lista.artistas[posicaoBusca].generoMusical);
                printf("Local de Nascimento: %s\n", lista.artistas[posicaoBusca].localNascimento);
                printf("Álbuns:\n%s\n", lista.artistas[posicaoBusca].albuns);
            }
            else
            {
                printf("----Artista não encontrado.----\n");
            }
            break;

        case 2:
            printf("\n\nDigite o nome do álbum que você deseja encontrar: ");
            getchar();
            fgets(albumBusca, sizeof(albumBusca), stdin);
            albumBusca[strcspn(albumBusca, "\n")] = '\0';

            resultadoBusca = busca_Sequencial_PorAlbum(&lista, albumBusca);

            if (resultadoBusca != -1)
            {
                printf("\nÁlbum encontrado no artista: \n");
                printf("Nome: %s\n", lista.artistas[resultadoBusca].nome);
                printf("Álbuns:\n%s\n", lista.artistas[resultadoBusca].albuns);
            }
            else
            {
                printf("\nÁlbum não encontrado em nenhum artista. \n");
            }
            break;

        case 3:
            printf("\n\nDigite o nome do novo artista: ");
            getchar();
            fgets(artista.nome, sizeof(artista.nome), stdin);
            artista.nome[strcspn(artista.nome, "\n")] = '\0';

            printf("Digite o gênero musical: ");
            fgets(artista.generoMusical, sizeof(artista.generoMusical), stdin);
            artista.generoMusical[strcspn(artista.generoMusical, "\n")] = '\0';

            printf("Digite o local de nascimento: ");
            fgets(artista.localNascimento, sizeof(artista.localNascimento), stdin);
            artista.localNascimento[strcspn(artista.localNascimento, "\n")] = '\0';

            printf("Digite os álbuns separados por vírgula: ");
            fgets(artista.albuns, sizeof(artista.albuns), stdin);
            artista.albuns[strcspn(artista.albuns, "\n")] = '\0';

            inserir_Artista_Ordenado(&lista, &artista, "artistas.txt");
            printf("\nArtista adicionado com sucesso!\n");
            break;

        case 4:
            printf("\n\nDigite o nome do artista a ser editado: ");
            getchar();
            fgets(nomeBusca, sizeof(nomeBusca), stdin);
            nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

            posicaoBusca = busca_Binaria_PorNome(&lista, nomeBusca);

            if (posicaoBusca != -1)
            {
                printf("\nDigite o novo nome do artista: ");
                fgets(novoNomeArtista, sizeof(novoNomeArtista), stdin);
                novoNomeArtista[strcspn(novoNomeArtista, "\n")] = '\0';

                printf("Digite o novo gênero musical: ");
                fgets(novoGeneroMusical, sizeof(novoGeneroMusical), stdin);
                novoGeneroMusical[strcspn(novoGeneroMusical, "\n")] = '\0';

                printf("Digite o novo local de nascimento: ");
                fgets(novoLocalNascimento, sizeof(novoLocalNascimento), stdin);
                novoLocalNascimento[strcspn(novoLocalNascimento, "\n")] = '\0';

                strcpy(artista.nome, novoNomeArtista);
                strcpy(artista.generoMusical, novoGeneroMusical);
                strcpy(artista.localNascimento, novoLocalNascimento);

                editar_Artista(&lista, posicaoBusca, &artista);
                printf("\nArtista editado com sucesso!\n");
            }
            else
            {
                printf("\nArtista não encontrado para ser editado.\n");
            }
            break;

        case 5:
            printf("\n\nDigite o nome do artista a ser removido: ");
            getchar();
            fgets(nomeBusca, sizeof(nomeBusca), stdin);
            nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

            posicaoBusca = busca_Binaria_PorNome(&lista, nomeBusca);

            if (posicaoBusca != -1)
            {
                remover_Artista(&lista, posicaoBusca);
                printf("\nArtista removido com sucesso!\n");
            }
            else
            {
                printf("\nArtista não encontrado para remoção.\n");
            }
            break;

        case 6:
            printf("\nSaindo...\n");
            break;

        default:
            printf("\nOpção inválida. Escolha uma opção de 1 a 6.\n");
            break;
        }

    } while (opcao != 6);

    Liberar_Lista(&lista);

    return 0;
}
