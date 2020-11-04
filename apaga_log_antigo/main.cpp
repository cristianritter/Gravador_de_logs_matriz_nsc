#include <iostream>
#include <fstream>
#include <string.h>
#include<stdio.h>
#include<io.h>
#include<stdlib.h>
using namespace std;

 char str_caminho[40] = "";


int le_config(char *fonte, char *destino, char separador)
{
    strcpy(destino, "");
    char* pch;
    int deate[2] {0,0};
    pch=strchr(fonte,separador);
    //  if (pch==NULL) cout << "nulo";
    int i = 0;
    if (separador=='=') i=1;
    while (pch!=NULL)
    {
        deate[i]=pch-fonte+1;
        i++;
        pch=strchr(pch+1,separador);
    }
    for (int i=deate[0]; i<deate[1]-1; i++)
    {
        char temp[5];
        *temp = fonte[i];
        strncat(destino, temp, 1);
    }
     //  cout << destino <<endl;
    return 0;
}

int copia_trecho(char *fonte, char *destino, int inicio, int fim)
{
    strcpy(destino, "");
    for (int i=inicio; i<fim; i++)
    {
        char temp[5];
        *temp = fonte[i];
        strncat(destino, temp, 1);
    }
 //   cout << destino;
    return 0;
}

int main()
{
    char linha[80];
    char comando[40];
    char argumento[40];

    FILE *arquivo = fopen("config.txt", "r+"); // cria ou abre o arquivo
    if(arquivo == NULL) // testa se o arquivo foi aberto com sucesso
    {
        printf("\n\nImpossivel abrir o arquivo!\n\n");
        return 0;
    }

    char Linha[100];

    while (!feof(arquivo))
    {
        fgets(linha, 100, arquivo);
        le_config(linha, comando, '=');
        le_config(linha, argumento, '"');
        cout << "\nCOMANDO:" <<comando;
        cout << "\nARGUMENTO:"<<argumento <<endl;

        if (!strcmp(comando, "PATH")) strcpy(str_caminho, argumento);

    }
    fclose(arquivo);


    struct _finddata_t struct_arq;
    intptr_t dado;
    dado = _findfirst(str_caminho, &struct_arq);
    if (dado == -1L)
        printf("\nnenhum arquivo encontrado\n");
    else
    {
        do
        {
         long int valor_data;
         char nome_arquivo[40], data;
         int dia, mes, ano;
            printf("\n%s\n",struct_arq.name);
            strcpy(nome_arquivo, struct_arq.name);
            cout << nome_arquivo << endl;
         sscanf(nome_arquivo, "%d-%d-%d - %*s", &ano, &mes, &dia);
         valor_data = dia+(mes*30)+ano*365;
         cout << valor_data <<endl;
         }
        while(_findnext(dado, &struct_arq) == 0);
    }

    system("pause");

    return 0;
}
