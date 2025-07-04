
# OoT bitset

Ocarina of Time has a succinct data structure for storing many flags in a
compact way.

oot_bitsets ie. (`u16 bits[30]`) are compact bitsets used in Ocarina of Time
save files that tracks hundreds of one-bit flags. e.g., whether you've talked
to an NPC, triggered a cutscene, etc.

In the above example, 30 * u16 words stores up to 480 flags, each ID
(see below) is an index into this bitset.

## Usage

The entire "library" is just a few static inline functions:

```c
#define bitset_word(set, flag)  ((set)[bitset_index(flag)])

static inline uint16_t bitset_index(uint16_t flag) { 
	return flag >> 4;
}

static inline uint16_t bitset_mask (uint16_t flag) { 
	return 1u << (flag & 0xF);
}

static inline bool bitset_get(uint16_t *set, uint16_t flag) {
	return (bitset_word(set, flag) & bitset_mask(flag)) != 0;
}

static inline void bitset_set(uint16_t *set, uint16_t flag) {
	bitset_word(set, flag) |= bitset_mask(flag);
}

static inline void bitset_clear(uint16_t *set, uint16_t flag) {
	bitset_word(set, flag) &= ~bitset_mask(flag);
}
```

These bitsets are very simple, all you need is an array of uint16_t words.

You just need to ensure you have enough storage space to store all of the bits
you might need. OoT used 30 words to store up to 480 flags.

```c
#include <stdio.h>
#include <assert.h>

#include "oot_bitset.h"

enum game_events {
	// first word
	FLAG_MET_RUTO_FIRST_TIME,               // 0x00
	FLAG_PLAYED_SONG_FOR_ADULT_MALON,       // 0x01
	FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG,  // 0x02
	FLAG_TALKED_TO_MALON_FIRST_TIME,        // 0x03
	FLAG_TALKED_TO_TALON_IN_RANCH_HOUSE,    // 0x04
	FLAG_TOLD_EPONA_IS_SCARED,              // 0x05
	FLAG_HAS_DEKU_STICK_UPGRADE,            // 0x06
	FLAG_HAS_DEKU_NUT_UPGRADE,              // 0x07

	// second word
	FLAG_SAW_BOB   = 0x10, // 2nd word, 1st  bit
	FLAG_SAW_ALICE = 0x1A, // 2nd word, 11th bit
};

int main() {
	// we have
	uint16_t flags[10] = {0};
	int is_set = 0;

	bitset_set(flags, FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG);
	is_set = bitset_get(flags, FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG);
	printf("talked to malon after song? %s\n", is_set? "yeah":"nope");
	// prints 'yeah'

	// 2nd word
	bitset_set(flags, FLAG_SAW_BOB);
	bitset_set(flags, FLAG_SAW_ALICE);

	printf("ok! words: 0x%04x 0x%04x\n", flags[0], flags[1]);

	return 1;
}

// word FEDCBA9876543210
// ---------------------
// 0x0_ 0000000000000100  flags set: FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG (0x02)
// 0x1_ 0000010000000001  flags set: FLAG_SAW_BOB (0x10), FLAG_SAW_ALICE   (0x1A)
 
```


## Encoding

Each flag is a unique 16-bit coordinate where:
  - Upper 12 bits (flag >>  4): word index (0-29) (maximum of 4096 words)
  - Lower  4 bits (flag & 0xF): bit  index (0-15)

```
Layout:
   15         4  3         0
  [ word index ][ bit index ]

Example:
    Flag (hex)   Word   Bit
    ----------   -----  ---
    0x75         7      5
    0x61         6      1
    0x1AC        26     12

    0x75   word 7,  bit 5
    0x61   word 6,  bit 1
    0x1AC  word 26, bit 12
```

Because hex digits are 4 bits each, you can visually parse a flag as "word:bit":
These can simply by increasing values in an enum:

```c
enum FlagsOfInterest {
  FLAG_HAS_SEEN_BOB   = 0x00, // 1st word, 1st bit
  FLAG_HAS_SEEN_ALICE = 0x01, // 1st word, 2nd bit

  // you can even organize different bits by different words
  FLAG_HAS_SEEN_LINK  = 0x10, // 2nd word, 1st bit
  FLAG_HAS_SEEN_ZELDA = 0x1A, // 2nd word, 11th bit
}
```
