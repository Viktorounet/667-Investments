/**
 * @author Victor Julien
 * @details Get values
 */

#include "fetcher.h"
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include "../UI/UI.h"
#include "../UI/graph2/SDL_graph.h"
#include "../UI/graph/plot.h"
#include "../Algorithms/bot.h"

/* money structure declaration */
struct Money
{
  char          *id;
  int           rank;
  char          *symbol;
  char          *name;
  float         supply;
  float         maxSupply;
  float         marketCapUsd;
  float         volumeUsd24Hr;
  float         priceUsd;
  float         changePercent24Hr;
  float         vwap24Hr;
  float         usd_possess;
  float         nb_possess;
  float         limit;
  struct Money  *next;
};

struct Money *btc;
struct Money *eth;
struct Money *yfi;

/** @brief  Update structure
 * @return void
 */
void get_price()
{
  struct Money *tmp = btc;
  btc = get_strc("bitcoin");
  if(tmp != NULL)
  {
  btc->nb_possess = tmp->nb_possess;
  btc->usd_possess = tmp->usd_possess;
  btc->limit = tmp->limit;
  }
  btc->next = tmp;
  tmp = eth;
  eth = get_strc("ethereum");
  if(tmp != NULL)
  {
  eth->nb_possess = tmp->nb_possess;
  eth->usd_possess = tmp->usd_possess;
  eth->limit = tmp->limit;
  }
  eth->next = tmp;
  tmp = yfi;
  yfi = get_strc("yearn-finance");
  if(tmp != NULL)
  {
  yfi->nb_possess = tmp->nb_possess;
  yfi->usd_possess = tmp->usd_possess;
  yfi->limit = tmp->limit;
  }
  yfi->next = tmp;
}

/** @brief  Read the api from "api.coincap.io" and write it into "output.txt"
 * @param[in] *char         The name of the crypto
 * @return void
 */
void update_value(char *name)
{
    CURL *curl;
    CURLcode res;
    char url[50] = "api.coincap.io/v2/assets/";
    strcat(url, name);
    //printf("Downloading your data at adress: %s\n", url);

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);

        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* create an output file and prepare to write the response */
        char name_txt[20] = "";
        for (size_t i = 0; *(name+i) != '\0'; i++)
            name_txt[i] = name[i];
        strcat(name_txt, ".txt");
        FILE *output_file = fopen(name_txt, "w");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* errors handle */
        if(res != CURLE_OK) {
            errx(EXIT_FAILURE, "curl_easy_perform() failed: %sn", curl_easy_strerror(res));
        }

        /* cleanup */
        curl_easy_cleanup(curl);
	fclose(output_file);
    }
}

/** @brief  Print the struct money
 * @param[in] struct Money          The money struct
 * @return void
 */
void printstruct(struct Money *money)
{
    printf("---------------------------------------------------------------------\nid = %s\nrank = %d\nsymbol = '%s'\nname = '%s'\nsupply = %f\nmaxSupply = %f\nmarketCapUsd = '%f'\nvolumeUsd24Hr = '%f'\npriceUsd = %f\nchangePercent24Hr = '%f'\nvwap24Hr = '%f'\n---------------------------------------------------------------------\n",
	 money->id, money->rank, money->symbol, money  ->name, money->supply, money->maxSupply, money->marketCapUsd, money->volumeUsd24Hr, money->priceUsd, money->changePercent24Hr, money->vwap24Hr);
    /*printf("id = %s, rank = %d, symbol = '%s', name = '%s', supply = %f, maxSupply = %f, marketCapUsd = '%f', volumeUsd24Hr = '%f', priceUsd = %f, changePercent24Hr = '%f', vwap24Hr = '%f'\n",
	 money.id, money.rank, money.symbol, money.name, money.supply, money.maxSupply, money.marketCapUsd, money.volumeUsd24Hr, money.priceUsd, money.changePercent24Hr, money.vwap24Hr);*/
}

/** @brief  Print all struct money
 * @param[in] struct Money      The money struct list
 * @param[in] int               The lenght of the list
 * @return void
 */
void print_all_struct(struct Money **l_money, int len)
{
    for (int i = 0; i < len; i++)
    {
        if (*(l_money+i) == NULL)
        {
            printf("Struc is NULL\n");
            continue;
        }
        printstruct(*(l_money+i));
    }
    if (*(l_money+len) == NULL)
        printf("---------------------------------------------------------------------\n");
}

/** @brief  Copy a string and return it
 * @param[in] char*         The string
 * @param[in] size_t        The lenght of the string
 * @return char*
 */
char *strcopy(char *tmp, size_t len)
{
    char *str = calloc(len, sizeof(char));
    for (size_t i = 0; i < len ; i++) *(str+i) = *(tmp+i);

    return str;
}


/** @brief  Analyse the downloaded file from API to put it into a structure
 * @param[in] char*         The buffer
 * @return struct Money
 */
struct Money *getmoney(char *buf)
{
    int pick_data = 0;
    int i_tmp = 0;
    int a = 0;
    struct Money *money = calloc(1, sizeof(struct Money));
    char *tmp = malloc(1*sizeof(char));
    size_t len = 0;

    //printf("Creating struct money...\n");
    for (int i = 1; a < 11; i++)
    {
        //printf("%d : char is %c\n", i, *(buf+i)+'\0');
        if (pick_data == 0 && *(buf+i-1) == ':'  && *(buf+i) == '"')
        {
            //printf("Relevant data found at index %d\n", i);
            pick_data++;
            i_tmp = 0;
            len = 0;
            //printf("Creating temporary buffer for the data.\n");
            while (*(buf+i+len+1) != '"')
            {
                if (*(buf+i+len+1) == '.')
                    *(buf+i+len+1) = ',';
                len++;
            }
            tmp = realloc(tmp, (len+1)*sizeof(char));
            //printf("Buffer of size %ld created.\n", len+1);
        }
        else if (pick_data == 0 && *(buf+i-1) == ':'  && *(buf+i) == 'n')
        {
            a++;    
        }

        else if (pick_data == 1 && *(buf+i) == '"')
        {
            //printf("End of data, storing it in struct.\n");
            pick_data = 0;
            *(tmp+i_tmp) = '\0';
            switch(a)
            {
                case 0: ;
                    money->id = strcopy(tmp, len+1);
                    //printf("id is now %s\n", tmp);
                    break;
                case 1:
                    money->rank = atof(tmp);
                    break;
                case 2:
                    money->symbol = strcopy(tmp, len+1);
                    //printf("symbol is now %s\n",tmp);
                    break;
                case 3:
                    money->name = strcopy(tmp, len+1);
                    //printf("name is now %s\n", tmp);
                    break;
                case 4:
                    money->supply = atoll(tmp);
                    break;
                case 5:
                    money->maxSupply = atof(tmp);
                    break;
                case 6:
                    money->marketCapUsd = atof(tmp);
                    break;
                case 7:
                    money->volumeUsd24Hr = atof(tmp);
                    break;
                case 8:
                    money->priceUsd = atof(tmp);
                    //printf("price is now %s\n", tmp);
                    break;
                case 9:
                    money->changePercent24Hr = atof(tmp);
                    break;
                case 10:
                    money->vwap24Hr = atof(tmp);
                    break;
                default:
                    break;
            }
	    a++;
        }
        else if (pick_data == 1)
        {
            //printf("Adding char %c in the temporary buffer.\n", *(buf+i));
            *(tmp+i_tmp) = *(buf+i);
            i_tmp++;
        }
    }
    
    money->usd_possess = 0;
    money->nb_possess = 0;
    money->limit = 0;
    money->next = NULL;

    //printf("Struct money created.\n");
    //printf("END OF FOR\n");

    free(tmp);
    return money;
}

/** @brief  Get data in file and write it into buffer
 * @param char*             The filename
 * @return struct Money
 */
struct Money *Get_from_File(char *fname)
{
    struct Money *money = NULL;
    size_t length;
    FILE *f = fopen (fname, "r");

    if (f)
    {
        //printf("%s is open\n", fname);
        fseek(f, 0L, SEEK_END);
        length = ftell(f);
        //printf("length is %ld\n", length);
        fseek (f, 0, SEEK_SET);
        char *buffer = calloc(length, sizeof(char));
        if (buffer)
	    {
            int e = fread(buffer, length+1, 1, f);
	        if (e == -1)
                err(1,"Coufn't read file %s\n", fname);
		    fclose (f);

            money = getmoney(buffer);
        }
        free(buffer);
    }

    return money;
}

/** @brief  Verify if it is on alphabet
 * @param[in] char*             The money name to check
 * @return int
 */
int checkspell(char *arg)
{
    int i = 0;
    while (*(arg+i) != '\0')
    {
        if (!((*(arg+i) >= 'a' && *(arg+i) <= 'z') || (*(arg+i) >= 'A' && *(arg+i) <= 'Z') || *(arg+i)=='-'))
            return 0;
        i++;
    }
    return i;
}

/** @brief  Get the structure
 * @param[in] char*             The name of the struct
 * @return struct Money
 */
struct Money *get_strc(char* name)
{
    if (checkspell(name) == 0)
    {
        printf("Invalid argument : %s\n---------------------------------------------------------------------\n", name);
        return NULL;
    }

    // Get each object member and assign it to the struct.
    char name_txt[20] = "";
    for (size_t i = 0; *(name+i) != '\0'; i++)
        name_txt[i] = name[i];
    strcat(name_txt, ".txt");
    struct Money *money = Get_from_File(name_txt);

    if (money == NULL)
    {
        printf("Invalid crypto's name : %s\n---------------------------------------------------------------------\n", name);
    }
    else
    {
        //printf("Struct created: %s\n", name);
        //printstruct(money);

    }

    return money;
}

/** @brief  Get a list of structures
 * @param[in] argv              The list of structs
 * @param[in] argc              The number of struct
 * @return struct void
 */
struct Money **get_strc_list(int argc, char** argv)
{
  struct Money **l_money = malloc((argc)*sizeof(struct Money *));

  for (int i = 0; i < argc; i++)
  {
    struct Money * money = get_strc(argv[i]);
    *(l_money+i) = money;
    free(money);
  }
  //print_all_struct(l_money, argc-1);

  return l_money;
}

/** @brief  Execute an extern program
 * @param[in] argv          The file path and arguments
 * @return int
 */
static int exec_prog(const char **argv)
{
    pid_t   my_pid;
    int     status, timeout;

    if (0 == (my_pid = fork())) {
            if (-1 == execve(argv[0], (char **)argv , NULL)) {
                    perror("child process execve failed [%m]");
                    return -1;
            }
    }
    return 0;
}

/** @brief  Loop function
 * @return void
 */
void loop() 
{
  int p = 0;
  while(1)
  {
    update_value("bitcoin");
    update_value("ethereum");
    update_value("yearn-finance");
    get_price();
    if (p > 4)
    {
        const char *my_argv[64] = {"./pl", NULL};
        int rc = exec_prog(my_argv);
    }
    update_possess_money_price();
    switch (on_money)
    {
        case 0:
            on_btc_graph_button_toggled();
                if (p)
                    write_history(btc);
            break;
        case 1:
            on_eth_graph_button_toggled();
                if (p)
                    write_history(eth);
            break;
        case 2:
            on_yfi_graph_button_toggled();
                if (p)
                    write_history(yfi);
            break;
        default:
            break;
    }
    sleep(1);
    update_image(p);
    p += 1;
    sleep(4);
  }
}
