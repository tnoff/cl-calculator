#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/*
    t-noff
    Date:april 17, 2013
    
    command line calculator
    solves:
        + - * / ^ ()
    extras:
        K --> *1024 
        M --> *1024*1024
        G --> *1024*1024*1024
        example: 1K+2 ==> 1*1024+2
*/
int string_length(char *string)
{
    //get string length
    //Taken from the interwebs:
    //http://fxr.watson.org/fxr/source/libkern/strlen.c?v=DFBSD
    char *s;
    for ( s = string; *s; ++s);
    return(s-string);
}
int compare_strings(char one[], char two[])
{
    //compare if two strings are equal
    int length_one = string_length(one);
    int count;
    if ( length_one != string_length(two) )
        return 0;
    for ( count = 0; count < length_one; count += 1)
    {
        if ( one[count] != two[count] )
            return 0;
    }
    return 1;
}
int is_digit(char ch)
{
    //check if digit or . character
    int x = ch - '0';
    int y = ch - '9';
    if ( ( x >= 0 ) && ( x <= 9) && ( y >= -9) && ( y <= 0) )
        return 1;
    if (ch == '.')
        return 1;
    return 0;
}
char *sub_string(char buffer[], int start, int end)
{
    //get substring from start to end
    int count, i;
    char *output = realloc(NULL,end - start + 1);
    for ( count = start, i = 0; count <= end; count += 1, i += 1)
        output[i] = buffer[count];
    return output;
}
char *replace_sub_string(char buffer[], int start, int end, char input[])
{
    //Delete substring from start-end, replace with input
    int length = string_length(buffer);
    int input_length = string_length(input);
    char *output = realloc(NULL, length - (end - start + 1) + input_length);
    int index = 0, count;
    for ( count = 0; count < start; count += 1,index += 1)
        output[index] = buffer[count];
    for ( count = 0; count < input_length; count += 1, index += 1)
        output[index] = input[count];
    for ( count = end + 1; count < length; count += 1,index += 1)
        output[index] = buffer[count];
    return output;
}
double string_to_digit(char buffer[])
{
    return atof(buffer);
}
char *digit_to_string(double number)
{
    /*
     * convert double to string, remove all trailing zeros
     */
    char *output = realloc(NULL, 8);
    int count,length;
    sprintf(output,"%f",number);
    //replace all needless 0's at end
    length = string_length(output);
    for ( count = length - 1; count >= 0; count -= 1)
    {
        if ( ( output[count] != '0' ) && ( output[count] != ' ') )
            break;
    }
    if ( output[count] == '.')
        count -= 1;
    return sub_string(output,0,count);
}
char *lexer(int argc, char* argv[])
{
    /*
     * Parse command line, remove all blank spaces
     */ 
    char *output;
    int data_size = 0, i , x, length, count = 0;
    for ( i = 1; i < argc; i += 1)
        data_size += string_length(argv[i]);
    output = realloc(NULL,data_size + 1);
    for ( i = 1; i < argc; i += 1)
    {
        length = string_length(argv[i]);
        for ( x = 0; x < length; x += 1)
        {
            if ( argv[i][x] != ' '){
                output[count] = argv[i][x];
                count += 1;
            }
        }
    } 
    return output;
}
int check_paren(char buffer[])
{
    /*
     * Check if valid parenthesis
     */ 
    int counter = 0;
    int length = string_length(buffer);
    int t;
    for ( t = 0; t < length; t += 1)
    {
        if ( buffer[t] == '(')
            counter += 1;
        if ( buffer[t] == ')')
            counter -= 1;
        if ( counter < 0)
            return 0;
    }
    if ( counter == 0)
        return 1;
    return 0;
}
int find_character(char buffer[], char ch)
{
    //Incrememnt through buffer and return index of first ch
    int length = string_length(buffer), count;
    for ( count = 0; count < length; count += 1)
    {
        if (buffer[count] == ch)
            return count;
    }
    return -1;
}
int *left_argument(char buffer[],int index)
{
    /*
     * Get left argument for operation
     * Start at index, increment back until non-digit found
     * Then check if '-' at start to indicate negative
     * return start(0) and end(1) values
     */ 
    int count;
    int *values;
    values = (int *)malloc(2 * sizeof(int));
    for ( count = index - 1; count >= 0; count -= 1)
    {
        if ( !is_digit(buffer[count]))
            break;
    }
    if ( count != 0)
    {
        if (!(( buffer[count] == '-') && ( !is_digit(buffer[count - 1]))))
            count += 1;
    }
    values[0] = count;
    values[1] = index - 1; 
    return values;
}
int *right_argument(char buffer[],int index)
{
    /*
     * Get right argument for operation
     * Start at index, incrememnt through buffer until a non-digit found
     * Return start (0) and end(1) values 
     */ 
    int count;
    int *values;
    values = (int *)malloc(2 * sizeof(int));
    int length = string_length(buffer);
    for ( count = index + 1; count < length; count += 1)
    {
        if ( ! is_digit(buffer[count]) )
            break;
    }
    if ( ( count == index + 1) && ( buffer[index + 1] == '-'))
    {
        for ( count = index + 2; count < length; count += 1)
        {
            if ( ! is_digit(buffer[count]))
                break;
        }
    }
    values[0] = index + 1;
    values[1] = count - 1;
    return values;
}
char *doubles(char buffer[])
{
    /*
     *For addition and subtraction, replace all double +-'s
     */
    //http://s294.beta.photobucket.com/user/XpichewX/media/doubles.jpg.html
    int length = string_length(buffer);
    int count;
    for ( count = 0; count < length - 1; count += 1)
    {
        if ( ( buffer[count] == '+') && ( buffer[count + 1] == '-') ){
            buffer = replace_sub_string(buffer,count,count+1,"-");
            break;
        }if ( ( buffer[count] == '+') && ( buffer[count + 1] == '+') ){
            buffer = replace_sub_string(buffer,count,count+1,"+");
            break;
        }if ( ( buffer[count] == '-') && ( buffer[count + 1] == '+') ){
            buffer = replace_sub_string(buffer,count,count+1,"-");
            break;
        }if ( ( buffer[count] == '-') && ( buffer[count + 1] == '-') ){
            buffer = replace_sub_string(buffer,count,count+1,"+");
            break;
        }
    }
    return buffer;
}
char *perform_operation(char buffer[],char operator)
{
    /*
     * Input a buffer and an operator to use
     * Increment through buffer, check for operator
     * If operator found, get left and right arg
     * Using left and right arg, solve for result
     * Delete sub_string from left to right arg
     * Replace with result
     */
    int index = find_character(buffer, operator);
    int *left, *right;
    double x, y;
    char *input;
    if ( index != - 1)
    {
        left = left_argument(buffer,index);
        right = right_argument(buffer,index);
        if ( right[0] > right[1])
        {
            printf("ERROR, no right argument:%s\n",buffer);
            printf("Index of operator:%d\n",index);
            printf("Left argument:%d->%d\n",left[0],left[1]);
            printf("Right argument:%d->%d\n",right[0],right[1]);
            exit(1);
        }
        if ( left[0] == -1)
            x = 0.0;
        else
            x = string_to_digit(sub_string(buffer,left[0],left[1]));
        y = string_to_digit(sub_string(buffer,right[0],right[1]));
        if ( operator == '+')
            input = digit_to_string(x + y);
        else if ( operator == '-')
            input = digit_to_string(x - y);        
        else if ( operator == '*')
            input = digit_to_string(x * y);
        else if ( operator == '/')
            input = digit_to_string(x / y);
        else if ( operator == '^')
            input = digit_to_string( pow(x,y));
        buffer = replace_sub_string(buffer,left[0],right[1],input);
    }
    return buffer;
}
char *replace_KMG(char buffer[])
{
    /*
     * Go through buffer, find K M or G
     * replace with equivalent value
     */
    int length = string_length(buffer);
    int count;
    for ( count = 0; count < length; count += 1)
    {
        if ( buffer[count] == 'K'){
            buffer = replace_sub_string(buffer,count,count,"*2^10");
            break;
        }
        if ( buffer[count] == 'M'){
            buffer = replace_sub_string(buffer,count,count,"*2^20");
            break;
        }if ( buffer[count] == 'G'){
            buffer = replace_sub_string(buffer,count,count,"*2^30");
            break;
        }
    }
    return buffer;
}
char *perform_KMG(char buffer[])
{
    //perform KMG until buffer not changed
    char *string = buffer;
    do
    {
        buffer = string;
        string = replace_KMG(buffer);
    }while ( ! compare_strings(buffer,string));
    return string;
}
char *perform_addsub(char buffer[])
{
    //check for doubles ( ++, --, +- , -+)
    //perform all addtions ( until buff doesnt change)
    //perform all subtractions ( until buff doesnt change)
    char *string = buffer;
    int first_add, first_sub;
    do
    {
        buffer = string;
        string = doubles(buffer);
    }while ( ! compare_strings(buffer,string));
    do{
        buffer = string;
        first_add = find_character(buffer,'+');
        first_sub = find_character(buffer,'-');
        if (first_sub == 0)
            first_sub = -1;
        if (( first_add == - 1) && (first_sub == -1))
            break;
        if ( first_sub == - 1)
            string = perform_operation(buffer,'+');
        else if ( first_add = -1)
            string = perform_operation(buffer,'-');
        else if ( first_add < first_sub)
            string = perform_operation(buffer,'+');
        else 
            string = perform_operation(buffer,'-');
    }while(!compare_strings(buffer,string)); 
    return string;
}
char *perform_muldiv(char buffer[])
{
    //Perform all multiplications ( until buffer doesnt change)
    //Perform all divisions ( until buffer doesnt change)
    char *string = buffer;
    do
    {
        buffer = string;
        string = perform_operation(buffer,'*');
    }while( ! compare_strings(buffer,string));
    do 
    {
        buffer = string;
        string = perform_operation(buffer,'/');
    }while( ! compare_strings(buffer,string));
    return string;
}
char *perform_exponent(char buffer[])
{
    //Perform all exponents ( unitl buffer doesnt change)
    char *string = buffer;
    do
    {
        buffer = string;
        string = perform_operation(buffer,'^');
    }while(!compare_strings(buffer,string));
    return string;
}
char *solve_sub_string(char buffer[])
{
    /* 
     * Solve basic expression without ( or )
     * First go through, replace all K-M-G's
     * Follow PEMDAS
     *      Solve All exponents
     *      Solve All multiplication/division
     *      Solve All add/subtract
     * Return this result
     */
    char *string = buffer;
    do
    {
        buffer = string;
        string = perform_KMG(buffer);
    }while ( ! compare_strings(buffer,string));
    do
    {
        buffer = string;
        string = perform_exponent(buffer);
    }while (!compare_strings(buffer,string));
    do
    {
        buffer = string;
        string = perform_muldiv(buffer);
    }while ( ! compare_strings(buffer,string));
    do
    {
        buffer = string;
        string = perform_addsub(buffer);
    }while ( ! compare_strings(buffer,string));
    return string;
}
char *solve_sub_paren(char buffer[])
{
    /*
     * Solve all parenthesis in buffer first
     * Find first ) in buffer, then trace back to its (
     * This way this sub-buffer will not contain other ()
     * Solve this sub-buffer, then replace with result
     */
    char *string;
    int length = string_length(buffer);
    int count, left = -1, right = -1;
    for ( count = 0; count < length; count += 1)
    {
        if ( buffer[count] == ')')
        {
            right = count;
            break;
        }
    }
    for ( count; count >= 0; count -= 1)
    {
        if ( buffer[count] == '(')
        {
            left = count;
            break;
        }
    }
    if (( left == - 1) || ( right == -1))
        return buffer;
    string = solve_sub_string(sub_string(buffer,left+1,right-1));
    buffer = replace_sub_string(buffer,left,right,string);
    return buffer;
}
char *solve(char buffer[])
{
    /*
     * Follow PEMDAS
     * Solve all parenthesis first
     * Then solve remaining buffer 
     */
    char *string = buffer;
    do
    {
        buffer = string;
        string = solve_sub_paren(buffer);
    }while ( ! compare_strings(string,buffer));
    do
    {
        buffer = string;
        string = solve_sub_string(buffer);
    }while ( ! compare_strings(string,buffer));
    return string;
}

int main(int argc, char* argv[])
{
    int check;
    char *answer;
    char *buffer = lexer(argc,argv);
    check = check_paren(buffer);
    if ( check == 0)
    {
        printf("Error In Paren:%s\n",buffer);
        exit(1);
    }
    answer = solve(buffer);
    printf("%s=",buffer);
    printf("%s\n",answer);
}
