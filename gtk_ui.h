#include <gtk/gtk.h>

typedef struct row_struct_t
{
    int id_int;
    
    GtkWidget* id;
    GtkWidget* progress;
    GtkWidget* status;
    GtkWidget* result;
} row_struct_t;

void update_row_active(int);

void update_row_inactive_completed(int, int);

void update_row_work(int, float, double);

void add_row(int);

void init_table();

void create_UI(int);

void show_ui();
