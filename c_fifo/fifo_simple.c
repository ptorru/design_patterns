/***********************************************************************************

    MIT License
    
    Copyright (c) 2021 Pedro Torruella
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

**********************************************************************************/
		    
#include <stdio.h>
#include <assert.h>


// This is our control structure.
// Will hold everything related to the fifo - But not the Data!
struct st_fifo_simple
{
    unsigned long length;
    unsigned long level;
    unsigned long rd_idx;
    unsigned long wr_idx;
    char empty;
    char full;
    char *data;
};


// purpose: Use me to correctly initialize your fifo!
//          Assumes that your data buffer is empty!
void fifo_init(struct st_fifo_simple * fifo, unsigned long length, char * data)
{
    fifo->length = length;
    fifo->data = data;
    fifo->rd_idx = 0;
    fifo->wr_idx = 0;
    fifo->empty = (char) 1;
    fifo->full = (char) 0;
}


// purpose: Internal -  Implements pointer logic
//
// NOTE: Not implemented through pointers in order to allow look-ahead
//       increment.
unsigned long increment_idx(unsigned long idx, unsigned long top)
{
    idx++;
    if (idx == top)
    {
        idx = 0;
    }
    return idx;
}


// purpose: Internal - Updates the level status variable
char update_level(struct st_fifo_simple * fifo)
{
    if ( (fifo->rd_idx < fifo->length) && (fifo->rd_idx >= 0) &&
         (fifo->wr_idx < fifo->length) && (fifo->wr_idx >= 0))
    {
        if (fifo->wr_idx > fifo->rd_idx)
        {
            fifo->level = (fifo->wr_idx - fifo->rd_idx);
        }
        else
        {
            fifo->level = ((fifo->length - fifo->rd_idx) + fifo->wr_idx);
        }
	return 0;
    }
    else
    {
        return -1;
    }
}


// purpose: Puts a new data into the FIFO.
// returns: 0 if everything was OK!
//         -1 if something went wrong.
char fifo_put(struct st_fifo_simple * fifo, char in_data)
{
    if (fifo->full == 0)
    {
	fifo->data[fifo->wr_idx] = in_data;
	fifo->wr_idx = increment_idx(fifo->wr_idx, fifo->length);
	fifo->full == 0;
	if(fifo->wr_idx == fifo->rd_idx) fifo->full = 1;
	fifo->empty = 0;
	return update_level(fifo);
    }
    else
    {
	return -1;
    }
}


// purpose: Takes an item from the FIFO and puts it into *out_data
// returns: 0 if everything was OK!
//         -1 if something went wrong.
char fifo_take(struct st_fifo_simple *fifo, char *out_data)
{
    if (fifo->empty == 0)
    {
	*out_data = fifo->data[fifo->rd_idx];
	fifo->rd_idx = increment_idx(fifo->rd_idx,fifo->length);
	if(fifo->wr_idx == fifo->rd_idx) fifo->empty = (char) 1;
	fifo->full = (char) 0;
	return update_level(fifo);
    }
    else
    {
	return -1;
    }

}


// Functions for testing purposes only!
void test_write(char * data, int elems, struct st_fifo_simple *test_fifo)
{
    char status=0;
    for(int i=0; i < elems; i++)
    {
	status = fifo_put(test_fifo, data[i]);
	assert(status == 0);
    }
}


void test_print(int elems, struct st_fifo_simple *test_fifo)
{
    char character;
    char status=0;
    for(int i=0; i < elems; i++)
    {
	status = fifo_take(test_fifo, &character);
	assert(status == 0);
	printf("%c", character);
    }
}


#define ELEMENTS 20


// This main tries to run the minimum set of tests you would expect.
void main()
{
    // Give me some memory
    char mensaje[ELEMENTS] = {'H', 'o', 'l', 'a', ' ', 'U', 'S', 'B', ' ',
			      'c', 'o', 'm', 'o',
                              ' ', 'e', 's', 't', 'a', 'n', '?'};
    char fifo_data[ELEMENTS];
    char status = 0;

    // Create Fifo structure
    struct st_fifo_simple myfifo;
    fifo_init( &myfifo, ELEMENTS, fifo_data);


    // Test 0 - Try to read from empty Fifo
    // Let's cheat double-using status...
    status = fifo_take(&myfifo, &status);
    assert(status == -1);
    assert(myfifo.empty == 1);

    // Test 1
    printf("Test 1 - First for loop, store and then print.\n");
    test_write(mensaje, ELEMENTS, &myfifo);
    assert(myfifo.empty == 0);
    assert(myfifo.full == 1);
    test_print(ELEMENTS, &myfifo);
    assert(myfifo.empty == 1);
    assert(myfifo.full == 1);
    printf("\n");

    // Test 2
    printf("Test 2 - Try to write to full fifo.\n");
    test_write(mensaje, ELEMENTS, &myfifo);
    status = fifo_put(&myfifo, mensaje[1]);
    assert(status == -1);

    // flush...
    test_print(ELEMENTS, &myfifo);
    printf("\n");

    // Test 3
    printf("Test 3 - Lets try weird wr/rd's...\n");
    test_write(mensaje, ELEMENTS/2, &myfifo);
    test_write(mensaje, ELEMENTS/2, &myfifo);

    test_print(5, &myfifo);
    test_write(mensaje, 5, &myfifo);
    test_print(ELEMENTS, &myfifo);
    printf("\n");
}
