#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define No_of_chars 256                 /* Number of character symbols */
#define EOF_symbol (No_of_chars + 1)    /* Index of EOF symbol */
#define No_of_symbols (No_of_chars + 1) /* Total number of symbols */


/* Translation tables between characters and symbol indexes */
int char_to_index[No_of_chars];            /* To index from character */
unsigned char index_to_char[No_of_symbols + 1]; /* To character from index */

/* Adaptive source model */
int freq[No_of_symbols + 1];              /* Symbol frequencies */
int cum_freq[No_of_symbols + 1];          /* Cumulative symbol frequencies */

/* Arithmetic encoding/decoding definitions */
#define Code_value_bits 16                 /* Number of bits in a code value */
typedef long code_value;                   /* Type of an arithmetic code value */
#define Top_value (((long)1 << Code_value_bits) - 1) /* Largest code value */
#define First_qtr (Top_value / 4 + 1)      /* Point after first quarter */
#define Half (2 * First_qtr)               /* Point after first half */
#define Third_qtr (3 * First_qtr)          /* Point after third quarter */
#define Max_frequency 16383                /* Maximum allowed frequency count */

/* Bit output routines */
int buffer;                     /* Bits buffered for output */
int bits_to_go;                 /* Number of bits free in buffer */

/* Current state of the encoding */
code_value low, high;           /* Ends of the current code region */
long bits_to_follow;            /* Number of opposite bits to output */

/* Function declarations */
void start_model(void);
void start_encoding(void);
void encode_symbol(int symbol, int cum_freq[]);
void update_model(int symbol);
void done_encoding(void);
void start_outputing_bits(void);
void output_bit(int bit);
void done_outputing_bits(void);
void bit_plus_follow(int bit);

void start_model(void) {
    int i;
    for (i = 0; i < No_of_chars; i++) {
        char_to_index[i] = i + 1;
        index_to_char[i + 1] = (unsigned char)i;
    }
    for (i = 0; i <= No_of_symbols; i++) {
        freq[i] = 1;
        cum_freq[i] = No_of_symbols - i;
    }
    freq[0] = 0; /* Freq[0] must not be the same as freq[1] */
}

void start_outputing_bits(void) {
    buffer = 0;
    bits_to_go = 8;
}

void output_bit(int bit) {
    buffer >>= 1;
    if (bit) buffer |= 0x80;
    bits_to_go -= 1;
    if (bits_to_go == 0) {
        putc((char)buffer, stdout);
        bits_to_go = 8;
    }
}

void done_outputing_bits(void) {
    putc((char)(buffer >> bits_to_go), stdout);
}

void bit_plus_follow(int bit) {
    output_bit(bit);
    while (bits_to_follow > 0) {
        output_bit(!bit);
        bits_to_follow -= 1;
    }
}

void start_encoding(void) {
    low = 0;
    high = Top_value;
    bits_to_follow = 0;
}

void encode_symbol(int symbol, int cum_freq[]) {
    long range;
    range = (long)(high - low) + 1;
    high = low + (range * cum_freq[symbol - 1]) / cum_freq[0] - 1;
    low = low + (range * cum_freq[symbol]) / cum_freq[0];
    for (;;) {
        if (high < Half) {
            bit_plus_follow(0);
        } else if (low >= Half) {
            bit_plus_follow(1);
            low -= Half;
            high -= Half;
        } else if (low >= First_qtr && high < Third_qtr) {
            bits_to_follow += 1;
            low -= First_qtr;
            high -= First_qtr;
        } else {
            break;
        }
        low = 2 * low;
        high = 2 * high + 1;
    }
}

void done_encoding(void) {
    bits_to_follow += 1;
    if (low < First_qtr)
        bit_plus_follow(0);
    else
        bit_plus_follow(1);
}

void update_model(int symbol) {
    int i;
    if (cum_freq[0] == Max_frequency) {
        int cum = 0;
        for (i = No_of_symbols; i >= 0; i--) {
            freq[i] = (freq[i] + 1) / 2;
            cum_freq[i] = cum;
            cum += freq[i];
        }
    }
    for (i = symbol; freq[i] == freq[i - 1]; i--)
        ;
    if (i < symbol) {
        int ch_i = index_to_char[i];
        int ch_symbol = index_to_char[symbol];
        index_to_char[i] = (unsigned char)ch_symbol;
        index_to_char[symbol] = (unsigned char)ch_i;
        char_to_index[ch_i] = symbol;
        char_to_index[ch_symbol] = i;
    }
    freq[i] += 1;
    while (i > 0) {
        i -= 1;
        cum_freq[i] += 1;
    }
}
int main(int argc, char *argv[]) {
    int ticker = 0;
    fprintf(stderr, "Arithmetic coding on ");
    if (argc > 1) {
        freopen(argv[1], "rb", stdin);
        fprintf(stderr, "%s", argv[1]);
    } else {
        fprintf(stderr, "stdin");
    }
    fprintf(stderr, " to ");
    if (argc > 2) {
        freopen(argv[2], "wb", stdout);
        fprintf(stderr, "%s", argv[2]);
    } else {
        fprintf(stderr, "stdout");
    }
    fprintf(stderr, "\n");
    start_model();
    start_outputing_bits();
    start_encoding();
    for (;;) {
        int ch, symbol;
        if ((ticker++ % 1024) == 0)
            putc('.', stderr);
        ch = getc(stdin);
        if (ch == EOF) break;
        symbol = char_to_index[ch];
        encode_symbol(symbol, cum_freq);
        update_model(symbol);
    }
    encode_symbol(EOF_symbol, cum_freq);
    done_encoding();
    done_outputing_bits();
    putc('\n', stderr);
    return 0;
}
