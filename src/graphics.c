#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[] ) {
    /* GtkWidget is the storage type for widgets */
   GtkWidget *wind;
	 GdkWindow *window;
	GdkWindowAttr *attr = calloc(1,sizeof(GdkWindowAttr));
	gpointer userdata;
   GdkGC *gc; 
    /* This is called in all GTK applications. Arguments are parsed
 *      * from the command line and are returned to the application. */
    gdk_init(&argc, &argv);
    
    /* create a new window */
    wind = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    /* Sets the border width of the window. */
    gtk_container_set_border_width (GTK_CONTAINER (wind), 100);
	attr->title = "hello";
	attr->window_type = GDK_WINDOW_CHILD;
	attr->width = 50;
	attr->height = 50;
	window = gdk_window_new(NULL,attr,3);

gdk_window_show(window);
	 gc = gdk_gc_new(window);
	gdk_draw_rectangle (window, gc, TRUE, 0, 0, 20, 20);
/* This packs the button into the window (a gtk container). */
    //gtk_container_add (GTK_CONTAINER (window), rect);
    
    /* The final step is to display this newly created widget. */
    
    /* and the window */
    gtk_widget_show (wind);
    
    /* All GTK applications must have a gtk_main(). Control ends here
 *      * and waits for an event to occur (like a key press or
 *           * mouse event). */
    gtk_main ();
    
    return(0);
}    


