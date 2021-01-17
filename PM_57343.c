/*
 
 TRABALHO REALIZADO POR:
 
        @Marco  57343
 
    PM 2020-2021 - Gestão Maritima
 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_FILENAME 30
#define MAX_NAVIOS   100
#define MAX_PORTOS   54
#define MAX_VIAGENS  27
#define MAX_NOMES    100
#define MAX_TIPO     15

#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_RESET   "\x1b[0m"

/*-********************************************************************************************************************************************************************************
 _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____
|   __|  |  |   | |     |   __|   __|  |   | |  _  |  |  |     |     |   __|
|   __|  |  | | | |  |  |   __|__   |  | | | |     |  |  |-   -|  |  |__   |
|__|  |_____|_|___|_____|_____|_____|  |_|___|__|__|\___/|_____|_____|_____|
 
********************************************************************************************************************************************************************************-*/
void fimFuncoes(){
    printf("Prima enter para seguir...\n");
    getpass("");
    system("clear");
    /*getchar();
    printf("Prima a tecla enter para seguir...");
    *ch = getchar();
    while(*ch != '\n'){
        *ch = getchar();
    }*/
}
/*-**************************************************************
Estrutura _navio - estrutura que armazena todas informacões
de um determinado navio
******************************************-***********************/
struct _navio{
    int identidade;
    char *nome;
    char *tipo;
    int capacidade;
    int ano;
    char *pais;
    float combustivel;
    float custo;
    struct _navio *proxNavio;
};

typedef struct _navio* Navio; //passa a definir Navio como um apontador para a variavel navio

/*-**************************************************************
criaNavio - função que inicializa todas as variáveis da estrutura
associada a cada um dos navios
Parametros: identidade - identidade a associar ao navio
            nome - nome a associar ao navio
            tipo - tipo a associar ao navio
            capacidade - capacidade associada ao navio
            ano - ano a associar ao navio
            pais - pais de origem do navio
            combustivel - combustivel necessario ao navio
            custo - custo associado ao combustivel do navio
Retorno: n - se o navio for criado com sucesso (objeto com tudo do navio)
         NULL - se houver algum erro na alocação de memoria
***************************************************************-*/
Navio criaNavio(int identidade, char *nome, char *tipo, int capacidade, int ano, char *pais, float combustivel, float custo){
    Navio n;
    n = (Navio)malloc(sizeof(struct _navio));
    if (n == NULL){
        free(n);
        return NULL;
    }
    
    n -> nome = (char*)malloc(strlen(nome) + 1); //alocar memoria para o nome do navio e espaço para o NULL
    if (n -> nome == NULL){
        free(n -> nome);
        free(n);
        return NULL;
    }
    
    n -> tipo = (char*)malloc(strlen(tipo) + 1); //alocar memoria para o tipo do navio e espaço para o NULL
    if (n -> tipo == NULL){
        free(n -> tipo);
        free(n -> nome);
        free(n);
        return NULL;
    }
    
    n -> pais = (char*)malloc(strlen(pais) + 1); //alocar memoria para o pais do navio e espaço para o NULL
    if (n -> pais == NULL){
        free(n -> pais);
        free(n -> tipo);
        free(n -> nome);
        free(n);
        return NULL;
    }
    
    n -> identidade = identidade;
    
    strcpy(n -> nome, nome);
    
    strcpy(n -> tipo, tipo);
    
    n -> capacidade = capacidade;
    
    n -> ano = ano;
    
    strcpy(n -> pais, pais);
    
    n -> combustivel = combustivel;
    
    n -> custo = custo;
    
    n -> proxNavio = NULL;
    
    return n;
}



/*-**************************************************************
leuNavios - função que trata ler o ficheiro com as informações dos
navios e organizar tudo nas estruturas respetivas
Parametros: ficheiroNavios - a função recebe como parametro o nome
            onde vai buscar todas as informações de certo navio
            navios - recebe um apontador para a cabeça da lista de estruturas
Retorno: 0 - se houve algum erro a ler o ficheiro dos navios
         1 - se sucedeu na leitura do ficheiro dos navios
******************************************-***********************/
int leuNavios(char *ficheiroNavios, Navio *navios, int *contadorNavios){
    FILE *fp; //apontador para o ficheiro que se pretende abrir
    Navio navio;
    int controloFim;
    int identidade;
    char nome[MAX_NOMES];
    char tipo[MAX_NOMES];
    int capacidade;
    int ano;
    char pais[MAX_NOMES];
    float combustivel;
    float custo;
    
    fp = fopen(ficheiroNavios, "r");
    
    //fp não abriu ficheiro por não o ter encontrado
    if (fp == NULL)
        return 0;
    
    if(fscanf(fp, "%d;%[^;];%[^;];%d;%d;%[^;];%f;%f;", &identidade, nome, tipo, &capacidade, &ano, pais, &combustivel, &custo) != 8)
        return 0;
    
    if((*navios = criaNavio(identidade, nome, tipo, capacidade, ano, pais, combustivel, custo)) == NULL){
        printf(COLOR_RED "Erro ao atribuir cabeça\n" COLOR_RESET);
        return 0;
    }
    else
        (*contadorNavios)++;
    
    navio = *navios; //à variavel navio é atribuida a cabeça da lista de estruturas ligadas
    
    while(feof(fp) != EOF){ //ler o ficheiro
        if((controloFim = fscanf(fp, "%d;%[^;];%[^;];%d;%d;%[^;];%f;%f;", &identidade, nome, tipo, &capacidade, &ano, pais, &combustivel, &custo)) != 8){
            if(controloFim == EOF)
                break;
            return 0;
        }
        if((navio -> proxNavio = criaNavio(identidade, nome, tipo, capacidade, ano, pais, combustivel, custo)) == NULL)
            return 0;
        (*contadorNavios)++;
        navio = navio -> proxNavio;
    }
    
    fclose(fp);
    return 1;
}

/*-**************************************************************
 devolveNavio - função que devolve o navio com o id correspondente
 ao recebido como parâmetro
 Parametros: navios - apontador para a cabeça da lista de estruturas
             idNavio - id no navio que se pretende devolver
 Devolve: navios - navio apontado por navios
          NULL - id nao encontrado
******************************************-**********************/
Navio devolveNavio(Navio navios, int idNavio){
    while(navios != NULL){
        if(navios -> identidade == idNavio)
            return navios;
        navios = navios -> proxNavio;
    }
    return NULL;
}

/*-**************************************************************
 existeNavio - função que verifica se existe um navio com uma
 identidade respetiva à recebida em parametro
 Parametros: navios - apontador para a cabeça da lista de estruturas
             idNavio - navio que se pretende verificar se
             existe ou nao
 Devolve: 1 - caso o navio seja encontrada na lista
          0 - caso o navio não exista na lista
******************************************-**********************/
int existeNavio(Navio navios, int idNavio){
    while(navios != NULL){
        if(navios -> identidade == idNavio)
            return 1;
        navios = navios -> proxNavio;
    }
    return 0;
}

/*-**************************************************************
 devolveTipoNavio - função que dada uma identidade de um navio
 devolve (com recurso a função devolveNavio) o tipo de navio
 presente na estrutura de dados do navio
 Parametros: navios - apontador para a cabeça da lista de esturturas
             idNavio - navio que se pretende encontrar e devolver tipo
 Devolve: navio -> tipo, tipo do navio encontrado
******************************************-**********************/
char *devolveTipoNavio(Navio navios, int idNavio){
    Navio navio = devolveNavio(navios, idNavio); //navio tem o navio com o tipo
    return navio -> tipo;
}

/*-**************************************************************
 listarNavio - função que vai percorrer a lista ligada começando na
 cabeça da lista ligada e percorrendo todos os elementos até nao
 ter proximo
 Parametros: navios - apontador para a cabeça da lista de estruturas
******************************************-***********************/
void listarNavios(Navio navios){
    while(navios != NULL){
        printf(COLOR_GREEN "Id:" COLOR_RESET " %5d | " COLOR_GREEN"Nome:" COLOR_RESET" %25s | " COLOR_GREEN "Tipo:" COLOR_RESET " %14s | " COLOR_GREEN "Capacidade:" COLOR_RESET " %5d | " COLOR_GREEN "Ano: " COLOR_RESET " %4d | " COLOR_GREEN "Pais:" COLOR_RESET " %40s | " COLOR_GREEN "Combustivel:" COLOR_RESET " %8.2f |" COLOR_GREEN " Custo: " COLOR_RESET "%8.2f\n", navios -> identidade, navios -> nome, navios -> tipo, navios -> capacidade, navios -> ano, navios -> pais, navios -> combustivel, navios -> custo);
        navios = navios -> proxNavio;
    }
    fimFuncoes();
}

/*-********************************************************************************************************************************************************************************
_____ _____ _____    _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____
|   __|     |     |  |   __|  |  |   | |     |   __|   __|  |   | |  _  |  |  |     |     |   __|
|   __|-   -| | | |  |   __|  |  | | | |  |  |   __|__   |  | | | |     |  |  |-   -|  |  |__   |
|__|  |_____|_|_|_|  |__|  |_____|_|___|_____|_____|_____|  |_|___|__|__|\___/|_____|_____|_____|
 
********************************************************************************************************************************************************************************-*/
                                                                                                





/*-********************************************************************************************************************************************************************************
_____ _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____
|   __|  |  |   | |     |     |   __|   __|  |  _  |     | __  |_   _|     |   __|
|   __|  |  | | | |   --|  |  |   __|__   |  |   __|  |  |    -| | | |  |  |__   |
|__|  |_____|_|___|_____|_____|_____|_____|  |__|  |_____|__|__| |_| |_____|_____|
 
********************************************************************************************************************************************************************************-*/

/*-**************************************************************
Estrutura _porto - estrutura que armazena todas informacões
de um determinado porto
******************************************-***********************/
struct _porto{
    int identidade;
    char *nome;
    char *pais;
    float custo;
    struct _porto *proxPorto;
};

typedef struct _porto* Porto;

/*-**************************************************************
criaPorto - função que inicializa todas as variáveis da estrutura
associada a cada um dos portos
Parametros: identidade - identidade a associar ao porto
            nome - nome a associar ao porto
            pais - pais de origem do porto
            custo - custo associado a estadio de certo navio no porto
Retorno: p - se o porto for criado com sucesso (objeto com tudo do porto)
         NULL - se houver algum erro na alocação de memoria
***************************************************************-*/
Porto criaPorto(int identidade, char *nome, char *pais, float custo){
    Porto p;
    
    p = (Porto)malloc(sizeof(struct _porto));
    if (p == NULL){
        free(p);
        return NULL;
    }
    
    p -> identidade = identidade;

    p -> nome = (char*)malloc(strlen(nome) + 1);
    if (p -> nome == NULL){
        free(p -> nome);
        free(p);
        return NULL;
    }
    
    strcpy(p -> nome, nome);
    
    p -> pais = (char*)malloc(strlen(pais) + 1);
    if(p -> pais == NULL){
        free(p -> nome);
        free(p -> pais);
        return NULL;
    }
    
    strcpy(p -> pais, pais);
    
    p -> custo = custo;
    
    p -> proxPorto = NULL;
    
    return p;
}

/*-**************************************************************
leuPortos - função que trata ler o ficheiro com as informações dos
portos e organizar tudo nas estruturas respetivas
Parametros: ficheiroPortos - a função recebe como parametro o nome
            onde vai buscar todas as informações de certo porto
            portos - recebe um apontador para a cabeça da lista de estruturas
Retorno: 0 - se houve algum erro a ler o ficheiro dos portos
         1 - se sucedeu na leitura do ficheiro dos portos
******************************************-***********************/
int leuPortos(char *ficheiroPortos, Porto *portos, int *contadorPortos){
    FILE *fp;
    Porto porto;
    int controloFim;
    int identidade;
    char nome[MAX_NOMES];
    char pais[MAX_NOMES];
    float custo;

    fp = fopen(ficheiroPortos, "r");

    if(fp==NULL){
        printf("Erro ao abrir ficheiro\n");
        return 0;
    }
    
    if(fscanf(fp, "%d;%[^;];%[^;];%f;", &identidade, nome, pais, &custo) != 4){
        return 0;
    }
        
    if((*portos = criaPorto(identidade, nome, pais, custo)) == NULL){
        printf("Erro ao atribuir cabeça\n");
        return 0;
    }
    else
        (*contadorPortos)++;
    
    porto = *portos; //à variavel navio é atribuida a cabeça da lista de estruturas ligadas
    
    while(feof(fp) != EOF){ //ler o ficheiro
        if((controloFim = fscanf(fp, "%d;%[^;];%[^;];%f;", &identidade, nome, pais, &custo)) != 4){
            if(controloFim == EOF)
                break;
            return 0;
        }
        if((porto -> proxPorto = criaPorto(identidade, nome, pais, custo)) == NULL)
            return 0;
        (*contadorPortos)++;
        porto = porto -> proxPorto;
    }
    fclose(fp);
    return 1;
}

/*-**************************************************************
 listarNavio - função que vai percorrer a lista ligada começando na
 cabeça da lista ligada e percorrendo todos os elementos até nao
 ter proximo
 Parametros: portos - apontador para a cabeça da lista de estruturas
******************************************-***********************/
void listarPortos(Porto portos){
    while(portos != NULL){
        printf(COLOR_GREEN "Id: " COLOR_RESET "%3d | " COLOR_GREEN "Nome: " COLOR_RESET "%40s | " COLOR_GREEN "Pais:" COLOR_RESET " %40s | " COLOR_GREEN "Custo:" COLOR_RESET " %12.2f\n", portos -> identidade, portos -> nome, portos -> pais, portos -> custo);
        portos = portos -> proxPorto;
    }
    fimFuncoes();
}

/*-**************************************************************
 existePorto - função que verifica se existe um porto com uma
 identidade respetiva à recebida em parametro
 Parametros: portos - apontador para a cabeça da lista de estruturas
             identidadePorto - porto que se pretende verificar se
             existe ou nao
 Devolve: 1 - caso o porto seja encontrada na lista
          0 - caso o porto não exista na lista
******************************************-**********************/
int existePorto(Porto portos, int identidadePorto){
    while(portos != NULL){
        if(portos -> identidade == identidadePorto)
            return 1;
        portos = portos -> proxPorto;
    }
    return 0;
}

/*-**************************************************************
 devolvePorto - função que devolve o porto com o id correspondente
 ao recebido como parâmetro
 Parametros: portos - apontador para a cabeça da lista de estruturas
             identidadePorto - id do porto que se pretende devolver
 Devolve: portos - porto encontrado
          NULL - id nao encontrado
******************************************-**********************/
Porto devolvePorto(Porto portos, int identidadePorto){
    while(portos != NULL){
        if(portos -> identidade == identidadePorto)
            return portos;
        portos = portos -> proxPorto;
    }
    return NULL;
}

/*-********************************************************************************************************************************************************************************
 _____ _____ _____    _____ _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____
|   __|     |     |  |   __|  |  |   | |     |     |   __|   __|  |  _  |     | __  |_   _|     |   __|
|   __|-   -| | | |  |   __|  |  | | | |   --|  |  |   __|__   |  |   __|  |  |    -| | | |  |  |__   |
|__|  |_____|_|_|_|  |__|  |_____|_|___|_____|_____|_____|_____|  |__|  |_____|__|__| |_| |_____|_____|
                                                                                                       
********************************************************************************************************************************************************************************-*/






/*-********************************************************************************************************************************************************************************
 _____ _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____
|   __|  |  |   | |     |     |   __|   __|  |  |  |     |  _  |   __|   __|   | |   __|
|   __|  |  | | | |   --|  |  |   __|__   |  |  |  |-   -|     |  |  |   __| | | |__   |
|__|  |_____|_|___|_____|_____|_____|_____|   \___/|_____|__|__|_____|_____|_|___|_____|
                                                                                        
 ********************************************************************************************************************************************************************************-*/

/*-**************************************************************
Estrutura _viagem - estrutura que armazena todas informacões
de uma determinada viagem
******************************************-***********************/
struct _viagem{
    int identidade;
    int identidadeBarco;
    int identidadeOrigem; //identidade correspondente ao porto de origem da viagem
    int identidadeDestino; //identidade correspondente ao porto de destino da viagem
    int dataPartida; //criar vetor para inserir de maneira correta os meses etc
    int dataChegada;
    int carga;
    float pagamento;
    struct _viagem *proxViagem;
};

typedef struct _viagem* Viagem;

/*-**************************************************************
criaViagem - função que inicializa todas as variáveis da estrutura
associada a cada uma das viagens
Parametros: identidade - identidade a associar a viagem
            identidadeBarco - identidade do barco a viajar
            identidadeOrigem
            identidadeDestino
            dataPartida - data de partida do barco
            dataChegada - data de chegada do barco
            carga - carga a transportar na viagem
            pagamento - total a pagar pela viagem
Retorno: v - se a viagem for com sucesso (objeto com tudo da viagem)
         NULL - se houver algum erro na alocação de memoria
***************************************************************-*/
Viagem criaViagem(int identidade, int identidadeBarco, int identidadeOrigem, int identidadeDestino, int dataPartida, int dataChegada, int carga, float pagamento){
    Viagem v;
    
    v = (Viagem)malloc(sizeof(struct _viagem));
    if (v == NULL){
        free(v);
        return NULL;
    }
    
    v -> identidade = identidade;
    
    v -> identidadeBarco = identidadeBarco;
    
    v -> identidadeOrigem = identidadeOrigem;
    
    v -> identidadeDestino = identidadeDestino;
    
    v -> dataPartida = dataPartida;
    
    v -> dataChegada = dataChegada;
    
    v -> carga = carga;
    
    v -> pagamento = pagamento;
    
    v -> proxViagem = NULL;
    
    return v;
}

/*-**************************************************************
leuViagens - função que trata ler o ficheiro com as informações dos
viagens e organizar tudo nas estruturas respetivas
Parametros: ficheiroViagens - a função recebe como parametro o nome
            onde vai buscar todas as informações de certa viagem
            viagens - recebe um apontador para a cabeça da lista de estruturas
Retorno: 0 - se houve algum erro a ler o ficheiro dos portos
         1 - se sucedeu na leitura do ficheiro dos portos
******************************************-***********************/
int leuViagens(char *ficheiroViagens, Viagem *viagens, Viagem *ultimaViagem, Porto portos, Navio navios, int *contadorViagens){
    FILE *fp;
    Viagem viagem;
    int controloFim;
    int identidade;
    int identidadeBarco;
    int identidadeOrigem;
    int identidadeDestino;
    int dataPartida;
    int dataChegada;
    int carga;
    float pagamento;

    fp = fopen(ficheiroViagens, "r");

    if(fp==NULL){
        printf(COLOR_RED "Erro ao abrir ficheiro\n" COLOR_RESET);
        return 0;
    }
    while(1){
        if(fscanf(fp, "%d;%d;%d;%d;%d;%d;%d;%f;", &identidade, &identidadeBarco, &identidadeOrigem, &identidadeDestino, &dataPartida, &dataChegada, &carga, &pagamento) != 8){
            printf("Erro na leitura de ficheiro\n");
            return 0;
        }
        if((*viagens = criaViagem(identidade, identidadeBarco, identidadeOrigem, identidadeDestino, dataPartida, dataChegada, carga, pagamento)) != NULL){
            if(!existeNavio(navios, identidadeBarco))
                continue;
            else if(!existePorto(portos, identidadeOrigem))
                continue;
            else if(!existePorto(portos,identidadeDestino))
                continue;
            (*contadorViagens)++;
        }
        else{
            printf(COLOR_RED "Erro ao atribuir cabeça\n" COLOR_RESET);
            return 0;
        }
        break;
    }
    
    viagem = *viagens; //à variavel viagem é atribuida a cabeça da lista de estruturas ligadas

    while(feof(fp) != EOF){ //ler o ficheiro
        if((controloFim = fscanf(fp, "%d;%d;%d;%d;%d;%d;%d;%f;", &identidade, &identidadeBarco, &identidadeOrigem, &identidadeDestino, &dataPartida, &dataChegada, &carga, &pagamento)) != 8){
            if(controloFim == EOF)
                break;
            return 0;
        }
        else{
            if(!existeNavio(navios, identidadeBarco))
                continue;
            else if(!existePorto(portos, identidadeOrigem))
                continue;
            else if(!existePorto(portos,identidadeDestino))
                continue;
        }
        if((viagem -> proxViagem = criaViagem(identidade, identidadeBarco, identidadeOrigem, identidadeDestino, dataPartida, dataChegada, carga, pagamento)) == NULL)
            return 0;
        (*contadorViagens)++;
        viagem = viagem -> proxViagem;
    }
    
    *ultimaViagem = viagem;
    
    fclose(fp);
    return 1;
}

/*-**************************************************************
 listarViagem - função que vai percorrer a lista ligada começando na
 cabeça da lista ligada e percorrendo todos os elementos até nao
 ter proximo
 Parametros: viagens - apontador para a cabeça da lista de estruturas
******************************************-***********************/
void listarViagens(Viagem viagens, Navio navios){
    Navio navio;
    while(viagens != NULL){
        navio = devolveNavio(navios, viagens -> identidadeBarco);
        if(navio == NULL)
            continue;
        printf(COLOR_GREEN "Id:" COLOR_RESET" %4d | " COLOR_GREEN "Navio:" COLOR_RESET " %4d | " COLOR_GREEN "Barco =" COLOR_RESET " %25s  | " COLOR_GREEN "Identidade Origem =" COLOR_RESET " %4d | " COLOR_GREEN "Identidade Destino =" COLOR_RESET " %4d | " COLOR_GREEN "Data Partida = " COLOR_RESET "%2d | " COLOR_GREEN "Data Chegada = " COLOR_RESET "%2d | " COLOR_GREEN "Carga = " COLOR_RESET "%5d | " COLOR_GREEN "Pagamento = " COLOR_RESET " %7.6f\n", viagens -> identidade, navio -> identidade, navio -> nome, viagens -> identidadeOrigem, viagens -> identidadeDestino, viagens -> dataPartida, viagens -> dataChegada, viagens -> carga, viagens -> pagamento); //ver %7.6f e %2d em quais da p meter
        viagens = viagens -> proxViagem;
    }
    fimFuncoes();
}

/*-**************************************************************
 existeViagem - função que verifica se existe uma viagem com uma
 identidade respetiva à recebida em parametro
 Parametros: viagens - apontador para a cabeça da lista de estruturas
             identidadeViagem - viagem que se pretende verificar se
             existe ou nao
 Devolve: 1 - caso a viagem seja encontrada na lista
          0 - caso a viagem não exista na lista
******************************************-**********************/
int existeViagem(Viagem viagens, int identidadeViagem){
    while(viagens != NULL){
        if(viagens -> identidade == identidadeViagem)
            return 1;
        viagens = viagens -> proxViagem;
    }
    return 0;
}

/*-**************************************************************
 contarViagens - função que conta e devolve o numero de elementos
 da lista
 Parametros: viagens - apontador para a cabeça da lista de estruturas
 Devolve: contadorViagens - numero de viagens que ha na lista de viagens
******************************************-**********************/
int contarViagens(Viagem viagens){
    int contadorViagens = 0;
    while(viagens != NULL){
        contadorViagens++;
        viagens = viagens -> proxViagem;
    }
    return contadorViagens;
}

/*-***************************************************************
 devolveViagem - função que devolve uma determinada viagem conforme
 o seu ID
 Parametros: viagens - apontador para a cabeça da lista de estruturas
             idViagem - viagem que se vai devolver
 Devolve: viagens - apontador para a estrutura que contem o id de
          viagem igual ao recebido como parametro
 ******************************************-**********************/
Viagem devolveViagem(Viagem viagens, int idViagem){
    while(viagens != NULL){
        if(viagens -> identidade == idViagem)
            return viagens;
        viagens = viagens -> proxViagem;
    }
    return NULL;
}

/*-**************************************************************
 devolveViagem - função que devolve a viagem com o id correspondente
 ao recebido como parâmetro
 Parametros: viagens - apontador para a cabeça da lista de estruturas
             idNavio - id no navio que se pretende na viagem para a devolver
 Devolve: viagens - viagem que tem o navio
          NULL - id nao encontrado
******************************************-**********************/
Viagem* devolveViagens(Viagem viagens, int idNavio){
    Viagem *viagensID;
    viagensID = malloc(sizeof(struct _viagem) * contarViagens(viagens));
    int counter = 0;
    if(viagensID == NULL){
        printf("Erro a alocar memória\n");
        return NULL;
    }
    Viagem *posicaoInicial;
    posicaoInicial = viagensID;
    while(viagens != NULL){
        if(viagens -> identidadeBarco == idNavio){
            counter++;
            *viagensID = viagens;
            viagensID++;
        }
        viagens = viagens -> proxViagem;
    }
    if(counter == 0){ //caso nao haja nada a preencher
        //printf("Não há viagens a corresponder aos dados.\n");
        return NULL;
    }
    return posicaoInicial;
}

/*-**************************************************************
 devolveViagensPortosIguais - função que dado um porto de partida
 e um porto de destino devolve um vetor com as estruturas que tem
 um porto de destino e de chegada iguais aos recebidos como parametro
 Parametros: viagens - apontador para a cabeça da lista de estruturas
             portoPartida - porto de partida a encontrar
             portoDestino - porto de destino a encontrar
 Devolve: posicaoInicial - apontador para o começo do vetor de estruturas
******************************************-**********************/
Viagem* devolveViagensPortosIguais(Viagem viagens, int portoPartida, int portoDestino){
    Viagem *viagensPortos = malloc(sizeof(struct _viagem) * contarViagens(viagens));
    if(viagensPortos == NULL)
        return NULL;
    Viagem *posicaoInicial = viagensPortos;
    while(viagens != NULL){
        if(viagens -> identidadeOrigem == portoPartida && viagens -> identidadeDestino == portoDestino){
            *viagensPortos = viagens;
            viagensPortos++;
        }
        viagens = viagens -> proxViagem;
    }
    return posicaoInicial;
}

/*-**************************************************************
primeiraViagemLivre - funçao que indica qual é a viagem que o utilizador
vai poder adicionar se quiser manter uma ordem crescente
Parametros: viagens - apontador para a cabeça da lista de estruturas
Devolve: counter + 1 - a primeira posição que o utilizador pode usar
 para nao haver sobreposição
******************************************-**********************/
int primeiraViagemLivre(Viagem viagens){
    int counter = 1;
    Viagem aux;
    aux = viagens -> proxViagem;
    while(aux != NULL){
        if(viagens -> identidade + 1 != aux -> identidade){
            return (counter + 1);
        }
        viagens = viagens -> proxViagem;
        aux = aux -> proxViagem;
        counter++;
    }
    return (counter + 1);
}

/*-**************************************************************
atribuiProximaViagem - função que adiciona a viagem recebida como
 parametro ao fim da lista ligada de viagens
 Parametros: viagens - apontador para a cabeça da lista de estruturas
             viagemAdicionar - viagem a adicionar ao fim da lista
******************************************-**********************/
void atribuiProximaViagem(Viagem viagens, Viagem viagemAdicionar){
    while(1){
        viagens = viagens -> proxViagem;
        if(viagens -> proxViagem == NULL){
            viagens -> proxViagem = viagemAdicionar;
            break;
        }
    }
}

      
/*-********************************************************************************************************************************************************************************
_____ _____ _____    _____ _____ _____ _____ _____ _____ _____    _____ _____ _____ _____ _____ _____ _____
|   __|     |     |  |   __|  |  |   | |     |     |   __|   __|  |  |  |     |  _  |   __|   __|   | |   __|
|   __|-   -| | | |  |   __|  |  | | | |   --|  |  |   __|__   |  |  |  |-   -|     |  |  |   __| | | |__   |
|__|  |_____|_|_|_|  |__|  |_____|_|___|_____|_____|_____|_____|   \___/|_____|__|__|_____|_____|_|___|_____|
 
 ********************************************************************************************************************************************************************************-*/


//FUNÇOES AUXILIARES MERGE SORT

//Lista que vamos utilizar para ordenar os lucros com o algoritmo MERGE SORT
struct _tempList{
    int identidade;
    float lucro;
    struct _tempList *seguinte;
};

typedef struct _tempList *TempList;

/*-**************************************************************
novoLucro - função que inicializa todas as variáveis da estrutura
 associada a cada um dos novos lucros para ordenar
******************************************-**********************/
TempList novoLucro (int id, float lucro){
    TempList tempList = malloc (sizeof (struct _tempList));

    tempList -> identidade = id;
    tempList -> lucro = lucro;
    tempList -> seguinte = NULL;

    return tempList;
}

/*-**************************************************************
agrupaLista - função que agrupa duas sublistas que serão recebidas
 como parametro que derivam da função que divide a lista
Parametros: aux1- primeira sublista
            aux2- segunda sublista
 Devolve: um apontador (agrupado) para o começo da lista já que
 resulta da junção das duas
******************************************-**********************/
TempList agrupaLista(TempList aux1, TempList aux2){
    TempList agrupado = NULL;
  
    if (aux1 == NULL)
        return (aux2);
    else if (aux2 == NULL)
        return (aux1);

    if (aux1 -> lucro > aux2 -> lucro) {
        agrupado = aux1;
        agrupado -> seguinte = agrupaLista (aux1 -> seguinte, aux2);
    }
    else {
        agrupado = aux2;
        agrupado -> seguinte = agrupaLista (aux1, aux2 -> seguinte);
    }
    return agrupado;
}

/*-**************************************************************
divideLista - função que vai dividindo a lsita em grupos para depois
os ordenarmos grupo a grupo
******************************************-**********************/
void divideLista(TempList head, TempList *head1, TempList *head2) {
    TempList quick;
    TempList slow;
    slow = head;
    quick = head -> seguinte;
  
    //quick avança dois nos e slow avança de um em um
    while (quick != NULL) {
        quick = quick -> seguinte;
        if (quick != NULL) {
            slow = slow -> seguinte;
            quick = quick -> seguinte;
        }
    }

    //slow vai ficar antes do ponto medio entao vamos partir a lista em dois a partir deste ponto
    *head1 = head ;
    *head2 = slow -> seguinte;
    slow -> seguinte = NULL; // cortar a lista a meio
}

void ordenaLucros (TempList *lucro){
    TempList head = *lucro;
    TempList aux1;
    TempList aux2;
  
    if ((head == NULL) || (head -> seguinte == NULL)) {
        return;
    }
  
    //partir entre duas sublistas
    divideLista (head, &aux1, &aux2);
  
    ordenaLucros(&aux1); //ordena a primeira sublista (chamada recursiva)
    ordenaLucros(&aux2); //ordena a segunda sublista (chamada recursiva)
  
    //junta as duas listas apenas numa
    *lucro = agrupaLista(aux1, aux2);
}

void seguinteLucro(TempList lucros, TempList nextLucro){
    while(1){
        lucros = lucros -> seguinte;
        if(lucros -> seguinte == NULL){
            lucros -> seguinte = nextLucro;
        }
    }
}

//FIM FUNÇÕES AUXILIARES MERGE SORT


void listarLucros(TempList lucros, Navio navios){
    Navio navio = NULL;
    while(lucros != NULL){
        navio = devolveNavio(navios, lucros -> identidade);
        if(navio == NULL){
            lucros = lucros -> seguinte;
            continue;
        }
        printf(COLOR_BLUE "Id:" COLOR_RESET " %3d | " COLOR_BLUE "Nome:" COLOR_RESET " %17s | " COLOR_BLUE "Pais:" COLOR_RESET " %30s | " COLOR_BLUE "Ano:" COLOR_RESET " %4d | " COLOR_BLUE "Lucro:" COLOR_RESET " %.2f\n", navio -> identidade, navio -> nome, navio -> pais, navio -> ano, lucros -> lucro); //falta adicionar cenas do navio
        navios = navios -> proxNavio;
        lucros = lucros -> seguinte;
    }
}

/*-**************************************************************
 verificaNumero - função que verifica se os numeros introduzidos
 pelo utilizador estão dentro do intervalo pretendido
 Parametros: num - numero introduzido pelo utilizador
             min - numero minimo possivel de introduzir
             max - numero maximo possivel de introduzir
 Retorno: 0 - se nao pertence ao intervalo
          1 - se pertence ao intervalo
***************************************************************-*/
char verificaNumero(int num, int min, int max){
    return num >= min && num <= max;
}

/*-**************************************************************
 inicializarEstruturas - função responsavel pelo controlo de inicialização
 das estruturas e leitura sucedida ou nao dos ficheiros
 Parametros: navios - apontador para uma lista ligada de estruturas _navio
             portos - apontador para uma lista ligada de estruturas _porto
             viagens - apontador para uma lista ligada de estruturas _viagens
 Retorno: 0 - se houve algum erro de leitura num ficheiro
          1 - se a inicialização de tudo foi sucedida
 ***************************************************************-*/
int inicializarEstruturas(Navio *navios, Porto *portos, Viagem *viagens, Viagem *ultimaViagem){
    char ficheiroNavios[MAX_FILENAME]  = "textfiles/ships.txt";
    char ficheiroPortos[MAX_FILENAME]  = "textfiles/ports.txt";
    char ficheiroViagens[MAX_FILENAME] = "textfiles/trips.txt";
    int contadorNavios = 0;
    int contadorPortos = 0;
    int contadorViagens = 0;
    
    if(!leuNavios(ficheiroNavios, navios, &contadorNavios)){
        printf(COLOR_RED "Erro na leitura de navios\n" COLOR_RESET);
        return 0;
    }
    
    if(!leuPortos(ficheiroPortos, portos, &contadorPortos)){
        printf(COLOR_RED "Erro na leitura de portos\n" COLOR_RESET);
        return 0;
    }
    
    if(!leuViagens(ficheiroViagens, viagens, ultimaViagem, *portos, *navios, &contadorViagens)){
        printf(COLOR_RED "Erro na leitura de viagens\n" COLOR_RESET);
        return 0;
    }
    system("clear");
    printf(COLOR_BLUE "BEM VINDO À GESTÃO MARITIMA!\n\n\n" COLOR_RESET);
    printf(COLOR_RED "\t\t\t\t\tForam contados %d navios, %d portos, %d viagens!\n\n\n" COLOR_RESET, contadorNavios, contadorPortos, contadorViagens);
    
    return 1;
}

void listarCargaTransportada(Navio navios, Porto portos, Viagem viagens){
    int idNavio;
    Navio navio;
    Porto portoOrigem, portoDestino;
    int diaPartida;
    int diaChegada;
    Viagem* viagensID;
    Viagem viagem;
    float somaCargas = 0, peso;
    fflush(stdin);
    while(1){
        printf("Introduza o ID do navio: ");
        fflush(stdin);
        if(scanf(" %d", &idNavio) == 1)
            if(!existeNavio(navios, idNavio))
                printf(COLOR_RED "O navio não existe!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza o dia de partida: ");
        fflush(stdin);
        if(scanf(" %d", &diaPartida) == 1)
            if(!verificaNumero(diaPartida, 1, 31))
                printf(COLOR_RED "Erro! Insira por favor um número de 1 a 31!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza o numero de chegada: ");
        fflush(stdin);
        if(scanf(" %d", &diaChegada) == 1)
            if(!verificaNumero(diaChegada, diaPartida + 1, 31))
                printf(COLOR_RED "Erro! O numero do dia de chegada deve ser maior que o número do dia de partida e menor que 31!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados\n" COLOR_RESET);
    }
    navio = devolveNavio(navios, idNavio);
    viagensID = devolveViagens(viagens, idNavio); //viagensID é o vetor com todas as viagens que pretendemos analisar
    if(viagensID == NULL){
        printf(COLOR_BLUE "%s com carga total de %.2fkg em viagens entre os dias %d até %d\n" COLOR_RESET, navio -> nome, somaCargas, diaPartida, diaChegada);
        fimFuncoes();
        return;
    }
    while(viagensID != NULL){
        viagem = *viagensID;
        if(viagem == NULL){
            break;
        }
        portoOrigem = devolvePorto(portos, viagem -> identidadeOrigem);
        portoDestino = devolvePorto(portos, viagem -> identidadeDestino);
        if(viagem -> dataPartida >= diaPartida && viagem -> dataChegada <= diaChegada){
            peso = viagem -> carga * navio -> capacidade / 100.0;
            printf("%s viagem de %s para %s entre os dias %d e %d: %d%%  --> %.2f\n", navio -> nome, portoOrigem -> nome, portoDestino -> nome, viagem -> dataPartida, viagem -> dataChegada, viagem -> carga, peso);
            somaCargas += peso;
        }
        viagensID++;
    }
    printf(COLOR_BLUE "%s com carga total de %.2fkg em viagens entre os dias %d até %d\n" COLOR_RESET, navio -> nome, somaCargas, diaPartida, diaChegada);
    fimFuncoes();
}

void guardarViagens(Viagem viagens, Viagem *ultimaViagem){
    FILE *fp;
    Viagem viagem = *ultimaViagem;
    int viagensGuardadas = 0;
    fp = fopen("textfiles/trips.txt", "a");
    while (viagem -> proxViagem != NULL){
        viagem = viagem -> proxViagem;
        //printf("id viagem = %d\n", viagem -> identidade);
        //fprintf(fp, "\n");
        fprintf(fp, "\n%d;%d;%d;%d;%d;%d;%d;%f", viagem -> identidade, viagem -> identidadeBarco, viagem -> identidadeOrigem, viagem -> identidadeDestino, viagem -> dataPartida, viagem -> dataChegada, viagem -> carga, viagem -> pagamento);
        viagensGuardadas++;
    }
    *ultimaViagem = viagem;
    if(viagensGuardadas == 0)
        printf(COLOR_RED "Não tinha viagens por guardar!\n" COLOR_RESET);
    else
        printf(COLOR_BLUE "Foram guardadas %d viagens no ficheiro\n" COLOR_RESET, viagensGuardadas);
    fclose(fp);
    fimFuncoes();
}

void cargaEntrePortos(Navio navios, Porto portos, Viagem viagens){
    int idPortoOrigem, idPortoDestino, contaViagens = 0;
    Porto portoOrigem, portoDestino;
    Viagem *viagensPortos, viagem;
    Navio navio;
    float peso, somasCargas = 0;
    while(1){
        printf("Introduza id do porto de origem: ");
        fflush(stdin);
        if(scanf(" %d", &idPortoOrigem) == 1)
            if(!existePorto(portos, idPortoOrigem))
                printf(COLOR_RED "Porto não existe!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    portoOrigem = devolvePorto(portos, idPortoOrigem);
    while(1){
        printf("Introduza id do porto de destino: ");
        fflush(stdin);
        if(scanf(" %d", &idPortoDestino) == 1)
            if(!existePorto(portos, idPortoDestino))
                printf(COLOR_RED "Porto não existe!\n" COLOR_RESET);
            else if(idPortoOrigem == idPortoDestino)
                printf(COLOR_RED "Inseriu um porto de destino igual ao porto de origem!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    portoDestino = devolvePorto(portos, idPortoDestino);
    viagensPortos = devolveViagensPortosIguais(viagens, idPortoOrigem, idPortoDestino);
    while(*viagensPortos != NULL){
        viagem = *viagensPortos;
        navio = devolveNavio(navios, viagem -> identidadeBarco);
        peso = viagem -> carga * navio -> capacidade / 100.0;
        somasCargas += peso;
        contaViagens++;
        viagensPortos++;
    }
    printf(COLOR_BLUE "A carga total de %.2fkg transportada entre %s e %s em %d viagens\n" COLOR_RESET, somasCargas, portoOrigem -> nome, portoDestino -> nome, contaViagens);
    fimFuncoes();
}

void rendimentoEconomicoViagem(Navio navios, Porto portos, Viagem viagens){
    int idViagem;
    Navio navio;
    Viagem viagem;
    Porto portoOrigem;
    Porto portoDestino;
    float rendimento;
    while(1){
        printf("Introduza ID da viagem: ");
        fflush(stdin);
        if(scanf(" %d", &idViagem) == 1)
            if(!existeViagem(viagens, idViagem))
                printf(COLOR_RED "Viagem não existe!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    viagem = devolveViagem(viagens, idViagem);
    navio = devolveNavio(navios, viagem -> identidadeBarco);
    portoOrigem = devolvePorto(portos, viagem -> identidadeOrigem);
    portoDestino = devolvePorto(portos, viagem -> identidadeDestino);
    rendimento = viagem -> pagamento - ((portoOrigem -> custo + portoDestino -> custo) + (navio -> combustivel + navio -> custo) * (viagem -> dataChegada - viagem -> dataPartida + 1));
    if(rendimento > 0)
        printf(COLOR_GREEN "Rendimento da viagem = +%.2f\n" COLOR_RESET, rendimento);
    else
        printf(COLOR_RED "Rendimento da viagem = %.2f\n" COLOR_RESET, rendimento);
    fimFuncoes();
}

void adicionarViagem(Navio navios, Porto portos, Viagem viagens, Viagem ultimaViagem){
    int idViagem, idNavio, idPortoOrigem, idPortoDestino, diaPartida, diaChegada, ocupacao;
    float preco;
    Navio navio;
    Porto portoOrigem, portoDestino;
    Viagem novaViagem;
    while(1){
        printf("Introduza o ID da viagem (%d está disponivel): ", primeiraViagemLivre(viagens));
        fflush(stdin);
        if(scanf(" %d", &idViagem) == 1)
            if(existeViagem(viagens, idViagem))
                printf(COLOR_RED "A viagem que quer ocupar já está disponível!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados" COLOR_RESET);
    }
    while(1){
        printf("Introduza o ID do navio: ");
        fflush(stdin);
        if(scanf(" %d", &idNavio) == 1)
            if(!existeNavio(navios, idNavio))
                printf(COLOR_RED "Navio não existe!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados\n" COLOR_RESET);
    }
    navio = devolveNavio(navios, idNavio);
    while(1){
        printf("Introduza id do porto de origem: ");
        fflush(stdin);
        if(scanf(" %d", &idPortoOrigem) == 1)
            if(!existePorto(portos, idPortoOrigem))
                printf(COLOR_RED "Porto não existe!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    portoOrigem = devolvePorto(portos, idPortoOrigem);
    while(1){
        printf("Introduza id do porto de destino: ");
        fflush(stdin);
        if(scanf(" %d", &idPortoDestino) == 1)
            if(!existePorto(portos, idPortoDestino))
                printf(COLOR_RED "Porto não existe!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    portoDestino = devolvePorto(portos, idPortoDestino);
    while(1){
        printf("Introduza o dia de partida: ");
        fflush(stdin);
        if(scanf(" %d", &diaPartida) == 1)
            if(!verificaNumero(diaPartida, 1, 31))
                printf(COLOR_RED "Erro! Insira por favor um número de 1 a 31!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza o numero de chegada: ");
        fflush(stdin);
        if(scanf(" %d", &diaChegada) == 1)
            if(!verificaNumero(diaChegada, diaPartida + 1, 31))
                printf(COLOR_RED "Erro! O numero do dia de chegada deve ser maior que o número do dia de partida e menor que 31!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza ocupação: ");
        fflush(stdin);
        if(scanf(" %d", &ocupacao) == 1)
            if(!verificaNumero(ocupacao, 0, 100))
                printf(COLOR_RED "A ocupação tem de estar entre 0 e 100!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza preço: ");
        fflush(stdin);
        if(scanf(" %f", &preco) == 1)
            if(preco < 0)
                printf(COLOR_RED "Tem de ter um preço positivo!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    novaViagem = criaViagem(idViagem, idNavio, idPortoOrigem, idPortoDestino, diaPartida, diaChegada, ocupacao, preco);
    atribuiProximaViagem(viagens, novaViagem);
    printf(COLOR_BLUE "Viagem adicionada com sucesso\n" COLOR_RESET);
    printf(COLOR_BLUE "Navio:" COLOR_RESET " %s\n", navio -> nome);
    printf(COLOR_BLUE "Partida:" COLOR_RESET " %s\n", portoOrigem -> nome);
    printf(COLOR_BLUE "Destino:" COLOR_RESET " %s\n", portoDestino -> nome);
    printf(COLOR_BLUE "Ocupacao:" COLOR_RESET " %d\n", ocupacao);
    printf(COLOR_BLUE "Preço:" COLOR_RESET " %.2f\n", preco);
    fimFuncoes();
}

void adicionarViagemAutomatica(Navio navios, Porto portos, Viagem viagens, Viagem ultimaViagem){
    int identidadePortoPartida, identidadePortoDestino, diaPartida, diaChegada, carga;
    float maximo, rendimento;
    char *tipoNavio;
    int identidadeViagem;
    TempList lucros = NULL, lucroPrev = NULL;
    TempList *apontadorLucros = malloc(sizeof(struct _tempList));
    Porto portoOrigem, portoDestino;
    Viagem viagem;
    Navio cabecaNavio = navios;
    tipoNavio = (char*)malloc(sizeof(char) * 50);
    while(1){
        printf("Introduza id do porto de origem: ");
        fflush(stdin);
        if(scanf(" %d", &identidadePortoPartida) == 1)
            if(!existePorto(portos, identidadePortoPartida))
                printf(COLOR_RED "Porto não existe!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    portoOrigem = devolvePorto(portos, identidadePortoPartida);
    while(1){
        printf("Introduza id do porto de destino: ");
        fflush(stdin);
        if(scanf(" %d", &identidadePortoDestino) == 1)
            if(!existePorto(portos, identidadePortoDestino))
                printf(COLOR_RED "Porto não existe!\n" COLOR_RESET);
            else if(identidadePortoDestino == identidadePortoPartida)
                printf(COLOR_RED "Inseriu um porto de destino igual ao porto de origem!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    portoDestino = devolvePorto(portos, identidadePortoDestino);
    while(1){
        printf("Introduza o dia de partida: ");
        fflush(stdin);
        if(scanf(" %d", &diaPartida) == 1)
            if(!verificaNumero(diaPartida, 1, 31))
                printf(COLOR_RED "Erro! Insira por favor um número de 1 a 31!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza o numero de chegada: ");
        fflush(stdin);
        if(scanf(" %d", &diaChegada) == 1)
            if(!verificaNumero(diaChegada, diaPartida + 1, 31))
                printf(COLOR_RED "Erro! O numero do dia de chegada deve ser maior que o número do dia de partida e menor que 31!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza o maximo que pretende pagar: ");
        fflush(stdin);
        if(scanf(" %f", &maximo))
            if(maximo <= 0)
                printf(COLOR_RED "O número maximo tem de ser positivo!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados\n" COLOR_RESET);
    }
    while(1){
        printf("Introduza carga: ");
        fflush(stdin);
        if(scanf(" %d", &carga) == 1)
            if(!verificaNumero(carga, 0, 100))
                printf(COLOR_RED "A ocupação tem de estar entre 0 e 100!\n" COLOR_RESET);
            else
                break;
        else
            printf(COLOR_RED "Erro na introdução de dados!\n" COLOR_RESET);
    }
    
    while(1){
        printf("Introduza o tipo [Bulker, Container, Tanker]: "); //escolher um random
        fflush(stdin);
        fgets(tipoNavio, MAX_TIPO, stdin);
        tipoNavio[strlen(tipoNavio) - 1] = '\0';
        if(!strcmp(tipoNavio, "Bulker") || !strcmp(tipoNavio, "Container") || !strcmp(tipoNavio, "Tanker"))
            break;
        else
            printf(COLOR_RED "Tipo de navio invalido!\n" COLOR_RESET);
    }
    identidadeViagem = primeiraViagemLivre(viagens);//ver vai adicionar viagem
    //percorrer todas as viagens as que tiverem barco do tipo tipoNavio adicionar a lista
    while(navios != NULL){
        if(!strcmp(navios -> tipo, tipoNavio)){
            rendimento = maximo - ((portoOrigem -> custo + portoDestino -> custo) + (navios -> combustivel + navios -> custo) * (diaChegada - diaPartida + 1));
            if(rendimento > 0){
                if(lucroPrev == NULL){
                    lucroPrev = novoLucro(navios -> identidade, rendimento); //inicializa a cabeça
                    *apontadorLucros = lucroPrev; //apontador guarda a primeira estrutura
                }
                else{
                    lucros = novoLucro(navios -> identidade, rendimento);
                    lucroPrev -> seguinte = lucros;
                    lucroPrev = lucros;
                }
            }
        }
        navios = navios -> proxNavio;
    }
    if(*apontadorLucros == NULL){
        printf(COLOR_RED "Não foram encontrados quaisquer navios para esta especificação:\n" COLOR_RESET);
        printf("\tPartida: %s (%s) no dia %d\n", portoOrigem -> nome, portoOrigem -> pais, diaPartida);
        printf("\tChegada: %s (%s) no dia %d\n", portoDestino -> nome, portoDestino -> pais, diaChegada);
        //tipo toupper
        printf("\tCarga: %d.00 (%s)\n", carga, tipoNavio);
        fimFuncoes();
        return;
    }
    ordenaLucros(apontadorLucros);
    lucros = *apontadorLucros;
    viagem = criaViagem(identidadeViagem, lucros -> identidade, portoOrigem -> identidade, portoDestino -> identidade, diaPartida, diaChegada, carga, maximo); //viagem com maior lucro
    printf("Viagem adicionada com sucesso!\n");
    atribuiProximaViagem(viagens, viagem);
    navios = cabecaNavio;
    listarLucros(*apontadorLucros, navios);
    free(apontadorLucros);
    free(tipoNavio);
    fimFuncoes();
}

void destroiViagens(Viagem viagens){
    Viagem aux = viagens;
    while(aux != NULL){
        aux = viagens -> proxViagem;
        free(viagens);
        viagens = aux;
    }
}

void destroiNavios(Navio navios){
    Navio aux = navios;
    while(aux != NULL){
        aux = navios -> proxNavio;
        free(navios -> nome);
        free(navios -> tipo);
        free(navios -> pais);
        free(navios);
        navios = aux;
    }
}

void destroiPortos(Porto portos){
    Porto aux = portos;
    while(aux != NULL){
        aux = portos -> proxPorto;
        free(portos -> nome);
        free(portos -> pais);
        free(portos);
        portos = aux;
    }
}

void destroiEstaleiro(Viagem viagens, Navio navios, Porto portos){
    destroiViagens(viagens);
    destroiNavios(navios);
    destroiPortos(portos);
}

void saidaPrograma(Viagem viagens, Navio navios, Porto portos, Viagem *ultimaViagem){; //viagem criada apenas para comparar o que esta em ultima viagem
    char resposta;
    while(viagens -> proxViagem != NULL)
        viagens = viagens -> proxViagem;
    if(viagens == *ultimaViagem){
        printf("Adeus!\n");
    }
    else{
        printf("Pretende guardar as viagens(s/n)? ");
        fflush(stdin);
        resposta = getchar();
        if(resposta == 's')
            guardarViagens(viagens, ultimaViagem);
        else if(resposta == 'n'){
            printf("Adeus\n");
        }
    }
    destroiEstaleiro(viagens, navios, portos);
}

int main(){
    Navio navios;
    Porto portos;
    Viagem viagens, ultimaViagem; //ultimaViagem guardada!!
    inicializarEstruturas(&navios, &portos, &viagens, &ultimaViagem);
    char escolha;
    
    do{
        printf("0) Sair do Programa\n");
        printf("1) Listar Frota\n");
        printf("2) Listar Porto\n");
        printf("3) Listar Viagens\n");
        printf("4) Listar toda a carga de um navio num determinado periodo de dias\n");
        printf("5) Guardar viagens no ficheiro de texto\n");
        printf("6) Carga transportada entre dois portos\n");
        printf("7) Analisar o rendimento economico de uma determinada viagem\n");
        printf("8) Adicionar uma viagem\n");
        printf("9) Adicionar uma viagem (auto)\n");

        scanf(" %c", &escolha);

        switch(escolha){
            case '0' : saidaPrograma(viagens, navios, portos, &ultimaViagem); break;
            case '1' : listarNavios(navios); break;
            case '2' : listarPortos(portos); break;
            case '3' : listarViagens(viagens, navios); break;
            case '4' : listarCargaTransportada(navios, portos, viagens); break;
            case '5' : guardarViagens(viagens, &ultimaViagem); break;
            case '6' : cargaEntrePortos(navios, portos, viagens); break;
            case '7' : rendimentoEconomicoViagem(navios, portos, viagens); break;
            case '8' : adicionarViagem(navios, portos, viagens, ultimaViagem); break;
            case '9' : adicionarViagemAutomatica(navios, portos, viagens, ultimaViagem); break;
            default : printf("Escolha uma opção válida por favor!\n"); break;
        }
    }while(escolha != '0');
    return 0;
}
