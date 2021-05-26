/*
 * flagString = 0, The previous element is not a StringConst
 *            = 1, The previous element is a StringConst
 *            = 2, String concatenation with the operator + only works if two strings are present.
 *
 * flagNum    = 0, The previous element is not an IntConst
 *            = 1, The previous element is an IntConst
 *            = 2, Following operators (+, -, *, /) must follow after a number
 *            = 3, Following operators (++, --) must follow after a number
 *
 * flagIden   = 0, The previous element is not an Identifier
 *            = 1, The previous element is an Identifier
 *
 * flagKey    = 0, The previous element is not a keyword
 *            = 1, The previous element is one of the following keywords (while, for, if)
 *            = 2, The previous element is one of the following keywords (int, long, float, goto)
 *            = 3, The previous element is one of the following keywords (break, continue)
 *            = 4, For the flagKey 1, after the statement brackets, curly brackets must follow.
 *            = 5, The previous element is the following keyword (do)
 *
 * flagOp     = 0, The previous element is not an operator
 *            = 1, The previous element is one of the following operators (+, -, *, /)
 *            = 2, The previous element is one of the following operators (++, --)
 *            = 3, The previous element is the following operator (:=)
 *
 * Avoids do while interference
 * flagDo     = 0, curly bracket comes after the while statement ->  while(){
 *            = 1, EOL comes after the while statement -> do{ }while();

 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXCHAR 2048

int counter = 1;
int countOfBracket = 0;
int comCheckCnt = 0;
char input[MAXCHAR];
char output[MAXCHAR] ;
char bracket[300];


void printErrorMessage(char* str)
{
    char text[20];
    strcat(output, "Line ");
    sprintf(text, "%d", counter);
    strcat(output, text);
    strcat(output," Error Message:");
    strcat(output,str);
}
void printStatements(char* stateType, char* content)
{
    strcat(output, stateType);
    strcat(output,content);
    strcat(output,")\n");
}

char *remove_white_endLine(char *str)
{
    int i = 0, j = 0;
    while (str[i])
    {
        if (str[i] != '\n')
        str[j++] = str[i];
        i++;
    }
    str[j] = '\0';
    return str;
}

int  isValidDelimiter(char ch) {
    if (ch == ' ' ||ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ':' ||
        ch == ';'  || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' )
        return 1;
    return -1;
}

int isValidOperator(char ch){
    if (ch == '+' || ch == '-' || ch == '*' ||
        ch == '/' || ch == ':' )
        return 1;
    return -1;
}

/* Controlling the next/incoming closing bracket with the latest item in the bracket array.
 * If they match, return true and remove the controlled bracket in the array; if not, return false.
 * */
int controlBrackets(char ch)
{
    int length = strlen(bracket);
    int index;
    int flag = 0;
    for(int i = length - 1; i >= 0 ; i--)
    {
        index = i;
        if((bracket[i] == '(' && ch == ')') || (bracket[i] == '{' && ch == '}') || (bracket[i] == '[' && ch == ']'))
        {
            flag = 1;
            break;
        }
        else if(bracket[i] == '[' || bracket[i] == '{'|| bracket[i] == '(')
        {
            break;
        }
    }
    if(flag == 1)
    {
        bracket[index] = '0';
    }
    return flag;
}

/* Checking the incoming brackets from the input buffer.
 * If it's an opening bracket, it outputs the corresponding name,
 * and pushes is into the bracket array to be controlled later on in the controlBrackets() function.
 * If it's a closing bracket, it calls controlBrackets() to evaluate; if passes, it outputs the corresponding name as well.
 * */
char *isValidBrackets(char ch)
{
    switch(ch) {
        case '(':

            bracket[countOfBracket] = ch;
            countOfBracket++;
            return "LeftPar";
        case ')':

            if(controlBrackets(ch) == 0)
                printErrorMessage("Parentheses do not match.\n");
            return "RightPar";
        case '[':

            bracket[countOfBracket] = ch;
            countOfBracket++;
            return "LeftSquareBracket";
        case ']':

            if(controlBrackets(ch) == 0)
                printErrorMessage("Parentheses do not match.\n");
            return "RightSquareBracket";
        case '{':

            bracket[countOfBracket] = ch;
            countOfBracket++;
            return "LeftCurlyBracket";
        case '}':

            if(controlBrackets(ch) == 0)
                printErrorMessage("Parentheses do not match.\n");
            return "RightCurlyBracket";
        default:
            return NULL;
    }

}

// Checks if the string value is correctly typed. Controls the quotation marks.
int  isValidString(char* str) {


    int count = 0;
    for(int i = 0 ; i < strlen(str); i++){
        if(str[i] == '\"')
            count++;
    }
    if(count != 2)
        return -1;

    if(strlen(str) > 2)
    {
        if (str[0] == '\"' && str[strlen(str) - 1] =='\"')
            return 1;
        else if(str[0] == '\"' || str[strlen(str) - 1] =='\"')
            return  0;
    }

    return -1;
}

// The input is parsed using the delimiter.
char* subString(char* str, int left, int right) {
    int i;
    char* subStr = (char*)malloc( sizeof(char) * (right - left + 2));
    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return (subStr);
}

// Checks if the Integer value is correctly typed.
int isValidInteger(char* str) {
    int i;
    int len = strlen(str);
    if (len == 0)
        return (-1);
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2'&& str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9' || (str[i] == '-' && i > 0))
            return (-1);
    }
    return (1);
}

// Checks if the Float value is correctly typed.
int isRealNumber(char* str) {
    int i, len = strlen(str);
    int hasDecimal = -1;
    if (len == 0)
        return (-1);
    for (i = 0; i < len; i++) {
        if (str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i]       != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9' && str[i] != '.' || (str[i] == '-' && i > 0))
            return (-1);
        if (str[i] == '.')
            hasDecimal = 1;
    }
    return (hasDecimal);
}

// Checks if the keyword value is correctly typed.
int isValidKeyword(char* str) {
    if (!strcmp(str, "if") || !strcmp(str, "else") || !strcmp(str, "while") || !strcmp(str, "do") ||    !strcmp(str, "break") || !strcmp(str, "continue") || !strcmp(str, "int")
        || !strcmp(str, "float") || !strcmp(str, "return") || !strcmp(str,    "char") || !strcmp(str, "case")
        || !strcmp(str, "long")  || !strcmp(str, "static") || !strcmp(str, "goto") ||  !strcmp(str, "enum") ||  !strcmp(str, "for") ||  !strcmp(str, "const") ||  !strcmp(str, "record"))
        return (1);
    return (-1);
}

/* Checks if the keyword value is correctly typed.
 * Checks identifiers, which can only begin with a letter.
 * Can not start with numbers, underscore, dash etc.
 * */
int isvalidIdentifier(char* str){
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || str[0] == '_'|| isValidDelimiter(str[0]) == 1)
        return -1;
    int i;
    for(i = 0 ;i< strlen(str); i++){

        if(!((97<= (int)str[i] && (int)str[i] <=122) || (48<= (int)str[i] && (int)str[i] <=57)
        || str[i] == '_'))
            return -1;

    }
    return 1;
}

void lexAnalyzer(char *str)
{
    int left = 0, right = 0;
    int flagString = 0, flagNum = 0, flagIden = 0, flagKey = 0, flagOp = 0, flagDo = 0;
    int flagHeadLine = 0;
    int length = strlen(str);

    /* Calculates a valid substring by generating intervals using right and left properties. */
    while (right <= length && left <= right) {

        if (isValidDelimiter(str[right]) == -1) // Increase the right property until it hits a delimiter.
            right++;

        /* If an element is detected, left and right properties are matched with each other to check the delimiter coming afterwards.*/
        if (isValidDelimiter(str[right]) == 1 && left == right) {
            /* If the delimiter is an operator and cases are not 1 and 3. (check the glossary at the top of the page) */
            if (isValidOperator(str[right]) == 1 && flagKey == 0 ){

                 if(flagNum == 1)
                    flagNum = 2;
                /* Controlling the double operators (++, --, :=) */
                if((right + 1) < length && (str[right] == ':' || str[right] == '+' || str[right] == '-')){

                    if(str[right] == ':'){

                        if(flagOp == 1 || flagOp == 3){
                            printErrorMessage( " Can not use another operator with an assignment operator.");
                            return;
                        }

                        if(str[right + 1] == '='){
                            flagOp = 3;
                            if(flagIden != 1){
                                printErrorMessage( " The assignment cannot be done this way.");
                                return;
                            }
                            flagIden = 0; // An identifier can be used after an operator.
                            printStatements("Operator(", ":=");
                        }
                        else{
                            printErrorMessage( " After : is not \"=\"\n");
                            return;
                        }
                        right++;
                        if((right + 1) < length){
                            char ch = str[right + 1];
                            if(ch == '-'){
                                printErrorMessage( " Negative number cannot be used.\n");
                                return;
                            }
                            else if(ch == '+' || ch == '*' || ch == '/' || ch == ':' || ch == ';' || ch == '='|| ch == '?' || ch == '_'){
                                printErrorMessage( " The assignment cannot be done this way.\n");
                                return;
                            }
                        }
                    }
                    else if(str[right] == '+'  && flagOp != 1){

                        if(str[right + 1] == '+' && (flagOp == 3 || flagOp == 0)){
                            if(flagString == 1){
                                printErrorMessage( " ++ cannot be used after string\n");
                                return;
                            }
                            if(flagNum == 2)
                                flagNum = 3;
                            printStatements("Operator(", "++");
                            flagOp = 2;
                            right++;

                        }
                        /* If the next operator is not +, only the + operator gets through. */
                        else if(flagOp == 0 && flagHeadLine != 0){
                            printStatements("Operator(", "+");
                            flagOp = 1;
                            flagIden = 0;
                            if(flagString == 1)
                                flagString = 2;
                        }
                        else if(flagHeadLine == 0){
                            printErrorMessage( " Cannot start a new line with the operator.\n");
                            return;
                        }
                        else{
                            printErrorMessage( " Wrong operator usage\n");
                            return;
                        }

                    }
                    else if(str[right] == '-'  && flagOp != 1){
                        if(flagString == 1){
                            printErrorMessage( " --  or - cannot be used after string\n");
                            return;
                        }
                        if(str[right + 1] == '-' && (flagOp == 3 || flagOp == 0)) {
                            if(flagNum == 2)
                                flagNum = 3;
                            printStatements("Operator(", "--");
                            flagOp = 2;

                            right++;
                        }
                        /* If the next operator is not -, only the - operator gets through. */
                        else if (flagOp == 0 && flagHeadLine != 0){
                            flagOp = 1;
                            flagIden = 2;
                            printStatements("Operator(", "-");
                        }
                        else if(flagHeadLine == 0){
                            printErrorMessage( " Cannot start a new line with the operator.\n");
                            return;
                        }
                        else{
                            printErrorMessage( " Wrong operator usage\n");
                            return;
                        }

                    }
                    else{
                        printErrorMessage( " Wrong operator usage\n");
                        return;
                    }

                }
                else if (flagOp != 0){
                    printErrorMessage( " Wrong operator usage\n");
                    return;
                }
                else{
                   if(flagHeadLine == 0){
                        printErrorMessage( " Cannot start a new line with the operator.\n");
                        return;
                    }
                    flagOp = 1;
                    if(flagString == 1){
                        printErrorMessage( " * and / Operators cannot be used after string\n");
                        return;
                    }
                    if(str[right] == '*')
                        printStatements("Operator(", "*");
                    else
                        printStatements("Operator(", "/");
                    flagIden = 2;
                }

            }
            else if((flagKey != 0) && isValidOperator(str[right]) == 1){
                printErrorMessage( " Wrong keyword usage.\n");
                return;
            }
            else if(str[right] == ';'){
                /* EOL cannot be following some keywords. (case 1, 2, 4, 5) */
                if((flagKey != 3 && flagKey != 0 ) && !(flagDo == 1 && flagKey == 4)){
                    printErrorMessage( " Wrong keyword usage.\n");
                    return;
                }
                /* EOL can only follow the ++ and -- operators */
                else if(flagOp == 1 || flagOp == 3){
                    printErrorMessage( " Wrong operator usage\n");
                    return;
                }
                /* Termination of do while loop. */
                if(flagDo == 1 && flagKey == 4)
                    flagDo =0;

                //After a line successfully processed, all properties must be reset
                flagString = 0;
                flagNum = 0;
                flagIden = 0;
                flagKey = 0;
                flagOp = 0;
                flagHeadLine = 0;
                counter++;
                strcat(output,"EndOfLine\n");
            }
            /* Comment brackets have unique error handlings.
             * If an asteriks(*) follows a bracket, the body is neglected until it hits a closing comment bracket. */
            else if(str[right] == '(' && (right + 1) < length )
            {
                if(str[right + 1] == '*')
                {
                    comCheckCnt++;
                    right++;
                    while(right + 1< length ){
                        right++;
                        if(str[right] == '*' && str[right + 1] == ')'){
                            comCheckCnt--;
                            right++;
                        }
                        else if(str[right] == '(' && str[right + 1] == '*')
                            comCheckCnt++;

                        if(comCheckCnt == 0)
                            break;
                    }
                }
                /*Controlling the left bracket control and its cases.*/
                else if(strcat(output, isValidBrackets(str[right])) != NULL && flagOp != 2)
                {
                    strcat(output,"\n");
                    /*Check glossary for flagKey's*/
                    if(flagKey == 4 || flagKey == 5){
                        printErrorMessage( " Keyword usage is incorrect. \"{\" parentheses must be followed .\n");
                        return;
                    }
                    if(flagKey == 1){
                        int pCount = 1;
                        int index = right + 1;
                        while(right + 1< length ){
                            right++;
                            if(str[right] == '(')
                                pCount++;
                            if(str[right] == ')'){
                                pCount--;
                                if(pCount == 0){
                                    flagKey = 4; // while(){ check //while((i+1)<3)
                                    break;
                                }

                            }
                        }
                        if(pCount != 0){
                            printErrorMessage( " Keyword usage is incorrect. \"(\" parentheses must be closed .\n");
                            return;
                        }
                        char* subStr = subString(str, index, right - 1);
                        lexAnalyzer(subStr);// Check the statement of the keyword.
                        strcat(output, isValidBrackets(str[right]));// Last bracket of the statement.
                        strcat(output,"\n");
                    }
                    if(str[right] == '('  && flagIden == 1){
                        printErrorMessage( " Identifier usage is incorrect. \"(\" parentheses can not  be used .\n");
                        return;
                    }
                }
                //++ or --
                else if(flagOp == 2){
                    printErrorMessage( " Parentheses usage is incorrect.\n");
                    return;
                }
            }
            else if(str[right] != ' '){
                if(strcat(output, isValidBrackets(str[right])) != NULL){

                    strcat(output,"\n");
                    if(flagKey == 1 && str[right] != '('){
                        printErrorMessage( "Keyword usage is incorrect. \"(\" parentheses must be followed .\n");
                        return;
                    }
                    if((flagKey == 4 && str[right] == '[') || (flagKey == 5 && str[right] != '{')){
                        printErrorMessage( "Keyword usage is incorrect. \"{\" parentheses must be followed .\n");
                        return;
                    }
                    if(str[right] == '{')
                        flagKey = 0;
                    if((str[right] == '{' || str[right] == '[') && (flagIden == 1 || flagOp == 2)){
                        printErrorMessage( " In some cases brackets cannot be closed if there's a wrong operator. Only operators that supports this feature are ++ and --.\n");
                        return;
                    }
                    if((str[right] == '}' || str[right] == ')' || str[right] == ']') &&  (flagOp == 1 || flagOp == 3 )){
                        printErrorMessage( " Before closing the bracket, there cannot be an assignment or a single operator.\n");
                        return;
                    }
                }
            }

            if (strstr(output, "Error Message") != NULL)
                return;

            right++;
            left = right;
        }
        /*Get substring in between delimiters until it ends*/
        else if (isValidDelimiter(str[right]) == 1 && left != right || (right == length && left != right)) {

            if(str[left] == '\"'){
            int flag = 0;
                if(flagString == 1){
                    printErrorMessage( " Incorrect use of double quotes on string.\n");
                    return;
                }
                while (right < length){
                    if(str[right - 1] == '\"'){// Check end of the string.
                        flag =1;
                        break;
                    }
                    right++;
                }
                if(flag == 0){
                    printErrorMessage( " Incorrect use of double quotes on string.\n");
                    return;
                }

            }
            char* subStr = subString(str, left, right - 1);

            if (isValidKeyword(subStr) == 1)
            {
                if(flagString != 0 || flagNum  != 0  ||  flagIden != 0  || flagKey != 0 || flagOp !=0){
                    printErrorMessage( " Wrong keyword usage.\n");
                    return;
                }
                if (!strcmp(subStr, "if")  || !strcmp(subStr, "for") || !strcmp(subStr, "while"))
                    flagKey = 1;
                else  if(!strcmp(subStr, "int") || !strcmp(subStr, "long") || !strcmp(subStr, "float")  || !strcmp(subStr, "char") || !strcmp(subStr, "goto"))
                    flagKey = 2;
                else if(!strcmp(subStr, "break") || !strcmp(subStr, "continue"))
                    flagKey = 3;
                else if(!strcmp(subStr, "do")){
                    flagKey = 5;
                    flagDo = 1;
                }
                printStatements("Keyword(", subStr);
            }
            else if (isValidInteger(subStr) == 1){

                if(strlen(subStr) > 10){
                    printErrorMessage( " Integers  can not be greater than 10 digits.\n");
                    return;
                }
                else if(flagIden == 1){
                    printErrorMessage( " Integers  can not be used after an identifier.\n");
                    return;
                }
                else if(flagString == 1){
                    printErrorMessage( " Integers  can not be used after a string.\n");
                    return;
                }
                else if(flagString == 2){
                    printErrorMessage(" String can not be summed with Integer string.\n");
                    return;
                }
                else if(flagNum == 1){
                    printErrorMessage( " There cannot be a space in between numbers.\n");
                    return;
                }
                else if(flagNum == 3){
                    printErrorMessage( " The usage of the operator is incorrect.\n");
                    return;
                }
                else if(flagKey != 0){
                    printErrorMessage( " Only the keyword return can be present before a number.\n");
                    return;
                }
                else if(flagOp == 2){
                    printErrorMessage( " Wrong operator usage\n");
                    return;
                }
                flagNum = 1;
                printStatements("IntConst(", subStr);
            }
            else if (isRealNumber(subStr) == 1 ){
                if(strlen(subStr) > 10){
                    printErrorMessage( " Float  can not be greater than 10 digits.\n");
                    return;
                }
                else if(flagIden == 1){
                    printErrorMessage( " Float  can not be used after an identifier.\n");
                    return;
                }
                else if(flagString == 1){
                    printErrorMessage( " Float  can not be used after a string.\n");
                    return;
                }
                else if(flagString == 2){
                    printErrorMessage( " String can not be summed with Float string.\n");
                    return;
                }
                else if(flagNum == 1){
                    printErrorMessage( " There cannot be a space in between float.\n");
                    return;
                }
                else if(flagNum == 3){
                    printErrorMessage( " The usage of the operator is incorrect.\n");
                    return;
                }
                else if(flagKey != 0){
                    printErrorMessage( " Wrong keyword usage.\n");
                    return;
                }
                else if(flagOp == 2){
                    printErrorMessage( " Wrong operator usage\n");
                    return;
                }
                flagNum = 1;
                printStatements("Float(", subStr);
            }
            else if(isValidString(subStr) == 1)
            {
                if(flagString == 1) {
                    printErrorMessage(" String can not be used after a string.\n");
                    return;
                }
                if(flagNum == 1){
                    printErrorMessage(" String can not be used after a number.\n");
                    return;
                }
                if(flagNum == 2){
                    printErrorMessage(" String can not be summed with Float .\n");
                    return;
                }
                if(flagIden == 1){
                    printErrorMessage(" String can not be used after an Identifier.\n");
                    return;
                }
                if(flagIden == 2){
                    printErrorMessage(" String can not be used after an Identifier.\n");
                    return;
                }
                else if(flagKey != 0){
                    printErrorMessage( " Wrong keyword usage.\n");
                    return;
                }
                else if(flagOp == 2){
                    printErrorMessage( " Wrong operator usage\n");
                    return;
                }

                flagString = 1;
                printStatements("StringConst(", subStr);
            }
            else if (isvalidIdentifier(subStr) == 1 && isValidDelimiter(str[right - 1]) == -1 && isValidString(subStr) == -1 && (strlen(subStr) != 0))
            {
                if(strlen(subStr) > 20){
                    printErrorMessage(" Variable length can not be greater than 20 character.\n");
                    return;
                }
                else if(flagString == 1){

                    printErrorMessage(" Identifier can not be used after a string.\n");
                    return;
                }
                else if(flagIden == 1){
                    printErrorMessage(" The use of identifier is incorrect.\n");
                    return;
                }
                else if(flagNum == 1){
                    printErrorMessage(" Identifier can not be used after a number.\n");
                    return;
                }
                else if(flagKey == 1  || flagKey == 3  || flagKey == 5){
                    printErrorMessage("After this keyword an identifier can not be used.\n");
                    return;
                }
                flagKey = 0;
                flagIden = 1;
                flagHeadLine = 1;
                printStatements("Identifier(", subStr);
            }
            else if (isvalidIdentifier(subStr) == -1 && (isValidDelimiter(str[right - 1]) == -1 )) {
                if(flagString == 1) {
                    printErrorMessage(" Incorrect use of double quotes on string.\n");
                }
                else{
                    printErrorMessage(" Invalid variable. Variable must start with a letter and must continue with a letter, number or \"_\" \n");
                }
                return;
            }
            flagOp = 0;
            left = right;
        }
    }

}
void readFile()
{
    FILE* file = fopen("code.Ceng", "r");
    if(!file){
        printf("\n Unable to open : %s ", "test.Ceng");
        exit(-1);
    }
    char line[MAXCHAR];
    // Reads file and tosses it into a one-line string.
    while (fgets(line, sizeof(line), file)) {

        remove_white_endLine(line); // delete '\n'
        int length = strlen(line);
        for(int i = 0; i < length; i++){
            line[i] = tolower(line[i]); // case insensitivity
        }
        strcat(input, line);
    }
    fclose(file);
}
void writeFile()
{
    FILE *lex = fopen("code.lex","w"); // create a .lex file
    fprintf (lex,output);
    fclose(lex);
}
int main()
{

    readFile();
    lexAnalyzer(input);
    for(int i = 0; i < strlen(bracket) && strstr(output, "Error Message") == NULL; i++)
    {
        if(bracket[i] != '0')
            printErrorMessage("Brackets numbers do not match.");
    }
    if(comCheckCnt > 0)
        printErrorMessage(" The comment is not closed. *) must be added.");
    else if(comCheckCnt < 0)
        printErrorMessage(" The comment is not opened. (* must be added.");

    printf("Result \n----------\n");
    printf("%s", output);

    writeFile();

    return 0;
}