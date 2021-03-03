// The following aims to be a very simple implementation of a
// Multiple Event Receptor State Machine
//
// Inspired by Chapter 5.4 from
// Design Patterns for Embedded Systems in C
// by Bruce Powell Douglass


#include <stdio.h>
#include <assert.h>
#include <ctype.h>

#define MAXCHAR (7)


typedef enum state_s
{
    IDLE,
    RECEIVING,
    PRINTING
} state_t;


typedef struct fsm_s
{
    state_t state;
    char buffer[MAXCHAR];
    int count;
} fsm_t;


// The following functions implement the transtions into the
// states
void into_idle(fsm_t *self)
{
    self->state = IDLE;
    self->count = 0;
}


void into_receiving(fsm_t *self, char c)
{
    assert( self->count < MAXCHAR);
    self->state = RECEIVING;
    self->buffer[self->count] = c;
    self->count++;
}


int into_printing(fsm_t *self)
{
    assert(self->count > 0);
    self->state = PRINTING;
    printf("%c", self->buffer[MAXCHAR-self->count]);
    self->count--;
    if(self->count != 0) return 0;
    printf("\n");
    return 1;
}


// The following functions are called depending on each event
void ev_number(fsm_t *self, char c)
{
    switch(self->state)
    {
    case IDLE:
	into_receiving(self,c);
	break;
    case RECEIVING:
	into_receiving(self,c);
	if(self->count == MAXCHAR) into_printing(self);
	break;
    }
}


int ev_other(fsm_t *self, char c_in)
{
    into_idle(self);
    if(c_in == 'q')
    {
        return 1;
    }
    return 0;
}

// see: https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c
void flush()
{
  int c;
  while ((c = getc(stdin)) != '\n' && c != EOF) {}
}

int main(int argc, char **argv)
{
    char c_in;
    fsm_t fsm;

    into_idle(&fsm);
    
    printf("Insert some numbers and we will encode them for you...\n");
    while(1)
    {
	if(fsm.state != PRINTING)
	{
	    c_in = getc(stdin);
	    flush();

	    if(isdigit(c_in))
	    {
	        ev_number(&fsm,c_in);
	    }
	    else
	    {
	        // Exit if ev_other returns 1
	        if(ev_other(&fsm, c_in))
		{
		    break;
		}
	    }
	}
	else
	{
	    if(into_printing(&fsm)) break;
	}
    }
    return 0;
}
