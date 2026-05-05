#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Configurações do jogo
#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça de Tetris
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura para a Fila Circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente;
    int tras;
    int quantidade;
} FilaCircular;

// Estrutura para a Pilha de Reserva
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} PilhaReserva;

// Variável global para garantir IDs únicos
int contadorId = 0;

// Função para gerar uma peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L', 'S', 'Z', 'J'};
    Peca p;
    p.nome = tipos[rand() % 7];
    p.id = contadorId++;
    return p;
}

// --- Funções da Fila Circular ---

void inicializarFila(FilaCircular *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
    for (int i = 0; i < TAM_FILA; i++) {
        f->tras = (f->tras + 1) % TAM_FILA;
        f->itens[f->tras] = gerarPeca();
        f->quantidade++;
    }
}

Peca dequeue(FilaCircular *f) {
    Peca p = f->itens[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->quantidade--;
    
    // Repõe automaticamente a peça removida para manter a fila cheia
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = gerarPeca();
    f->quantidade++;
    
    return p;
}

// --- Funções da Pilha ---

void inicializarPilha(PilhaReserva *p) {
    p->topo = -1;
}

int empilhar(PilhaReserva *p, Peca peca) {
    if (p->topo < TAM_PILHA - 1) {
        p->itens[++(p->topo)] = peca;
        return 1; // Sucesso
    }
    return 0; // Pilha cheia
}

Peca desempilhar(PilhaReserva *p) {
    return p->itens[(p->topo)--];
}

// --- Interface e Exibição ---

void exibirEstado(FilaCircular *f, PilhaReserva *p) {
    printf("\n========================================\n");
    printf("ESTADO ATUAL\n");
    
    // Exibe Fila (frente para trás)
    printf("Fila de peças: ");
    for (int i = 0; i < TAM_FILA; i++) {
        int idx = (f->frente + i) % TAM_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    
    // Exibe Pilha (topo para base)
    printf("\nPilha de reserva (Topo -> Base): ");
    if (p->topo == -1) printf("[Vazia]");
    else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n========================================\n");
}

int main() {
    srand(time(NULL)); // Semente para números aleatórios
    
    FilaCircular fila;
    PilhaReserva pilha;
    int opcao = -1;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    while (opcao != 0) {
        exibirEstado(&fila, &pilha);
        printf("Opções de Ação:\n");
        printf("1 - Jogar peça (Fila)\n");
        printf("2 - Reservar peça (Fila -> Pilha)\n");
        printf("3 - Usar peça reservada (Pilha)\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca p = dequeue(&fila);
                printf("\n>> Jogou a peça [%c %d]!\n", p.nome, p.id);
                break;
            }
            case 2: {
                if (pilha.topo < TAM_PILHA - 1) {
                    Peca p = dequeue(&fila);
                    empilhar(&pilha, p);
                    printf("\n>> Peça [%c %d] movida para a reserva.\n", p.nome, p.id);
                } else {
                    printf("\n!! Erro: Reserva cheia!\n");
                }
                break;
            }
            case 3: {
                if (pilha.topo != -1) {
                    Peca p = desempilhar(&pilha);
                    printf("\n>> Usou peça da reserva: [%c %d]!\n", p.nome, p.id);
                } else {
                    printf("\n!! Erro: Reserva vazia!\n");
                }
                break;
            }
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    }

    return 0;
}