# OoT Bitset  

*A compact flag system inspired by **The Legend of Zelda: Ocarina of Time***  

Ocarina of Time stores hundreds of one-bit “event” flags—*talked to an NPC*,
*played a song*, etc.—inside just 30 `uint16_t` words. This repo packages that
idea into a tiny, header-only C99 helper so you can pack **480 flags** (or up
to 65 536 if you need) without wasting space.

---

## Why use it?

* **Tiny footprint** – 30 × `uint16_t` = 60 bytes for 480 flags  
* **Header-only** – drop `oot_bitset.h` into any C/C++ project  
* **Branch-free** – bit-twiddling only; no loops or dynamic memory  
* **Flexible** – grow the array if you need more than 480 flags  

---

## Quick start

```c
#include "oot_bitset.h"

enum GameEvents {
    FLAG_MET_RUTO_FIRST_TIME            = 0x00,
    FLAG_PLAYED_EPONA_SONG_AS_ADULT     = 0x01,
    FLAG_TALKED_TO_MALON_FIRST_TIME     = 0x02,
    /* … */
    FLAG_SAW_BOB   = 0x10,  // 2nd word, bit 0
    FLAG_SAW_ALICE = 0x1A   // 2nd word, bit 10
};

int main(void) {
    uint16_t flags[30] = {0};           // enough for 480 flags

    bitset_set  (flags, FLAG_SAW_BOB);
    bitset_clear(flags, FLAG_TALKED_TO_MALON_FIRST_TIME);

    printf("Saw Bob?  %s\n", bitset_get(flags, FLAG_SAW_BOB) ? "yes" : "no");
}
````

Compile with any C99-compatible compiler:

```bash
cc -std=c99 demo.c -o demo
```

---

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

OoT uses 30 words ➜ 480 flags. Need more? Just enlarge the array.

---

## Flag encoding

Each flag is a 16-bit *coordinate*:

| Bits | 15 … 4 *(12 bits)* | 3 … 0 *(4 bits)* |
| ---- | ------------------ | ---------------- |
| Use  | **word index**     | **bit index**    |
| Max  | 0–4095 words       | 0–15 bits        |

> Because hexadecimal digits are 4 bits, you can read a flag as “word\:bit”.
> E.g. `0x1AC` → word 26, bit 12.

---

## Example output

```
Words[0] = 0x0004   // FLAG_PLAYED_EPONA_SONG_AS_ADULT
Words[1] = 0x0401   // FLAG_SAW_BOB | FLAG_SAW_ALICE
```
