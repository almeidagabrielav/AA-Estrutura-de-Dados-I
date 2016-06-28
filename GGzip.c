#include <stdio.h>
#include <stdlib.h>

typedef struct NO No;

struct NO{
    char simbolo;
    int frequencia;

    No* fesq;
    No* fdir;
};

typedef struct ARVORE{
    No* raiz;
} Arvore;

typedef struct ITEM{
    char simbolo;
    char* codigo;
} Item;

No* criar_no(char simbolo, int frequencia){
    No* no = (No*)malloc(sizeof(No));

    if(no == NULL){
        puts("Nao foi possivel criar um No");
        exit(1);
    }

    no->simbolo = simbolo;
    no->frequencia = frequencia;
    no->fesq = NULL;
    no->fdir = NULL;

    return no;
}

Arvore* criar_arvore(){
    Arvore* arvore = (Arvore*)malloc(sizeof(Arvore));

    if(arvore == NULL){
        puts("Nao foi possivel criar Arvore");
        exit(1);
    }

    return arvore;
}

Item* criar_item(){
    Item* item = (Item*)malloc(sizeof(Item));

    if(item == NULL){
        puts("Não foi possivel criar um Item");
        exit(1);
    }

    item->simbolo = (char)0;
    item->codigo = NULL;

    return item;
}

void organiza(No* nos[], int tamanho){
    int i, troca;
    No* paux;

    do{
        troca = 0;
        for(i = 0; i < tamanho - 1; i++){
            if(nos[i]->frequencia < nos[i+1]->frequencia){
                paux = nos[i+1];
                nos[i+1] = nos[i];
                nos[i] = paux;
                troca = 1;
            }
        }
    }while(troca);
}

Arvore* arvore_ruffman(No* vetor[],int tamanho){
    if(tamanho > 1){
        int i;
        while(vetor[1] != NULL){
            No* no = criar_no((char)0, vetor[tamanho-2]->frequencia+vetor[tamanho-1]->frequencia);
            no->fesq = vetor[tamanho-1];
            no->fdir = vetor[tamanho-2];
            vetor[tamanho-2] = no;
            vetor[tamanho-1] = NULL;
            tamanho--;
            organiza(vetor,tamanho);
        }
    }

    Arvore* arvore = criar_arvore();
    arvore->raiz = vetor[0];

    return arvore;
}

void exibirEmOrdem(No *pRaiz){
    if(pRaiz != NULL){
        exibirEmOrdem(pRaiz->fesq);
        printf("%c", pRaiz->simbolo);
        exibirEmOrdem(pRaiz->fdir);
    }
}

int altura(No* raiz) {
   if (raiz == NULL)
      return -1;
   else {
      int he = altura(raiz->fesq);
      int hd = altura(raiz->fdir);
      if (he < hd) return hd + 1;
      else return he + 1;
   }
}

void criar_codigo_aux(Item* item[], No* raiz, char codigo[], int fim){
    if(raiz->fesq == NULL && raiz->fdir == NULL){
        int i, posicao = 0;
        while(1){
            if(item[posicao] == NULL){
                item[posicao] = criar_item();
                break;
            }
            posicao++;
        }

        item[posicao]->simbolo = raiz->simbolo;

        char *aux = (char*)malloc((fim+2)*sizeof(char));

        if(aux == NULL){
            printf("Erro ao criar cadeia de caracteres");
            exit(1);
        }

        for(i = 0; i <= fim; i++){
            aux[i] = codigo[i];
        }

        aux[fim+1] = '\0';
        item[posicao]->codigo = aux;

    }else{
        if(raiz->fesq != NULL){
            fim++;
            codigo[fim] = '0';
            criar_codigo_aux(item,raiz->fesq,codigo,fim);
            fim--;
        }

        if(raiz->fdir != NULL){
            fim++;
            codigo[fim] = '1';
            criar_codigo_aux(item,raiz->fdir,codigo,fim);
            fim--;
        }
    }
}


void criar_codigo(Arvore* arvore, Item* item[]){
    int tamanho = altura(arvore->raiz);
    char codigo[tamanho];
    criar_codigo_aux(item,arvore->raiz,codigo,-1);
}

unsigned char gerar_codigo(char save_aux[]){
    unsigned char save = 0;
    if(save_aux[0] == '1') save += 128;
    if(save_aux[1] == '1') save += 64;
    if(save_aux[2] == '1') save += 32;
    if(save_aux[3] == '1') save += 16;
    if(save_aux[4] == '1') save += 8;
    if(save_aux[5] == '1') save += 4;
    if(save_aux[6] == '1') save += 2;
    if(save_aux[7] == '1') save += 1;

    return save;
}

int armazena_codigo(FILE *file,FILE* saida, Item * item[], int quantidade){
    rewind(file);
    char letra, save_aux[9];
    save_aux[8] = '\0';
    unsigned char save;
    int i=0,y=0,save_aux_fim = 0,tamanho = 0;

    for(fscanf(file,"%c",&letra) ; !feof(file) ; fscanf(file,"%c",&letra)){
        for(i = 0; i < quantidade ; i++){
            if(item[i]->simbolo == letra){
                for(y = 0; item[i]->codigo[y] != '\0'; y++){
                    save_aux[save_aux_fim] = item[i]->codigo[y];
                    tamanho++;
                        if(save_aux_fim == 7){
                            save = gerar_codigo(save_aux);
                            fwrite(&save,sizeof(unsigned char),1,saida);
                            save_aux_fim = 0;
                        }
                        else
                            save_aux_fim++;
                }
            }
        }
    }

    if(save_aux_fim != 0){
        save = gerar_codigo(save_aux);
        fwrite(&save,sizeof(unsigned char),1,saida);
    }

    return tamanho;
}

void compactacao(int T_arquivo,int quantidade, int tamanho){
    printf("%d %d %d",T_arquivo,quantidade,tamanho);
    double comp1 = (1 - (double)(tamanho)/(T_arquivo*8))*100;
    double comp2 = (1 - (double)((sizeof(int)+quantidade*(sizeof(int)+sizeof(char)))*8 + tamanho)/(T_arquivo*8))*100;

    printf("Compactacao 1: %.2lf %%",comp1);
    printf("\nCompactacao 2: %.2lf %%\n",comp2);
}

void decodificar_aux(char codigo[], unsigned char letra){
  if(letra/128 == 1) codigo[0] = '1'; else codigo[0] = '0';
  if((letra%128)/64 == 1) codigo[1] = '1'; else codigo[1] = '0';
  if(((letra%128)%64)/32 == 1) codigo[2] = '1'; else codigo[2] = '0';
  if((((letra%128)%64)%32)/16)  codigo[3] = '1'; else codigo[3] = '0';
  if(((((letra%128)%64)%32)%16)/8) codigo[4] = '1'; else codigo[4] = '0';
  if((((((letra%128)%64)%32)%16)%8)/4 == 1) codigo[5] = '1'; else codigo[5] = '0';
  if(((((((letra%128)%64)%32)%16)%8)%4)/2 == 1) codigo[6] = '1'; else codigo[6] = '0';
  if((((((((letra%128)%64)%32)%16)%8)%4)%2)/1) codigo[7] = '1'; else codigo[7] = '0';
}

void decodificar(FILE *file, Arvore *arvore,char nome[]){
    char codigo[8],letra;
    int qLetras = arvore->raiz->frequencia;
    FILE *descompactado = fopen(nome,"w");

    int fim;
    No* no = arvore->raiz;
    while(qLetras > 0){

        fread(&letra,sizeof(unsigned char),1,file);
        decodificar_aux(codigo,letra);

        for(fim = 0; fim < 8; fim++){
            if(qLetras == 0)
                break;

            if(codigo[fim] == '1')
                no = no->fdir;
            else
                no = no->fesq;

            if(no->fesq == NULL && no->fdir == NULL){
                fprintf(descompactado,"%c",no->simbolo);
                no = arvore->raiz;
                qLetras--;
            }
        }
    }

}

void main(int argc, char *argv[]){
    if(argc == 3){
        FILE *file = fopen(argv[1],"r");

        if(file == NULL){
            puts("Arquivo nao encontrado\n");
            exit(1);
        }

        FILE *saida = fopen(argv[2],"wb");

        puts("Compactando Arquivo");
        int *vetor = (int*)calloc(128,sizeof(int));

        if(vetor == NULL){
            puts("Nao foi possivel criar vetor");
            exit(1);
        }

        char letra;

        int quantidade;
        for(quantidade = 0, fscanf(file,"%c",&letra) ; !feof(file) ; fscanf(file,"%c",&letra)){
            if(letra >=0 && letra <= 127){
                if(vetor[letra] == 0)
                    quantidade++;
                vetor[letra]++;
            }
        }

        No* nos[quantidade];
        int i,y;

        for(i = 0, y = 0; i < 128; i++){
            if(vetor[i]!= 0){
                nos[y] = criar_no((char)i,vetor[i]);
                y++;
            }
        }

        free(vetor);
        organiza(nos,quantidade);
        fwrite(&quantidade,sizeof(int),1,saida);
        for(i = 0; i < quantidade; i++){
            unsigned char auxchar = (unsigned char)nos[i]->simbolo;
            unsigned int auxint = (unsigned int)nos[i]->frequencia;
            fwrite(&auxchar,sizeof(unsigned char),1,saida);
            fwrite(&auxint,sizeof(unsigned int),1,saida);
        }

        Arvore* arvore = arvore_ruffman(nos,quantidade);

        Item *item[quantidade];
        for(i = 0; i < quantidade ;i++)
            item[i] = NULL;

        criar_codigo(arvore,item);

        int tamanho = armazena_codigo(file,saida,item,quantidade);

        compactacao(arvore->raiz->frequencia,quantidade,tamanho);

    } else if(argc == 2){
        FILE *file = fopen(argv[1],"rb");

        if(file == NULL){
            printf("Arquivo nao encontrado");
            exit(1);
        }
        int i, quantidade;
        char nome[50];

        for(i = 0; argv[1][i] != '\0'; i++){
            nome[i] = argv[1][i];
        }
        nome[i-4] = '.';
        nome[i-3] = 't';
        nome[i-2] = 'x';
        nome[i-1] = 't';
        nome[i] = '\0';


        puts("Descompactando Arquivo");

        fread(&quantidade,sizeof(int),1,file);
        No *vetor[quantidade];
        for(i = 0; i < quantidade; i++){
            char auxchar;
            int auxint;
            fread(&auxchar,sizeof(char),1,file);
            fread(&auxint,sizeof(int),1,file);
            vetor[i] = criar_no(auxchar,auxint);
        }

        Arvore *arvore = arvore_ruffman(vetor,quantidade);

        decodificar(file,arvore,nome);

    } else
        puts("Entrada invalida!");
}
