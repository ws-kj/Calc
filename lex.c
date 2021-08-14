#include <stdio.h>
#include <regex.h>
#include <string.h>
#include <stdlib.h>

#include "lex.h"

//nums 48-57 uppercase 65-90 lowercase 97-122                 

enum Tokens {
    T_INT,
    T_SPACE = ' ',
    T_ADD = '+',
    T_SUB = '-',
    T_DIV = '/',
    T_MUL = '*',
    T_LPAREN = '(',
    T_RPAREN = ')',
    T_SEMIC = ';'
};

struct Node {
    struct Node* left;
    struct Node* right;
    int token;
    char* rval;
};

//reverse of real order for depth generation
int tree_prec(int t1, int t2) {
    int n1, n2;
    switch(t1) {
        case T_LPAREN:
            n1=2;
            break;
        case T_RPAREN:
            n1=2;
            break;
        case T_DIV:
            n1=1;
            break;
        case T_MUL:
            n1=1;
            break;
        case T_ADD:
            n1=0;
            break;
        case T_SUB:
            n1=0;
            break;
        case T_INT:
            n1=-1;
            break;
        default:
            return -1;
    }
    switch(t2) {
        case T_LPAREN:
            n2=2;
            break;
        case T_RPAREN:
            n2=2;
            break;
        case T_DIV:
            n2=1;
            break;
        case T_MUL:
            n2=1;
            break;
        case T_ADD:
            n2=0;
            break;
        case T_SUB:
            n2=0;
            break;
        case T_INT:
            n2=-1;
            break;
        default:
            return -1;
    }

    if(n1 == -1 && n2 != -1) return 0;
    if(n2 == -1 && n1 != -1) return 1;
    if(n1 == -1 && n2 == -1) {
        printf("syntax error: missing operation\n");
        return -1;
    }

    if(n1 <= n2) return 1;
    return 0;

}

struct Node* build_tree(struct Node** list, int size) {
    struct Node* root = list[0];

    if(size == 1) return root;

    int min = 0;

    for(int i=0; i<size; i++) {
        if(list[i]->token != T_INT) {
            min = i;
            break; 
        }
    }

    for(int i=1; i<size; i++) {
        if(list[i]->token == T_INT) continue;
        if(tree_prec(list[i]->token, root->token)) min=i;
       root = list[min];
    }
    root = list[min];
    struct Node** left = malloc(sizeof(struct Node*) * min);
    for(int i=0; i<min; i++) { left[i] = list[i]; }
    struct Node** right = malloc(sizeof(struct Node*) * ((size-1)-min));
    int j=0;
    for(int i=min+1; i<size; i++) { right[j] = list[i]; j++; }
    root->left = build_tree(left, min);
    root->right = build_tree(right, (size-1)-min);
    return root;
}

int insert_node(struct Node* next) {
    if(next == NULL) return 1;
    return 0;
}

int tokenize(char* input) {
    //count tokens
    int tc;
    for(int i=0; i<strlen(input); i++) {
        char c = input[i];
        if(c == ' ' || c == '\0') {
        
        }  else if(c >= 48 && c <= 57) {
            int j=i;
            while(input[j] >= 48 && input[j] <= 57) {
                j += 1;
            }
            i = j-1;
            tc += 1;
        } else {
            tc += 1;
        }

    }
    struct Node* nodes[tc];

    //find tokens
    int nc;
    for(int i=0; i<strlen(input); i++) {
        char c = input[i];
        if(c == ' ' || c == '\0') {
        
        }  else if(c >= 48 && c <= 57) {
            int count=0;
            int j=i;
            while(input[j] >= 48 && input[j] <= 57) {
                count++;
                j += 1;
            }
            char* nstr = malloc(sizeof(char) * count);
            int ind = 0;
            for(int k=i; k<i+count; k++) {
                nstr[ind] = input[k];
                ind++;
            }
            i = j-1;
            struct Node* node = malloc(sizeof(struct Node));
            node->right = NULL;
            node->left = NULL;
            node->token = T_INT;
            node->rval = nstr;
            nodes[nc] = node;
            nc++;
        } else {
            int tok;
            switch(c) {
                case T_SPACE:
                    break;
                case T_ADD:
                    printf("OP_ADD ");
                    tok = T_ADD;
                    break;
                case T_SUB:
                    printf("OP_SUB ");
                    tok = T_SUB;
                    break;
                case T_DIV:
                    printf("OP_DIV ");
                    tok = T_DIV;
                    break;
                case T_MUL:
                    printf("OP_MUL ");
                    tok = T_MUL;
                    break;
                case T_LPAREN:
                    printf("LPAREN ");
                    tok = T_LPAREN;
                    break;
                case T_RPAREN:
                    printf("RPAREN ");
                    tok = T_RPAREN;
                    break;
                case T_SEMIC:
                    printf("SEMIC\n");
                    tok = T_SEMIC;
                    break;
                default:
                    printf("Unrecognized character: %c\n", c);
                    return 1;
            }
            struct Node* node = malloc(sizeof(struct Node*));
            node->token = tok;
            nodes[nc] = node;

            nc++;
        }

    }
    printf("\nTokens: %d\n", tc);

    struct Node* root = build_tree(nodes, tc);
    printf("%c ", root->token);
    return 0;
}

int main(int argc, char** argv) {

    char* example = "1 + 3 * 2";
    printf("Example: %s\n", example);
    tokenize(example);

    return 0;
}
