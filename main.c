/********* main.c ********
    Student Name 	= Jules Wong
    Student Number	= 101309829 
*/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_functions.h"

int main()
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }

    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);

    _Bool do_print_menu = true;
    while(do_print_menu){
        print_menu();
        printf("\nEnter your choice: ");
        int menu_choice = get_valid_int(1, 6);
        if(menu_choice == 6){
            exit;
        }
        switch(menu_choice){
            case 1: // Choice 1: Register a new user
                printf("\nRegistering new user...");
                const char * new_username;
                const char * new_password;
                new_username = get_string("\nEnter a username: ");
                new_password = get_string("Enter a password: ");
                user_t *new_user = add_user(users, new_username, new_password);

                if(new_user != NULL){
                    printf("\nNew user added successfully!\n");
                    break;
                }
                else{
                    printf("\nThere was an error adding a new user. Please try again.");
                    break; // return to main menu?
                }

            case 2: // Choice 2: Manage a user's profile
                printf("\nManaging user profiles...");
                const char *manage_username;
                manage_username = get_string("\nEnter a username to update their password: ");
                user_t *found_user = find_user(users, manage_username);
                if(found_user == NULL){ 
                    printf("\nUser not found. Returning to main menu...\n");
                    break; 
                }
                const char *old_password; 
                old_password = get_string("Enter the current password: ");
                if(strcmp(found_user->password, old_password) != 0){
                    printf("Incorrect password. Returning to main menu...\n");
                    break;
                }
                const char *update_password; 
                update_password = get_string("Enter the new password: ");
                strcpy(found_user->password, update_password);
                printf("Password for %s changed successfully. Thank you!\n", found_user->username);
                break; 

            case 3: // Choice 3: Manage a user's posts
                printf("\nManaging user posts...");
                char * poster_username; 
                poster_username = get_string("\nEnter the username of the user whose posts you want to manage: ");
                user_t *poster_user = find_user(users, poster_username);
                if(poster_user == NULL){
                    printf("\nUser not found. Returning to main menu...");
                    break; 
                }
                printf("\nManaging %s's posts...\n", poster_username);
                if(poster_user->posts == NULL){
                    printf("\nNote: No posts currently registered to this user");
                }
                _Bool print_my_post_menu = true;
                while(print_my_post_menu){
                    print_post_menu();
                    int post_menu_choice = get_valid_int(1, 3);
                    switch(post_menu_choice){
                        case 1:
                            printf("\nType your content here: ");
                            const char *post_content;
                            scanf(" %[^\n]", post_content);
                            add_post(poster_user, post_content);
                            display_all_user_posts(poster_user);
                            break;
                        case 2:
                            printf("\nDeleting a post from %s...", poster_username);
                            if(delete_post(poster_user) == false){
                                printf("\nUnable to delete most recent post. Returning to main menu...");
                                break;
                            }
                            printf("\nSuccessfully deleted most recent post. Printing all posts: \n");
                            display_all_user_posts(poster_user);
                            break;
                        case 3: 
                            print_my_post_menu = false;
                            break;
                    }
                }
                break;

            case 4: // Choice 4: Manage a user's friends 
                printf("\nManaging user friends...");
                char * friends_username; 
                friends_username = get_string("\nEnter the username of the user whose friends you want to manage: ");
                user_t *friends_user = find_user(users, friends_username);
                if(friends_user == NULL){
                    printf("\nUser not found. Returning to main menu...");
                    break; 
                }
                printf("\nManaging %s's friends...\n", friends_username);
                _Bool to_print_friend_menu = true;
                while(to_print_friend_menu){
                    print_friend_menu();
                    int friend_menu_choice = get_valid_int(1, 3);
                    switch(friend_menu_choice){
                        case 1:
                            char* new_friend_name;
                            new_friend_name = get_string("\nEnter the username of the friend you wish to add: ");
                            add_friend(friends_user, new_friend_name);
                            printf("\nFriend added to the list.");
                            display_user_friends(friends_user);
                            break;
                        case 2:
                            display_user_friends(friends_user);
                            char* friend_to_del;
                            friend_to_del = get_string("\nEnter the username of the friend you wish to delete: ");
                            _Bool del_friend_status = delete_friend(friends_user, friend_to_del);
                            if(del_friend_status){
                                printf("\nFriend deleted successfully. Printing current list of friends...");
                                display_user_friends(friends_user);
                                break;
                            }
                            printf("\nUnable to delete friend. Returning to post menu...");
                            break;
                        case 3:
                            to_print_friend_menu = false;
                            break;
                    }
                }
                break;
            case 5: // Choice 5: Display all posts from a given user
                printf("\nDisplaying posts from a user...");
                char * display_username;
                display_username = get_string("\nEnter the username of the user whose posts you want to display: ");
                user_t *display_user = find_user(users, display_username);
                if(display_user == NULL){
                    printf("\nUser not found. Returning to main menu...");
                    break; 
                }
                int num_posts = post_counter(display_user);
                if(num_posts == 0){
                    printf("\nUser has no posts. Returning to main menu...");
                    break;
                }
                printf("\nPrinting %s's posts...", display_user->username);
                if(num_posts <= 3){
                    display_all_user_posts(display_user);
                    printf("\nPrinted all posts. Returning to main menu...");
                    break;
                }
                printf("\nList of %s's posts: \n", display_user->username);
                display_posts_by_n(display_user, 3);
                printf("\nReturning to main menu...");
                break;
            case 6: 
                printf("\nThanks for using Facebook. Goodbye!");
                do_print_menu = false;
                teardown(users); // ??????????????????????????????????????????????????
                break;

        }
    }
 
}