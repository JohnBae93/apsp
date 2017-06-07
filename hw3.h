//
// Created by John on 2017-06-07.
//
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#define BUFSIZE 4096

typedef struct _adjalist {
    int id;
    int val;
    struct _adjalist *next;
} ADJALIST;

int getIndex(int min, int max);

void read_file(char *fname);
void make_adjacent_list(char *infile, char *outfile);

void make_output_file(char *file1, char *file2, int **matrix1, int **matrix2);

void init_P_Matrixs();
int find_APSP_predecessor(char *mcMatrixP_file, char *pMatrix_file);
void showP_path_cost(int from, int to);

void init_H_Matrixs();
int find_APSP_highest_numbered(char *mcMatrixH_file, char *hMatrix_file);
void showH_path_cost(int from, int to);
void find_Hpath(int from, int to);





