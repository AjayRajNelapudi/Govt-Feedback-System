//
//  main.c
//  Feedback
//
//  Created by Ajay Raj Nelapudi on 21/10/18.
//  Copyright © 2018 Ajay Raj Nelapudi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int feedbackPtr = 0, allFeedbacks = 6;
int logPtr = 0, allLogs = 100;
int qPtr = 0, allQ = 10;

struct FeedbackForm {
    int id;
    char phone[12];
    int response[10];
}feedback[6];

struct Log {
    int id;
    char phone[12];
    char timestamp[10];
    int status;
}logs[100];

struct Questions {
    char query[50];
    int freq;
}queries[10];

void init() {
    char numbers[][12] = {"7893303535", "9440899608", "9491783295", "7893302424", "9449789090", "7893303536"};
    for(int i=0; i<6; i++) {
        strcpy(feedback[i].phone, numbers[i]);
        for(int j=0; j<10; j++) {
            feedback[i].response[j] = 0;
        }
    }
    
    
    char askedQuestions[][50] = {
        "Are you satisfied with Govt hospitals?",
        "Are the doctors available all the time?",
        "Are you satisfied with hospitals hygeine?",
        "Are you satisfied with municipal services?",
        "Are you satisfied with water supply?",
        "Are you satisfied with govt schools?",
        "Are you satisfied with govt agriculture support?",
        "Are you satisfied with Police Department?",
        "Are you satisfied with Traffic System?",
        "Are you satisfied with Anna Canteen?"
    };
    
    for(int i=0; i<10; i++) {
        strcpy(queries[i].query, askedQuestions[i]);
        queries[i].freq = 0;
    }
}

int recordResponse(int id, int start, int stop) {
    for(int i=start; i<stop; i++) {
        int answer;
        printf("%s\nEnter your choice (1, 0, -1, 9 -> disconnect call) : ", queries[i].query);
        scanf("%d", &answer);
        if(answer == 9) return 1;
        feedback[id].response[i] = answer;
    }
    return 0;
}

int callNumberID(int id) {
    if(logPtr >= 100) return 1;
    
    char defaultTimestamp[] = "xx:xx:xx";
    printf("Calling %s ... \n", feedback[id].phone);
    logs[logPtr].id = id;
    strcpy(logs[logPtr].phone, feedback[id].phone);
    strcpy(logs[logPtr].timestamp, defaultTimestamp);
    logs[logPtr].status = 0;
    logPtr++;
    
    int satisfied;
    printf("Are you satisfied with all services (1/0) ? ");
    scanf("%d", &satisfied);
    if(satisfied) {
        for(int i=0; i<allQ; i++) {
            feedback[id].response[i] = 1;
        }
    } else {
        for(int i=0; i<3; i++) {
            printf("Choose Dept\n1. Hospital\n2. Municiplaity\n3. Police\nEnter your choice: ");
            int choice;
            scanf("%d", &choice);
        
            switch (choice) {
                case 1:
                    if(recordResponse(id, 0, 3)) return 1;
                    break;
                
                case 2:
                    if(recordResponse(id, 3, 7)) return 1;
                    break;
                
                case 3:
                    if(recordResponse(id, 7, 10)) return 1;
                    break;
            
            }
            
            int continueChoice;
            printf("Do you wish to continue (1/0) ? ");
            scanf("%d", &continueChoice);
            if(!continueChoice) break;
        }
    }
    
    logs[logPtr - 1].status = 1;
    return 0;
}

void updateLog(int ptr) {
    for(int i=ptr; i<logPtr; i++) {
        logs[i] = logs[i + 1];
    }
}

void redail() {
    for(int i=0; i<logPtr-1; i++) {
        if(!logs[i].status) {
            if(!callNumberID(i)) updateLog(i);
        }
    }
}

int* generateReports() {
    int *summary;
    summary = malloc(sizeof(int) * 10);
    for(int i=0; i<10; i++) {
        summary[i] = 0;
    }
    
    for(int i=0; i<allFeedbacks; i++) {
        for(int j=0; j<allQ; j++) {
            summary[j] += feedback[i].response[j];
        }
    }
    
    return summary;
}

void printAll()
{
    for(int i=0;i<6;i++)
    {
        printf("\n %s response is:",feedback[i].phone);
        for(int j=0;j<10;j++)
        {
            printf("%d ",feedback[i].response[j]);
        }
    }
    printf("\n");
}

int main() {
    //freopen("input.txt", "r", stdin);
    init();
    
    for(int i=0; i<6; i++) {
        callNumberID(i);
    }
    
    redail();
    
    int* summary = generateReports();
    printf("The summary is: ");
    for(int i=0; i<allQ; i++) {
        printf("%d ", summary[i]);
    }
    printf("\n");
    printAll();
    return 0;
}
