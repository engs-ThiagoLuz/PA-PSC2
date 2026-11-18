// Fila de execução
#pragma once
#include "tarefa.cpp"
#include "lista.cpp"
#include "pilha.cpp"

// Nó da fila: guarda apenas o id da tarefa 
struct NoFila {
    int idTarefa;
    NoFila* proximo;
};

// Fila FIFO com ponteiros para a frentee para trás.
struct FilaExecucao {
    NoFila* frente;
    NoFila* tras;
    int tamanho;
};

// Prepara uma fila vazia.
void inicializarFila(FilaExecucao& fila) {
    fila.frente = nullptr;
    fila.tras = nullptr;
    fila.tamanho = 0;
}

// Verifica se a fila está vazia.
bool filaVazia(const FilaExecucao& fila) {
    return fila.frente == nullptr;
}

// Verifica se um id já está na fila (travessia O(n)).
bool filaContem(const FilaExecucao& fila, int id) {
    for (NoFila* atual = fila.frente; atual != nullptr; atual = atual->proximo) {
        if (atual->idTarefa == id) return true;
    }
    return false;
}

// Enfileira (enqueue) um id no final da fila. Custo O(1).

void enfileirar(FilaExecucao& fila, int id) {
    NoFila* novo = new NoFila;
    novo->idTarefa = id;
    novo->proximo = nullptr;
    if (filaVazia(fila)) {
        fila.frente = novo; 
    } else {
        fila.tras->proximo = novo;
    }
    fila.tras = novo;
    fila.tamanho++;
}

// Desenfileira o id da frente e libera o nó. Custo O(1).
bool desenfileirar(FilaExecucao& fila, int& id) {
    if (filaVazia(fila)) return false;
    NoFila* removido = fila.frente;
    id = removido->idTarefa;
    fila.frente = removido->proximo;
    if (fila.frente == nullptr) fila.tras = nullptr;  // fila ficou vazia
    delete removido;
    removido = nullptr;
    fila.tamanho--;
    return true;
}

// Libera todos os nós restantes da fila.
void liberarFila(FilaExecucao& fila) {
    int descartado;
    while (desenfileirar(fila, descartado)) {
        // cada desenfileirar já faz o delete do nó
    }
}

// Executa a próxima tarefa da fila
Tarefa* executarProxima(ListaTarefas& lista, PilhaAcoes& pilha,
                        FilaExecucao& fila) {
    int id;
    while (desenfileirar(fila, id)) {
        Tarefa* tarefa = buscarPorId(lista, id);
        if (tarefa != nullptr && !tarefa->concluida) {
            empilhar(pilha, CONCLUSAO, *tarefa);  // snapshot antes de alterar
            concluirTarefa(lista, id);
            return tarefa;
        }
    }
    return nullptr;
}
