#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "markov_chain.h"
#define MAX_LINE 1000
#define MAX_TWEET_LENGTH 20
#define DECIMAL 10
#define FILE_PATH_ERROR "Error: incorrect file path"
#define FAILURE 1
#define SUCCESS 0
#define NUMBER_GIVEN 5
#define NUMBER_NOT_GIVEN 4
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"
#define DELIMITERS " \n\t\r"

int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
    if (fp == NULL || markov_chain == NULL)
    {
        return FAILURE;
    }

    char buffer[MAX_LINE];
    Node *previous_node = NULL;
    Node *current_node = NULL;

    while (fgets(buffer, sizeof(buffer), fp) != NULL && words_to_read > 0)
    {
        char *word = strtok(buffer, DELIMITERS);

        while (word != NULL && words_to_read > 0)
        {
            current_node = add_to_database(markov_chain, word);
            if (current_node == NULL)
            {
                return FAILURE;
            }

            if (previous_node != NULL)
            {
                if (add_node_to_frequency_list(previous_node->data, current_node->data) == FAILURE)
                {
                    return FAILURE;
                }
            }

            previous_node = current_node;
            word = strtok(NULL, DELIMITERS);
            words_to_read--;
        }
        previous_node = NULL;
    }
    return SUCCESS;
}



int main(int argc,char *argv[])
{
    if(argc != NUMBER_NOT_GIVEN && argc != NUMBER_GIVEN)
    {
        fprintf(stdout,NUM_ARGS_ERROR);
        return EXIT_FAILURE;
    }
    const char *file_path = argv[3];
    FILE *fp = fopen(file_path,"r");
    if (fp == NULL)
    {
        fprintf(stdout,FILE_PATH_ERROR);
        return EXIT_FAILURE;
    }

    MarkovChain *markov_chain = malloc(sizeof(MarkovChain));
    if(markov_chain == NULL)
    {
        fclose(fp);
        fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
        return EXIT_FAILURE;
    }
    LinkedList *data_base = malloc(sizeof(LinkedList));
    if(data_base == NULL)
    {
        fclose(fp);
        fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
        free(markov_chain);
        return EXIT_FAILURE;
    }
    markov_chain ->database = data_base;
    markov_chain ->database ->first = NULL;
    markov_chain ->database ->last =  NULL;
    markov_chain ->database ->size = 0;



    if(argc == NUMBER_NOT_GIVEN)
    {
        int database = fill_database(fp,INT_MAX,markov_chain);
        if(database == FAILURE)
        {
            free_database(&markov_chain);
            fclose(fp);
            return EXIT_FAILURE;
        }
    }
    if(argc == NUMBER_GIVEN)
    {
        long amount_of_words = strtol(argv[4],NULL,DECIMAL);
        int database = fill_database(fp,(int)amount_of_words,markov_chain);
        if(database == FAILURE)
        {
            free_database(&markov_chain);
            fclose(fp);
            return EXIT_FAILURE;
        }
    }

    long seed = strtol(argv[1],NULL,DECIMAL);
    srand(seed);
    long num_of_tweets = strtol(argv[2],NULL,DECIMAL);
    for(int i = 1; i <= (int)num_of_tweets ; i++)
    {
        fprintf(stdout,"Tweet %d: ",i);
        MarkovNode *cur_node = get_first_random_node(markov_chain);
        generate_tweet(cur_node,MAX_TWEET_LENGTH);
    }

    free_database(&markov_chain);
    fclose(fp);
    return EXIT_SUCCESS;
}




