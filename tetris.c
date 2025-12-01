#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// --- CONSTANTES ---
#define CAPACIDADE_FILA 5

// --- ESTRUTURAS DE DADOS ---

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome;  // Tipo da peça: 'I', 'O', 'T', 'L', etc.
    int id;     // Identificador único
} Peca;

// Estrutura da Fila Circular
typedef struct {
    Peca elementos[CAPACIDADE_FILA];
    int frente;      // Índice do início da fila
    int tras;        // Índice do final da fila (próxima posição livre)
    int tamanho;     // Quantidade atual de peças
    int proximoId;   // Contador para gerar IDs únicos
} FilaCircular;

// --- PROTÓTIPOS DAS FUNÇÕES ---
void inicializarFila(FilaCircular *fila);
bool filaVazia(FilaCircular *fila);
bool filaCheia(FilaCircular *fila);
Peca gerarPeca(FilaCircular *fila);
void enqueue(FilaCircular *fila); // Inserir nova peça
void dequeue(FilaCircular *fila); // Jogar (remover) peça
void mostrarFila(FilaCircular *fila);

// --- FUNÇÃO PRINCIPAL ---
int main() {
    // 1. Inicializar gerador de números aleatórios
    srand(time(NULL));

    // 2. Inicializar a estrutura da fila
    FilaCircular fila;
    inicializarFila(&fila);

    // 3. Preencher a fila inicial com peças (Requisito: Inicializar com número fixo)
    printf("Inicializando o sistema...\n");
    for (int i = 0; i < CAPACIDADE_FILA; i++) {
        enqueue(&fila); // O enqueue já chama o gerarPeca internamente neste design simplificado
    }

    int opcao;
    bool executando = true;

    // 4. Loop do Menu Principal
    while (executando) {
        printf("\n========================================\n");
        printf("      TETRIS STACK - NIVEL BASICO       \n");
        printf("========================================\n");

        // Exibir estado atual (Requisito: Exibir após cada ação)
        mostrarFila(&fila);

        printf("\n--- Opcoes de Acao ---\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");

        // Leitura segura da opção
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); // Limpar buffer em caso de letra
            opcao = -1;
        }

        switch (opcao) {
            case 1:
                dequeue(&fila);
                break;
            case 2:
                enqueue(&fila);
                break;
            case 0:
                executando = false;
                printf("Encerrando o jogo. Ate logo!\n");
                break;
            default:
                printf("\n[!] Opcao invalida! Tente novamente.\n");
        }

        // Pausa para leitura (opcional, melhora usabilidade)
        if (executando) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n'); 
            getchar();
        }
        
        // Limpar tela (opcional, funciona em Windows/Linux de forma simples)
         system("cls || clear"); 
    }

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES ---

// Inicializa os ponteiros e contadores da fila
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = 0;
    fila->tamanho = 0;
    fila->proximoId = 1; // IDs começam em 1
}

// Verifica se a fila está vazia
bool filaVazia(FilaCircular *fila) {
    return (fila->tamanho == 0);
}

// Verifica se a fila está cheia
bool filaCheia(FilaCircular *fila) {
    return (fila->tamanho == CAPACIDADE_FILA);
}

// Gera uma peça com tipo aleatório e ID sequencial
Peca gerarPeca(FilaCircular *fila) {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    
    // Escolhe um índice aleatório de 0 a 6
    int indice = rand() % 7;
    
    novaPeca.nome = tipos[indice];
    novaPeca.id = fila->proximoId;
    
    fila->proximoId++; // Incrementa para a próxima peça ter ID diferente
    return novaPeca;
}

// Adiciona uma nova peça ao final da fila (Circular)
void enqueue(FilaCircular *fila) {
    if (filaCheia(fila)) {
        printf("\n[!] A fila esta cheia! Nao e possivel adicionar mais pecas.\n");
        return;
    }

    // Gera a peça automaticamente
    Peca novaPeca = gerarPeca(fila);

    // Insere na posição 'tras'
    fila->elementos[fila->tras] = novaPeca;

    // Atualiza o índice 'tras' circularmente
    // Ex: Se CAPACIDADE é 5 e tras é 4, (4+1)%5 = 0 (volta ao início)
    fila->tras = (fila->tras + 1) % CAPACIDADE_FILA;
    fila->tamanho++;

    printf("\n>>> Peca inserida: [%c %d]\n", novaPeca.nome, novaPeca.id);
}

// Remove a peça da frente da fila (Circular)
void dequeue(FilaCircular *fila) {
    if (filaVazia(fila)) {
        printf("\n[!] A fila esta vazia! Nao ha pecas para jogar.\n");
        return;
    }

    // Pega a peça da frente
    Peca pecaRemovida = fila->elementos[fila->frente];

    // Atualiza o índice 'frente' circularmente
    fila->frente = (fila->frente + 1) % CAPACIDADE_FILA;
    fila->tamanho--;

    printf("\n>>> Peca jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
}

// Exibe a fila no formato solicitado
void mostrarFila(FilaCircular *fila) {
    printf("Fila de pecas: ");
    
    if (filaVazia(fila)) {
        printf("[ Vazia ]");
    } else {
        int indice = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            // Acessa o elemento atual
            Peca p = fila->elementos[indice];
            
            // Imprime no formato [Tipo ID]
            printf("[%c %d] ", p.nome, p.id);
            
            // Avança o índice circularmente apenas para visualização
            indice = (indice + 1) % CAPACIDADE_FILA;
        }
    }
    printf("\n(Capacidade: %d/%d)\n", fila->tamanho, CAPACIDADE_FILA);
}