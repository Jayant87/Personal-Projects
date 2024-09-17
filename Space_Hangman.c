#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ctype.h>
#include<string.h>

void hangman(int count);
int number_of_loss = 0;
int reset_compare = 0;
char const *question[10] = {"Guess What Microcontroller is most used ?" , "How do you say cool in Japnese" , "Who devloped/created C programming language",
"Which species falls under brinds but cant fly and lives in cold", "Closest Planet to the sun", "Closest star to our solar system", 
"Sun's __________ protects us from cosmic radation", "Neutron star that spins at a high speed is called",
 "If a star collapses and has slightly less mass than what is required for it to become a black hole .It becomes a ______ STAR", 
 "The mass of a galaxy is way more than the it should be based on the physically visible astronomical objects this is due to the presence of "};
char const *ans[10] = {"ARM", "KAKKOII", "DENNISRITCHIE", "PENGUIN", "MERCUREY", "PROXIMACENTAURI","HELIOSPHERE","PULSAR", "NEUTRON","DARK MATTER"};
char const *compare;
void reset(int* , const char**);

FILE *file_pointer;

int main(int argc,char* argv[])
{
    /*if(argv[1] == "hello")
    {
        printf("gottcha");
    }*/
    file_pointer = fopen("SPACEHANGMAN.txt", "w");
    if(file_pointer == NULL)
    {
        printf("File Not Present");
    }
   /* char *ch1 = "Welcome to Space Hangman ;>";
    while(ch1 != "\0")
    {
        fputc(*ch1, file_pointer);
        
    }*/
    printf("Welcome to Space Hangman ;>");
    srand(time(NULL)); 
    int test = rand();
    int random = test % 10;
    //printf("%ld", random);
    //printf("Hint: %s \n", question[random]);   
    compare = ans[random];
    char ch;

    while(1)
    {
        ch = *compare;    
        //printf("%c",ch); 
        char input;
        printf("\n Hint: %s \n", question[random]);  
        printf("Word: \t");
        fflush(stdin);
        scanf("%c", &input);
        if(number_of_loss == 3)
        {
            printf("Do you want to play again press (y/n) \n");
            fflush(stdin);
            char play;
            scanf("%c", &play);
            if(play == 'y')
            {
                reset(&random, ans);
                continue;
            }
            
            else
            fclose(file_pointer);
            exit(0);
        }
        input = toupper(input);

        
        if(input == ch)
        {
            compare++;
            reset_compare++;
            printf("\n _______________ \n\n");
            hangman(number_of_loss);
            printf("Good Guess! \n");
            if(*compare == '\0')
            {
                number_of_loss = 0;
                printf("\n\n\t\tYou win! \n");
                printf("Do you want to play again press (y/n) \n");
                fflush(stdin);
                char play;
                scanf("%c", &play);
                if(play == 'y')
                {
                    reset(&random, ans);
                    continue;
                }

                else
                fclose(file_pointer);
                exit(1);
            }
        }
        else
        {
            number_of_loss++;
            printf("\n _______________ \n");
            hangman(number_of_loss);
        }

    }
    return 0;
}

void hangman(int count)
{
    if(count == 1)
    {
        printf(" |               | \n");
        printf(" |               O \n\n\n \t\t\t\t 2 Lives Remains");
        fflush(stdin);
    }
    if(count == 2)
    {
        printf(" |               | \n");
        printf(" |               O \n");
        printf(" |               | \n");
        printf(" |              /|\\ \n\n\n \t\t\t\t 1 Life Remains");
        fflush(stdin);
    }
    if(count == 3)
    {
        printf(" |               | \n");
        printf(" |               O \n");
        printf(" |               | \n");
        printf(" |              /|\\ \n");
        printf(" |             | | | \n");
        printf(" |               | \n");
        printf(" |              / \\ \n");
        printf(" |             |   | \n\n\n\n \t\t\t\t\t\tYOU LOSE!!!!");
        fflush(stdin);
    }
}

void reset(int* random, const char** ans)
{
    //printf("array %s",ans[0]);
    srand(time(NULL)); 
    number_of_loss = 0;
    int test = rand();
    *random = test % 2;
    //printf("%d", *random);
   //printf("Hint: %s \n", question[*random]);
    compare = ans[*random];
    //fflush(stdin);   
}
