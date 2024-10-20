#include "GtkFileDialog.h"
#include <gtk/gtk.h>
#include <iostream>

GtkFileDialog::GtkFileDialog() {
    initializeGtk();
}

GtkFileDialog::~GtkFileDialog() {
    // GTK will clean up resources upon program exit, no need to explicitly call any cleanup.
}

void GtkFileDialog::initializeGtk() {
    gtk_init(0, nullptr);  // Initialize GTK
}

std::string GtkFileDialog::openFileDialog() {
    GtkWidget *dialog;
    GtkFileChooser *chooser;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
    gint res;

    dialog = gtk_file_chooser_dialog_new("Open Media File", nullptr, action,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Open", GTK_RESPONSE_ACCEPT, NULL);

    res = gtk_dialog_run(GTK_DIALOG(dialog));
    std::string file_path = "";
    if (res == GTK_RESPONSE_ACCEPT) {
        chooser = GTK_FILE_CHOOSER(dialog);
        char *filename = gtk_file_chooser_get_filename(chooser);
        file_path = std::string(filename);
        g_free(filename);  // Free the memory used by GTK for the filename
    }

    gtk_widget_destroy(dialog);  // Destroy the dialog after use
    while (g_main_context_iteration(nullptr, false));  // Process GTK events

    return file_path;
}
