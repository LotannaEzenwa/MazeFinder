#include <cairo-svg.h>
#include <gtk/gtk.h>

int main(int argc,char *argv[] ) {
    /* GtkWidget is the storage type for widgets */
    GtkWidget *window;
    GtkWidget *rect;
    GtkPixmap *pixMap;
    
    /* This is called in all GTK applications. Arguments are parsed
 *      * from the command line and are returned to the application. */
    gtk_init(&argc, &argv);
    
    /* create a new window */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    /* Sets the border width of the window. */
    gtk_container_set_border_width (GTK_CONTAINER (window), 100);
    
    /* Creates a new button with the label "Hello World". */
    gtk_draw_rectangle(pixMap,rect,5,5,10,10);
    
/* This packs the button into the window (a gtk container). */
    gtk_container_add (GTK_CONTAINER (window), rect);
    
    /* The final step is to display this newly created widget. */
    gtk_widget_show (rect);
    
    /* and the window */
    gtk_widget_show (window);
    
    /* All GTK applications must have a gtk_main(). Control ends here
 *      * and waits for an event to occur (like a key press or
 *           * mouse event). */
    gtk_main ();
    
    return(0);
}    


