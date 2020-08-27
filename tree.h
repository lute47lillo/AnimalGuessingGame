//  Animal Guessing Game
//  tree.h
//  HWF Trees
//
//  Created by Lute Lillo on 7/28/20.
//  Copyright © 2020 Lute Lillo. All rights reserved.
//

#ifndef tree_h
#define tree_h
#define MAX_STR_SIZE 1024

typedef struct list_node QNode;     //Creates Struct for the Linked List.

typedef struct tree_node{           //Struct of Questions for the binary tree
    struct tree_node *yes, *no;     //Contains pointers to next Question depending on yes or no.
    char ask[MAX_STR_SIZE];         //The question itself.
    char animal[50];                //The new animal that the user was thinking.
}Question, *QuestionPtr;


struct list_node{                  //Struct of Linked List
    Question *toQuestion;          //Points to the question to later get the text of the question
    int questionID;                //ID of the specific question
    int yesID;                     //Includes ID's of their child nodes.
    int noID;
    QNode *next;                   //Pointer to next element in the Linked List
    
};


#endif /* Tree_h */


