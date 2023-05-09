#include <stdlib.h>
#include <stdio.h>

int **create_int_matrix(int num_rows, int num_cols);

void delete_int_matrix(int **matrix, int num_rows, int num_cols);

int change(int coins[], int coin_size, int amount);

void display(int coins[], int coin_size, int amount);

int main() {
    int amount = 11;
    int coin_size = 3;
    int coins1[3] = {1, 2, 5};
    display(coins1, coin_size, amount);

    amount = 3;
    coin_size = 1;
    int coins2[1] = {2};
    display(coins2, coin_size, amount);

    amount = 9;
    coin_size = 2;
    int coins3[2] = {3, 4};
    display(coins3, coin_size, amount);

    amount = 97;
    coin_size = 3;
    int coins4[3] = {1, 4, 7};
    display(coins4, coin_size, amount);

    return 0;
}

int change(int coins[], int coin_size, int amount) {
    return -1;
}

void display(int coins[], int coin_size, int amount) {
    printf("\nCoins: ");
    for (int i = 0; i < coin_size; i++) {
        printf("%d  ", coins[i]);
    }
    printf("\n");
    printf("Amount = %d\n", amount);
    printf("Number of coins in change = %d\n", change(coins, coin_size, amount));
}

int **create_int_matrix(int num_rows, int num_cols) {
    int **matrix = (int **) malloc(num_rows * sizeof(int *));
    for (int i = 0; i < num_rows; i++) {
        matrix[i] = (int *) malloc(num_cols * sizeof(int));
    }
    return matrix;
}

void delete_int_matrix(int **matrix, int num_rows, int num_cols) {
    for (int i = 0; i < num_rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
