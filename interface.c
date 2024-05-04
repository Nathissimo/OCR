#include <gtk/gtk.h>
#include "main.h"

//interface
typedef struct Inter
{
    GtkWindow* window;              // Main window
    GtkButton* Bchose_image;       
    GtkButton* Bsolve;       
    GtkButton* Bcolor;       
    GtkButton* Bgrey;       
    GtkButton* Bblur;       
    GtkButton* Bdil_err;       
    GtkButton* Bsobel;       
    GtkButton* Bbin;       
    GtkButton* Bann;       
    GtkButton* Blig;       
    GtkButton* Bsudo;
    GtkImage* Gimage;
    int usless; //this is usless
} Inter;
//for the chose buttom
void on_fchose_image(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	//active another button
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bsolve),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bcolor),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bgrey),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bblur),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bdil_err),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bsobel),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bbin),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bann),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Blig),TRUE);
	gtk_widget_set_sensitive(GTK_WIDGET(inter->Bsudo),TRUE);
	//creat dialog windows
	GtkWidget *dialog;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;
	gint res;

	dialog = gtk_file_chooser_dialog_new ("Open File",
                                      NULL,
                                      action,
                                      ("_Cancel"),
                                      GTK_RESPONSE_CANCEL,
                                      ("_Open"),
                                      GTK_RESPONSE_ACCEPT,
                                      NULL);

	res = gtk_dialog_run (GTK_DIALOG (dialog));
	if (res == GTK_RESPONSE_ACCEPT)
  	{
    		char *filename;
    		GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
    		filename = gtk_file_chooser_get_filename (chooser);
		main_ocr(filename);
		GdkPixbuf *pix = gdk_pixbuf_new_from_file(filename,NULL);
		if(pix!=NULL)
		{
			gtk_image_set_from_pixbuf(inter->Gimage,pix);
		}
    		g_free (filename);
  	}

	gtk_widget_destroy (dialog);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fsolve(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"sudoku_result.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fcolor(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/colored.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fgrey(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/grayscale.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fblur(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/blur.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fdil_err(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/dilero.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fsobel(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/sobel.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fbin(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/binarization.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fann(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/analyse.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_flig(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/lines.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}
}

void on_fsudo(GtkButton *button, gpointer user_data)
{
	Inter* inter =user_data;
	GdkPixbuf *pix = gdk_pixbuf_new_from_file(
			"temp_files/sudoku.png",NULL);
	gtk_image_set_from_pixbuf(inter->Gimage,pix);
	if(button!=NULL)
	{
		inter->usless=1;
	}

}

// Main function.                                                               
int main ()                                               
{                                                                               
     // Initializes GTK.                                                         
     gtk_init(NULL, NULL);                                                       
                                                                                 
     // Loads the UI description and builds the UI.                              
     // (Exits if an error occurs.)                                              
     GtkBuilder* builder = gtk_builder_new();                                    
     GError* error = NULL;                                                       
     if (gtk_builder_add_from_file(builder, "solve.glade", &error) == 0)         
     {                                                                           
         g_printerr("Error loading file: %s\n", error->message);                 
         g_clear_error(&error);                                                  
         return 1;                                                               
     }                                                                           
                                                                                 
     // Gets the widgets.                                                        
     GtkWindow* window = 
	     GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.inter"));
     GtkButton* Bchose_image = 
	     GTK_BUTTON(gtk_builder_get_object(builder, "Bchose_image"));
     
     GtkButton* Bsolve = GTK_BUTTON(gtk_builder_get_object(builder, "Bsolve"));
     
     GtkButton* Bcolor = GTK_BUTTON(gtk_builder_get_object(builder, "Bcolor"));
     
     GtkButton* Bgrey = GTK_BUTTON(gtk_builder_get_object(builder, "Bgrey"));
     
     GtkButton* Bblur = GTK_BUTTON(gtk_builder_get_object(builder, "Bblur"));
     
     GtkButton* Bdil_err = 
	     GTK_BUTTON(gtk_builder_get_object(builder, "Bdil_err"));
     
     GtkButton* Bsobel = GTK_BUTTON(gtk_builder_get_object(builder, "Bsobel"));
     
     GtkButton* Bbin = GTK_BUTTON(gtk_builder_get_object(builder, "Bbin"));
     
     GtkButton* Bann = GTK_BUTTON(gtk_builder_get_object(builder, "Bann"));
     
     GtkButton* Blig = GTK_BUTTON(gtk_builder_get_object(builder, "Blig"));
     
     GtkButton* Bsudo = GTK_BUTTON(gtk_builder_get_object(builder, "Bsudo"));
     
     GtkImage* Gimage = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
     
     Inter inter=
     {
	     .Bsolve=Bsolve,
	     .Bcolor= Bcolor,
	     .Bgrey=Bgrey,
	     .Bblur=Bblur,
	     .Bdil_err=Bdil_err,
	     .Bsobel=Bsobel,
	     .Bbin=Bbin,
	     .Bann=Bann,
	     .Blig=Blig,
	     .Bsudo=Bsudo,
	     .Gimage=Gimage,
	     .usless=1
     };

     g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), &inter);
     g_signal_connect(Bchose_image, "clicked", 
		     G_CALLBACK(on_fchose_image), &inter);
     g_signal_connect(Bsolve, "clicked", G_CALLBACK(on_fsolve), &inter);
     g_signal_connect(Bcolor, "clicked", G_CALLBACK(on_fcolor), &inter);
     g_signal_connect(Bgrey, "clicked", G_CALLBACK(on_fgrey), &inter);
     g_signal_connect(Bblur, "clicked", G_CALLBACK(on_fblur), &inter);
     g_signal_connect(Bdil_err, "clicked", G_CALLBACK(on_fdil_err), &inter);
     g_signal_connect(Bsobel, "clicked", G_CALLBACK(on_fsobel), &inter);
     g_signal_connect(Bbin, "clicked", G_CALLBACK(on_fbin), &inter);
     g_signal_connect(Bann, "clicked", G_CALLBACK(on_fann), &inter);
     g_signal_connect(Blig, "clicked", G_CALLBACK(on_flig), &inter);
     g_signal_connect(Bsudo, "clicked", G_CALLBACK(on_fsudo), &inter);

     gtk_main();
     return 1;                                                             
 } 
