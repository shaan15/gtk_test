#include <gtk/gtk.h>
#include <glib.h>

GTimer *time_row;
unsigned long msecs;

static GtkWidget * 
create_row (const gchar *text)
{
  GtkWidget *row, *label, *button;
  row = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  label = gtk_label_new (text);
  gtk_container_add (GTK_CONTAINER (row), label);
  return row;
}

static void 
next_page (GtkWidget *app)
{
  time_row=g_timer_new();
  GtkWidget *list,*row,*s;
  gchar *text;
  int status;
  //gtk_init (NULL, NULL);

  //window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  //gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);

  list = gtk_list_box_new ();
  gtk_list_box_set_selection_mode (GTK_LIST_BOX (list), GTK_SELECTION_NONE);

  gint i;
  g_timer_reset (time_row);
  g_timer_start (time_row);
  for (i=1;i<=2000;i++)
  {
    text=g_strdup_printf("Row %d",i);
    row=create_row (text);
    //gtk_widget_show (row);
    gtk_list_box_insert (GTK_LIST_BOX (list),row,-1);
  }
  //gtk_widget_show (list);
  gtk_container_add (GTK_CONTAINER (app),list);
  g_timer_stop (time_row);
  printf ("Time to add row: %lf\n",g_timer_elapsed (time_row,&msecs));
  gtk_widget_show (app);

  gtk_main ();
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);


  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (next_page), &window);
  gtk_container_add (GTK_CONTAINER (button_box), button);
  gtk_container_add (GTK_CONTAINER (window), button_box);
  gtk_widget_show_all (window);
}

int main (int argc, 
          char **argv)
{

  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;

}



