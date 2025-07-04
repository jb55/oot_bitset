

#include <stdio.h>
#include <assert.h>

#include "oot_bitset.h"

enum game_events {
	FLAG_MET_RUTO_FIRST_TIME,               // 0x00
	FLAG_PLAYED_SONG_FOR_ADULT_MALON,       // 0x01
	FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG,  // 0x02
	FLAG_TALKED_TO_MALON_FIRST_TIME,        // 0x03
	FLAG_TALKED_TO_TALON_IN_RANCH_HOUSE,    // 0x04
	FLAG_TOLD_EPONA_IS_SCARED,              // 0x05
	FLAG_HAS_DEKU_STICK_UPGRADE,            // 0x06
	FLAG_HAS_DEKU_NUT_UPGRADE,              // 0x07

	FLAG_SAW_BOB   = 0x10,
	FLAG_SAW_ALICE = 0x1A,
};

int main() {
	// we have
	unsigned short flags[2] = {0};
	int is_set = 0;

	is_set = bitset_get(flags, FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG);
	assert(is_set == 0);

	bitset_set(flags, FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG);
	is_set = bitset_get(flags, FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG);
	assert(is_set);

	// 3rd bit set
	assert(bitset_word(flags, FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG) == 4);
	assert(bitset_index(FLAG_TALKED_TO_ADULT_MALON_AFTER_SONG) == 0);

	// 2nd word
	assert(bitset_index(FLAG_SAW_BOB) == 1);
	bitset_set(flags, FLAG_SAW_BOB);
	assert(bitset_get(flags, FLAG_SAW_BOB));
	bitset_set(flags, FLAG_SAW_ALICE);

	printf("ok! words: 0x%04x 0x%04x\n", flags[0], flags[1]);

	return 1;
}
