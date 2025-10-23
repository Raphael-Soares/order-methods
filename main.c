// O objetivo deste trabalho consiste em analisar e comparar a complexidade
// algorítmica dos métodos de ordenação estudados na disciplina (bubble-sort,
// insertion-sort, heap-sort, merge-sort, quick-sort e radix-sort). A análise
// deve ser realizada considerando a geração de um conjunto de números inteiros
// com tamanho variando entre 1 e 1000. Os números devem ser gerados prevendo
// apenas o caso médio (números gerados aleatoriamente). Para geração dos
// números aleatórios, sugere-se o uso da função rand e srand em C. Além disso,
// devido à natureza aleatória do caso médio, recomenda-se considerar o
// resultado médio de 30 execuções distintas para validade estatística.
//
// O resultado final do experimento deve ser exibido em um gráfico de linha,
// onde o eixo X representa o tamanho dos conjuntos de dados (1 a 1000 - mínimo)
// e o eixo Y representa o esforço computacional das operações de troca de
// valores no vetor e comparações. O gráfico deve apresentar 6 linhas, as quais
// representam cada método de ordenação considerado na análise. Cada equipe deve
// entregar um relatório com o gráfico e considerações sobre os resultados
// encontrados, bem como os códigos usados no experimento.

//---------- TODO----------
// 1. Criar as implementacoes iniciais
// 2. Separar cada implementacao num modulo separado?
// 3. Criar uma funcao de geracao de listas infinitas
//

#include <stdlib.h>

typedef struct {
  long comparisons;
  long swaps;
} Metrics;

Metrics metrics;

void reset_metrics() {
  metrics.comparisons = 0;
  metrics.swaps = 0;
}

void generate_random_list(int *list, long n) {
  for (long i = 0; i < n; i++) {
    list[i] = rand() % 10000;
  }
}

void swap(int *a, int *b) {
  metrics.swaps++;
  int temp = *a;
  *a = *b;
  *b = temp;
}

void bubble_sort(int *list, long n) {
  for (long i = 0; i < n - 1; i++) {
    for (long j = 0; j < n - i - 1; j++) {
      metrics.comparisons++;
      if (list[j] > list[j + 1]) {
        swap(&list[j], &list[j + 1]);
      }
    }
  }
}

void insertion_sort(int *list, long n) {
  for (long i = 1; i < n; i++) {
    int key = list[i];
    long j = i - 1;

    while (j >= 0) {
      metrics.comparisons++;
      if (list[j] > key) {
        metrics.swaps++;
        list[j + 1] = list[j];
        j = j - 1;
      } else {
        break;
      }
    }
    if (j + 1 != i) {
      list[j + 1] = key;
    }
  }
}

void heap_sort(int *list, long n);
void merge_sort(int *list, long n);
void quick_sort(int *list, long n);
void radix_sort(int *list, long n);

int main(int argc, char *argv[]) { return EXIT_SUCCESS; }
