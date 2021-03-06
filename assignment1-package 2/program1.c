/* DUKH Attack 
 * COMP10002 Foundations of Algorithms, Semester 1, 2021
 * Skeleton code written by Shaanan Cohney, April 2021
 * 
 * Full Name: Tanay Khandelwal      
 * Student Number: 1168569 
 * Date: 20th April, 2021            
 */

/****** Include libraries ******/

#include <stdio.h>
#include <stdlib.h>
/* Do NOT use the following two libraries in stage 1! */
#include <string.h>
#include <ctype.h>

/* Provides functions AES_encrypt and AES_decrypt (see the assignment spec) */
#include "aes.h"
/* Provides functions to submit your work for each stage.
 * See the definitions in a1grader.h, they are all available to use.
 * But don't submit your stages more than once... that's weird! */
#include "a1grader.h"

/****** Definitions of constants ******/

#define BOOK_LENGTH 1284         /* The maximum length of a cipher book */
#define MAX_MSG_LENGTH 1024      /* The maximum length of an encrypted message */
#define BLOCKSIZE 16             /* The length of a block (key, output) */
#define N_TIMESTEPS 20           /* number of timesteps */
#define N_OUTPUT_BLOCKS 2        /* number of output blocks */

// TODO Add your own #defines here, if needed



/****** Type definitions ******/
/* Recall that these are merely aliases, or shortcuts to their underlying types.
 * For example, block_t can be used in place of an array, length 16 (BLOCKSIZE)
 * of unsigned char, and vice versa. */

typedef char book_t[BOOK_LENGTH];     /* A cipherbook (1284 bytes) */
typedef unsigned char byte_t;         /* A byte (8 bits) */
typedef byte_t block_t[BLOCKSIZE];    /* A cipher bitset (block) (16 bytes) */
typedef byte_t msg_t[MAX_MSG_LENGTH]; /* An encrypted message (l bytes) */

// TODO Add your own type definitions here, if needed



/****** Function Prototypes ******
 * There are more functions defined in aes.h and grader.h */
// Scaffold

int read_hex_line(byte_t output[], int max_count, char *last_char);

// Hint: Variables passed by pointers should be modified in your stages' implementation!

void stage0(msg_t ciphertext, int *ciphertext_length, 
            block_t outputs[N_OUTPUT_BLOCKS], block_t timesteps[N_TIMESTEPS], 
            book_t cipherbook);
void stage1(book_t cipherbook, int *book_len);
void stage2(byte_t codebook[], int book_len, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], block_t key2);
void stage3(block_t key2, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], byte_t key1[], int cipher_length);
void stage4(byte_t key1[], byte_t ciphertext[], int cipher_length, 
            byte_t plaintext[]);

// stage 1
int isalphanumeric(char c); // function to check if a character is alphanumeric

// stage 2
void xor_blocks(block_t input1, block_t input2, block_t output);

/* The main function of the program */
// It is strongly suggested you do NOT modify this function.
int main(int argc, char *argv[])
{   
    //// Stage 0
    /* These will store our input from the input file */
    msg_t ciphertext;                  // encrypted message, to be decrypted in the attack
    int ciphertext_length = 0;         // length of the encrypted message
    book_t cipherbook;                 // book used to make key k2
    block_t timesteps[N_TIMESTEPS];    // timesteps used to generate outputs (hex)
    block_t outputs[N_OUTPUT_BLOCKS];  // outputs from the random number generator (hex)
    // Run your stage 0 code
    stage0(ciphertext, &ciphertext_length, outputs, timesteps, cipherbook);
    // And submit the results.  Don't delete this...
    submit_stage0(ciphertext_length, ciphertext, outputs, timesteps, cipherbook);
    
    //// Stage 1
    int book_len = 0;    // length of the cipher book after having removed punctuation
    stage1(cipherbook, &book_len);
    submit_stage1(cipherbook, book_len);

    //// Stage 2
    block_t key2;        // the key k2 (hexadecimal)
    stage2((byte_t *) cipherbook, book_len, outputs, timesteps, key2);
    submit_stage2(key2);

    //// Stage 3
    byte_t key1[MAX_MSG_LENGTH];       // the key k2 (hexadecimal)
    stage3(key2, outputs, timesteps, key1, ciphertext_length);    
    submit_stage3(key1);

    //// Stage 4
    byte_t plaintext[MAX_MSG_LENGTH];  // the plaintext output
    stage4(key1, ciphertext, ciphertext_length, plaintext);
    submit_stage4(plaintext);

    return 0;
}

/********* Scaffold Functions *********/

/* Reads a line in from stdin, converting pairs of hexadecimal (0-F) chars to
 * byte_t (0-255), storing the result into the output array, 
 * stopping after max_count values are read, or a newline is read.
 *
 * Returns the number of *bytes* read.
 * The last char read in from stdin is stored in the value pointed to by last_char.
 * If you don't need to know what last_char is, set that argument to NULL
 */
int read_hex_line(byte_t output[], int max_count, char *last_char)
{
    char hex[2];
    int count;
    for (count = 0; count < max_count; count++)
    {
        /* Consider the first character of the hex */
        hex[0] = getchar();
        if (hex[0] == '\n')
        {
            if (last_char)
            {
                *last_char = hex[0];
            }
            break;
        }
        /* Now the second */
        hex[1] = getchar();
        if (last_char)
        {
            *last_char = hex[0];
        }
        if (hex[1] == '\n')
        {
            break;
        }

        /* Convert this hex into an int and store it */
        output[count] = hex_to_int(hex); // (defined in aes.h)
    }
    return count - 1;
}

/*********Stage 0*********/

void stage0(msg_t ciphertext, int *ciphertext_length, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], book_t cipherbook) 
{
    /*
    stage0 reads in all the lines of input, using scanf(), getchar() and the 
    read_hex_line() function. Depending on the type of input, each type of 
    function is used. 
    */
    scanf("%d\n", ciphertext_length);  // reads line 1
    int i = 0; 
    for(i = 0; i < *ciphertext_length; i += BLOCKSIZE) {
         read_hex_line(&ciphertext[i], BLOCKSIZE, NULL); 
         scanf("\n");
    }// read line 2
        
    for(i = 0; i < N_OUTPUT_BLOCKS; i++) {
         read_hex_line(outputs[i], BLOCKSIZE, NULL); 
         scanf("\n");
    }// read line 3
    
    for(i = 0; i < N_TIMESTEPS; i++) {
         read_hex_line(timesteps[i], BLOCKSIZE, NULL); 
         scanf("\n");
    }// read line 4

    for(i = 0; i < BOOK_LENGTH; i++) {
        cipherbook[i] = getchar();
    }// read line 5
    
}     


void stage1(book_t cipherbook, int *book_len) 
{    
    /*
    Retains only the alphanumeric contents of cipherbook
    */
    //variable declarations
    book_t cipherbook_dummy;
    int book_length = 0; 
    int i;
    
    // alphanumeric characters stored in a dummy array
    for(i = 0; i < BOOK_LENGTH; i++) {
        if(isalphanumeric(cipherbook[i])){
            cipherbook_dummy[book_length] = cipherbook[i];
            book_length++;
        }
    }
    
    // original array is overwritten with the dummy array, with a new book_length
    for(i = 0; i < book_length;i++) {
        cipherbook[i] = cipherbook_dummy[i];
    }
    *book_len = book_length;
}

int isalphanumeric(char c) { 
    // function checks if a given character is an alphabet/number
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'))
        return 1; 
    return 0;
}

/********* Stage 2 Functions *********/

void stage2(byte_t codebook[], int book_len, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], block_t key2) 
{
    /* 
    stage2 calculates the various possible combinations of key2 and finds the 
    one using the provided equation. In order to calculate the left hand side 
    and right hand side, provided functions AES_encrypt and AES_decrypt along 
    with a new function xor_blocks is used
    */
    
    //variable declarations
    int i, j;
    int flag = 0;
    block_t key1; 
    block_t lhs_output;
    block_t rhs_output;
    block_t xor_output;
    block_t encrypt_output ;
    block_t decrypt_output;
    int counter = 0;
    
    for(i = 0; i < book_len; i++) {
        key1[counter++] = codebook[i]; // creates 16 byte blocks of the codebook
        if(counter == 16) {
            AES_encrypt(timesteps[10], key1, encrypt_output); 
            // encrypts T[i+1] with the possible key key1
            
            AES_decrypt(outputs[1], key1, decrypt_output); 
            // decrypts O[i+1] with the possible key key1
            
            xor_blocks(encrypt_output, decrypt_output, xor_output); 
            // XORs the output of the above statements
            
            AES_decrypt(xor_output, key1, lhs_output);
            //decrypts the xor_output with the possible key key1, giving the 
            // lhs output
            
            AES_encrypt(timesteps[9], key1, encrypt_output);
            // encrypts T[i] using the possible key key1
            
            xor_blocks(outputs[0], encrypt_output, rhs_output);
            // XORs the output of the encryption and O[i] = O[9]
            
            // loops returns flag's value as 1 if lhs = rhs, else 0
            for(j = 0; j < BLOCKSIZE; j++){
                if(lhs_output[j] == rhs_output[j])
                    flag = 1;
                else {
                    flag = 0;
                    break;
                }
            }
            //if flag = 1, key is found, which is stored in key2
            if(flag) {
                for(int k = 0; k < BLOCKSIZE; k++)
                    key2[k] = key1[k];
                break;
            }
            counter = 0;
        }
    }
}


void xor_blocks(block_t input1, block_t input2, block_t output) {
    /*
    function XORs two block_t variables and 
    stores the result in the output block 
    */ 
    
    for(int i = 0; i < BLOCKSIZE; i++) {
        output[i] = (input1[i] ^ input2[i]);
    }
}


/********* Stage 3 Functions *********/
void stage3(block_t key2, block_t outputs[N_OUTPUT_BLOCKS], 
            block_t timesteps[N_TIMESTEPS], byte_t key1[], int ciphertext_length) 
{
    /*
    function generates key1 using the X9.31 psuedorandom number generator
    */
    
    //variable declarations
    int i; 
    int j;
    block_t vi; 
    block_t encrypt_output;
    block_t xor_output; 
    block_t intermediate_value;
    block_t Oi;
    int counter_key1 = 0;
    
    // calculating the initial state of the generator V10
    AES_encrypt(timesteps[10], key2, encrypt_output);
    xor_blocks(encrypt_output, outputs[1], xor_output);
    AES_encrypt(xor_output, key2, vi);
    
    //X9.31 pseudorandom number generator
    for(i = 11; i <= (11 + (ciphertext_length/BLOCKSIZE)); i++){
        
        //stage 2
        AES_encrypt(timesteps[i], key2, intermediate_value);
        
        //stage 3
        xor_blocks(intermediate_value, vi, xor_output);
        AES_encrypt(xor_output, key2, Oi);
        
        //stage 4
        xor_blocks(Oi, intermediate_value, xor_output);
        AES_encrypt(xor_output, key2, vi);
        
        // storing the result of X9.31 in the key1
        for(j = 0; j < BLOCKSIZE; j++) {
        key1[counter_key1++] = Oi[j];
        }
    } 
}

/********* Stage 4 Functions *********/
void stage4(byte_t key1[], byte_t ciphertext[], int cipher_length, byte_t plaintext[])
{
    /*
    XORing key1 along with the ciphertext results in the original message!
    */
    //variable declarations
    block_t block_key1;
    block_t block_ciphertext;
    block_t block_plaintext; 
    int counter_plaintext = 0;
    int i, j;
    int counter_key1 = 0; 
    int counter_ciphertext = 0;
    
    /* 
    block_t's of key1 and ciphertext are made due to the xor_blocks function's 
    limitation of only being able to handle a maximum of byte_t[16] i.e. block_t
    , after which the blocks are XORed and stored in the plaintext array, which
    is then outputted. 
    */
    for(i = 0; i < cipher_length; i++) {
        block_key1[counter_key1++] = key1[i]; 
        block_ciphertext[counter_ciphertext++] = ciphertext[i];
        if(counter_key1 == 16) {
            xor_blocks(block_key1, block_ciphertext, block_plaintext);  
            
            for(j = 0; j < BLOCKSIZE; j++) {
                plaintext[counter_plaintext++] = block_plaintext[j];
            }
            counter_key1 = 0; 
            counter_ciphertext = 0;
        }
    }
}



/********* END OF ASSIGNMENT! *********/
/* If you would like to try the bonus stage, attempt it in a new file, bonus.c */
// Feel free to write a comment to the marker or the lecturer below...

//algorithms are fun!