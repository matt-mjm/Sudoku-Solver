#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void printBin(FILE* outFile, uint32_t bin, uint8_t bits);

typedef struct Board_s Board;
struct Board_s {
    uint8_t data[81];
    uint16_t rows[9];
    uint16_t cols[9];
    uint16_t boxes[9];
};

void initBoard(Board* board) {
    for (int i = 0; i < 81; i++) {
        board->data[i] = 0;
    }
    for (int i = 0; i < 9; i++) {
        board->rows[i] = 0;
        board->cols[i] = 0;
        board->boxes[i] = 0;
    }
}

int isValidChange(Board* board, uint8_t pos, uint8_t val) {
    return (board->rows[pos / 9] & (1 << val)) == 0 &&
        (board->cols[pos % 9] & (1 << val)) == 0 &&
        (board->boxes[(pos / 27) * 3 + (pos % 9) / 3] & (1 << val)) == 0;
}
void setBoardCell(Board* board, uint8_t pos, uint8_t val) {
    board->data[pos] = val;
    board->rows[pos / 9] |= (1 << val);
    board->cols[pos % 9] |= (1 << val);
    board->boxes[(pos / 27) * 3 + (pos % 9) / 3] |= (1 << val);
}
uint8_t resetBoardCell(Board* board, uint8_t pos) {
    uint8_t val = board->data[pos];

    board->data[pos] = 0;
    board->rows[pos / 9] &= ~(1 << val);
    board->cols[pos % 9] &= ~(1 << val);
    board->boxes[(pos / 27) * 3 + (pos % 9) / 3] &= ~(1 << val);

    return val;
}

int loadBoard(Board* board, FILE* inFile) {
    initBoard(board);

    int index = 0;
    while (!feof(inFile) && index < 81) {
        char ch = fgetc(inFile);
        if (ch == ';') while ((ch = fgetc(inFile)) != '\n' && !feof(inFile));
        if (ch >= '0' && ch <= '9') {
            setBoardCell(board, index++, (uint8_t)(ch - '0'));
        }
    }

    return index == 81;
}

void printBin(FILE* outFile, uint32_t bin, uint8_t bits) {
    // for (int i = bits - 1; i >= 0; i--) fputc((bin & (1 << i)) ? '1' : '0', outFile);
    for (int i = 0; i < bits; i++) fputc((bin & (1 << i)) ? '1' : '0', outFile);
}

void storeBoard(Board* board, FILE* outFile) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fputc((char)(board->data[i * 9 + j] + '0'), outFile);
            // if (j % 3 == 2) fputc(' ', outFile);
        }

        /*fputc(';', outFile);
        fputc(' ', outFile);
        printBin(outFile, board->rows[i] >> 1, 9);
        fputc(' ', outFile);
        printBin(outFile, board->cols[i] >> 1, 9);
        fputc(' ', outFile);
        printBin(outFile, board->boxes[i] >> 1, 9);*/

        fputc('\n', outFile);
    }
}

int updateBoardCell(Board* board, uint8_t pos, uint8_t startVal) {
    for (uint8_t val = startVal; val < 10; val++) {
        if (isValidChange(board, pos, val)) {
            setBoardCell(board, pos, val);
            return 1;
        }
    }
    return 0;
}

void solveBoard(Board* board) {
    uint32_t stack[256];
    uint32_t len = 0;
    uint8_t pos = 0;
    uint8_t val = 1;

    while (pos < 81) {
        if (board->data[pos] == 0) {
            if (updateBoardCell(board, pos, val)) {
                stack[len++] = pos;
                pos = pos + 1;
                val = 1;
            } else if (len > 0) {
                pos = stack[--len];
                val = resetBoardCell(board, pos) + 1;
            } else {
                printf("Fail\n");
                break;
            }
        } else {
            pos = pos + 1;
        }
    }
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

    Board board;

    loadBoard(&board, inFile);

    solveBoard(&board);

    storeBoard(&board, outFile);

    fclose(inFile);
    fclose(outFile);

    return 0;
}