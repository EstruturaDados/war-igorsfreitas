#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- CONSTANTES E ESTRUTURAS ---
#define TAMANHO_MISSAO 150
#define MAX_CORES_UNICAS 20 // Limite de cores diferentes que o sistema vai rastrear

typedef struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// --- PROTÓTIPOS DAS FUNÇÕES ---
void limparBuffer();
void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(const Territorio* mapa, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, const char* missoes[], int totalMissoes);
void exibirMissao(const char* missao, const char* corDoJogador);
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* corDoJogador);
void liberarMemoria(Territorio* mapa, char* missao, char** listaMissoes, int totalMissoes);

int main() {
    srand(time(NULL));

    // --- CONFIGURAÇÃO INICIAL DO JOGO ---
    int quantidadeTerritorios;
    printf("Quantos territorios o mundo tera? ");
    scanf("%d", &quantidadeTerritorios);
    limparBuffer();

    Territorio* mapaMundi = (Territorio*)malloc(quantidadeTerritorios * sizeof(Territorio));
    char* missaoJogador = (char*)malloc(TAMANHO_MISSAO * sizeof(char));

    if (mapaMundi == NULL || missaoJogador == NULL) {
        printf("Erro fatal: Falha ao alocar memoria.\n");
        free(mapaMundi);
        free(missaoJogador);
        return 1;
    }

    // 1. Cadastrar os territórios para saber quais exércitos existem.
    cadastrarTerritorios(mapaMundi, quantidadeTerritorios);

    // 2. Identificar todas as cores únicas no mapa.
    char coresUnicas[MAX_CORES_UNICAS][10];
    int numCoresUnicas = 0;
    for (int i = 0; i < quantidadeTerritorios; i++) {
        int corNova = 1;
        for (int j = 0; j < numCoresUnicas; j++) {
            if (strcmp(mapaMundi[i].cor, coresUnicas[j]) == 0) {
                corNova = 0; break;
            }
        }
        if (corNova && numCoresUnicas < MAX_CORES_UNICAS) {
            strcpy(coresUnicas[numCoresUnicas], mapaMundi[i].cor);
            numCoresUnicas++;
        }
    }

    // 3. Sortear aleatoriamente a cor do jogador dentre as cores únicas existentes.
    char corDoJogador[10];
    if (numCoresUnicas > 0) {
        int indiceCorSorteada = rand() % numCoresUnicas;
        strcpy(corDoJogador, coresUnicas[indiceCorSorteada]);
        printf("\n=========================================================\n");
        printf("SORTEIO REALIZADO! Voce comandara o exercito %s.\n", corDoJogador);
        printf("=========================================================\n");
    } else {
        printf("Nenhuma cor encontrada. Encerrando o jogo.\n");
        free(mapaMundi); free(missaoJogador); return 1;
    }

    // 4. Gerar a lista de missões com base nos exércitos inimigos.
    int totalMissoesPossiveis = 0;
    for(int i = 0; i < numCoresUnicas; i++){
        if(strcmp(coresUnicas[i], corDoJogador) != 0) totalMissoesPossiveis++;
    }
    totalMissoesPossiveis++; // +1 para a missão genérica
    
    char** missoesPossiveis = (char**)malloc(totalMissoesPossiveis * sizeof(char*));
    int missaoIdx = 0;
    for(int i = 0; i < numCoresUnicas; i++) {
        if(strcmp(coresUnicas[i], corDoJogador) != 0) {
            missoesPossiveis[missaoIdx] = (char*)malloc(TAMANHO_MISSAO * sizeof(char));
            sprintf(missoesPossiveis[missaoIdx], "Destruir o exercito %s.", coresUnicas[i]);
            missaoIdx++;
        }
    }
    missoesPossiveis[missaoIdx] = (char*)malloc(TAMANHO_MISSAO * sizeof(char));
    strcpy(missoesPossiveis[missaoIdx], "Conquistar todos os territorios do mapa.");

    // 5. Sortear uma missão da lista relevante.
    atribuirMissao(missaoJogador, (const char**)missoesPossiveis, totalMissoesPossiveis);

    // --- LOOP PRINCIPAL DO JOGO ---
    int vitoria = 0;
    while (!vitoria) {
        exibirMapa(mapaMundi, quantidadeTerritorios);
        exibirMissao(missaoJogador, corDoJogador);

        printf("--- MENU DE ACOES ---\n1 - Atacar\n2 - Verificar Missao\n0 - Sair\n");
        printf("Escolha sua acao: ");
        int escolhaAcao;
        scanf("%d", &escolhaAcao);
        limparBuffer();

        switch (escolhaAcao) {
            case 1: {
                // ... (Lógica de ataque, agora usando a variável corDoJogador para validação) ...
                printf("\n--- FASE DE ATAQUE ---\n");
                printf("Escolha o territorio atacante (1 a %d): ", quantidadeTerritorios);
                int atacanteIdx; scanf("%d", &atacanteIdx);
                printf("Escolha o territorio defensor (1 a %d): ", quantidadeTerritorios);
                int defensorIdx; scanf("%d", &defensorIdx);
                limparBuffer();
                atacanteIdx--; defensorIdx--;

                if (atacanteIdx < 0 || atacanteIdx >= quantidadeTerritorios || defensorIdx < 0 || defensorIdx >= quantidadeTerritorios) {
                    printf("\nOpcao invalida! Territorio nao existe.\n");
                } else if (strcmp(mapaMundi[atacanteIdx].cor, corDoJogador) != 0) { // <-- MUDANÇA AQUI
                    printf("\nAcao invalida! Voce so pode atacar com o exercito %s.\n", corDoJogador);
                } else if (atacanteIdx == defensorIdx) {
                    printf("\nAcao invalida! Um territorio nao pode atacar a si mesmo.\n");
                } else if (mapaMundi[atacanteIdx].tropas <= 1) {
                    printf("\nAcao invalida! O territorio atacante precisa ter mais de 1 tropa.\n");
                } else if (strcmp(mapaMundi[atacanteIdx].cor, mapaMundi[defensorIdx].cor) == 0) {
                    printf("\nAcao invalida! Voce nao pode atacar um territorio do seu proprio exercito.\n");
                } else {
                    atacar(&mapaMundi[atacanteIdx], &mapaMundi[defensorIdx]);
                    if (verificarMissao(missaoJogador, mapaMundi, quantidadeTerritorios, corDoJogador)) { // <-- MUDANÇA AQUI
                        vitoria = 1;
                        printf("\n\n****************************************\n");
                        printf("VITORIA! Voce cumpriu sua missao!\n");
                        printf("****************************************\n");
                        exibirMapa(mapaMundi, quantidadeTerritorios);
                    }
                }
                break;
            }
            case 2:
                exibirMissao(missaoJogador, corDoJogador);
                break;
            case 0:
                vitoria = 1;
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        if (!vitoria) {
            printf("\nPressione Enter para continuar...");
            getchar();
        }
    }

    liberarMemoria(mapaMundi, missaoJogador, missoesPossiveis, totalMissoesPossiveis);
    printf("\nJogo finalizado e memoria liberada com sucesso.\n");

    return 0;
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    printf("\n--- CADASTRO DE TERRITORIOS ---\n");
    for (int i = 0; i < quantidade; i++) {
        printf("--- Territorio %d ---\n", i + 1);
        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = 0;
        printf("Cor do Exercito: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = 0;
        printf("Numero de Tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();
        printf("\n");
    }
}

void exibirMapa(const Territorio* mapa, int quantidade) {
    printf("\n\n====================== MAPA DO MUNDO ======================\n");
    for (int i = 0; i < quantidade; i++) {
        printf("%d. %-10s \t(Exercito: %-10s, Tropas: %d)\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=========================================================\n");
}

void atribuirMissao(char* destino, const char* missoes[], int totalMissoes) {
    if (totalMissoes > 0) {
        int indiceSorteado = rand() % totalMissoes;
        strcpy(destino, missoes[indiceSorteado]);
    } else {
        strcpy(destino, "Nenhuma missao disponivel.");
    }
}

// Assinatura da função alterada para receber a cor do jogador
void exibirMissao(const char* missao, const char* corDoJogador) {
    printf("\n--- SUA MISSAO (Exercito %s) ---\n", corDoJogador);
    printf("%s\n", missao);
}

void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n--- RESULTADO DA BATALHA ---\n");
    printf("Ataque (%s): %d | Defesa (%s): %d\n", atacante->nome, dadoAtacante, defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("VITORIA DO ATAQUE! O territorio %s foi conquistado.\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas < 1) tropasTransferidas = 1;
        if (atacante->tropas - tropasTransferidas < 1) tropasTransferidas = atacante->tropas - 1;

        atacante->tropas -= tropasTransferidas;
        defensor->tropas = tropasTransferidas;
    } else {
        printf("VITORIA DA DEFESA! O atacante perdeu 1 tropa.\n");
        atacante->tropas--;
    }
}

// Assinatura da função alterada para receber a cor do jogador
int verificarMissao(const char* missao, const Territorio* mapa, int tamanho, const char* corDoJogador) {
    char corAlvo[20];
    if (sscanf(missao, "Destruir o exercito %s", corAlvo) == 1) {
        corAlvo[strcspn(corAlvo, ".")] = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corAlvo) == 0) return 0;
        }
        return 1;
    }

    if (strstr(missao, "Conquistar todos os territorios") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corDoJogador) != 0) return 0; // <-- MUDANÇA AQUI
        }
        return 1;
    }
    return 0;
}

void liberarMemoria(Territorio* mapa, char* missao, char** listaMissoes, int totalMissoes) {
    if (mapa != NULL) free(mapa);
    if (missao != NULL) free(missao);
    if (listaMissoes != NULL) {
        for (int i = 0; i < totalMissoes; i++) {
            if (listaMissoes[i] != NULL) {
                free(listaMissoes[i]);
            }
        }
        free(listaMissoes);
    }
}