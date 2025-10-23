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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void _heap_sort(int *list, long n, long i) {
  long largest = i;
  long left = 2 * i + 1;
  long right = 2 * i + 2;

  if (left < n) {
    metrics.comparisons++;
    if (list[left] > list[largest]) {
      largest = left;
    }
  }

  if (right < n) {
    metrics.comparisons++;
    if (list[right] > list[largest]) {
      largest = right;
    }
  }

  if (largest != i) {
    swap(&list[i], &list[largest]);
    _heap_sort(list, n, largest);
  }
}

void heap_sort(int *list, long n) {
  for (long i = n / 2 - 1; i >= 0; i--) {
    _heap_sort(list, n, i);
  }

  for (long i = n - 1; i > 0; i--) {
    swap(&list[0], &list[i]);
    _heap_sort(list, i, 0);
  }
}

void merge(int *list, long left, long mid, long right) {
  long n1 = mid - left + 1;
  long n2 = right - mid;

  int *L = (int *)malloc(n1 * sizeof(int));
  int *R = (int *)malloc(n2 * sizeof(int));

  for (long i = 0; i < n1; i++) {
    L[i] = list[left + i];
  }
  for (long j = 0; j < n2; j++) {
    R[j] = list[mid + 1 + j];
  }

  long i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    metrics.comparisons++;
    if (L[i] <= R[j]) {
      metrics.swaps++;
      list[k] = L[i];
      i++;
    } else {
      metrics.swaps++;
      list[k] = R[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    metrics.swaps++;
    list[k] = L[i];
    i++;
    k++;
  }

  while (j < n2) {
    metrics.swaps++;
    list[k] = R[j];
    j++;
    k++;
  }

  free(L);
  free(R);
}

void _merge_sort(int *list, long left, long right) {
  if (left < right) {
    long mid = left + (right - left) / 2;
    _merge_sort(list, left, mid);
    _merge_sort(list, mid + 1, right);
    merge(list, left, mid, right);
  }
}

void merge_sort(int *list, long n) {
  if (n > 1) {
    _merge_sort(list, 0, n - 1);
  }
}

long partition(int *list, long low, long high) {
  int pivo = list[high];
  long i = low - 1;

  for (long j = low; j < high; j++) {
    metrics.comparisons++;
    if (list[j] < pivo) {
      i++;
      swap(&list[i], &list[j]);
    }
  }
  swap(&list[i + 1], &list[high]);
  return i + 1;
}

void _quick_sort(int *list, long low, long high) {
  if (low < high) {
    long pi = partition(list, low, high);
    _quick_sort(list, low, pi - 1);
    _quick_sort(list, pi + 1, high);
  }
}

void quick_sort(int *list, long n) {
  if (n > 1) {
    _quick_sort(list, 0, n - 1);
  }
}

// radix
int get_max(int *list, long n) {
  int max = list[0];
  for (long i = 1; i < n; i++) {
    metrics.comparisons++;
    if (list[i] > max) {
      max = list[i];
    }
  }
  return max;
}

void _radix_sort(int *list, long n, int exp) {
  int *output = (int *)malloc(n * sizeof(int));
  int count[10] = {0};

  for (long i = 0; i < n; i++) {
    count[(list[i] / exp) % 10]++;
  }

  for (int i = 1; i < 10; i++) {
    count[i] += count[i - 1];
  }

  for (long i = n - 1; i >= 0; i--) {
    metrics.swaps++;
    output[count[(list[i] / exp) % 10] - 1] = list[i];
    count[(list[i] / exp) % 10]--;
  }

  for (long i = 0; i < n; i++) {
    list[i] = output[i];
  }

  free(output);
}

void radix_sort(int *list, long n) {
  int max = get_max(list, n);

  for (int exp = 1; max / exp > 0; exp *= 10) {
    _radix_sort(list, n, exp);
  }
}

int main(int argc, char *argv[]) { return EXIT_SUCCESS; }
