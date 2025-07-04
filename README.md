
# OoT bitset

Ocarina of Time has a succinct data structure for storing many flags in a
compact way.

oot_bitsets ie. (`u16 bits[30]`) are compact bitsets used in Ocarina of Time
for tracking file that tracks hundreds of one-bit flags. e.g., whether you've
talked to an NPC, triggered a cutscene, etc.

In the above example, 30 * u16 words stores up to 480 flags, each inftable ID
(see below) is an index into this bitvector.

oot_bitsets *must* operate on arrays io u16 words!

## Encoding

Each flag is a unique 16-bit ID where:
  - Upper 12 bits (flag >>  4): word index (0-29) (maximum of 4096 words)
  - Lower  4 bits (flag & 0xF): bit index (0-15)

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
  FLAG_HAS_SEEN_LINK  = 0x10, // 2nd word, 0th bit
  FLAG_HAS_SEEN_ZELDA = 0x1A, // 2nd word, 10th bit
}
```

or you can just keep it simple:

```c
enum FlagsOfInterest {
  FLAG_HAS_SEEN_BOB,
  FLAG_HAS_SEEN_ALICE,
  FLAG_HAS_SEEN_LINK,
  FLAG_HAS_SEEN_ZELDA,
}
```

