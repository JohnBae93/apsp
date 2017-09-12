#include "apsp.h"

ADJALIST **adjalist;

int input_size;
int **pMatrix;
int **mcMatrixP;
int **hMatrix;
int **mcMatrixH;

int main() {
    // you can change input and output file name here!
    char *infile = "input200.txt";

    char *adjaList_file = "adjaList.txt";
    char *mcMatrixP_file = "mcMatrixP.txt";
    char *mcMatrixH_file = "mcMatrixH.txt";
    char *pMatrix_file = "pMatrix.txt";
    char *hMatrix_file = "hMatrix.txt";

    int input = 0;
    int from, to;


    /*
     * Progrma UI
     */
    printf("Welcomeo to homework6 (All-Pair Shortest Path Problem)\n");

    while (1) {
        printf("(1)Make a graph\t(2)Find APSP with predecessor matrix\t");
        printf("(3)Find APSP with highest-numbered node matrix\n(4)Exit\t\t");

        input = getIndex(1, 4);
        switch (input) {
            case 1:
                make_adjacent_list(infile, adjaList_file);
                printf("Graph is made and store to %s\n", adjaList_file);
                printf("The number of nodes : %d\n", input_size);
                break;
            case 2:
                if(!find_APSP_predecessor(mcMatrixP_file, pMatrix_file)){
                    printf("Please make a graph first\n");
                    break;
                }
                printf("APSP are constructed\n");
                printf("Minimum cost matrix is stored in '%s' and predecessor matrix is stored in '%s'\n",
                       mcMatrixP_file, pMatrix_file);
                while (1) {
                    printf("(1)Show the SP (2)Main menu\t\t");
                    input = getIndex(1, 2);
                    if (input == 1) {
                        printf("Input a ID of Source node:\t\t");
                        from = getIndex(0, input_size - 1);

                        printf("Input a ID of destination node:\t\t");
                        to = getIndex(0, input_size - 1);

                        showP_path_cost(from, to);
                    } else
                        break;
                }
                break;
            case 3:
                if(!find_APSP_highest_numbered(mcMatrixH_file, hMatrix_file)){
                    printf("Plsease make a graph first\n");
                    break;
                }
                printf("APSP are constructed\n");
                printf("Minimum cost matrix is stored in '%s' and predecessor matrix is stored in '%s'\n",
                       mcMatrixH_file, hMatrix_file);
                while (1) {
                    printf("(1)Show the SP (2)Main menu\t\t");
                    input = getIndex(1, 2);
                    if (input == 1) {
                        printf("Input a ID of Source node:\t\t");
                        from = getIndex(0, input_size - 1);

                        printf("Input a ID of destination node:\t\t");
                        to = getIndex(0, input_size - 1);

                        showH_path_cost(from, to);
                    } else
                        break;
                }
                break;
            case 4:
                printf("Thank you for using this software!\n");
                exit(0);
        }
    }
}

// get input index min~max
int getIndex(int minimum, int maximum) {
    char *buf, *tmpstr;       // line buffer and garbage storage for user input
    int num = 0;             // integer returned by sscanf()
    int index;               // user input returned by this function

    // Prepare line buffer
    buf = (char *) malloc(BUFSIZE * sizeof(char));
    tmpstr = (char *) malloc(BUFSIZE * sizeof(char));
    assert(buf && tmpstr);

    // Get index
    while (!num) {
        printf("Choice?");
        fgets(buf, BUFSIZE, stdin);
        num = sscanf(buf, "%d%s", &index, tmpstr);
        if (num != 1) {
            num = 0;
            printf("[ERROR] index should be a non-negative integer.\n");
        } else if (index > maximum || index < minimum) {
            num = 0;
            printf("[ERROR] index should be integer from %d to %d.\n", minimum, maximum);
        }
    }
    free(buf);
    free(tmpstr);
    return index;
}

// read input file and make a adjacent list
void read_file(char *fname) {
    int stest = 1;
    int count_letters;
    input_size = 0;

    FILE *fp = fopen(fname, "r");
    if (!fp) {
        printf("There is no '%s' in directory", fname);
        exit(0);
    }

    char *buf = (char *) malloc(sizeof(char) * BUFSIZE);
    char *tmp = (char *) malloc(sizeof(char) * BUFSIZE);
    assert(buf && tmp);

    fgets(buf, BUFSIZE, fp);
    if (strlen(buf) == BUFSIZE - 1) {
        printf("File is too big. Each line should be less than %d word\n", BUFSIZE - 1);
        free(buf);
        free(tmp);
        exit(0);
    }
    char tmp_char;
    char *tmp_buf = buf;

    while (stest > 0) {
        stest = sscanf(buf, " %c%s%n", &tmp_char, tmp, &count_letters);
        input_size++;
        buf += count_letters;
    }
    input_size--;

    adjalist = (ADJALIST **) malloc(sizeof(ADJALIST *) * input_size);
    assert(adjalist);

    ADJALIST *tmp_adj;

    buf = tmp_buf;
    int tmp_int;

    for (int i = 0; i < input_size; ++i) {
        adjalist[i] = NULL;

        for (int j = 0; j < input_size; ++j) {
            sscanf(buf, "%s%n", tmp, &count_letters);
            buf += count_letters;
            if (tmp[0] != 'x' && tmp[0] != 'X' && tmp[0] != '0') {
                sscanf(tmp, "%d%s", &tmp_int, tmp);
                ADJALIST *new = (ADJALIST *) malloc(sizeof(ADJALIST));
                assert(new);

                new->id = j;
                new->val = tmp_int;
                new->next = NULL;
                tmp_adj = adjalist[i];

                if (tmp_adj == NULL)
                    adjalist[i] = new;
                else {
                    while (tmp_adj->next) {
                        tmp_adj = tmp_adj->next;
                    }

                    tmp_adj->next = new;
                    new->next = NULL;
                }
            }
        }

        buf = tmp_buf;
        fgets(buf, 1023, fp);
    }

    free(buf);
    free(tmp);
    fclose(fp);
}

// print out adjacent list to output file
void make_adjacent_list(char *infile, char *outfile) {
    FILE *fp = fopen(outfile, "w");
    if (!fp) {
        printf("There is error open %s", outfile);
        exit(0);
    }

    read_file(infile);
    ADJALIST *tmp;
    int id;

    fprintf(fp, "ID\tConnected nodes\n");
    for (int i = 0; i < input_size; ++i) {
        fprintf(fp, "%d\t", i);
        tmp = adjalist[i];

        while (tmp) {
            id = tmp->id;
            fprintf(fp, "%d ", id);
            tmp = tmp->next;
        }

        fprintf(fp, "\n");
    }
    fclose(fp);
}

// given 2 file name, print out 2 output file
void make_output_file(char *file1, char *file2, int **matrix1, int **matrix2) {
    FILE *fp1 = fopen(file1, "w");
    if (!fp1) {
        printf("There is error open %s", file1);
        exit(0);
    }

    FILE *fp2 = fopen(file2, "w");
    if (!fp2) {
        printf("There is error open %s", file2);
        exit(0);
    }

    for (int i = 0; i < input_size; ++i) {
        for (int j = 0; j < input_size; ++j) {
            if (matrix1[i][j] == INT_MAX)
                fprintf(fp1, "  INF");
            else
                fprintf(fp1, "%5d", matrix1[i][j]);
            if (matrix2[i][j] == INT_MAX)
                fprintf(fp2, "  NIL");
            else
                fprintf(fp2, "%5d", matrix2[i][j]);
        }
        fprintf(fp1, "\n");
        fprintf(fp2, "\n");
    }
    fclose(fp1);
    fclose(fp2);
}


// initialize 2 P matrix
void init_P_Matrixs() {
    pMatrix = (int **) malloc(sizeof(int *) * input_size);
    mcMatrixP = (int **) malloc(sizeof(int) * input_size);
    ADJALIST *tmp;

    for (int i = 0; i < input_size; ++i) {
        mcMatrixP[i] = (int *) malloc(sizeof(int) * input_size);
        pMatrix[i] = (int *) malloc(sizeof(int) * input_size);

        for (int j = 0; j < input_size; ++j) {
            pMatrix[i][j] = INT_MAX;
            if (i == j)
                mcMatrixP[i][j] = 0;
            else
                mcMatrixP[i][j] = INT_MAX;
        }

        tmp = adjalist[i];
        while (tmp) {
            pMatrix[i][tmp->id] = i;
            mcMatrixP[i][tmp->id] = tmp->val;
            tmp = tmp->next;
        }
    }

}

// First Floyd-Warshall Algorith.
int find_APSP_predecessor(char *mcMatrixP_file, char *pMatrix_file) {
    if(!adjalist)
        return 0;
    // initialize 2 matrix
    init_P_Matrixs();

    /*
     * Main Algorithms
     */
    // k : appended vertex
    for (int k = 0; k < input_size; ++k) {

        for (int i = 0; i < input_size; ++i) {
            for (int j = 0; j < input_size; ++j) {
                // renewal
                if (mcMatrixP[i][k] != INT_MAX && mcMatrixP[k][j] != INT_MAX) {
                    if (mcMatrixP[i][j] > mcMatrixP[i][k] + mcMatrixP[k][j]) {
                        mcMatrixP[i][j] = mcMatrixP[i][k] + mcMatrixP[k][j];
                        pMatrix[i][j] = pMatrix[k][j];  // predecessor of vertex j
                    }
                }
            }
        }
    }

    // print out
    make_output_file(mcMatrixP_file, pMatrix_file, mcMatrixP, pMatrix);
    return 1;
}

// Find Shortest path from pMatrix
void showP_path_cost(int from, int to) {
    int *stack = (int *) malloc(sizeof(int) * input_size);

    int top = -1;
    int tmp = to;
    stack[++top] = tmp;
    while (tmp != from) {
        tmp = pMatrix[from][tmp];
        stack[++top] = tmp;
    }

    printf("The sortest path from node %d to node %d : ", from, to);
    for (top; top >= 1; --top) {
        printf("%d - ", stack[top]);
    }
    printf("%d\n", to);
    printf("The cost of the shortest path : %d\n", mcMatrixP[from][to]);

    free(stack);
}


// initialize 2 H matrix
void init_H_Matrixs() {
    hMatrix = (int **) malloc(sizeof(int *) * input_size);
    mcMatrixH = (int **) malloc(sizeof(int) * input_size);
    ADJALIST *tmp;

    for (int i = 0; i < input_size; ++i) {
        mcMatrixH[i] = (int *) malloc(sizeof(int) * input_size);
        hMatrix[i] = (int *) malloc(sizeof(int) * input_size);

        for (int j = 0; j < input_size; ++j) {
            hMatrix[i][j] = INT_MAX;
            if (i == j)
                mcMatrixH[i][j] = 0;
            else
                mcMatrixH[i][j] = INT_MAX;
        }

        tmp = adjalist[i];

        while (tmp) {
            mcMatrixH[i][tmp->id] = tmp->val;
            tmp = tmp->next;
        }
    }

}

// Second Floyd-Warshall Algorith.
int find_APSP_highest_numbered(char *mcMatrixH_file, char *hMatrix_file) {
    if(!adjalist)
        return 0;
    // initialize 2 matrix
    init_H_Matrixs();

    /*
     * Main Algorithms
     */
    // k : appended vertex
    for (int k = 0; k < input_size; ++k) {

        for (int i = 0; i < input_size; ++i) {
            for (int j = 0; j < input_size; ++j) {
                // renewal
                if (mcMatrixH[i][k] != INT_MAX && mcMatrixH[k][j] != INT_MAX) {
                    if (mcMatrixH[i][j] > mcMatrixH[i][k] + mcMatrixH[k][j]) {
                        mcMatrixH[i][j] = mcMatrixH[i][k] + mcMatrixH[k][j];
                        hMatrix[i][j] = k;   // highest numbered vertex in path
                    }
                }
            }
        }
    }

    make_output_file(mcMatrixH_file, hMatrix_file, mcMatrixH, hMatrix);
    return 1;
}

// Find Shortest path from hMatrix
void showH_path_cost(int from, int to) {
    printf("The sortest path from node %d to node %d : ", from, to);

    printf("%d - ", from);
    find_Hpath(from, to);
    printf("%d\n", to);

    printf("The cost of the shortest path : %d\n", mcMatrixH[from][to]);
}

// Find Shortest path from pMatrix using recursive
void find_Hpath(int from, int to) {
    if (from == to)
        return;

    int middle = hMatrix[from][to];

    if (middle != INT_MAX) {
        find_Hpath(from, middle);
        printf("%d - ", middle);
        find_Hpath(middle, to);
    }

}
