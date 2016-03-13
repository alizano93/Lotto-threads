#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

//#include <gtk/gtk.h>
#include "gtk_ui.h"
#include "myThread.h"
#include "hashmap.h"

static float percent = 0.0;

map_t mymap;
static GtkWidget *window;
GtkWidget *table;
extern int totalProcess;
extern state *states;

/*
static gboolean inc_progress(gpointer data){
	percent += 0.05;
	if(percent > 1){
		percent = 0.0;	
	}
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(data), percent);
	char c[3];
	sprintf(c, "%d%%", (int)(percent * 100));
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(data), c);
}
*/

void update_row_active(char *id_char){

//	printf("%d", id_int);

	

	row_struct_t* thread;

//	char id_char[5];
//	sprintf(id_char, "%d", id_int);

//	printf("%s", id_char);

	int error = hashmap_get(mymap, id_char, (void**)(&thread));

        /* Make sure the value was both found and the correct number */

//	printf("%d", error);

        assert(error==MAP_OK);
        //assert(thread->id_char==id_char);

	gdk_threads_enter();

	gtk_label_set_text (GTK_LABEL (thread->status), "ACTIVE");

//	gdk_flush ();
	gdk_threads_leave();
}
/*
void update_row_inactive_completed(char *id_char, int finish){

	

	row_struct_t* thread;

//	char id_char[5];
//	sprintf(id_char, "%d", id_int);

	int error = hashmap_get(mymap, id_char, (void**)(&thread));

        // Make sure the value was both found and the correct number
        assert(error==MAP_OK);
        assert(thread->id_char==id_char);

	char *st;
	if(finish == 1){
		st = "COMPLETED";
	}else{
		st = "INACTIVE";
	}

	gdk_threads_enter();

	gtk_label_set_text (GTK_LABEL (thread->status), st);

//	gdk_flush ();
	gdk_threads_leave();
}
*/


void update_row_work(char *id_char, float percent, double result, int finish){



	row_struct_t* thread;

//	char id_char[5];
//	sprintf(id_char, "%d", id_int);

	int error = hashmap_get(mymap, id_char, (void**)(&thread));

        /* Make sure the value was both found and the correct number */
        assert(error==MAP_OK);
        //assert(thread->id_char==id_char);

	char *st;
	if(finish == 1){
		st = "COMPLETED";
	}else{
		st = "INACTIVE";
	}

	gdk_threads_enter();

	gtk_label_set_text (GTK_LABEL (thread->status), st);

	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(thread->progress), percent);
	char c[3];
	sprintf(c, "%d%%", (int)(percent * 100));
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(thread->progress), c);

	char d[60];
	sprintf(d, "%.50f", result);
	gtk_label_set_text (GTK_LABEL (thread->result), d);

//	gdk_flush ();
	gdk_threads_leave();
}

/*
void update_row(map_t mymap, Thread* current){

	row_struct_t* thread;


	int error = hashmap_get(mymap, current->tid, (void**)(&thread));

        // Make sure the value was both found and the correct number 
        assert(error==MAP_OK);
        assert(thread->id_int==current->tid);

	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(thread->progress), current->percent);
	char c[3];
	sprintf(c, "%d%%", (int)(current->percent * 100));
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(thread->progress), c);

	char st[10];
	if(current->finish == 1){
		st = "COMPLETED";
	}else{
		st = "INACTIVE";
	}

	gtk_label_set_text (GTK_LABEL (thread->status), st);

	char d[60];
	sprintf(d, "%.50f", current->result);
	gtk_label_set_text (GTK_LABEL (thread->result), d);
}
*/

double currentp = 0.0;

static gboolean progress_timeout( gpointer data ){

	int k;
	char *ids[] = {"1", "2", "3"};
	int allFinish = 1;
	
	for( k = 0; k < 3; k++){
		
		if(states[k].percent < 1.0){
			allFinish = 0;
		}

		update_row_work(ids[k], states[k].percent, states[k].result, states[k].finish);
		if(states[k].active){
			update_row_active(ids[k]);	
		}
	}
	if(allFinish){

		return FALSE;
	}	
	return TRUE;
}

int activeTimer(){
	int timer = g_timeout_add (10, progress_timeout, NULL);
	return timer;
}


void stopTimer(int timer){
	g_source_remove(timer);
}


void add_row(int id_int, char *id_char){

	int bar_width = 30;
	int bar_length = 450;
	int result_length = 200;

	/*
	ID	
	*/

	char c[10];
	sprintf(c, "%d", id_int);
	GtkWidget * id = gtk_label_new (c);

	/*
	Progress bar
	*/
	GtkWidget *progress = gtk_progress_bar_new();
//	g_timeout_add((i + 1) * 100, inc_progress, progress);

	float percent = 0;
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress), percent);
	char d[3];
	sprintf(d, "%d%%", (int)(percent * 100));
	gtk_progress_bar_set_text(GTK_PROGRESS_BAR(progress), d);

	gtk_widget_set_size_request(progress, bar_length, bar_width);

	/*
	Status and Result
	*/
	GtkWidget * status = gtk_label_new ("INACTIVE");
	GtkWidget * result = gtk_label_new ("0");

	/*
	Creating scroll panel for result
	*/
	GtkWidget* halign = gtk_alignment_new(0, 0, 0, 1);
	gtk_container_add(GTK_CONTAINER(halign), result);
	GtkWidget* scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_container_border_width (GTK_CONTAINER (scroll), 0);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);	
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scroll), halign);
	gtk_widget_set_size_request(scroll, result_length, bar_width);

	/*
	Attaching row values to the table
	*/
	gtk_table_attach(GTK_TABLE(table), id, 0, 1, id_int, id_int + 1, GTK_FILL, GTK_FILL, 10, 10);
	gtk_table_attach(GTK_TABLE(table), progress, 1, 2, id_int, id_int + 1, GTK_FILL, GTK_FILL,0, 10);
	gtk_table_attach(GTK_TABLE(table), status, 2, 3, id_int, id_int + 1, GTK_FILL, GTK_FILL, 0, 10);
	gtk_table_attach(GTK_TABLE(table), scroll, 3, 4, id_int, id_int + 1, GTK_FILL, GTK_FILL, 0, 10);

	row_struct_t* thread;
	thread = malloc(sizeof(row_struct_t));

	thread->id_int = id_int;
	thread->id_char = id_char;
	thread->id = id;
	thread->progress = progress;
	thread->status = status;
	thread->result = result;

//	printf("%d", id_int);

//	char id_char[5];
//	sprintf(id_char, "%d", id_int);

//	printf("%s", id_char);

	int error = hashmap_put(mymap, id_char, thread);
        assert(error==MAP_OK);
}



/*
******************************
ADDING ROWS DYNAMICALLY FUNCTION
******************************
*/
void init_table(){

	/*
	setting spacings for columns and rows
	*/
	gtk_table_set_col_spacings(GTK_TABLE(table), 10);
	gtk_table_set_row_spacings(GTK_TABLE(table), 0);

	/*
	Creating table headers
	*/
	GtkWidget * id_header = gtk_label_new ("Thread ID");
	GtkWidget * progress_header = gtk_label_new ("Progress");
	GtkWidget * status_header = gtk_label_new ("Status");
	GtkWidget * result_header = gtk_label_new ("Result");

	PangoFontDescription *df;
	df = pango_font_description_from_string("Monospace");
	pango_font_description_set_size(df, 20 * PANGO_SCALE);

	gtk_widget_modify_font(id_header, df);
	gtk_widget_modify_font(progress_header, df);
	gtk_widget_modify_font(status_header, df);
	gtk_widget_modify_font(result_header, df);

	gtk_table_attach(GTK_TABLE(table), id_header, 0, 1, 0, 1, GTK_FILL, GTK_FILL, 10, 10);
	gtk_table_attach(GTK_TABLE(table), progress_header, 1, 2, 0, 1, GTK_FILL, GTK_FILL, 0, 10);
	gtk_table_attach(GTK_TABLE(table), status_header, 2, 3, 0, 1, GTK_FILL, GTK_FILL, 0, 10);
	gtk_table_attach(GTK_TABLE(table), result_header, 3, 4, 0, 1, GTK_FILL, GTK_FILL, 0, 10);

}


void create_UI(int n_threads){

	mymap = hashmap_new();

	window = gtk_dialog_new ();

	table = gtk_table_new(n_threads, 4, FALSE);
		
	gtk_window_set_title(GTK_WINDOW(window), "Lottery Scheduler");
	gtk_window_set_default_size(GTK_WINDOW(window), 1000, 605);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);


	/*
	Creating main app title
	*/
	GtkWidget* app_title = gtk_label_new ("Thread Status Monitor");
	PangoFontDescription *df;
	df = pango_font_description_from_string("Monospace");
	pango_font_description_set_size(df, 30 * PANGO_SCALE);
	gtk_widget_modify_font(app_title, df);

	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(window)->vbox), app_title, FALSE, FALSE, 15);


	/*
	Creating info table
	*/
	GtkWidget* table_info = gtk_table_new(3, 2, TRUE);

	gtk_table_set_col_spacings(GTK_TABLE(table_info), 5);
	gtk_table_set_row_spacings(GTK_TABLE(table_info), 5);

	GtkWidget* label_info_name1 = gtk_label_new ("Threads Submitted:");
	GtkWidget* label_info_name2 = gtk_label_new ("Threads Running:");
	GtkWidget* label_info_name3 = gtk_label_new ("Threads Completed:");

	GtkWidget* label_info_value1 = gtk_label_new ("30");
	GtkWidget* label_info_value2 = gtk_label_new ("8");
	GtkWidget* label_info_value3 = gtk_label_new ("14");

	PangoFontDescription *df_info;
	df_info = pango_font_description_from_string("Monospace");
	pango_font_description_set_size(df_info, 15 * PANGO_SCALE);

	gtk_widget_modify_font(label_info_name1, df_info);
	gtk_widget_modify_font(label_info_name2, df_info);
	gtk_widget_modify_font(label_info_name3, df_info);
	gtk_widget_modify_font(label_info_value1, df_info);
	gtk_widget_modify_font(label_info_value2, df_info);
	gtk_widget_modify_font(label_info_value3, df_info);

	GtkWidget* halign_info_name1 = gtk_alignment_new(0, 0, 0, 1);
	GtkWidget* halign_info_name2 = gtk_alignment_new(0, 0, 0, 1);
	GtkWidget* halign_info_name3 = gtk_alignment_new(0, 0, 0, 1);
	GtkWidget* halign_info_value1 = gtk_alignment_new(0, 0, 0, 1);
	GtkWidget* halign_info_value2 = gtk_alignment_new(0, 0, 0, 1);
	GtkWidget* halign_info_value3 = gtk_alignment_new(0, 0, 0, 1);

	gtk_container_add(GTK_CONTAINER(halign_info_name1), label_info_name1);
	gtk_container_add(GTK_CONTAINER(halign_info_name2), label_info_name2);
	gtk_container_add(GTK_CONTAINER(halign_info_name3), label_info_name3);
	gtk_container_add(GTK_CONTAINER(halign_info_value1), label_info_value1);
	gtk_container_add(GTK_CONTAINER(halign_info_value2), label_info_value2);
	gtk_container_add(GTK_CONTAINER(halign_info_value3), label_info_value3);

	gtk_table_attach_defaults(GTK_TABLE(table_info), halign_info_name1, 0, 1, 0, 1 );
	gtk_table_attach_defaults(GTK_TABLE(table_info), halign_info_name2, 0, 1, 1, 2 );
	gtk_table_attach_defaults(GTK_TABLE(table_info), halign_info_name3, 0, 1, 2, 3 );

	gtk_table_attach_defaults(GTK_TABLE(table_info), halign_info_value1, 1, 2, 0, 1 );
	gtk_table_attach_defaults(GTK_TABLE(table_info), halign_info_value2, 1, 2, 1, 2 );
	gtk_table_attach_defaults(GTK_TABLE(table_info), halign_info_value3, 1, 2, 2, 3 );

		
	/*
	Attaching info table to main pannel
	*/
	GtkWidget* hbox_table_info = gtk_hbox_new(TRUE, 1);

	gtk_box_pack_start (GTK_BOX (hbox_table_info), table_info, FALSE, FALSE, 10);

	GtkWidget* halign_table_info = gtk_alignment_new(0, 0, 0, 0);
		
	gtk_container_add(GTK_CONTAINER(halign_table_info), hbox_table_info);

	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(window)->vbox), halign_table_info, FALSE, FALSE, 15);


	/*
	Creating scroll panel for progress table
	*/
	GtkWidget *scrolled_window = gtk_scrolled_window_new (NULL, NULL);
	    
	gtk_container_border_width (GTK_CONTAINER (scrolled_window), 10);
	    
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(window)->vbox), scrolled_window, TRUE, TRUE, 0);


	/*
	Creating progress table
	*/
//	GtkWidget *table = gtk_table_new(n_threads, 4, FALSE);

	/*
	Attaching progress table to scroll panel
	*/
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), table);



	/*
	init the table
	*/
	init_table();



	/*
	Creating Close Button
	*/

	GtkWidget* button = gtk_button_new_with_label("Close");

	g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);

	GtkWidget* hbox_button = gtk_hbox_new(TRUE, 1);

	gtk_box_pack_start (GTK_BOX (hbox_button), button, FALSE, FALSE, 10);

	GtkWidget* halign_button = gtk_alignment_new(1, 0, 0, 0);

	gtk_container_add(GTK_CONTAINER(halign_button), hbox_button);

	gtk_box_pack_start (GTK_BOX (GTK_DIALOG(window)->vbox), halign_button, FALSE, FALSE, 0);


	/*
	Adding quit buttom event
	*/
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void show_ui(){
	gtk_widget_show_all(window);
}


/*
******************************
MAIN FUNCTION
******************************
*/
/*
int main(int argc, char *argv[]) {

	int n_threads = 30;

	gtk_init(NULL, NULL);


	
	//Creating main window
	create_UI(n_threads);

	//Showing all widgets
	show_ui();

	//Starting main gtk-loop
	gtk_main();

	return 0;
}*/
