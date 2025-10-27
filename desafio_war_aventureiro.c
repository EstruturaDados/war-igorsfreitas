#include <stdio.h>
#include <stdlib.h> // Para malloc, calloc, free, rand, srand
#include <string.h> // Para strcpy, strcmp, strcspn
#include <time.h>   // Para time()

// Define a estrutura para armazenar os dados de um território
// Usamos um typedef para facilitar a declaração de variáveis do tipo 'struct Territorio'
typedef struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- PROTÓTIPOS DAS FUNÇÕES ---
// Declara as funções antes de usá-las para que o compilador as conheça.
void limparBuffer();
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(const Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);


int main() {
    // Inicializa o gerador de números aleatórios com uma semente baseada no tempo atual.
    // Isso garante que os resultados de 'rand()' sejam diferentes a cada execução.
    srand(time(NULL));

    int quantidadeTerritorios;
    printf("=========================================================\n");
    printf("BEM-VINDO AO JOGO DE CONQUISTA DE TERRITORIOS!\n");
    printf("Quantos territorios o mundo tera? ");
    scanf("%d", &quantidadeTerritorios);
    limparBuffer(); // Essencial para limpar o '\n' deixado pelo scanf

    // Alocação dinâmica de memória para o mapa do mundo.
    // Aloca espaço para 'quantidadeTerritorios' structs do tipo Territorio.
    Territorio* mapaMundi = (Territorio*)malloc(quantidadeTerritorios * sizeof(Territorio));

    // Verifica se a alocação de memória foi bem-sucedida.
    if (mapaMundi == NULL) {
        printf("Erro fatal: Falha ao alocar memoria para o mapa.\n");
        return 1; // Encerra o programa com um código de erro.
    }

    // Chama a função para cadastrar os dados de cada território.
    cadastrarTerritorios(mapaMundi, quantidadeTerritorios);

    int atacanteIdx, defensorIdx;

    // Loop principal do jogo (fase de ataque).
    while (1) {
        exibirMapa(mapaMundi, quantidadeTerritorios);

        printf("--- FASE DE ATAQUE ---\n");
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", quantidadeTerritorios);
        scanf("%d", &atacanteIdx);
        limparBuffer();

        if (atacanteIdx == 0) {
            break; // Sai do loop se o usuário digitar 0.
        }

        printf("Escolha o territorio defensor (1 a %d): ", quantidadeTerritorios);
        scanf("%d", &defensorIdx);
        limparBuffer();

        // --- Validações da jogada ---
        // Converte a escolha do usuário (base 1) para o índice do array (base 0).
        atacanteIdx--;
        defensorIdx--;

        if (atacanteIdx < 0 || atacanteIdx >= quantidadeTerritorios ||
            defensorIdx < 0 || defensorIdx >= quantidadeTerritorios) {
            printf("\nOpcao invalida! Territorio nao existe.\n");
        } else if (atacanteIdx == defensorIdx) {
            printf("\nAcao invalida! Um territorio nao pode atacar a si mesmo.\n");
        } else if (mapaMundi[atacanteIdx].tropas <= 1) {
            printf("\nAcao invalida! O territorio atacante precisa ter mais de 1 tropa.\n");
        } else if (strcmp(mapaMundi[atacanteIdx].cor, mapaMundi[defensorIdx].cor) == 0) {
            printf("\nAcao invalida! Voce nao pode atacar um territorio do seu proprio exercito.\n");
        } else {
            // Se todas as validações passarem, executa o ataque.
            // Passa os endereços dos territórios específicos para a função.
            atacar(&mapaMundi[atacanteIdx], &mapaMundi[defensorIdx]);
        }

        printf("\nPressione Enter para continuar para o proximo turno...");
        getchar(); // Aguarda o usuário pressionar Enter.
    }

    // Libera a memória alocada dinamicamente antes de encerrar.
    liberarMemoria(mapaMundi);
    printf("\nJogo finalizado e memoria liberada com sucesso.\n");

    return 0; // Encerra o programa com sucesso.
}

/**
 * @brief Limpa o buffer de entrada do teclado.
 * Essencial para ser usado após um scanf para que o próximo fgets funcione corretamente.
 */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * @brief Preenche os dados dos territórios do mapa.
 * @param mapa Ponteiro para o início do vetor de territórios.
 * @param quantidade O número total de territórios a serem cadastrados.
 */
void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    printf("\nVamos cadastrar os %d territorios iniciais do nosso mundo.\n\n", quantidade);
    for (int i = 0; i < quantidade; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        printf("Nome do Territorio: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0; // Remove o '\n'

        printf("Cor do Exercito: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0; // Remove o '\n'

        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer(); // Limpa o buffer após o scanf
        printf("\n");
    }
}

/**
 * @brief Exibe o estado atual de todos os territórios no mapa.
 * @param mapa Ponteiro (const) para o mapa. 'const' indica que a função não alterará os dados.
 * @param quantidade O número total de territórios.
 */
void exibirMapa(const Territorio* mapa, int quantidade) {
    printf("\n\nMAPA DO MUNDO - ESTADO ATUAL\n");
    printf("=========================================================\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %s (Exercito %s, Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=========================================================\n\n");
}

/**
 * @brief Simula um ataque entre dois territórios.
 * @param atacante Ponteiro para o território atacante.
 * @param defensor Ponteiro para o território defensor.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Gera um número aleatório de 1 a 6 para cada lado.
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("O atacante %s rolou um dado e tirou: %d\n", atacante->nome, dadoAtacante);
    printf("O defensor %s rolou um dado e tirou: %d\n", defensor->nome, dadoDefensor);

    // Compara os resultados dos dados para determinar o vencedor.
    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA DO ATAQUE! O defensor foi conquistado.\n");

        // Regras de vitória conforme as instruções:
        // 1. O território defensor muda de cor.
        strcpy(defensor->cor, atacante->cor);

        // 2. Metade das tropas do atacante são transferidas para o novo território.
        int tropasTransferidas = atacante->tropas / 2;
        // Garante que o atacante mantenha pelo menos 1 tropa.
        if (atacante->tropas - tropasTransferidas < 1) {
            tropasTransferidas = atacante->tropas - 1;
        }

        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas;

    } else {
        printf("DERROTA DO ATAQUE! O atacante perdeu 1 tropa.\n");

        // Regra de derrota: o atacante perde 1 tropa.
        atacante->tropas--;
    }
}

/**
 * @brief Libera a memória que foi alocada dinamicamente para o mapa.
 * @param mapa Ponteiro para o mapa a ser liberado.
 */
void liberarMemoria(Territorio* mapa) {
    // 'free' libera o bloco de memória apontado por 'mapa'.
    // É boa prática verificar se o ponteiro não é nulo antes de liberá-lo.
    if (mapa != NULL) {
        free(mapa);
    }
}