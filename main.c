#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

#define TOP_N 3

void displayMenu() {
    printf("\nPlease select an option:\n");
    printf("0. Print the current text\n");
    printf("1. Input a text which will then be printed.\n");
    printf("2. Read a text from a file.\n");
    printf("3. Encrypt a text with a specified shift.\n");
    printf("4. Decrypt a text with a known shift.\n");
    printf("5. Compute and display the frequency distribution of a text.\n");
    printf("6. Breaking the encrypted text using frequency analysis.\n");
    printf("7. Exit.\n");
}

void readTextFromFile(const char *filename, char *text) {

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    if (fileSize <= 0) {
        printf("Error: Empty file.\n");
        fclose(file);
        return ;
    }

    for (int i = 0; i < fileSize; i++) {
        text[i] = fgetc(file);
    }

    text[fileSize] = '\0';

    fclose(file);
}

void encryptText(char *text, int shift) {

    int i=0;

    while (text[i]!='\0') {

        char character=text[i];

        if (character>='A' && character<='Z') {
            text[i]=((character-'A'+shift+26)%26)+'A';
        }
        else if (character>='a' && character<='z') {
            text[i]=((character-'a'+shift+26)%26)+'a';
        }

        i+=1;

    }
}

void decryptText(char *text, int shift) {

    int i=0;

    while (text[i]!='\0') {

        char character=text[i];

        if (character>='A' && character<='Z') {
            text[i]=((character-'A'-shift+26)%26)+'A';
        }
        else if (character>='a' && character<='z') {
            text[i]=((character-'a'-shift+26)%26)+'a';
        }

        i+=1;

    }
}

void read_distribution(const char *filename, double distribution[ALPHABET_SIZE]) {

    FILE *file = fopen(filename, "r");
    
    if (file == NULL) {
        printf("Error: Could not open file. %s\n", filename);
        return;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (fscanf(file, "%lf", &distribution[i]) != 1) {
            printf("Error: Failed to read number.\n");
            break;
        }
    }
    
    fclose(file);
    
}

void displayPercentageOcurrence(double distribution[ALPHABET_SIZE]) {

    int number=0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (distribution[i]!=0) {
            number+=distribution[i];
        }
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (distribution[i]!=0) {
            double percentage=(distribution[i]/number)*100;
            printf("%c:%.2lf%\n", 65+i ,percentage);
        }
    }
}

void compute_histogram(const char *text, double histogram[ALPHABET_SIZE]) {

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            histogram[text[i] - 'a']++;
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            histogram[text[i] - 'A']++;
        }
    }

}

double chi_squared_distance(const double hist1[ALPHABET_SIZE],const double hist2[ALPHABET_SIZE]) {

    double chi_squared = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (hist2[i]!=0) {
            double aux= (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]);
            chi_squared += aux/hist2[i];
        }
    }

    return chi_squared;
}

double euclidian_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {

    double euclidian_distance = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        euclidian_distance += (hist1[i] - hist2[i]) * (hist1[i] - hist2[i]);
    }

    return sqrt(euclidian_distance);
}

double cosine_distance(const double hist1[ALPHABET_SIZE], const double hist2[ALPHABET_SIZE]) {

    double sum1 = 0, sum2 = 0, sum3 = 0;

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        sum1 += hist1[i] * hist2[i];
        sum2 += hist1[i] * hist1[i];
        sum3 += hist2[i] * hist2[i];
    }

    if (sum1 == 0 || sum2 == 0) {
        return 1.0;
    }

    double cosine_distance = 1 - sum1/(sqrt(sum2)*sqrt(sum3));

    return cosine_distance;
}

void break_caesar_cipher(const char* text, int top_shifts[TOP_N], double top_distances[TOP_N], double (*distance_function)(const double[], const double[])) {

    char newText[strlen(text) + 1];
    strcpy(newText, text);
    double distribution[ALPHABET_SIZE];
    read_distribution("distribution.txt", distribution);

    for (int n = 1; n < ALPHABET_SIZE; n++) {

        double histogram[ALPHABET_SIZE];
        int i=0;
        while (newText[i]!='\0') {
            char character=text[i];
            if (character>='A' && character<='Z') {
                newText[i]=((character-'A'-n+26)%26)+'A';
            }
            else if (character>='a' && character<='z') {
                newText[i]=((character-'a'-n+26)%26)+'a';
            }
            i+=1;
        }

        compute_histogram(newText, histogram);

        double distance = distance_function(histogram, distribution);

        if (distance<top_distances[0]) {
            top_shifts[2] = top_shifts[1];
            top_shifts[1] = top_shifts[0];
            top_distances[2] = top_distances[1];
            top_distances[1] = top_distances[0];
            top_distances[0] = distance;
            top_shifts[0] = n;
        }
        else if (distance<top_distances[1]) {
            top_distances[2] = top_distances[1];
            top_shifts[2] = top_shifts[1];
            top_distances[1] = distance;
            top_shifts[1] = n;
        }
        else if (distance<top_distances[2]) {
            top_distances[2] = distance;
            top_shifts[2] = n;
        }

    }

}

int main(void) {

    double distribution[ALPHABET_SIZE];
    read_distribution("distribution.txt", distribution);

    char option;
    char text[99999];

    system("chcp 65001");

    do {

        displayMenu();
        scanf(" %c", &option);

        switch (option) {

            case '0': {

                printf("%s",text);

                break;

            }

            case '1': {

                getchar(); //removes any stray \n left in text

                fgets(text,9999,stdin);

                printf("Your inputed text is: %s\n", text);

                break;

            }

            case '2': {

                char filename[1000];

                printf("Enter the name of the file:");
                scanf("%9999999s", filename);

                readTextFromFile(filename, text);

                printf("Your text read from the file is: %s\n", text);

                break;

            }

            case '3': {

                int shift;

                printf("Enter the shift number:");
                scanf("%d", &shift);

                encryptText(text,shift);

                break;
            }

            case '4': {

                int shift;

                printf("Enter the shift number:");
                scanf("%d", &shift);

                decryptText(text,shift);

                break;

            }

            case '5': {

                double distributionText[ALPHABET_SIZE]={0};

                compute_histogram(text, distributionText);

                displayPercentageOcurrence(distributionText);

                break;
            }

            case '6': {

                double distributionText[ALPHABET_SIZE]={0};
                compute_histogram(text, distributionText);

                int top_shifts[TOP_N]={0};
                double top_distances[TOP_N]={INFINITY};
                char method[20];

                printf("Enter the distance metric you'd like to use(chi_squared/euclidian/cosine):\n");
                scanf("%s", method);

                double (*distance_function)(const double[], const double[]) = nullptr;

                if (strcmp(method,"chi_squared")==0) {
                    distance_function = chi_squared_distance;
                }
                else if (strcmp(method,"euclidian")==0) {
                    distance_function = euclidian_distance;
                }
                else if (strcmp(method,"cosine")==0) {
                    distance_function = cosine_distance;
                }
                else {
                    printf("Error: Invalid method\n");
                    break;
                }

                break_caesar_cipher(text,top_shifts,top_distances,distance_function);

                printf("Most likely shift:%d\n with the distance:%lf\n",top_shifts[0],top_distances[0]);

                break;

            }

        }
    }while (option != '7');

    return 0;

}

/* problem/ identifying the problem/ headers(fiecare functie si folosinta) / rezultate/ cazuri functionare/ identificare probleme */

