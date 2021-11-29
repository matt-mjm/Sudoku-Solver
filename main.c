#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Board_s Board;
struct Board_s {
    char data[81];
    int rows[10];
    int cols[10];
    int boxes[10];
};
typedef struct Stack_s Stack;
struct Stack_s {
    int len;
    char data[162];
};

int loadBoard(Board* board, FILE* inFile) {
    int index = 0;
    while (!feof(inFile) && index < 81) {
        char ch = fgetc(inFile);
        if (ch >= '0' && ch <= '9') {
            board->data[index++] = ch - '0';
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board->rows[i] |= 1 << board->data[i * 9 + j];
            board->cols[j] |= 1 << board->data[i * 9 + j];
            board->boxes[(i / 3) * 3 + (j / 3)] |= 1 << board->data[i * 9 + j];
        }
    }

    return index == 81;
}

void storeBoard(Board* board, FILE* outFile) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fputc(board->data[i * 9 + j] + '0', outFile);
        }
        fputc('\n', outFile);
    }
}

void solveBoard(Board* board) {
    
}

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("Usage: %s [options] [inFile] [outFile]\n", argv[0]);
        return 1;
    }

    FILE* inFile = fopen(argv[2], "r");
    if (inFile == NULL) {
        printf("Failed to open %s for reading.\n", argv[2]);
        return 1;
    }

    FILE* outFile = fopen(argv[3], "w");
    if (outFile == NULL) {
        printf("Failed to open %s for writing.\n", argv[3]);
    }

    int options = atoi(argv[1]);

    static Board board;

    loadBoard(&board, inFile);

    solveBoard(&board);

    storeBoard(&board, stdout);

    fclose(inFile);
    fclose(outFile);

    return 0;
}