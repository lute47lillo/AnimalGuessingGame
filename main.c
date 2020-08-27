//  Animal Guessing Game
//  main.c
//  HWF Trees
//
//  Created by Lute Lillo on 7/29/20.
//  Copyright © 2020 Lute Lillo. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MaxLine 128                 //Maximum space for the line.

#define TRUE 1
#define FALSE 0

#include "tree.h"                  //Includes the structs.

const char* negative[] = {"no", "nope", "negative", "naw", "wrong"};//Valid negative answers
const char* positive[] = {"yes", "affirmative", "of course", "right", "yep"};//Positives

Question* find_node(int searchID, QNode* look) { //Auxiliar function that finds the desired node.
    if(look == NULL){
        return NULL;                        //Base Case, where the linked list is empty.
    }
    if(searchID == 0){                      //If ID is to a leave node, set to null.
        return NULL;
    }
    
    QNode* p = look;
    
    while(p!=NULL){                         //Traverses and finds.
        if(p->questionID == searchID){      //If found, return the location. Else, move pointer
            return p->toQuestion;
        }
        p = p->next;                        //to the next and repeat.
    }
    return NULL;                            //If p == NULL, return NULL
}

Question* load_tree(){
    
    FILE* input = fopen("animal.txt", "r");               //Open the file to read from it.
    
    char line[MaxLine];                            //Create line to retrieve from file.
    QNode* head = NULL;                            //Head of the linked list
    Question* root = (Question*)malloc(sizeof(Question));   //Root of the Tree to be returned.
    

    while(fgets(line, sizeof(line), input) != NULL){   //while there are lines to be read in file
        
        QNode* reading = (QNode*)malloc(sizeof(QNode));   //Node to read the data.
        reading->questionID = atoi(strtok(line, ","));       //loading the question ID
        
    
        reading->toQuestion = (Question*)malloc(sizeof(Question)); //Allocates memory for the Q.
        char* tempCH;
        tempCH = strtok(NULL, ",");
        strcpy(reading->toQuestion->ask, tempCH);
        //printf("text: %s\n", reading->toQuestion->ask);       //Debug

        // load NO ID
        reading->noID = atoi(strtok(NULL, ","));
        //printf("NoID: %d\n", reading->noID);                  //Debug
        
        // load YES ID
        reading->yesID = atoi(strtok(NULL, ","));
        //printf("YesID: %d\n", reading->yesID);                //Debug
      
        reading->next = head;                                  //Inserts in linked list and moves
        head = reading;
   
    }
    
    
    fseek(input, 0, SEEK_END);                          // go to end of file
    if (ftell(input) == 0){
        //Creates a basic Tree with one root and two leaves if the file is empty.
           strcpy(root->ask, "Does it have a fur?");
                        
            Question* childLeft = (Question*)malloc(sizeof(Question));   //Creates the NO leave
            root->no = childLeft;
            strcpy(childLeft->ask, "Is it a snake?");
            childLeft->no = NULL;                            //Assigns their leaves to NULL
            childLeft->yes = NULL;
                    
            Question* childRight = (Question*)malloc(sizeof(Question)); //Create the YES leave.
            root->yes = childRight;
            strcpy(childRight->ask, "Is it a cat?");
            childRight->no = NULL;                      //Assigns their leaves to NULL
            childRight->yes = NULL;
     }

    
    QNode* holdStart = head;                //Traverses the list to set the pointer correctly
    while(head != NULL){
        head->toQuestion->yes = find_node(head->yesID, head);       //Sets the Yes pointer
        head->toQuestion->no = find_node(head->noID, head);         //Sets the No pointer
        head = head->next;
    }
    
    head = holdStart;
    free(holdStart);
    
    while(head != NULL){                        //Finds the root of the tree and returns to start
        if(head->questionID == 1){              //playing with it.
            root = head->toQuestion;
        }
        head = head->next;
    }
        
    return root;                //Returns root.
}

int postorderT(Question* t, FILE* output, int counterID){   //Traverses the tree.
   
    int lastID = counterID;                 //Keeps track of QuestionID's, NoID's and YesID's
    int noID = 0;
    int yesID = 0;
    
    if(t->no != NULL && t->yes != NULL){                  //If both leaves are null.
        
        noID = counterID + 1;
        
        yesID = 1 + postorderT(t->no, output, noID);     //Recursive call for the left branch
        
        lastID = postorderT(t->yes, output, yesID);      //Recursive call for the right branch

    }
  
    fprintf(output, "%d,%s,%d,%d\n", counterID, t->ask, noID, yesID); //Prints to file.
    
    return lastID;
}

void save_tree(Question* rootPrint){                //Saves the tree for next times playing
    FILE* output;
    output = fopen("animal.txt", "w");              //Opens the file to write on it.
    int counterID = 1;
    postorderT(rootPrint, output, counterID);       //Traverses the tree in postorder.
    fclose(output);                                 //Closes the file.
}


void cleanup_tree(Question* root){
    if(root == NULL){                        //Base case where the root is empty and
        printf("Is null\n");                 //it prints null.
        return;
    }
    
    if(root->yes == NULL && root->no == NULL){          //Base case where root has no leaves.
        free(root);
        
        return;
    }else{
        if(root->no != NULL){                    //Checks for No Leaves first, goes to the
            cleanup_tree(root->no);                 //deepest with leaves, first deletes NO's
        }
        if(root->yes !=NULL){                    //Next  time it will check for YES' leaves
            cleanup_tree(root->yes);                //And will free them.
        }
    }

}
char* get_user_input(){                     //Gets every kind of input from the user.
    
    char getAns[MAX_STR_SIZE];
    if (!fgets(getAns, sizeof(getAns), stdin)) {
        return NULL;                                // If fails the reading, returns NULL
    }
    
    int length = (int)strlen(getAns);
    if (length > 0 && getAns[length - 1] == '\n') {
     getAns[--length] = '\0';      //Removes the newline.
    }
    char* answer = malloc(length +1);  //Allocates space in memory (1 + len) for the answer.
    if(!answer){                    //Checks for error on answer.
        return NULL;
    }
    
    return strcpy(answer, getAns);  //Copies the temp asnwer into the return answer.
    
}

int get_yes_or_no(){
    char* answer = get_user_input();            //Gets the answer from the user. YES or NO
    
    for(int c =0; answer[c];c++){               //Converts the answer to all lower case.
        answer[c] = tolower(answer[c]);
    }
    
    for(int i = 0; i<5;i++){                    //Checks for negative valid answers.
        if(strcmp(answer, negative[i])==0){
            
            return -1;
        }else{                                  //Or checks for positive valid answers.
            if(strcmp(answer, positive[i])==0){
                
                return 0;
            }
        }
    }
    
    return EXIT_SUCCESS;
}

void ask(Question* askit){
    printf("%s\n", askit->ask);                     //Prompt the question to the console.
    int userAsnwer = get_yes_or_no();               //Get a yer or no for the question.
    
    if(askit->no == NULL && askit->yes == NULL){        //Base case. Both options are leaves.
        if(userAsnwer == 0){                            //If user chooses yes.
            printf("OUHHH! The computer guessed! It looks that's smarter than you...\n");
            return;
        }else{                                          //If user chooses no.
            printf("What was the name of the animal you were thinking?\n");
            char* getAnimal = get_user_input();         //Get the animal that the user
            strcpy(askit->animal, getAnimal);           //was thinking.
           
            
            Question* newNo = (Question*)malloc(sizeof(Question));     //New Question to guess.
            askit->no = newNo;                      //Set the no pointer of prev to new Question
            printf("Please enter a yes/no question.\n");
            
            char* newQ = get_user_input();         //Get the new question
            strcpy(newNo->ask, askit->ask);       //The new Question is the previous Q. asked.
            
            strcpy(askit->ask, newQ);           //Actual question is refreshed.
            newNo->no = NULL;                   //Leaves for that question are created.
            newNo->yes = NULL;
            
            
            
            Question* newYes = (Question*)malloc(sizeof(Question)); //Create the yes node.
            askit->yes = newYes;                                    //Assigning it.
            
            
            char baseUseA[20] = "Is it a ";         //Create the new sentence for the tree
            char baseUseB[20] = "?";              //Using the animal that it was and
            strcat(baseUseA, askit->animal);        //assigning it to a Yes node.
            strcat(baseUseA, baseUseB);
            strcpy(newYes->ask, baseUseA);
            
            
            
            newYes->no = NULL;                      //Their new nodes go to NULL and NULL. Create
            newYes->yes = NULL;                     //the leaves for the Answer New answer Yes.
            return;
           
        }
        
    }else if(userAsnwer == 0){                      //If there are internal nodes. Call recursive
        ask(askit->yes);                            //until it is found(Yes answer) or no more
    }else{                                          //questions are available.
        ask(askit->no);
    }

}

int main(int argc, const char * argv[]) {
   
    Question* root = (Question*)malloc(sizeof(Question));     //Create roote Node
    
    root = load_tree();                       //Loads the last tree.
    
    int play = 0;                           //Set play to default 0 (TRUE)
    while(play == 0){                       //Play until the user decides not to play.
        ask(root);
        printf("Do you want to play again? (Yes/No)\n");
        play = get_yes_or_no();             //A no answer will stop execution.
    }
    
    save_tree(root);                        //Saves tree for next time.
    cleanup_tree(root);                     //Frees the nodes of the tree.
    free(root);                             //Frees the root.
    return 0;

}
