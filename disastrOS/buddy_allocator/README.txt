Guida ai file:

Architettura a 3 layer: 1. allocatore con malloc free   2. allocatore dei buddies   3 liste collegate

1. buddy_allocator.h
    -definizione BuddyListItem (idx,lvl,start,size,parent,buddy)
    -definizione BuddyAllocator (free lits, pool alloc, buffer)

2. buddy_allocator.c
    -calcSize (dell'allocatore)
    -init allocatore
    -getBuddy
    -releaseBuddy
    -malloc
    -free

