#include "backup_gtk_ui.h"

int main(int argc, char * argv[]){

	int nThreads = 5;
	
	gtk_init(NULL,NULL);
	
	create_UI(nThreads);

	int i = 1;
	while(i <= nThreads){
		add_row(i);
		i++;
	}

	show_ui();

	gtk_main();

	return 0;
}
