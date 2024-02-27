msg_queue_m.o: src/channels/msg_queue_m.c include/utility.h \
 include/shared_memory_m.h include/semaphore_m.h include/utility.h \
 include/msg_queue_m.h
database_m.o: src/channels/database_m.c include/utility.h \
 include/shared_memory_m.h include/semaphore_m.h include/utility.h \
 include/database_m.h include/semaphore_m.h
shared_memory_m.o: src/channels/shared_memory_m.c include/utility.h \
 include/shared_memory_m.h include/semaphore_m.h include/utility.h \
 include/shared_memory_m.h
atom_operations.o: src/atom_operations.c include/utility.h \
 include/shared_memory_m.h include/semaphore_m.h include/utility.h
semaphore_m.o: src/semaphore_m.c include/utility.h \
 include/shared_memory_m.h include/semaphore_m.h include/utility.h \
 include/semaphore_m.h
utility.o: src/utility.c include/utility.h include/shared_memory_m.h \
 include/semaphore_m.h include/utility.h
