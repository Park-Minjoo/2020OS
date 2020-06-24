#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "smalloc.h" 

sm_container_t sm_head = {
	0,
	&sm_head,
	&sm_head,
	0
};

sm_container_ptr sm_unused_containers = 0x0 ;

static
void *
_data (sm_container_ptr e){
	return ((void *) e) + sizeof(sm_container_t) ;
}

static 
void 
sm_container_split (sm_container_ptr hole, size_t size)
{
	sm_container_ptr remainder = (sm_container_ptr) (_data(hole) + size) ;

	remainder->dsize = hole->dsize - size - sizeof(sm_container_t) ;	
	remainder->status = Unused ;
	remainder->next = hole->next ;
	remainder->prev = hole ;
	hole->dsize = size ;
	hole->next->prev = remainder ;
	hole->next = remainder ;

	sm_container_ptr itr = 0x0;
	if (sm_unused_containers == 0x0){
		sm_unused_containers = remainder;
	}
	else{
		if(hole == hole->next){
			sm_container_ptr itr = 0x0 ;
			for (itr = sm_unused_containers ; itr->prev != 0x0 && itr->prev!= hole ; itr = itr-> prev);
				
				itr->prev = remainder;
		}else if(hole == hole->prev){
			remainder->prev= hole->prev;
			sm_unused_containers = remainder;
		}
		
		else{
			sm_container_ptr itr = 0x0 ;
			for(itr = sm_unused_containers ; itr->prev !=hole ; itr = itr->prev);		
				remainder->prev = hole-> prev;;
				itr->prev = remainder;
		}
			
	}

  	if (hole == hole->next)
                hole->next = remainder ;
}

void print_unused_memory()
{
	sm_container_ptr itr = 0x0;
	printf("unused linked list\n");
	for(itr = sm_unused_containers ; itr!=0x0; itr = itr->prev){
		printf("%8d ->", (int)itr->dsize);
	}
	printf("\n");
}

void merge_unused_memory()
{
	sm_container_ptr itr = 0x0;
	for(itr = sm_unused_containers ; itr->prev!=0x0; itr = itr->prev){
		if(itr->prev== itr->next){
			sm_container_ptr next = itr->next->next;
			sm_container_ptr unused = itr->next->prev;
			itr->next = next;
			itr->prev= unused;
			itr->dsize += itr->next->dsize + sizeof(sm_container_t) ;
		}
	}
}

static 
void * 
retain_more_memory (int size)
{
	sm_container_ptr hole ;
	int pagesize = getpagesize() ;
	int n_pages = 0 ;

	n_pages = (sizeof(sm_container_t) + size + sizeof(sm_container_t)) / pagesize  + 1 ;
	hole = (sm_container_ptr) sbrk(n_pages * pagesize) ;
	if (hole == 0x0)
		return 0x0 ;
	
	hole->status = Unused ;
	hole->dsize = n_pages * getpagesize() - sizeof(sm_container_t) ;
	return hole ;
}

void * 
smalloc (size_t size) 
{
	sm_container_ptr hole = 0x0, itr = 0x0 ;
	size_t bestFit =-1;

	for (itr = sm_head.next ; itr != &sm_head ; itr = itr->next) {
		if (itr->status == Busy)
			continue ;
		if (itr->dsize == size) {
			hole = itr ;
			break ; 
		}
		else if (size + sizeof(sm_container_t) < itr->dsize) {
			if(bestFit == -1){//init
				hole = itr ;
				bestFit = itr->dsize;
			}else if(bestFit>itr->dsize){
				hole = itr ;
				bestFit = itr->dsize;
			}
		}
	}
	if (hole == 0x0) {
		hole = retain_more_memory(size) ;
		if (hole == 0x0)
			return 0x0 ;
		hole->next = &sm_head ;
		hole->prev = sm_head.prev ;
		(sm_head.prev)->next = hole ;
		sm_head.prev = hole ;
	}
	if (size < hole->dsize) 
		sm_container_split(hole, size) ;
	hole->status = Busy ;
	return _data(hole) ;

}

void print_mem_uses()
{
    sm_container_ptr itr ;
    int i = 0 ;
    int sum=0 ;
    int uses=0;
	for (itr = sm_head.next ; itr != &sm_head ; itr = itr->next) {
        sum+=itr->dsize;
        if(itr->status == Busy) uses+=itr->dsize;
    }
    printf("Total retained size for smalloc : %8d\n", sum);
    printf("Total allocated size            : %8d\n", uses);
    printf("Retaind but not allocated size  : %8d\n", sum-uses);
    
    
}

void 
sfree (void * p)
{
	sm_container_ptr itr ;
	for (itr = sm_head.next ; itr != &sm_head ; itr = itr->next) {
		if (p == _data(itr)) {
			itr->status = Unused ;
			break ;
		}
	}
	merge_unused_memory();
}

void 
print_sm_containers ()
{
	sm_container_ptr itr ;
	int i ;

	printf("==================== sm_containers ====================\n") ;
	for (itr = sm_head.next, i = 0 ; itr != &sm_head ; itr = itr->next, i++) {
		printf("%3d:%p:%s:", i, _data(itr), itr->status == Unused ? "Unused" : "  Busy") ;
		printf("%8d:", (int) itr->dsize) ;

		int j ;
		char * s = (char *) _data(itr) ;
		for (j = 0 ; j < (itr->dsize >= 8 ? 8 : itr->dsize) ; j++) 
			printf("%02x ", s[j]) ;
		printf("\n") ;
	}
	printf("\n") ;

}

void * srealloc(void * p, size_t nsize){
	sm_container_ptr hole = 0x0, itr = 0x0 ;
        size_t bestFit =-1;

        for (itr = sm_head.next ; itr != &sm_head ; itr = itr->next) {
                if (itr->status == Busy)
                        continue ;
                if (itr->dsize == nsize) {
                        hole = itr ;
                        break ;
                }
                else if (nsize + sizeof(sm_container_t) < itr->dsize) {
                        if(bestFit == -1){//init
                                hole = itr ;
                                bestFit = itr->dsize;
                        }else if(bestFit>itr->dsize){
                                hole = itr ;
                                bestFit = itr->dsize;
                        }
                }
        }
        if (hole == 0x0) {
                hole = retain_more_memory(nsize) ;
                if (hole == 0x0)
                        return 0x0 ;
                hole->next = &sm_head ;
                hole->prev = sm_head.prev ;
                (sm_head.prev)->next = hole ;
                sm_head.prev = hole ;
        }
        if (nsize < hole->dsize)
                sm_container_split(hole, nsize) ;
        	hole->status = Busy ;
		memcpy(srealloc,smalloc, nsize);
        return _data(hole) ;
}

void sshrink(){
	sm_container_ptr hole = 0x0, itr = 0x0;

	for(itr = sm_unused_containers ; itr->prev!=0x0; itr = itr->prev){
                if(itr->prev== itr->next){
                        sm_container_ptr next = itr->next->next;
                        sm_container_ptr unused = itr->next->prev;
                        itr->next = next;
                        itr->prev= unused;
                        itr->dsize += itr->next->dsize + sizeof(sm_container_t) ;
                	sbrk(-itr->dsize);
		}
	} 
}

