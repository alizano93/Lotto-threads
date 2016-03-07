#include <gtk/gtk.h>

typedef struct row_struct_t
{
    int id_int;
    char* id_char;
    GtkWidget* id;
    GtkWidget* progress;
    GtkWidget* status;
    GtkWidget* result;
} row_struct_t;

void update_row_active(char*);

//void update_row_inactive_completed(char*, int);

void update_row_work(char*, float, double, int);

void add_row(int, char*);

void init_table();

void create_UI(int);

void show_ui();
