#include "ARPong.h"

const size_t field_geometry_quads = 11;
const float field_geometry[] = {
	/* 1 */
	-1.0f, 0.0f, 0.0f,

	-25.0f,   25.0f, 0.0f,
	-25.0f, -100.0f, 0.0f,
	-25.0f, -100.0f, 50.0f,
	-25.0f,   25.0f, 50.0f,

	/* 2 */
	0.0f, 1.0f, 0.0f,

	 -25.0f, 25.0f,  0.0f,
	 220.0f, 25.0f,  0.0f,
	 220.0f, 25.0f, 50.0f,
	 -25.0f, 25.0f, 50.0f,

	/* 3 */
	1.0f, 0.0f, 0.0f,

	220.0f, -100.0f, 0.0f,
	220.0f,   25.0f, 0.0f,
	220.0f,   25.0f, 50.0f,
	220.0f, -100.0f, 50.0f,

	/* 4 */
	0.0f, -1.0f, 0.0f,

	-25.0f, -100.0f, 0.0f,
	-15.0f, -100.0f, 0.0f,
	-15.0f, -100.0f, 50.0f,
	-25.0f, -100.0f, 50.0f,

	/* 5 */
	0.0f, -1.0f, 0.0f,

	220.0f, -100.0f, 0.0f,
	210.0f, -100.0f, 0.0f,
	210.0f, -100.0f, 50.0f,
	220.0f, -100.0f, 50.0f,

	/* 6 */
	-1.0f, 0.0f, 0.0f,

	210.0f,   15.0f, 0.0f,
	210.0f, -100.0f, 0.0f,
	210.0f, -100.0f, 50.0f,
	210.0f,   15.0f, 50.0f,

	/* 7 */
	1.0f, 0.0f, 0.0f,

	-15.0f,   15.0f, 0.0f,
	-15.0f, -100.0f, 0.0f,
	-15.0f, -100.0f, 50.0f,
	-15.0f,   15.0f, 50.0f,

	/* 8 */
	0.0f, -1.0f, 0.0f,

	-15.0f, 15.0f, 0.0f,
	210.0f, 15.0f, 0.0f,
	210.0f, 15.0f, 50.0f,
	-15.0f, 15.0f, 50.0f,

	/* 9 */
	0.0f, 0.0f, 1.0f,

	-25.0f, -100.0f, 50.0f,
	-15.0f, -100.0f, 50.0f,
	-15.0f,   25.0f, 50.0f,
	-25.0f,   25.0f, 50.0f,

	/* 10 */
	0.0f, 0.0f, 1.0f,

	-15.0f, 15.0f, 50.0f,
	210.0f, 15.0f, 50.0f,
	210.0f,  25.0f, 50.0f,
	-15.0f,  25.0f, 50.0f,

	/* 11 */
	0.0f, 0.0f, 1.0f,

	210.0f, -100.0f, 50.0f,
	220.0f, -100.0f, 50.0f,
	220.0f,   25.0f, 50.0f,
	210.0f,   25.0f, 50.0f,
};

const size_t pad_geometry_quads = 2;
const float pad_geometry[] = {

	/* Up */
	0.0f, 0.0f, 1.0f,

	-60.0f, -45.0f, 10.0f,
	 45.0f, -45.0f, 10.0f,
	 45.0f,  45.0f, 10.0f,
	-60.0f,  45.0f, 10.0f,

	/* Pad */
	-1.0f, 0.0f, 0.0f,

	-60.0f,  45.0f, -45.0f,
	-60.0f, -45.0f, -45.0f,
	-60.0f, -45.0f,  45.0f,
	-60.0f,  45.0f,  45.0f,

};
