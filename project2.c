#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QUESTION_LENGTH 100
#define ANSWER_LENGTH 30 
#define ADDITIONAL_SIGNS 2

typedef struct {
    char Question[QUESTION_LENGTH];
    char Answer[ANSWER_LENGTH]; 
}QuestionData; 


int getNumberOfFileQuestions(FILE* file)
{
    int counter = 1; 
    char temp = getc(file);
    while (temp != EOF){
        if(temp == '\n')
        {
            counter ++; 
        }
        temp = getc(file);
    };
    fseek(file, 0, SEEK_SET);
    return counter; 
};

void QnAArrayInitialization(QuestionData* QnA, int numberOfQnA)
{
    for(int i = 0; i < numberOfQnA; i++)
    {
        memset(&QnA[i].Question, '\0', QUESTION_LENGTH);
        memset(&QnA[i].Answer, '\0', ANSWER_LENGTH);
    }
}


void read_array_from_file(FILE* file, QuestionData* QnA, int numberOfQuestions){
    
    static int j, p;
    static int i = 0;
    
    char line[QUESTION_LENGTH + ANSWER_LENGTH + ADDITIONAL_SIGNS];
    char question[QUESTION_LENGTH];
    char answer[ANSWER_LENGTH];

    while ( fgets (line, 128, file ) != NULL ){ /* read a line */
        memset(question, '\0', QUESTION_LENGTH);
        memset(answer, '\0', ANSWER_LENGTH);

        for (j = 0; j <= 128; j ++){
            question[j] = line[j];
            if (line[j] == '?'){
                break;
            }
        }
        j ++;
        for (p = 0; p <= 128; p ++){
            j ++;
            if (line[j] == '\0'){
                break;
            }
            if (line[j] >= 65 && line[j] <= 90){
                line[j] = line[j] + 32;
            }
            if (line [j] >= 97 && line[j] <= 122){
                answer[p] = line[j];
            }
        }
       
        memcpy( (QnA[i].Question),question,QUESTION_LENGTH);
        memcpy( (QnA[i].Answer),answer,ANSWER_LENGTH);

        i ++;
    }
 
}

int displayQuestions(QuestionData* QnA, int numberOfQuestions, int difficulty, int* incorrectQuestions){
    int correctQuestions = 0;
    int totalQuestionsSoFar = 0;
    printf("----START OF QUIZ----\n");
    srand(time(NULL));
    int answeredQuestions[numberOfQuestions];
    for (int j = 0; j < numberOfQuestions; j ++){
        int currentQuestion = rand() % numberOfQuestions;
        int questionCheck = 0;
        while (questionCheck != 1){
            currentQuestion = rand() % numberOfQuestions;
            questionCheck = 1;
            for (int m = 0; m < numberOfQuestions; m ++){
                if (currentQuestion == answeredQuestions[m]){
                    questionCheck = 0;
                    break;
                }
            }
        }
        answeredQuestions[j] = currentQuestion;
        printf("Question %d \n", j + 1);
        printf("%s \n", QnA[currentQuestion].Question);
        if (difficulty == 1){
            printf("%c ", QnA[currentQuestion].Answer[0]);
            for (int m = 0; m <= strlen(QnA[currentQuestion].Answer)-3; m ++){
                printf("- ");
            }
            printf("%c\n", QnA[currentQuestion].Answer[strlen(QnA[currentQuestion].Answer)- 1]);
        }
        else if (difficulty == 2){
            int numberCheck = -1;
            int k = rand() % (strlen(QnA[currentQuestion].Answer));
            int p = rand() % (strlen(QnA[currentQuestion].Answer));
            while ( p == k){
                p = rand() % (strlen(QnA[currentQuestion].Answer));
            }
            int smallerInt;
            int biggerInt;
            if (k > p){
                smallerInt = p;
                biggerInt = k;
            }
            else {
                smallerInt = k;
                biggerInt = p;
            }
            for (int m = 0; m < smallerInt; m ++){
                printf("- ");
            }
            printf("%c ", QnA[currentQuestion].Answer[smallerInt]);
            for (int m = smallerInt + 1; m < biggerInt ; m ++){
                printf("- ");
            }
            printf("%c ", QnA[currentQuestion].Answer[biggerInt]);
            for (int m = biggerInt + 1; m < strlen(QnA[currentQuestion].Answer); m ++){
                printf("- ");
            }
            printf("\n");
        }
        else if (difficulty == 3){
            for (int m = 0; m < strlen(QnA[currentQuestion].Answer); m ++){
                printf("- ");
            }
            printf("\n");
        }
        else if (difficulty == 4){
            char clue[strlen(QnA[currentQuestion].Answer)];
            int numberCheck[strlen(QnA[currentQuestion].Answer)];
            for (int i = 0; i < strlen(QnA[currentQuestion].Answer); i ++){
                int k = rand() % (strlen(QnA[currentQuestion].Answer));
                int randomCheck = 0;
                while (randomCheck != 1){
                    k = rand() % (strlen(QnA[currentQuestion].Answer));
                    randomCheck = 1;
                    for (int m = 0; m < strlen(QnA[currentQuestion].Answer); m ++){
                        if (k == numberCheck[m]){
                            randomCheck = 0;
                            break;
                        }
                    }
                }
                numberCheck[i] = k;
                clue[k] = QnA[currentQuestion].Answer[i];
            }
            for (int z = 0; z < strlen(QnA[currentQuestion].Answer); z ++){
                printf("%c ", clue[z]);
            }
            printf("\n");
        }
        else if (difficulty == 5){
            printf("?\n");
        }
        char userAnswer[ANSWER_LENGTH];
        scanf("%s",userAnswer);
        char answer[ANSWER_LENGTH];
        int letterCheck = 1;
        for (int i = 0; i < strlen(QnA[currentQuestion].Answer); i ++){
           if (QnA[currentQuestion].Answer[i] != '\n'){
                if (userAnswer[i] >= 65 && userAnswer[i] <= 90){
                    userAnswer[i] = userAnswer[i] + 32;
                }
                if (userAnswer[i] == QnA[currentQuestion].Answer[i]){
                }
                else {
                   letterCheck = 0;
                }
            }
        } 
        if (letterCheck == 1){
            printf("CORRECT \n");
            correctQuestions ++;
        }
        else {
            printf("INCORRECT \n");
            incorrectQuestions[currentQuestion] = 1;
        }
        totalQuestionsSoFar ++;
        printf("Current score = %d/%d\n",correctQuestions,totalQuestionsSoFar);
        printf("\n");
    }
    return correctQuestions;
}

void saveToFile(char* filename, int correctQuestions, int totalQuestions, int difficulty){
    FILE* file;
    file = fopen("quiz_history.txt","a"); 
    fprintf(file,"Name of quiz: %s, Score: %d/%d, Difficulty: %d\n", filename,correctQuestions,totalQuestions,difficulty);
    fclose(file);
    
}

void reviewQuiz(int* incorrectQuestions, QuestionData* QnA, int numberOfQuestions){
    int reviewQuiz;
    printf("Would you like to see the answers to the incorrect questions? (1 = yes, 0 = no)\n");
    scanf("%d", &reviewQuiz);
    if (reviewQuiz == 1){
        for (int i = 0; i < numberOfQuestions; i ++){
            if (incorrectQuestions[i] == 1){
                printf("%s \n %s \n", QnA[i].Question,QnA[i].Answer);
            }
        }
    }
}

void main(void) {
    FILE *file;
    srand(time(NULL));
    char fileName[256];
    printf("Please enter the name of the file followed by a .txt >>> \n");
    scanf("%s",fileName);
    int difficulty;
    printf("Please select your difficulty level (1-6)\n 1. Easy \n 2. Intermediate \n 3. Hard \n 4. Randomized letters \n 5. No clues \n 6. Random\n");
    scanf("%d",&difficulty);
    while (difficulty <= 0 || difficulty > 6){
        printf("The delected difficulty level does not exist\n");
        printf("Please select your difficulty level (1-6)\n 1. Easy \n 2. Intermediate \n 3. Hard \n 4. Randomized letters \n 5. No clues \n 6. Random\n");
        scanf("%d",&difficulty);
    }
    if (difficulty == 6){
        difficulty = rand() % 5 + 1;
    }
    printf("%d", difficulty);
    if ((file = fopen(("./%s",fileName), "r")) == NULL ){
        printf("Error opening file");
    };

    int numberOfQuestions = getNumberOfFileQuestions(file);
    int incorrectQuestions[numberOfQuestions];
    for (int i = 0; i < numberOfQuestions; i ++){
        incorrectQuestions[i] = 0;
    }
    printf("Number of questions: %d \n\n", numberOfQuestions);
    QuestionData* QnA = (QuestionData*)malloc(sizeof(QuestionData) * numberOfQuestions); 
    QnAArrayInitialization(QnA, numberOfQuestions); 

    read_array_from_file(file, QnA, numberOfQuestions);

    int correctQuestions = displayQuestions(QnA,numberOfQuestions, difficulty,incorrectQuestions);
    printf("you got %d out of %d questions correct\n",correctQuestions,numberOfQuestions);

    reviewQuiz(incorrectQuestions,QnA,numberOfQuestions);

    saveToFile(fileName,correctQuestions,numberOfQuestions,difficulty);

    printf("Thanks for taking the quiz");

    fclose(file);
    free(QnA);

}
