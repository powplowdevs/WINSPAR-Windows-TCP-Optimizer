#include <Includes/gtk/gtk.h>

// Function to handle the "destroy" event
void on_window_closed(GtkWidget* widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char* argv[]) {
    // Initialize GTK+
    gtk_init(&argc, &argv);

    // Create a new window
    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Hello, GTK+");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_closed), NULL);

    // Create a label
    GtkWidget* label = gtk_label_new("Hello, World!");

    // Add the label to the window
    gtk_container_add(GTK_CONTAINER(window), label);

    // Show the window and its contents
    gtk_widget_show_all(window);

    // Run the GTK+ main loop
    gtk_main();

    return 0;
}
