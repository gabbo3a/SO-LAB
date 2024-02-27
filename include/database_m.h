#ifndef DBM_H
#define DBM_H

typedef struct {
    pid_t        pid;
    unsigned int atom_number;
    bool         is_slag;
} AtomRecord;

void init_db();
void clear_db();
void close_db();

size_t get_number_atom_records();   // get_db_size() 
int get_atom_index(const pid_t pid);

AtomRecord read_atom_record(const pid_t pid);
AtomRecord read_atom_record_i(size_t index);

void write_atom_record(size_t index, const AtomRecord atom);
void add_atom_record(const AtomRecord atom);

void print_all_db();

#endif
