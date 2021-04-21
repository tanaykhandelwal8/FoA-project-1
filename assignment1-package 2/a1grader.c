#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "a1grader.h"

#define STAGE_HEADER "Stage %d\n==========\n"	/* stage header format string */
#define WORDWRAP 80
#define C_RST "\x1B[0m"
#define C_RED "\x1B[0;31m"
#define CONTROL_CHAR "‧"

/* print stage header given stage number */
static void print_stage_header(int stage_num) 
{
	printf(STAGE_HEADER, stage_num);
}

// Prints the bytes of a 16-byte block in hexadecimal
// A space is put between each byte (2 hexadigits)
// Between each block of 8 bytes, a space is placed.
static void print_array_hex(byte_t a[], int len) {
    for (int i = 0; i < len; i++)
    {
        if (i == 8) putchar(' ');
        printf("%02x ", a[i]);
    }
}

static void safe_putchar(char c) {
    if (isprint(c)) {
        putchar(c);
    } else {
        #ifdef COLOUR_CTRL_CHARS
        printf(C_RED CONTROL_CHAR C_RST);
        #else
        printf(CONTROL_CHAR);
        #endif
    }
}

/* Prints out the contents of a char array */
// Non-printable (control) characters are printed as ‧ in red.
static void print_array_char(byte_t a[], int len)
{
    for (int i = 0; i < len; i++)
    {
        safe_putchar(a[i]);
    } 
}

// Prints the hexadigits, followed by the ASCII representation of the block
// (No address printed)
static void hexdump_block(block_t block) {
    print_array_hex(block, BLOCKSIZE);
    printf(" ");
    print_array_char(block, BLOCKSIZE);
}


// Prints a byte array of length len, in groups of 16 (BLOCKSIZE) bytes
// For each line: 
// 1. Print the bit number of the first byte of each block, in hexadecimal
// 2. Print the hexadecimal and ASCII representation of each block.
// Example output for one block:
//  0x0000: 58 2f 50 37 97 39 c0 82  1c cc d2 73 3f 96 d5 ef   X/P7‧9‧‧‧‧‧s?‧‧‧
//  -> the byte 58 (in hexadecimal) is byte 0x0000 in the array, and prints to
//     the ASCII value X.
void hexdump(byte_t a[], int len) {
    if (len == 0) {
        printf("(Empty byte array given...)\n");
        return;
    }

    int i;
    for (i = 0; i < len / BLOCKSIZE; i++) {
        int a_pos = i * BLOCKSIZE;
        printf("0x%04x: ", a_pos);
        hexdump_block(a + a_pos);
        putchar('\n');
    }
    int rem;
    if ((rem = len % BLOCKSIZE) != 0) {
        int a_pos = i * BLOCKSIZE;
        printf("0x%04x: ", a_pos);
        print_array_hex(a + a_pos, rem);
        printf("(<- extra bytes not fitting a block)");
        putchar('\n');
    }
}



static void print_wrapped_string(char *str, int len, int wrap_width) {
    if (len == 0) return;

    safe_putchar(str[0]);
    for (int i = 1; i < len; i++) {
        if (i % wrap_width == 0) putchar('\n');
        safe_putchar(str[i]);
    }
} 

static int string_is_printable(char *str, int len) {
    for (int i = 0; i < len; i++) {
        if (!isprint(str[i])) {
            return 0;
        }
    }
    return 1;
}

// To make submit_stage4 look a little nicer, we'll just store the length
// submitted in stage 0.
static int _ciphertext_length;

void submit_stage0(int cipher_length, msg_t ciphertext, 
                    block_t outputs[N_OUTPUT_BLOCKS], block_t timesteps[N_TIMESTEPS],
                    book_t cipherbook) 
{
    // The code for grading this stage is hidden to students.  
    // The code below is to help with your development.
    print_stage_header(0);

    _ciphertext_length = cipher_length;
    printf("Length of encrypted ciphertext (bytes): %d\n", cipher_length);

    printf("Encrypted ciphertext, as hexadecimal bytes and ASCII: (below)\n");
    hexdump(ciphertext, cipher_length);
    putchar('\n');

    printf("Outputs, as hexadecimal bytes and ASCII: (below)\n");
    for (int i = 0; i < N_OUTPUT_BLOCKS; i++) {
        printf("  O_%2d: ", i + 9);
        hexdump_block(outputs[i]);
        putchar('\n');
    }
    putchar('\n');

    printf("Timesteps, as hexadecimal bytes and ASCII: (below)\n");
    for (int i = 0; i < N_TIMESTEPS; i++) {
        printf("  T_%2d: ", i);
        hexdump_block(timesteps[i]);
        putchar('\n');
    }
    putchar('\n');

    printf("Cipherbook: (below, wordwrapped to %d chars)\n", WORDWRAP);
    if (!string_is_printable(cipherbook, BOOK_LENGTH)) {
        printf("(WARN! Cipherbook has unprintable / invalid characters, replaced with " CONTROL_CHAR ")\n");
    } 
    print_wrapped_string(cipherbook, BOOK_LENGTH, WORDWRAP);
    putchar('\n');
}

void submit_stage1(book_t cipherbook, int book_len) 
{
    // The code for grading this stage is hidden to students.  
    // The code below is to help with your development.
    putchar('\n');
    print_stage_header(1);
    printf("Punctuation stripped cipherbook length: %d\n", book_len);
    printf("Punctuation stripped cipherbook, as hexadecimal bytes and ASCII: (below)\n");
    hexdump((byte_t *) cipherbook, book_len);
}

void submit_stage2(block_t key2) 
{
    // The code for grading this stage is hidden to students.  
    // The code below is to help with your development.
    putchar('\n');
    print_stage_header(2);

    printf("Key k2: ");
    hexdump_block(key2);
    putchar('\n');
    
}

void submit_stage3(byte_t *key1) 
{
    // The code for grading this stage is hidden to students.  
    // The code below is to help with your development.
    putchar('\n');
    print_stage_header(3);
    printf("Key k1, as hexadecimal bytes and ASCII: (below)\n");
    hexdump(key1, _ciphertext_length);
}

void submit_stage4(byte_t *plaintext) 
{
    // The code for grading this stage is hidden to students.  
    // The code below is to help with your development.
    putchar('\n');
    print_stage_header(4);
    printf("Decrypted ciphertext, as hexadecimal bytes and ASCII: (below)\n");
    hexdump(plaintext, _ciphertext_length);
    putchar('\n');

    printf("Decrypted ciphertext, as plaintext: (below)\n");
    print_array_char(plaintext, _ciphertext_length);
    putchar('\n');

}
