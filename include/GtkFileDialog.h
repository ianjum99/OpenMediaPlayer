#ifndef GTK_FILE_DIALOG_H
#define GTK_FILE_DIALOG_H

#include <string>

class GtkFileDialog {
public:
    GtkFileDialog();
    ~GtkFileDialog();
    std::string openFileDialog(); // Function to open a file dialog and return the selected file path

private:
    void initializeGtk();  // Function to initialize GTK
};

#endif // GTK_FILE_DIALOG_H
