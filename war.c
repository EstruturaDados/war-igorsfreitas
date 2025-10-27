#include <stdio.h>
#include <string.h> // Necessário para a função strcspn

// Define uma estrutura para armazenar os dados de um território
struct Territorio {
    char nome[50];
    char corDoExercito[30];
    int numeroDeTropas;
};

int main() {
    // Cria um array para armazenar os 5 territórios
    struct Territorio territorios[5];
    int i; // Variável de controle do laço

    // Imprime o cabeçalho inicial
    printf("=========================================================\n\n");
    printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo.\n\n");

    // Laço para cadastrar cada um dos 5 territórios
    for (i = 0; i < 5; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        // Pede e armazena o nome do território
        printf("Nome do Territorio: ");
        // Usamos fgets para ler a linha inteira, permitindo nomes com espaços
        fgets(territorios[i].nome, 50, stdin);
        // Remove o caractere de nova linha (\n) que o fgets captura
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;

        // Pede e armazena a cor do exército
        printf("Cor do Exercito (ex: Azul, Verde): ");
        fgets(territorios[i].corDoExercito, 30, stdin);
        // Remove o caractere de nova linha (\n)
        territorios[i].corDoExercito[strcspn(territorios[i].corDoExercito, "\n")] = 0;

        // Pede e armazena o número de tropas
        printf("Numero de Tropas: ");
        scanf("%d", &territorios[i].numeroDeTropas);

        // **Importante:** Limpa o buffer do teclado.
        // O scanf deixa um "\n" no buffer após ler o número.
        // Este código consome esse "\n" para que o próximo fgets funcione corretamente.
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("\n"); // Adiciona uma linha em branco para separar os cadastros
    }

    // Seção Bônus: Mostra todos os territórios que foram cadastrados
    printf("=========================================================\n\n");
    printf("Resumo dos territorios cadastrados:\n\n");
    for (i = 0; i < 5; i++) {
        printf("--- Territorio %d ---\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor: %s\n", territorios[i].corDoExercito);
        printf("Tropas: %d\n\n", territorios[i].numeroDeTropas);
    }

    return 0;
}