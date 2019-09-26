#include <string.h>
#include "buffer.h";

#define DEFAULT_FLAGS  0xFFFC
#define SET_EOB  0xFFFE
#define RESET_EOB 0xFFFD
#define CHECK_EOB 0x002
#define SET_R_FLAG 0xFFFD
#define RESET_R_FLAG 0xFFFE
#define CHECK_R_FLAG 0x0001

Buffer* b_allocate(short init_capacity, char inc_factor, char o_mode) {
	

	if (init_capacity < 0 || init_capacity > SHRT_MAX - 1 || init_capacity == NULL) {
		return NULL;
	}

	Buffer* init = (Buffer*)calloc(1, sizeof(Buffer));
	if (init_capacity == 0) {
		init->cb_head = (char*)malloc(200);
		if (o_mode == 'f') {
			strcpy(inc_factor, '0');
		}
		else {
			strcpy(inc_factor, '15');
		}
	}
	else {
		init->cb_head = (char*)malloc(sizeof(char) * init_capacity);
		if ((o_mode == 'f' || (unsigned char)inc_factor == 0) || ((unsigned char)inc_factor == 0 && (unsigned char)inc_factor != 0)) {
			strcpy(init->mode, '0');
			strcpy(init->inc_factor, '0');
		}
		else if (o_mode == 'a') {
			if (1 <= inc_factor <= 255) { 
				strcpy(init->mode, '1');
				strcpy(init->inc_factor, inc_factor);
			}
			else {
				return NULL;
			}
		}
		else if (o_mode == 'm') {
			if (1 <= inc_factor <= 100) { //check for what 1 - 255 inclusive means
				strcpy(init->mode, '-1');
				strcpy(init->inc_factor, inc_factor);
			}
			else {
				return NULL;
			}
		}
		else {
			return NULL;
		}
	}
	init->capacity = init_capacity;
	init->flags = 0xFFFC;

	return init;
}

Buffer* b_addc(pBuffer const pBD, char symbol)
{
	if (pBD == NULL) {
		return NULL;
	}

	short new_cap; /*local variable to assign new memory for Buffer */
	char* new_Array = NULL; /* local variable for resizing Buffer */
	short available_space; /* new available space */
	short new_inc; /* The new inc_factor */

	if (pBD->flags & CHECK_R_FLAG == 1) {

	}
}

int b_clear(Buffer* const pBD)
{
	if (pBD == NULL) 
	{
		return 0;
	}

	free(pBD->cb_head);
	free(pBD);
}

int b_isFull(Buffer* const pBD)
{
	if (pBD == NULL)
	{
		return -1;
	}
	else if (strlen(pBD->cb_head) == 199) 
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

short b_limit(Buffer* const pBD)
{
	if (pBD == NULL)
	{
		return -1;
	}

	return (short)strlen(pBD->cb_head);
}

short b_capacity(Buffer* pBD)
{
	if (pBD == NULL) {
		return -1;
	}
	return (short)sizeof(pBD->cb_head);
}

short b_mark(pBuffer const pBD, short mark)
{
	if (pBD == NULL) {
		return -1;
	}

	if (0 <= mark <= pBD->addc_offset)
	{
		pBD->markc_offset = mark;
		return pBD->markc_offset;
	}
	else {
		return -1;
	}
}

/*This method doesn't specify -1 to return just says to notify calling function*/
int b_mode(Buffer* const pBD)
{
	if (pBD == NULL) {
		return -1;
	}

	return pBD->mode;
}

size_t b_incfactor(Buffer* const pBD)
{
	if (pBD == NULL) {
		return 0x100;
	}

	return fabs(pBD->inc_factor);
}

int b_load(FILE* const fi, Buffer* const pBD)
{
	
}

int b_isempty(Buffer* const pBD)
{
	if (pBD == NULL) {
		return -1;
	}

	if (pBD->addc_offset == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

char b_getc(Buffer* const pBD)
{
	if (pBD == NULL) {
		return -2;
	}

	if (pBD->getc_offset == pBD->addc_offset) {
		/*Using bitwise operation it sets the flags field eob bit to 1 and returns 0*/
	}
	else {
		/* using bitwise operation sets eob to 0*/
	}

	char temp = pBD->getc_offset;
	pBD->getc_offset++;
	return temp;
}

int b_eob(Buffer * const pBD)
{
	/*The function returns the value of the flags field determined only by the eob bit.
	A bitwise operation must be used to return the value of the flags field. If a run-time error is
	possible, it should return -1.*/
}

int b_print(Buffer* const pBD, char nl)
{
	int counter = 0;
	while (b_eob(pBD)) {
		printf("%c", b_getc(pBD));
		counter++;
	}

	if (nl != 0) {
		printf("\n");
	}
	return counter;
}

Buffer* b_compact(Buffer* const pBD, char symbol)
{
	/*For all operational modes of the buffer the function shrinks (or in some cases may expand) the
	buffer to a new capacity. The new capacity is the current limit plus a space for one more character.
	In other words the new capacity is addc_offset + 1 converted to bytes. The function uses realloc()
	to adjust the new capacity, and then updates all the necessary members of the buffer descriptor
	structure. Before returning a pointer to Buffer, the function adds the symbol to the end of the
	character buffer (do not use b_addc(), use addc_offset to add the symbol) and increments
	addc_offset. The function must return NULL if for some reason it cannot to perform the required
	operation. It must set the r_flag bit appropriately.
	*/
}

char b_rflag(Buffer* const pBD)
{

}

short b_retract(Buffer* const pBD)
{

}

short b_reset(Buffer* const pBD)
{

}

short b_getcoffset(Buffer * const pBD)
{

}

int b_rewind(Buffer* const pBD)
{

}

char* b_location(Buffer* const pBD)
{

}

