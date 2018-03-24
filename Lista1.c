// Matricula: 160014981
// Aluno: Mateus Nascimento Nóbrega

// Matricula: 160034591
// Aluno: Lucas Hiroshi Horinouchi

// Considere que cada pessoa possui um IDP (Identificador Pessoal) e atraves dele é possivel saber o local, ano e nome.
// O ID possui o seguinte formato AAA BBB CCC no qual os 3 primeiros digitos representam a cidade de nascimento, os proximos
// 3 o ano e o ultimos 3 o seu identificador pessoal com o qual e possivel encontrar o seu nome.

// Os ID das cidades são mantidos em uma lista duplamente encadeada. 
// Com os anos de nascimento sendo uniformentente distribuidos.
// E os nomes estando organizados por indices para facilitar a busca.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CARACTERES 100
#define ANO_TAM 1000
#define NOME_TAM 1000
#define CIDADE_TAM 1000

typedef struct _ano 
{
    int idAno;
    int ano;
}Ano;

typedef struct _nome
{
    int idNome;
    char nome[MAX_CARACTERES];
}Nome;

typedef struct _cidade
{
    int idCidade;
    struct _cidade* prox;
    struct _cidade* ant;
    char nomeCidade[MAX_CARACTERES];
    Nome nomes[NOME_TAM];
}Cidade;

void popularAno(Ano ano[]);
void buscarAno(Ano ano[], int tam, int idAnoBusca);

Cidade* popularCidade(Cidade *inicioCidade);
Cidade* buscarLista(Cidade* inicio, int idCidadeBuscado);

void insereLista(Cidade** inicio, int idCidade );
void listaLibera(Cidade* inicio);

void buscarNome(Cidade *cidade, int idNomeBusca);

int main() 
{
    // Declarações        
    Cidade *inicioCidade=NULL, *cidade;
    Ano ano[ANO_TAM];
    int idCidadeBusca, idNomeBusca, idAnoBusca;

    // Instruções    
    printf("Digite o ID no formato AAA BBB CCC somente numeros\n");
    scanf("%d %d %d", &idAnoBusca ,&idCidadeBusca, &idNomeBusca);

    // Buscar ano usando interpolação
    popularAno(ano);
    buscarAno(ano, ANO_TAM, idAnoBusca);
    
    // Buscar cidade usando busca sequencial
    inicioCidade = popularCidade(inicioCidade);
    cidade =  buscarLista(inicioCidade, idCidadeBusca);

    // Busca Binaria em Nomes
    buscarNome(cidade, idNomeBusca);
    
    printf("-----------------------------------------------");
    printf("\nTestes\n");
    
    printf("Casos Ideais\n");
    
    idAnoBusca = 999;
    idCidadeBusca = 999;
    idNomeBusca = 999;
    buscarAno(ano, ANO_TAM, idAnoBusca);
    cidade =  buscarLista(inicioCidade, idCidadeBusca);
    buscarNome(cidade, idNomeBusca);
    
    // Libera o espaco ocupado pela lista
    listaLibera(inicioCidade);
    
    return 0;
}

// ============================= SUBPROGRAMAS =============================

//-------------------------------------- ANO --------------------------------------
void popularAno(Ano ano[])
{
    int aux, auxIdAno = 1,auxAno = 1970;

    for(aux = 0; aux<ANO_TAM; aux++)
    {
        ano[aux].idAno = auxIdAno;
        ano[aux].ano = auxAno;
        auxAno = auxAno + 1;
        auxIdAno = auxIdAno + 1;
    }
}

void buscarAno(Ano ano[], int tam, int idAnoBusca)
{
    clock_t inicio_tempo = clock();
    int  lim_inf = 0,  lim_sup = (tam - 1), encontrou = 0;
    
    while ( lim_inf <=  lim_sup && idAnoBusca >=  ano[ lim_inf].idAno && idAnoBusca <=  ano[ lim_sup].idAno)
    {
        int pos =  lim_inf + (((double)( lim_sup- lim_inf) /
              ( ano[lim_sup].idAno - ano[ lim_inf].idAno))*(idAnoBusca -  ano[ lim_inf].idAno));
 
        // Se encontrar
        if ( ano[pos].idAno == idAnoBusca)
        {
            printf("Ano de nascimento %d\n",ano[pos].ano);
            encontrou = 1;   
        }

        // Se idAnoBusca for maior esta para cima
        if ( ano[pos].idAno < idAnoBusca)
             lim_inf = pos + 1;
        // Se idAnoBusca for menor esta para baixo
        else
             lim_sup = pos - 1;
    }
    if(encontrou == 0)
        printf("Ano não encontrado.\n");
    clock_t fim_tempo = clock();
    double tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
    printf("Segundos gastos %f Interpolação \n\n", tempo_gasto);
}

//-------------------------------------- CIDADE --------------------------------------
Cidade* popularCidade(Cidade *inicioCidade)
{
    int auxIdCidade = 1, aux;
    
    for(aux = 0; aux<CIDADE_TAM; aux++)
    {
        insereLista(&inicioCidade, auxIdCidade);
        auxIdCidade = auxIdCidade + 1;
    }  
    return inicioCidade;  
}

void insereLista(Cidade** inicio, int idCidade)
{
    char nomeCidade[MAX_CARACTERES], nome[MAX_CARACTERES];
    Cidade* cidade;
    int aux, auxIdNome=1, auxNome = 999;

    
    cidade = (Cidade*) malloc(sizeof(Cidade));
    cidade->idCidade=idCidade;
    sprintf(nomeCidade, "%d", idCidade/3);
    strcpy(cidade->nomeCidade, nomeCidade);
    for(aux=0;aux<NOME_TAM;aux++)
    {
        cidade->nomes[aux].idNome = auxIdNome;
        sprintf(nome, "%d", auxNome);
        strcpy(cidade->nomes[aux].nome, nome);
        auxIdNome = auxIdNome + 1;
        auxNome = auxNome + 7;
    }
    if(*inicio == NULL)
    {
        cidade->ant = NULL;
        cidade->prox = NULL;
    }
    else
    { 
        cidade->prox = *inicio;
        cidade->ant = NULL;
        (*inicio)->ant = cidade;
    }
    *inicio = cidade;  
}

Cidade*  buscarLista(Cidade* inicio, int idCidadeBuscado) 
{ 
    clock_t inicio_tempo = clock();
    Cidade* aux;
    int encontrou = 0;
    for (aux=inicio; aux!=NULL; aux=aux->prox)
    {
        if(aux->idCidade==idCidadeBuscado)
        {
            printf("Cidade Nascimento %s\n", aux->nomeCidade);
            encontrou = 1;
            clock_t fim_tempo = clock();
            double tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
            printf("Segundos gastos %f Sequencial\n\n", tempo_gasto);
            return aux;
        }
    }
    if(encontrou == 0)    
        printf("Cidade não encontrada.\n");
    clock_t fim_tempo = clock();
    double tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
    printf("Segundos gastos %f Sequencial \n\n", tempo_gasto);
    return aux;
}

void listaLibera(Cidade* inicio)
{
    Cidade* aux = inicio;
    while(aux!=NULL)
    {
        Cidade* libera =aux->prox;
        free(aux);
        aux = libera; 
    }
}


//-------------------------------------- NOME --------------------------------------
void buscarNome (Cidade *cidade, int idNomeBusca)
{
    clock_t inicio_tempo = clock();
    int lim_sup = NOME_TAM-1, lim_inf = 0, meio, encontrou = 0; 

    if(cidade==NULL)
    {
        printf("Nome não encontrado.\n");
    }else
    {
        while (lim_inf <= lim_sup)
        {
            meio = (lim_inf + lim_sup)/2;
            if (idNomeBusca == cidade->nomes[meio].idNome)
            {
                printf("Nome %s\n",cidade->nomes[meio].nome);
                encontrou = 1;
            }
            if (idNomeBusca < cidade->nomes[meio].idNome)
                lim_sup = meio-1;
            else
                lim_inf = meio+1;
        }
        if(encontrou==0)
            printf("Nome não encontrado.\n");
    }
    clock_t fim_tempo = clock();
    double tempo_gasto = (double)(fim_tempo - inicio_tempo) / CLOCKS_PER_SEC;
    printf("Segundos gastos %f Busca Binaria \n\n", tempo_gasto);
}
