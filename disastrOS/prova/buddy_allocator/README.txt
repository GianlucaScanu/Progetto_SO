Guida ai file: 

Architettura a 3 layer: 1. malloc/free dell'allocatore   2. allocatore dei buddies   3. poolAlloc e liste collegate

1. libbuddy.a
    -libreria inclusa in tutti i file attraverso il makefile

2. buddy_allocator.h
    -definizione BuddyListItem (idx,lvl,start,size,parent,buddy)
    -definizione BuddyAllocator (free lits, pool alloc, buffer)

3. buddy_allocator.c
    -calcSize (dell'allocatore)
    -init allocatore
    -getBuddy
    -releaseBuddy
    -malloc
    -free

    Top bottom (da inserire poi come bottom up)

    1st Layer
        FREE: recupero buddy address -> call releaseBuddy
        MALLOC: recupero buddy level -> call getBuddy

    2nd Layer
        RELEASEBUDDY: inserimento nella freelist -> controlli su parent e buddy -> DestroyListItem e merge
        GETBUDDY: controlli sul level -> if (!disponibile):
                                            ricorsione
                                            createListItem per left e right
                                         else
                                            detach da freelist
                                            return
        INIT: inizializzazione poolAlloc, freelists, primo nodo
        CREATELISTITEM: PoolAllocator_getBlock -> riempe campi dell'oggetto -> pushBack nella freelist
        DESTROYLISTITEM: detach dalla freelist -> PoolAllocator_releaseBlock

    3rd Layer
        POOL ALLOCATOR:
            pool_allocator.h: definizione struttura poolAlloc (memoria, freelist, itemSize, numero di items, ...)
            GETBLOCK: campi allocatore per gestire la lista collegata -> return buffer+offset
            RELEASEBLOCK: recupero idx dall'indirizzo parametro -> set del a->first nell'allocatore

        LISTE COLLEGATE:
            linked_list.h : definizione ListItem e ListHead
            Insert: controlli e inserimento in lista 
            Detach: controlli e rimozione dalla lista
            PopFront, popBack, pushFront, pushBack derivano tutte da insert, detach