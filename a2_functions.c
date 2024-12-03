/*****************
    Student Name 	= Jules Wong
    Student Number	= 101309829 
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_functions.h"

// Your solution goes here
int get_valid_int(int start, int end){
    int int_choice = 0;
    scanf(" %d", &int_choice);
    while(int_choice < start || int_choice > end ){ /*checks to make sure integer is valid*/
        printf("Not a valid choice, please try again: ");
        scanf(" %d", &int_choice); /* continuously updates value if integer is invalid*/
    }
    return int_choice;
}

char* get_string(char *message){
    char *string = (char*)malloc(sizeof(char));
    printf("%s", message);
    scanf("%s", string);
    return string;
}

_Bool get_y_n(char *message){
    char response;
    printf("\n%s", message);
    scanf(" %c", &response);
    while (response != 'Y' && response != 'y' && response != 'N' && response != 'n') {
        printf("\nError. Please enter Y or N: ");
        scanf(" %c", &response);
    }
    if(response == 'Y' || response == 'y'){
        return true;
    }
    else if(response == 'N' || response == 'n'){
        return false;
    }
}

user_t *add_user(user_t *users, const char *username, const char *password){
    user_t *new_user = (user_t*)malloc(sizeof(user_t));
    strcpy(new_user->username, username);
    strcpy(new_user->password, password);
    new_user->next = NULL;   
    new_user->friends = NULL; 
    new_user->posts = NULL; 
    
    if (users == NULL) { // when list is empty
        return new_user;
    }

    user_t *current_user = users;
    if (strcmp(current_user->username, new_user->username) > 0) { // when new user is first in alpha order
        new_user->next = users;
        return new_user;
    }

    // when new user is somewhere in the middle or end of the alphabetical order
    while (current_user->next != NULL && strcmp(current_user->next->username, new_user->username) < 0) {
        current_user = current_user->next;
    }
    new_user->next = current_user->next;
    current_user->next = new_user;
    return users; 
}

user_t *find_user(user_t *users, const char *username){
    user_t *cur_user = users;
    while(cur_user != NULL){
        if(strcmp(cur_user->username,username) == 0){
            return cur_user; // return user with matching username
        }
        cur_user = cur_user->next;
    }
    return NULL; // return NULL if user not found
}

friend_t *create_friend(const char *username){
    friend_t *my_friend = (friend_t*)malloc(sizeof(friend_t));
    strcpy(my_friend->username, username);
    my_friend->next = NULL;
    return my_friend;
}

void add_friend(user_t *user, const char *friend){
    friend_t *friend_to_add = create_friend(friend);

    if (user->friends == NULL) {
        user->friends = friend_to_add;
        return;
    }

    friend_t *cur_friend = user->friends;
    if (strcmp(cur_friend->username, friend_to_add->username) > 0) { 
        friend_to_add->next = cur_friend;
        user->friends = friend_to_add;
        return;
    }

    while (cur_friend->next != NULL && strcmp(cur_friend->next->username, friend_to_add->username) < 0) {
        cur_friend = cur_friend->next;
    }
    friend_to_add->next = cur_friend->next;
    cur_friend->next = friend_to_add;

}

_Bool delete_friend(user_t *user, char *friend_name){
    if(user->friends == NULL){
        return false;
    }
    
    friend_t *cur_friend = user->friends;
    friend_t *placeholder = NULL;

    while(cur_friend != NULL){
        if(strcmp(cur_friend->username, friend_name) == 0){
            if(placeholder == NULL){
                user->friends = cur_friend->next;
            }
            else{
                placeholder->next = cur_friend->next;
            }
            free(cur_friend);
            return true;
        }
        placeholder = cur_friend;
        cur_friend = cur_friend->next;
    }
    return false;
}

post_t *create_post(const char *text){
    post_t *new_post = (post_t *)malloc(sizeof(post_t));
    strcpy(new_post->content, text);
    new_post->next = NULL;
    return new_post;
}

void add_post(user_t *user, const char *text){
    post_t *new_post = create_post(text);
    post_t *cur_post = user->posts;

    if(user->posts == NULL){
        user->posts = new_post;
        return;
    }
    new_post->next = cur_post;
    user->posts = new_post;
}

_Bool delete_post(user_t *user){
    if(user->posts == NULL){
        return false;
    }
    post_t *cur_post = user->posts;
    user->posts = user->posts->next;
    cur_post = NULL;
    free(cur_post);
    return true;
}

void display_all_user_posts(user_t *user){
    post_t *cur_post = user->posts;
    printf("\nList of %s's posts: \n", user->username);
    for(int i = 1; cur_post != NULL; i++){
        printf("%d - %s\n", i, cur_post->content);
        cur_post = cur_post->next;
    }
}

void display_user_friends(user_t *user){
    friend_t *cur_friend = user->friends;
    printf("\nList of %s's friends: \n", user->username);
    for(int i = 1; cur_friend != NULL; i++){
        printf("%d - %s\n", i, cur_friend->username);
        cur_friend = cur_friend->next;
    }
}

void display_posts_by_n(user_t *user, int number){
    post_t *cur_post = user->posts; 
    _Bool display_more_posts = true; 
    int post_display = 1; 
    while(display_more_posts){
        for(int i = 0; i < number && cur_post != NULL; cur_post = cur_post->next, i++, post_display++){
            printf("%d - %s\n", post_display, cur_post->content);
        }
        if(cur_post == NULL){
            printf("\nPrinted all posts.");
            display_more_posts = false;
            return;
        }
        display_more_posts = get_y_n("\nWould you like to display more posts? Y/N: ");
    }
}

void teardown(user_t *users){
    user_t *cur_user = users;
    user_t *placeholder = NULL;

    while(cur_user != NULL){
        placeholder = cur_user->next; 

        // Free each friend
        friend_t *cur_friend = cur_user->friends; 
        while(cur_friend != NULL){
            friend_t *del_friend = cur_friend;
            cur_friend = cur_friend->next; 
            free(del_friend);
        }

        // Free each post
        post_t *cur_post = cur_user->posts;
        while(cur_post != NULL){
            post_t *del_post = cur_post; 
            cur_post = cur_post->next; 
            free(del_post);
        }

        free(cur_user);
        cur_user = placeholder;
    }
}

void print_menu(){ // ONLY prints the menu
    printf("\n******************************\n");
    printf("            MAIN MENU         \n");
    printf("******************************\n");
    printf("1. Register a new user\n2. Manage a user's profile (change password)\n3. Manage a user's posts (add/remove)\n4. Manage a user's friends (add/remove)\n5. Display a user's posts\n6. Exit\n");
}

void print_post_menu(){ // ONLY prints the menu
    printf("\n******************************\n");
    printf("            POST MENU         \n");
    printf("******************************\n");
    printf("1. Add a new post\n2. Remove a post\n3. Return to main menu\n");
}

void print_friend_menu(){
    printf("\n******************************\n");
    printf("           FRIENDS MENU         \n");
    printf("******************************\n");
    printf("1. Add a new friend\n2. Remove a friend\n3. Return to main menu\n");
}


/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
