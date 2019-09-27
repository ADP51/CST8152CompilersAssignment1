#include <string.h>
#include "buffer.h"

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
		pBd->flags &= RESET_R_FLAG; /* Reset r_flag if need be */
	}
	
	if(b_isFull(pBD) == 0){ /*If buffer is not full*/
		pBD->cb_head[addc_offset] = symbol; /*Add sybol to buffer and increment addc_offset*/
		pBD->addc_offset++;
		return pBD;
	} 
	
	if(pBD->mode == '0') {
		return NULL; /*If the buffer is full and Operational mode is 0 then return null*/
	}
	
	if(pBD->mode == '1') {
		new_cap = pBD->capacity + (short)pBD->inc_factor; /* Creates new capacity by adding inc_factor to current cap */
		if(new_cap > 0 && new_cap > (SHRT_MAX - 1)) {
			new_cap = SHRT_MAX - 1; /* If new_cap is positive but exceeds the max allowed value reassing to the max value */
		} 
		
		if(new_cap < 0){ /* If overflow has occured return NULL */
			return NULL;
		}
	}
	
	if(pBD->mode == '-1'){
		available_space = (SHRT_MAX - 1) - pBD->capacity;
		new_inc = available_space * pBD->inc_factor / 100;
		new_cap = pBD->capacity + new_inc;
		
		if(0 > new_cap || new_cap > (SHRT_MAX - 1)){
			new_cap = SHRT_MAX - 1; /* If the new_cap is larger than the allowed max but the current cap is not full assign max cap */
		} 
		
		
	}
	
	new_Array = realloc(pBD->cb_head, new_cap); /* Reallocate buffer size with new_cap*/
	if(new_Array == NULL){ /* Check to make sure realloc worked */
		return NULL;
	}
	
	pBD->cb_head = new_Array;
	new_Array = NULL;
	pBD->flags &= SET_R_FLAG;/*Need to set r_flag to 1*/
	pBD->cb_head[addc_offset] = symbol; /*Add sybol to buffer and increment addc_offset*/
	pBD->addc_offset++;
	pBD->capacity = new_cap;
	return pBD;
}

int b_clear(Buffer* const pBD)
{
	if (pBD == NULL) {
		return RT_FAIL_1;
	}
	
	pBD->addc_offset = 0;
	pBD->getc_offset = 0;
	pBD->mark_offset = 0;
	strcpy(pBD->mode, '0');
	pBD->flags &= RESET_R_FLAG;
	pBD->flags &= RESET_EOB;

	return TRUE;
}

int b_free(Buffer * const pBD){
	if(pBD == NULL){
		return RT_FAIL_1;
	}
	
	free(pBD->cb_head);
	free(pBD);
}

int b_isFull(Buffer* const pBD)
{
	if (pBD == NULL) {
		return RT_FAIL_1;
	}
	
	if (pBD->addc_offset == pBD->capacity) {
		return 1;
	}

	return 0;
	
}

short b_limit(Buffer* const pBD)
{
	if (pBD == NULL)
	{
		return RT_FAIL_1;
	}

	return addc_offset - 1;
}

short b_capacity(Buffer* pBD)
{
	if (pBD == NULL) {
		return RT_FAIL_1;
	}
	return pBD->capacity;
}

short b_mark(pBuffer const pBD, short mark)
{
	if (pBD == NULL) {
		return RT_FAIL_1;
	}

	if (0 <= mark <= pBD->addc_offset)
	{
		pBD->markc_offset = mark;
		return pBD->markc_offset;
	}
	else {
		return RT_FAIL_1;
	}
}

/*This method doesn't specify -1 to return just says to notify calling function*/
int b_mode(Buffer* const pBD){
	if (pBD == NULL) {
		return RT_FAIL_1;
	}

	return pBD->mode;
}

size_t b_incfactor(Buffer* const pBD){
	if (pBD == NULL) {
		return 0x100;
	}

	return (unsigned char)pBD->inc_factor;
}

int b_load(FILE* const fi, Buffer* const pBD){
	if(pBD == NULL || fi == NULL){
		return RT_FAIL_1;
	}
	char next; /*The next char from the file*/
	
	while(1){
		next = fgetc(fi);
		if(feof(fi)){ /*Detect end of file before calling b_addc()*/
			break;
		}
		if(b_addc(pBD, next) == NULL){ 
			ungetc(next,fi);
			return LOAD_FAIL;
		}
	}
}

int b_isempty(Buffer* const pBD)
{
	if (pBD == NULL) {
		return RT_FAIL_1;
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
		return RT_FAIL_2;
	}

	if (pBD->getc_offset == pBD->addc_offset) {
		pBD->flags |= SET_EOB; /*Using bitwise operation it sets the flags field eob bit to 1 and returns 0*/
		return 0;
	}
	
	pBD->flags &= RESET_EOB;/* using bitwise operation sets eob to 0*/
	char temp = pBD->getc_offset;
	pBD->getc_offset++;
	return temp;
}

int b_eob(Buffer * const pBD)
{
	/*The function returns the value of the flags field determined only by the eob bit.
	A bitwise operation must be used to return the value of the flags field. If a run-time error is
	possible, it should return -1.*/
	if(pBD == NULL){
		return RT_FAIL_1;
	}
	
	return (pBD->flags >> 1) & 1 /* Will return the EOB bit */
}

int b_print(Buffer* const pBD, char nl) {
	if(pBD == NULL || nl ==NULL){
		return RT_FAIL_1;
	}
	
	int counter = 0;
	
	while (b_eob(pBD) == 0) {
		printf("%c", b_getc(pBD));
		counter++;
	}

	if (nl != 0) {
		printf("\n");
	}
	return counter;
}

	/*For all operational modes of the buffer the function shrinks (or in some cases may expand) the
	buffer to a new capacity. The new capacity is the current limit plus a space for one more character.
	In other words the new capacity is addc_offset + 1 converted to bytes. The function uses realloc()
	to adjust the new capacity, and then updates all the necessary members of the buffer descriptor
	structure. Before returning a pointer to Buffer, the function adds the symbol to the end of the
	character buffer (do not use b_addc(), use addc_offset to add the symbol) and increments
	addc_offset. The function must return NULL if for some reason it cannot to perform the required
	operation. It must set the r_flag bit appropriately.
	*/
Buffer* b_compact(Buffer* const pBD, char symbol) {
	if(pBD == NULL){
		return RT_FAIL_1;
	}
	
	char * temp_array; /* temp array for realloc */
	short new_cap; /* The new capacity */
	
	new_cap = pBD->addc_offset + 1;
	if(new_cap < 0 || new_cap == SHRT_MAX){
		return NULL;
	}
	
	if(temp_array = realloc(pBD->cb_head, sizeof(char)*new_cap) == NULL){
		return NULL;
	}
	
	pBD->addc_offset++;
	pBD->cb_head = temp_array;
	temp_array = NULL;
	pBD->capacity = new_cap;
	pBD->flags |= SET_R_FLAG
	strcpy(pBD->cb_head[pBD->addc_offset], symbol);
	return pBD;
}

char b_rflag(Buffer* const pBD) {
	if(pBD == NULL){
		return RT_FAIL_1;
	}
	
	return pBD->flags & 1 /* Will return the EOB bit */
}

short b_retract(Buffer* const pBD) {

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

