
// busca sequencial x busca binária 

#pragma once
#include <chrono>
#include "tarefa.cpp"
#include "lista.cpp"

// Resultado de uma busca: posição no vetor (-1 se não achou) e
// número de comparações feitas.
struct ResultadoBusca {
    int posicao;
    long comparacoes;
};

// Busca sequencial: examina elemento por elemento.

ResultadoBusca buscaSequencial(const int vetor[], int tamanho, int alvo) {
    ResultadoBusca resultado = {-1, 0};
    for (int i = 0; i < tamanho; i++) {
        resultado.comparacoes++;
        if (vetor[i] == alvo) {
            resultado.posicao = i;
            break;
        }
    }
    return resultado;
}

// Busca binária: compara com o meio e descarta metade do vetor.
ResultadoBusca buscaBinaria(const int vetor[], int tamanho, int alvo) {
    ResultadoBusca resultado = {-1, 0};
    int inicio = 0;
    int fim = tamanho - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;   // evita overflow 
        resultado.comparacoes++;
        if (vetor[meio] == alvo) {
            resultado.posicao = meio;
            break;
        }
        if (vetor[meio] < alvo) inicio = meio + 1;
        else fim = meio - 1;
    }
    return resultado;
}

// Insertion Sort em vetor de inteiros. 
void ordenarVetor(int vetor[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        int chave = vetor[i];
        int j = i - 1;
        while (j >= 0 && vetor[j] > chave) {
            vetor[j + 1] = vetor[j];   // desloca para abrir espaço
            j--;
        }
        vetor[j + 1] = chave;
    }
}

// Copia os ids da lista para um vetor dinâmico (new[]) já ordenado.

int* copiarIdsOrdenados(const ListaTarefas& lista) {
    if (lista.tamanho == 0) return nullptr;
    int* vetor = new int[lista.tamanho];
    int posicao = 0;
    for (const Tarefa* t = lista.inicio; t != nullptr; t = t->proxima) {
        vetor[posicao++] = t->id;
    }
    ordenarVetor(vetor, lista.tamanho);
    return vetor;
}

// Mede o tempo médio de uma busca, repetindo-a várias vezes.

double medirMicrossegundos(ResultadoBusca (*busca)(const int[], int, int),
                           const int vetor[], int tamanho, int alvo,
                           int repeticoes) {
    volatile long acumulador = 0; // impede o compilador de remover o laço
    auto inicio = chrono::steady_clock::now();
    for (int i = 0; i < repeticoes; i++) {
        acumulador += busca(vetor, tamanho, alvo).posicao;
    }
    auto fim = chrono::steady_clock::now();
    chrono::duration<double, micro> duracao = fim - inicio;
    return duracao.count() / repeticoes;
}

// Compara as duas buscas num vetor sintético de n elementos procurando o último elemento.
void compararDesempenho(int n) {
    int* vetor = new int[n];
    for (int i = 0; i < n; i++) vetor[i] = i * 2;
    int alvo = vetor[n - 1];
    ResultadoBusca seq = buscaSequencial(vetor, n, alvo);
    ResultadoBusca bin = buscaBinaria(vetor, n, alvo);
    double tempoSeq = medirMicrossegundos(buscaSequencial, vetor, n, alvo, 100);
    double tempoBin = medirMicrossegundos(buscaBinaria, vetor, n, alvo, 100);
    cout << "n = " << n << "\n"
         << "  sequencial: " << seq.comparacoes << " comparacoes, "
         << tempoSeq << " us\n"
         << "  binaria:    " << bin.comparacoes << " comparacoes, "
         << tempoBin << " us\n";
    delete[] vetor;
    vetor = nullptr;
}
