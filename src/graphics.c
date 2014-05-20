int main (int argc, char *argv[]) {
	GtkWidget *window;
	printf("begin ");
	gtk_init(&argc,&argv);
	printf("after init ");
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_main();
	int MazePort = atoi(argv[1]);
	int MazeWidth = atoi(argv[2]);
	int MazeHeight = atoi(argv[3]);
	parselog(MazePort,MazeWidth,MazeHeight);
	return(0);
}

