#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define min 2
#define max 16


//Struct for translation to dec system,
//integer and fractional parts are separated, fractional is multiplied by Scope
struct Dec {
    long long Integer;
    long long Fractional;
    long long int Scope;
};

//Gets char letter or digit, returns int equivalent
int
GetInt(char Symbol) {
    int Digit = 0;
    if (Symbol - '0' <= 9 && Symbol - '0' >= 0) {
        Digit = (int) (Symbol - '0');
    }
    if (Symbol >= 'A' && Symbol <= 'F') {
        Digit = (int) (Symbol - 'A' + 10);
    }
    return Digit;
}

//Gets int from 0 to 16, returns digit or letter for that value
char
GetChar(int Digit) {
    char x = 0;
    if (Digit <= 9 && Digit >= 0) {
        x = (char) (Digit + (int) '0');
    }
    if (Digit <= 15 && Digit >= 10) {
        x = (char) (Digit + (int) 'A' - 10);
    }
    return x;
}

//Checks that both systems are in [2,16]
int
SystemsCheck(int FirstBase, int SecondBase) {
    if (FirstBase > max || FirstBase < min || SecondBase > max ||
        SecondBase < min) {
        return 1;
    } else {
        return 0;
    }
}

//This function checks if the input string is correct and belongs to first system
int
InputStringCheck(int FirstBase, char *String) {
    int DotIndex = -1;
    int dots = 0;
    char *Alphabet = "1234567890ABCDEF.\0";
    for (int i = 0; i < (int) strlen(String); i++) {
        if (String[i] == '.') {
            dots++;
            DotIndex = i;
        } else if (GetInt(String[i]) >= 0 && GetInt(String[i]) < 16) {
            if (GetInt(String[i]) >= FirstBase) {
                return 1;
            }
        }
        if (DotIndex == 0 || DotIndex == (int) (strlen(String) - 1) || dots > 1) {
            return 1;
        }
        int flag = 1;
        int k = 0;
        while (k < (int) strlen(Alphabet)) //Check for incompatible symbols
        {
            if (String[i] == Alphabet[k]) {
                flag = 0;
                break;
            }
            k++;
        }
        if (flag == 1) {
            return 1;
        }
    }
    return 0;
}

//Gets number in first system written in str, returns its value in dec system
//Int and fractional parts are separated, second is multiplied to integer value
struct Dec
XtoDec(char *string, int FirstBase) {
    long long Integer = 0;
    long double Fractional = 0;
    int IntegerAmount;
    int FractionalAmount;
    char * Dot = strchr(string, '.');
    int Length = (int)strlen(string);
    if (Dot == NULL) {
        IntegerAmount = Length;
        FractionalAmount = 0;
    } else {
        IntegerAmount = (int) (Dot - string);
        FractionalAmount = (int) (Length - (Dot - string));
    }
    if (strchr(string, '.') - string == 1 && string[0] == '0') {
        FractionalAmount = Length;
    }

    if (IntegerAmount != 0) {
        for (int i = 0; i < IntegerAmount; i++) {
            Integer = Integer * FirstBase + GetInt(string[i]);
        }
    }
    if (FractionalAmount != 0) {
        for (int i = (int) (strlen(string) - 1); i > Dot - string;
             i--) {
            Fractional = (Fractional + GetInt(string[i]));
            Fractional /= FirstBase;
        }
    }

    long long int FractionalScoped = 0;
    long long int scope = 100000000000;
    if (Fractional != 0) {
        FractionalScoped = (long long int) (Fractional * scope);
    }
    struct Dec answer = {Integer, FractionalScoped, scope};
    return answer;
}

//Translation of integer part from dec to second system
char *
DecToSecondInt(long long int Integer, int SecondBase) {
    char BufferInt[100];
    int Index = 0;
    long long Number = Integer;
    while (Number / SecondBase) //Calculating digits in new system
    {
        int x = (int) (Number % SecondBase);
        Number /= SecondBase;
        BufferInt[Index++] = GetChar(x);
    }
    BufferInt[Index++] =
            (char) (GetChar((int) Number)); //Transform char * to str
    BufferInt[Index++] = '\0';
    char *OutputInt; //Copying into output array with malloc memory, then I check if malloc is correct
    if ((OutputInt = malloc(sizeof(char) * Index)) == NULL) {
        perror(
                "Its awful that I live in the same country with empty strings  ©\n  Leo Di Caprio\n");
    }
    strcpy(OutputInt, BufferInt);
    int i = 0; //Reversing order of digits in array
    int j = (int) (strlen(OutputInt) - 1);
    while (i < j) {
        char temp = OutputInt[i];
        OutputInt[i++] = OutputInt[j];
        OutputInt[j--] = temp;
    }
    return OutputInt;
}

//Gets fractional part in dec view, returns all the digits after dot in second system
char *
DecToSecondFractional(long long Fractional, long long int scope, int SecondBase) {
    //If fractional part is empty, here we return "0" and exit the function
    if (Fractional == 0) {
        char *nullSrt = (char *) malloc(2 * sizeof(char));
        strcpy(nullSrt, "0");
        return nullSrt;
    }
    char BufferFrac[14];
    long long NumberFrac = Fractional;
    int i = 0;
    while (i < 13) //Calculating new digits in while-cycle
    {
        long long int a = NumberFrac * SecondBase;
        int x = (int) (a / scope);
        BufferFrac[i++] = GetChar(x);
        NumberFrac = NumberFrac * SecondBase - (a / scope) * scope;
        if (NumberFrac == 0) {
            break;
        }
    }
    BufferFrac[i++] = '\0';
    char *OutputFrac; //Malloc check
    if ((OutputFrac = (char *) malloc(sizeof(char) * i)) == NULL) {
        perror(
                "Its awful that I live in the same country with empty strings  ©\n  Leo Di Caprio\n");
    }
    strcpy(OutputFrac, BufferFrac);
    return OutputFrac;
}

int
main() {
    char InputString[14];
    int FirstSystem;
    int SecondSystem;
    if ((scanf("%d%d%13s", &FirstSystem, &SecondSystem, InputString)) !=
        3) //Getting input values, checking for empty variables
    {
        return 1;
    }
    if (SystemsCheck(FirstSystem, SecondSystem)) {
        printf("bad input");
        return 0;
    }
    int length = (int) strlen(InputString);
    for (int i = 0; i < length; ++i) //Making all letters uppercase
    {
        InputString[i] = toupper((unsigned) InputString[i]);
    }
    if (InputStringCheck(FirstSystem, InputString)) {
        printf("bad input");
        return 0;
    }
    struct Dec DecView =
            XtoDec(InputString, FirstSystem); //Translation of the number
    char *OutputInteger = DecToSecondInt(DecView.Integer, SecondSystem);
    char *OutputFractional =
            DecToSecondFractional(DecView.Fractional, DecView.Scope, SecondSystem);
    printf("%s.%s", OutputInteger, OutputFractional); //Output of the answer
    free(OutputInteger); //Cleaning memory after malloc
    free(OutputFractional);
    return 0;
}
