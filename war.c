#include <stdio.h>
#include <string.h>

struct Territorio {
    char nome[50];
    char corDoExercito[30];
    int numeroDeTropas;
};

int main() {
    
    struct Territorio territorios[5];
    int i;

    printf("=========================================================\n\n");
    printf("Vamos cadastrar os 5 territorios iniciais do nosso mundo.\n\n");

    for (i = 0; i < 5; i++) {
        printf("--- Cadastrando Territorio %d ---\n", i + 1);

        printf("Nome do Territorio: ");
        fgets(territorios[i].nome, 50, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;

        printf("Cor do Exercito (ex: Azul, Verde): ");
        fgets(territorios[i].corDoExercito, 30, stdin);
        territorios[i].corDoExercito[strcspn(territorios[i].corDoExercito, "\n")] = 0;

        printf("Numero de Tropas: ");
        scanf("%d", &territorios[i].numeroDeTropas);
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("\n");
    }

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