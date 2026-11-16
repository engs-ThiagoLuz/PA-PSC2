// Pilha de desfazer
#pragma once
#include "tarefa.cpp"
#include "lista.cpp"

// Tipos de ação.
enum TipoAcao { ADICAO, REMOCAO, CONCLUSAO };

// Nó da pilha de desfazer
struct Acao {
    TipoAcao tipo;
    Tarefa copia;
    Acao* abaixo;
};

// Pilha LIFO.
struct PilhaAcoes {
    Acao* topo;
    int tamanho;
};

using namespace std;

// Prepara uma pilha vazia.
void inicializarPilha(PilhaAcoes& pilha) {
    pilha.topo = nullptr;
    pilha.tamanho = 0;
}

// Verifica se a pilha está vazia.
bool pilhaVazia(const PilhaAcoes& pilha) {
    return pilha.topo == nullptr;
}

// Empilha (push) uma ação, guardando uma cópia da tarefa.
void empilhar(PilhaAcoes& pilha, TipoAcao tipo, const Tarefa& tarefa) {
    Acao* nova = new Acao;
    nova->tipo = tipo;
    nova->copia = tarefa;
    nova->copia.proxima = nullptr; 
    nova->abaixo = pilha.topo;
    pilha.topo = nova;
    pilha.tamanho++;
}

// nDesempilha (pop) a ação do topo e libera o nó.
bool desempilhar(PilhaAcoes& pilha, Acao& destino) {
    if (pilhaVazia(pilha)) return false;
    Acao* removida = pilha.topo;
    destino = *removida;
    destino.abaixo = nullptr;
    pilha.topo = removida->abaixo;
    delete removida;
    removida = nullptr;
    pilha.tamanho--;
    return true;
}

// Libera todos os nós restantes da pilha.
void liberarPilha(PilhaAcoes& pilha) {
    Acao* atual = pilha.topo;
    while (atual != nullptr) {
        Acao* abaixo = atual->abaixo;
        delete atual;
        atual = abaixo;
    }
    pilha.topo = nullptr;
    pilha.tamanho = 0;
}

// Recoloca na lista uma tarefa removida, mantendo o id original.
void restaurarTarefa(ListaTarefas& lista, const Tarefa& modelo) {
    Tarefa* nova = criarTarefa(modelo.id, modelo.titulo,
                               modelo.prioridade, modelo.prazo);
    nova->concluida = modelo.concluida;
    anexarNo(lista, nova);
}

// Desfaz a última ação registrada
bool desfazerUltimaAcao(ListaTarefas& lista, PilhaAcoes& pilha) {
    Acao acao;
    if (!desempilhar(pilha, acao)) return false;
    if (acao.tipo == ADICAO) {
        removerTarefa(lista, acao.copia.id); // desfaz o cadastro
    } else if (acao.tipo == REMOCAO) {
        restaurarTarefa(lista, acao.copia); // desfaz a remoção
    } else {
        Tarefa* tarefa = buscarPorId(lista, acao.copia.id);
        if (tarefa != nullptr) tarefa->concluida = false;
    }
    return true;
}
