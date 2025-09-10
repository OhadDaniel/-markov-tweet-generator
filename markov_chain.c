#include <string.h>
#include <stdio.h>
#include "markov_chain.h"
#define SUCCESS 0
#define FAILURE 1



Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
    if(markov_chain == NULL || data_ptr == NULL)
    {
        return NULL;
    }
    Node* current = markov_chain ->database ->first;
    while(current != NULL)
    {
        if(strcmp(current ->data ->data, data_ptr) == SUCCESS)
        {
            return current;
        }
        current = current ->next;
    }
    return NULL;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
    Node *existing_node = get_node_from_database(markov_chain, data_ptr);
    if (existing_node != NULL)
    {
        return existing_node;
    }

    MarkovNode *new_markov_node = malloc(sizeof(MarkovNode));
    if (new_markov_node == NULL)
    {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }

    new_markov_node ->data  = malloc(sizeof(char) * strlen(data_ptr)  +1);

    new_markov_node->data = strcpy(new_markov_node ->data , data_ptr);
    if (new_markov_node->data == NULL)
    {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        return NULL;
    }
    new_markov_node->frequency_list = NULL;
    new_markov_node->frequency_count = 0;


    if (add(markov_chain->database, new_markov_node) != 0)
    {
        fprintf(stdout, ALLOCATION_ERROR_MASSAGE);
        free(new_markov_node->data);
        free(new_markov_node);
        return NULL;
    }
    return markov_chain->database->last;
}

int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node)
{
    if(first_node == NULL)
    {
        return FAILURE;
    }

    if(first_node->frequency_list == NULL)
    {
        first_node->frequency_list = malloc(sizeof(MarkovNodeFrequency));
        if (first_node->frequency_list == NULL)
        {
            fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
            return FAILURE;
        }
        first_node->frequency_list[0].markov_node = second_node;
        first_node->frequency_list[0].frequency = 1;
        first_node ->frequency_count = 1;
        return SUCCESS;
    }

    for(int i = 0; i < (first_node -> frequency_count);i++)
    {
        if(first_node ->frequency_list[i].markov_node == second_node)
        {
            first_node ->frequency_list[i].frequency++;
            return SUCCESS;
        }
    }

    MarkovNodeFrequency *new_list = realloc(first_node ->frequency_list,(first_node ->frequency_count +1) * sizeof(MarkovNodeFrequency));
    if(new_list == NULL)
    {
        fprintf(stdout,ALLOCATION_ERROR_MASSAGE);
        return FAILURE;
    }
    first_node->frequency_list = new_list;
    first_node->frequency_list[first_node->frequency_count].markov_node = second_node;
    first_node->frequency_list[first_node->frequency_count].frequency = 1;
    first_node->frequency_count++;

    return SUCCESS;

}

void free_database(MarkovChain ** ptr_chain)
{
    Node *current = (*ptr_chain) ->database ->first;
    while (current != NULL)
    {
        Node *next = current->next;
        if (current->data->frequency_list != NULL) {
            free(current->data->frequency_list);
        }
        free(current->data->data);
        free(current->data);
        free(current);

        current = next;
    }
    free((*ptr_chain) ->database);
    free(*ptr_chain);
}

int get_random_number(int max_number)
{
    return rand() % max_number;
}

MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
    int max_number = markov_chain ->database ->size;
    int chosen_num;
    int i = 0;
    Node *chosen_node = markov_chain->database->first;
    char* legal_word = chosen_node->data->data;
    while (true)
    {
        chosen_num = get_random_number(max_number);
        while (i < chosen_num)
        {
            chosen_node = chosen_node->next;
            legal_word = chosen_node->data->data;
            i++;
        }
        if (legal_word != NULL)
        {
            size_t word_length = strlen(legal_word);
            if (word_length > 0 && legal_word[word_length - 1] == '.') {
                i = 0;
                chosen_node = markov_chain->database->first;
                legal_word = chosen_node->data->data;
                continue;
            }
            else
            {
                return chosen_node->data;
            }
        }
    }
    return NULL;
}

MarkovNode* get_next_random_node(MarkovNode *cur_markov_node)
{
    if (cur_markov_node == NULL || cur_markov_node ->frequency_list== NULL)
    {
        return NULL;
    }
    int unique_words = cur_markov_node ->frequency_count;
    int count_frequencies = 0;
    int i = 0;
    while(i < unique_words)
    {
        count_frequencies += cur_markov_node -> frequency_list[i].frequency;
        i++;
    }
    int random_number = get_random_number(count_frequencies);
    i=0;
    while (random_number - cur_markov_node->frequency_list[i].frequency >= 0)
    {
        random_number -= cur_markov_node->frequency_list[i].frequency;
        i++;
    }

    return cur_markov_node->frequency_list[i].markov_node;
}

void generate_tweet(MarkovNode *first_node, int max_length)
{
    MarkovNode *cur_node = first_node;
    char *first_word = cur_node ->data;
    fprintf(stdout,"%s", first_word);
    int word_count = 1;

    while( word_count < max_length)
    {
        MarkovNode *next_node = get_next_random_node(cur_node);

        char *word = next_node ->data;
        size_t word_length = strlen(word);

        if(word[word_length-1] == '.')
        {
            fprintf(stdout," %s\n", word);
            break;
        }
        else
        {
            fprintf(stdout," %s", word);
            cur_node = next_node;
        }

        word_count++;

        if((int)word_length == max_length)
        {
            fprintf(stdout,"\n");
        }
    }
}



