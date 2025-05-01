#include <math.h>
#include <stdio.h>
#include <stdlib.h>

long long factorial(int n);
long long cube(int n);
double natural_log(int n);
double log_base2(int n);
double exponential(int n);
long long power_of_two(int n);
double ln_of_ln(int n);
double lg_of_lg(int n);
double n_log2(int n);
long long n_power_of_two(int n);

void print_to_terminal(const char *function_names[], double values[][101],
                       int num_functions, int size);

void write_to_csv(const char *filename, const char *function_names[],
                  double values[][101], int num_functions, int size);

int main() {
  const int max_n = 100;
  const int limited_n = 20;
  double values[11][max_n + 1];
  const char *function_names[] = {"Factorial",         "Cube",
                                  "Natural Log (ln)",  "Log Base 2 (lg)",
                                  "Exponential (e^n)", "Power of 2 (2^n)",
                                  "ln(ln(n))",         "lg(lg(n))",
                                  "n*lg(n)",           "n*2^n"};

  // Factorial
  for (int i = 0; i <= max_n; i++) {
    values[0][i] = (i <= limited_n) ? factorial(i) : NAN;
  }

  // Cube
  for (int i = 0; i <= max_n; i++) {
    values[1][i] = cube(i);
  }

  // Natural Logarithm (ln)
  for (int i = 0; i <= max_n; i++) {
    values[2][i] = natural_log(i);
  }

  // Log base 2 (lg)
  for (int i = 0; i <= max_n; i++) {
    values[3][i] = log_base2(i);
  }

  // Exponential (e^n)
  for (int i = 0; i <= max_n; i++) {
    values[4][i] = (i <= limited_n) ? exponential(i) : NAN;
  }

  // Power of 2 (2^n)
  for (int i = 0; i <= max_n; i++) {
    values[5][i] = (i <= limited_n) ? power_of_two(i) : NAN;
  }

  // ln(ln(n))
  for (int i = 0; i <= max_n; i++) {
    values[6][i] = ln_of_ln(i);
  }

  // lg(lg(n))
  for (int i = 0; i <= max_n; i++) {
    values[7][i] = lg_of_lg(i);
  }

  // n * lg(n)
  for (int i = 0; i <= max_n; i++) {
    values[8][i] = n_log2(i);
  }

  // n * 2^n
  for (int i = 0; i <= max_n; i++) {
    values[9][i] = (i <= limited_n) ? n_power_of_two(i) : NAN;
  }

  printf("\n--- First 5 Functions ---\n");
  print_to_terminal(function_names, values, 5, max_n + 1);

  printf("\n--- Next 5 Functions ---\n");
  print_to_terminal(function_names + 5, values + 5, 5, max_n + 1);

  write_to_csv("output.csv", function_names, values, 10, max_n + 1);

  printf("\nData has been written to output.csv\n");
  return 0;
}

long long factorial(int n) {
  if (n == 0 || n == 1)
    return 1;
  long long fact = 1;
  for (int i = 2; i <= n; i++) {
    fact *= i;
  }
  return fact;
}

long long cube(int n) { return (long long)n * n * n; }

double natural_log(int n) { return (n > 0) ? log(n) : NAN; }

double log_base2(int n) { return (n > 0) ? log2(n) : NAN; }

double exponential(int n) { return exp(n); }

long long power_of_two(int n) { return (long long)pow(2, n); }

double ln_of_ln(int n) { return (n > 1) ? log(log(n)) : NAN; }

double lg_of_lg(int n) { return (n > 1) ? log2(log2(n)) : NAN; }

double n_log2(int n) { return (n > 0) ? n * log2(n) : 0; }

long long n_power_of_two(int n) { return (long long)n * (long long)pow(2, n); }

void print_to_terminal(const char *function_names[], double values[][101],
                       int num_functions, int size) {
  // Print headers
  printf("%-10s", "n");
  for (int i = 0; i < num_functions; i++) {
    printf("%-20s", function_names[i]);
  }
  printf("\n");

  // Print values
  for (int i = 0; i < size; i++) {
    printf("%-10d", i);
    for (int j = 0; j < num_functions; j++) {
      if (isnan(values[j][i])) {
        printf("%-20s", "N/A");
      } else {
        printf("%-20.2e", values[j][i]);
      }
    }
    printf("\n");
  }
}

void write_to_csv(const char *filename, const char *function_names[],
                  double values[][101], int num_functions, int size) {
  FILE *file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error opening file: %s\n", filename);
    return;
  }

  // Write header
  fprintf(file, "n");
  for (int i = 0; i < num_functions; i++) {
    fprintf(file, ",%s", function_names[i]);
  }
  fprintf(file, "\n");

  // Write values
  for (int i = 0; i < size; i++) {
    fprintf(file, "%d", i);
    for (int j = 0; j < num_functions; j++) {
      if (isnan(values[j][i])) {
        fprintf(file, ",N/A");
      } else {
        fprintf(file, ",%.2e", values[j][i]);
      }
    }
    fprintf(file, "\n");
  }

  fclose(file);
}