#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// métricas e contadores
typedef struct {
  long operations;
} Metrics;

Metrics metrics;

void reset_metrics() { metrics.operations = 0; }

void swap(int *a, int *b) {
  metrics.operations++;
  int temp = *a;
  *a = *b;
  *b = temp;
}

// bubble
void bubble_sort(int *list, long n) {
  for (long i = 0; i < n - 1; i++) {
    for (long j = 0; j < n - i - 1; j++) {
      metrics.operations++;
      if (list[j] > list[j + 1]) {
        swap(&list[j], &list[j + 1]);
      }
    }
  }
}

// insertion
void insertion_sort(int *list, long n) {
  for (long i = 1; i < n; i++) {
    int key = list[i];
    long j = i - 1;
    while (j >= 0) {
      metrics.operations++;
      if (list[j] > key) {
        metrics.operations++;
        list[j + 1] = list[j];
        j--;
      } else {
        break;
      }
    }
    if (j + 1 != i) {
      list[j + 1] = key;
    }
  }
}

// heap
void _heap_sort(int *list, long n, long i) {
  long largest = i;
  long left = 2 * i + 1;
  long right = 2 * i + 2;

  if (left < n) {
    metrics.operations++;
    if (list[left] > list[largest]) {
      largest = left;
    }
  }

  if (right < n) {
    metrics.operations++;
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

// merge
void merge(int *list, long left, long mid, long right) {
  long n1 = mid - left + 1;
  long n2 = right - mid;

  int *L = (int *)malloc(n1 * sizeof(int));
  int *R = (int *)malloc(n2 * sizeof(int));

  for (long i = 0; i < n1; i++)
    L[i] = list[left + i];
  for (long j = 0; j < n2; j++)
    R[j] = list[mid + 1 + j];

  long i = 0, j = 0, k = left;

  while (i < n1 && j < n2) {
    metrics.operations++;
    if (L[i] <= R[j]) {
      metrics.operations++;
      list[k++] = L[i++];
    } else {
      metrics.operations++;
      list[k++] = R[j++];
    }
  }

  while (i < n1) {
    metrics.operations++;
    list[k++] = L[i++];
  }
  while (j < n2) {
    metrics.operations++;
    list[k++] = R[j++];
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

// quick
long partition(int *list, long low, long high) {
  int pivo = list[high];
  long i = low - 1;
  for (long j = low; j < high; j++) {
    metrics.operations++;
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
  if (n > 1)
    _quick_sort(list, 0, n - 1);
}

// radix
int get_max(int *list, long n) {
  int max = list[0];
  for (long i = 1; i < n; i++) {
    metrics.operations++;
    if (list[i] > max)
      max = list[i];
  }
  return max;
}

void _radix_sort(int *list, long n, int exp) {
  int *output = (int *)malloc(n * sizeof(int));
  int count[10] = {0};

  for (long i = 0; i < n; i++)
    count[(list[i] / exp) % 10]++;
  for (int i = 1; i < 10; i++)
    count[i] += count[i - 1];

  for (long i = n - 1; i >= 0; i--) {
    metrics.operations++;
    output[count[(list[i] / exp) % 10] - 1] = list[i];
    count[(list[i] / exp) % 10]--;
  }
  for (long i = 0; i < n; i++)
    list[i] = output[i];

  free(output);
}

void radix_sort(int *list, long n) {
  int max = get_max(list, n);
  for (int exp = 1; max / exp > 0; exp *= 10) {
    _radix_sort(list, n, exp);
  }
}

void generate_random_list(int *arr, long n) {
  for (long i = 0; i < n; i++) {
    arr[i] = rand() % 10000;
  }
}

// executa um algoritmo e devolve a média das operações
void testar_algoritmo(void (*sort_func)(int *, long), const char *name,
                      long size, int runs) {
  long total_operations = 0;
  for (int run = 0; run < runs; run++) {
    int *arr = (int *)malloc(size * sizeof(int));
    generate_random_list(arr, size);

    reset_metrics();
    sort_func(arr, size);
    total_operations += metrics.operations;

    free(arr);
  }
  printf("%ld,%ld\n", size, total_operations / runs);
}

void executar_experimento(const char *nome, void (*sort_func)(int *, long),
                          long max_size, int step, int runs) {
  printf("=== %s ===\n", nome);
  printf("Size,Operations\n");
  for (long size = 1; size <= max_size; size += step) {
    testar_algoritmo(sort_func, nome, size, runs);
  }
  printf("\n");
}

int main() {
  srand(time(NULL));
  freopen("output.txt", "w", stdout);

  const int RUNS = 30;
  const long MAX_SIZE = 1000;
  const int STEP = 50;

  executar_experimento("Bubble Sort", bubble_sort, MAX_SIZE, STEP, RUNS);
  executar_experimento("Insertion Sort", insertion_sort, MAX_SIZE, STEP, RUNS);
  executar_experimento("Heap Sort", heap_sort, MAX_SIZE, STEP, RUNS);
  executar_experimento("Merge Sort", merge_sort, MAX_SIZE, STEP, RUNS);
  executar_experimento("Quick Sort", quick_sort, MAX_SIZE, STEP, RUNS);
  executar_experimento("Radix Sort", radix_sort, MAX_SIZE, STEP, RUNS);

  fclose(stdout);
  printf("concluído, resultados salvos em output.txt\n");
  return 0;
}
