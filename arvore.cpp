// Árvore binária de busca
#pragma once
#include <cctype>
#include "tarefa.cpp"
#include "lista.cpp"

// Nó da árvore (não diferencia maiúsculas)
struct NoArvore {
    string chave;
    string titulo;
    int idTarefa;
    NoArvore* esquerda; // chaves menores
    NoArvore* direita; // chaves maiores ou iguais
};

// Raiz da árvore e quantidade de nós.
struct ArvoreTitulos {
    NoArvore* raiz;
    int tamanho;
};

// Prepara uma árvore vazia.
void inicializarArvore(ArvoreTitulos& arvore) {
    arvore.raiz = nullptr;
    arvore.tamanho = 0;
}

// Converte um texto para minúsculas.
string minusculas(const string& texto) {
    string resultado = texto;
    for (size_t i = 0; i < resultado.size(); i++) {
        resultado[i] = tolower(static_cast<unsigned char>(resultado[i]));
    }
    return resultado;
}

// Aloca (new) um nó de árvore para um título.
NoArvore* criarNoArvore(const string& titulo, int id) {
    NoArvore* no = new NoArvore;
    no->chave = minusculas(titulo);
    no->titulo = titulo;
    no->idTarefa = id;
    no->esquerda = nullptr;
    no->direita = nullptr;
    return no;
}

// Insere um nó na subárvore de forma recursiva mantendo a ordem da BST.
NoArvore* inserirNo(NoArvore* no, NoArvore* novo) {
    if (no == nullptr) return novo; // caso base: achou o lugar
    if (novo->chave < no->chave) {
        no->esquerda = inserirNo(no->esquerda, novo);
    } else {
        no->direita = inserirNo(no->direita, novo);
    }
    return no;
}

// Insere um título na árvore.
void inserirTitulo(ArvoreTitulos& arvore, const string& titulo, int id) {
    arvore.raiz = inserirNo(arvore.raiz, criarNoArvore(titulo, id));
    arvore.tamanho++;
}

// Busca recursiva por chave. Média O(log n); pior caso O(n).
const NoArvore* buscarNo(const NoArvore* no, const string& chave) {
    if (no == nullptr || no->chave == chave) return no;
    if (chave < no->chave) return buscarNo(no->esquerda, chave);
    return buscarNo(no->direita, chave);
}

// Travessia IN-ORDEM: esquerda, nó, direita. É imprimido em ordem alfabética.

void percorrerEmOrdem(const NoArvore* no, const ListaTarefas& lista) {
    if (no == nullptr) return;
    percorrerEmOrdem(no->esquerda, lista);
    const Tarefa* tarefa = buscarPorId(lista, no->idTarefa);
    if (tarefa != nullptr) imprimirTarefa(tarefa);
    percorrerEmOrdem(no->direita, lista);
}

// Calcula a altura da árvore. Árvore vazia = altura 0.

int alturaArvore(const NoArvore* no) {
    if (no == nullptr) return 0;
    int alturaEsq = alturaArvore(no->esquerda);
    int alturaDir = alturaArvore(no->direita);
    return 1 + (alturaEsq > alturaDir ? alturaEsq : alturaDir);
}

// Libera os nós em PÓS-ORDEM com delete.
void liberarNos(NoArvore* no) {
    if (no == nullptr) return;
    liberarNos(no->esquerda);
    liberarNos(no->direita);
    delete no;
    no = nullptr;
}

// Libera toda a árvore e volta ao estado vazio.
void liberarArvore(ArvoreTitulos& arvore) {
    liberarNos(arvore.raiz);
    arvore.raiz = nullptr;
    arvore.tamanho = 0;
}

// Reconstrói o índice a partir da lista atual
void construirIndice(const ListaTarefas& lista, ArvoreTitulos& arvore) {
    liberarArvore(arvore);
    for (const Tarefa* t = lista.inicio; t != nullptr; t = t->proxima) {
        inserirTitulo(arvore, t->titulo, t->id);
    }
}
