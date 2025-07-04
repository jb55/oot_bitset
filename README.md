# OoT Bitset  

*A no-frills flag system inspired by **The Legend of Zelda: Ocarina of Time***

Need to pack **hundreds (or thousands) of one-bit flags**—“talked to an NPC”,
“opened a chest”, etc.—into a save file without wasting bytes?  OoT solved this
by storing flags in an array of `uint16_t` words. This header-only helper gives
you the same trick in C/C++:

* **Space-efficient** – 1 word = 16 flags. Pick any word count you like.  
* **Single header** – drop `oot_bitset.h` in and go.  
* **Branch-free** – simple bit-twiddling; no loops or malloc.  
* **Infinitely scalable** – need 10 flags or 10 000? Just resize the array.

## Quick start

```c
#include "oot_bitset.h"

enum GameEvents {
    FLAG_MET_RUTO_FIRST_TIME        = 0x00, // word 0, bit 0
    FLAG_TALKED_TO_MALON_FIRST_TIME = 0x02, // word 0, bit 2
    /* … */
    FLAG_SAW_BOB   = 0x10, // word 1, bit 0
    FLAG_SAW_ALICE = 0x1A  // word 1, bit 10
};

int main(void) {
    /*  Choose the size that fits YOUR game:                *
     *  words × 16 = max flags                              */
    uint16_t flags[30] = {0};   // 30 words ⇒ 480 flags (OoT’s choice)

    bitset_set(flags, FLAG_SAW_BOB);
    printf("Saw Bob? %s\n", bitset_get(flags, FLAG_SAW_BOB) ? "yes" : "no");
}
````

Compile with any C99 compiler:

```bash
cc -std=c99 demo.c -o demo
```

## API reference

```c
#define bitset_word(set, flag)  ((set)[bitset_index(flag)])

static inline uint16_t bitset_index(uint16_t flag) {  // word (0-4095)
    return flag >> 4;
}
static inline uint16_t bitset_mask(uint16_t flag) {   // bit  (0-15)
    return 1u << (flag & 0xF);
}

static inline bool bitset_get(uint16_t *set, uint16_t flag) {
    return bitset_word(set, flag) & bitset_mask(flag);
}
static inline void bitset_set(uint16_t *set, uint16_t flag) {
    bitset_word(set, flag) |= bitset_mask(flag);
}
static inline void bitset_clear(uint16_t *set, uint16_t flag) {
    bitset_word(set, flag) &= ~bitset_mask(flag);
}
```

### Sizing the array

```
max_flags = words × 16
words     = ceil(max_flags / 16)
```

Use as few or as many words as your project needs. *OoT* used 30 words (480 flags), but nothing stops you from using 1 word (16 flags) or 4 096 words (65 536 flags).

## Flag encoding

| Bits | 15 … 4 *(12 bits)* | 3 … 0 *(4 bits)* |
| ---- | ------------------ | ---------------- |
| Use  | **word index**     | **bit index**    |
| Max  | 0–4095 words       | 0–15 bits        |

Because each hex digit is 4 bits, you can read a flag as “word\:bit”.
Example: `0x1AC` → word 26, bit 12.

## Example output

```
Words[0] = 0x0004   // FLAG_TALKED_TO_MALON_FIRST_TIME
Words[1] = 0x0401   // FLAG_SAW_BOB | FLAG_SAW_ALICE
```
