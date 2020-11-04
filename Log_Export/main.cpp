#include <iostream>
#include <windows.h>
#include <winable.h> /* Dev-C++ specific */
#include <string>
#include<time.h>
#include<stdio.h>
#include<fstream>
#include<io.h>
using namespace std;
//GLOBAIS
HWND hwndEncontrada;


/* This is a function to simplify usage of sending keys */
void GenerateKey(int vk, BOOL bExtended)
{
    KEYBDINPUT  kb = {0};
    INPUT       Input = {0};

    /* Generate a "key down" */
    if (bExtended)
    {
        kb.dwFlags  = KEYEVENTF_EXTENDEDKEY;
    }
    kb.wVk  = vk;
    Input.type  = INPUT_KEYBOARD;
    Input.ki  = kb;
    SendInput(1, &Input, sizeof(Input));

    /* Generate a "key up" */
    ZeroMemory(&kb, sizeof(KEYBDINPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    kb.dwFlags  =  KEYEVENTF_KEYUP;
    if (bExtended)
    {
        kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    }
    kb.wVk = vk;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    SendInput(1, &Input, sizeof(Input));

    return;
}

void LigaAlt()
{
    KEYBDINPUT  kb = {0};
    INPUT       Input = {0};
    BOOL bExtended = TRUE;

    /* Generate a "key down" */
    if (bExtended)
    {
        kb.dwFlags  = KEYEVENTF_EXTENDEDKEY;
    }
    kb.wVk  = VK_MENU;
    Input.type  = INPUT_KEYBOARD;
    Input.ki  = kb;
    SendInput(1, &Input, sizeof(Input));

    /* Generate a "key up" */
    ZeroMemory(&kb, sizeof(KEYBDINPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    kb.dwFlags  =  KEYEVENTF_KEYUP;
    if (bExtended)
    {
        kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    }
    kb.wVk = VK_MENU;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    SendInput(1, &Input, sizeof(Input));
}

void DesligaAlt()
{
    KEYBDINPUT  kb = {0};
    INPUT       Input = {0};
    BOOL bExtended = TRUE;

    /* Generate a "key up" */
    ZeroMemory(&kb, sizeof(KEYBDINPUT));
    ZeroMemory(&Input, sizeof(INPUT));
    kb.dwFlags  =  KEYEVENTF_KEYUP;
    if (bExtended)
    {
        kb.dwFlags |= KEYEVENTF_EXTENDEDKEY;
    }
    kb.wVk = VK_MENU;
    Input.type = INPUT_KEYBOARD;
    Input.ki = kb;
    SendInput(1, &Input, sizeof(Input));
}
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    char buffer[128];
    int written = GetWindowTextA(hwnd, buffer, 128);
    if (written && strstr(buffer, "All Events") != NULL)
    {
        hwndEncontrada = hwnd;
        return FALSE;
    }
    return TRUE;
}

void abrirjanelaencontrada(HWND hwnd)
{
                ShowWindow(hwnd, SW_MAXIMIZE);
                SetForegroundWindow(hwnd);
                SetFocus(hwnd);
                Sleep(2000);
}

int le_config(char *fonte, char *destino, char separador)
{
    strcpy(destino, "");
    char* pch;
    int deate[2] = {0,0};
    pch=strchr(fonte,separador);
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
    return 0;
}

int ler_config(int* tempo, char* str_diretorio, int* tempodevida)
{
    char linha[90];
    char comando[40];
    char argumento[40];
    FILE *arquivo = fopen("config.txt", "r+"); // cria ou abre o arquivo
    if(arquivo == NULL) // testa se o arquivo foi aberto com sucesso
    {
        cout << "\n\nLOG EXPORT - TODOS OS DIREITOS RESERVADOS\n" ;
        cout << "\n\nImpossivel abrir o arquivo de config. Coloque-o na pasta do aplicativo. \n\n";
        system("pause");
        exit(1);
    }
    while (!feof(arquivo))
    {
        fgets(linha, 100, arquivo);
        le_config(linha, comando, '=');
        le_config(linha, argumento, '"');
        if (!strcmp(comando,"TEMPO"))
        {
            *tempo = atoi(argumento);
        }
        if (!strcmp(comando,"PATH"))
        {
            strcpy(str_diretorio,argumento);
        }
        if (!strcmp(comando,"TEMPODEVIDA"))
        {
            *tempodevida = atoi(argumento);
        }
    }
    fclose(arquivo);
    return 1;
}

int main()
{

    time_t t, intervalo, ultimo_save = 0;
    struct tm *local;
    struct _finddata_t struct_arq;
    intptr_t dado;
    char buscar_em[50], str_diretorio[50];
    int tempodevida, tempo;

    ler_config(&tempo, str_diretorio, &tempodevida);
    intervalo = (time_t)tempo*3600;
    sprintf(buscar_em, "%s*.csv", str_diretorio);

    while (1)
    {
        t = time(NULL);
        local=localtime(&t);
        system("cls");
        cout << "\n\nLOG EXPORT - TODOS OS DIREITOS RESERVADOS\n\n\n" ;
        cout << "Tempo de intervalo de exportacao: " << intervalo <<" segundos" <<endl;
        cout << "Buscando no diretorio: " << buscar_em << endl;
        dado = _findfirst(buscar_em, &struct_arq);
        if (dado == -1L)
            printf("\nNenhum arquivo de log encontrado, verifique o diretorio em config.txt \n\n\n\n\n");
        else
        {
            do
            {
                long int valor_data;
                char nome_arquivo[50], restante[50];
                int dia, mes, ano;
                strcpy(nome_arquivo, struct_arq.name);
                sscanf(nome_arquivo, "%d-%d-%d-%s", &ano, &mes, &dia, &restante[0]);
                valor_data = dia+(mes*30)+ano*365;
                if ((local->tm_year+1900)*365 + (local->tm_mon+1)*30 + (local->tm_mday) > valor_data + tempodevida)
                {
                    char comando_del[50];
                    sprintf(comando_del, "del %s%d-%d-%d-%s", str_diretorio, ano, mes, dia, restante);
                    system(comando_del);
                }
            }
            while(_findnext(dado, &struct_arq) == 0);
        }
        _findclose(dado);
        EnumWindows(EnumWindowsProc, 0);
        if (hwndEncontrada == NULL)
        {
            cout << ">>>> ATENCAO <<<<\n";
            cout << "LogViewer esta fechado? \n";
            cout << "\nCertifique-se de que All Events Dinamic display esta aberto e maximizado dentro de LogViewer";
        }
        else
        {
            char nome_arquivo[30];
            if (t > (ultimo_save + intervalo))
            {
                abrirjanelaencontrada(hwndEncontrada);
                LigaAlt();
                Sleep(300);
                GenerateKey('F',FALSE);
                Sleep(300);
                GenerateKey('E',FALSE);
                Sleep(300);
                DesligaAlt();
                Sleep(300);
                HWND hwndExport = FindWindowA(NULL,"Export To...");
                if (hwndExport != NULL)
                    cout << "\n Encontrado: 'Export To...' - Handle: " << hwndExport;
                HWND child = FindWindowEx(hwndExport, NULL, "ComboBoxEx32", NULL);
                if (child != NULL)
                    cout << "\n Encontrado child 'ComboBox32' - Handle: " << child;
                HWND child2 = FindWindowEx(child, NULL, "ComboBox", NULL);
                if (child2 != NULL)
                    cout << "\n Encontrado child2 'ComboBox' - Handle: " << child2;
                HWND child3 = FindWindowEx(child2, NULL, "Edit", NULL);
                if (child3 != NULL)
                {
                    sprintf(nome_arquivo, "%s%d-%d-%d-%dh", str_diretorio, local->tm_year+1900, local->tm_mon+1, local->tm_mday, local->tm_hour);
                    cout << "\n Encontrado child3 'Edit' - Handle: " << child3;
                    time(&ultimo_save);
                    SendMessage(child3, WM_SETTEXT, 0, (LPARAM)nome_arquivo);
                    Sleep(500);
                    GenerateKey(VK_RETURN,TRUE);
                    Sleep(500);
                }
            }
            cout << "\nArquivo de log foi salvo com sucesso em " << nome_arquivo;
            cout << "\n\n";
            cout << "Proximo arquivo log sera salvo em " << ((ultimo_save + intervalo) - t) << " segundos\n\n" << endl;
        }
        Sleep(1000);
    }
    return 0;
}
