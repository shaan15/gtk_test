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
next_page (GtkApplication *app, 
           GtkBox *box)
{
  time_row=g_timer_new();
  GtkWidget *list,*row,*s,*window;
  gchar *text;
  int status;
  gtk_init (NULL, NULL);

  //window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  //gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);

  list = gtk_list_box_new ();
  gtk_list_box_set_selection_mode (GTK_LIST_BOX (list), GTK_SELECTION_NONE);
  gtk_box_pack_start (box, list, TRUE, TRUE, 300);

  //s = gtk_scrolled_window_new (NULL, NULL);
  //gtk_container_add (GTK_CONTAINER (app), s);
  //gtk_container_add (GTK_CONTAINER (app), box);

  gint i;
  g_timer_reset (time_row);
  g_timer_start (time_row);
  for (i=1;i<=2000;i++)
  {
    text=g_strdup_printf("Row %d",i);
    row=create_row (text);
    gtk_list_box_insert (GTK_LIST_BOX (list),row,-1);
  }
  g_timer_stop (time_row);
  printf ("Time to add row: %lf\n",g_timer_elapsed (time_row,&msecs));
  gtk_widget_show_all (box);
  gtk_main ();
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkBox *box;
  GtkWidget *button;
  GtkWidget *button_box;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);

  box=gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 300);

  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  

  gtk_box_pack_start (box, button_box, TRUE, TRUE, 300);
  button = gtk_button_new_with_label ("Hello World");
  g_signal_connect (button, "clicked", G_CALLBACK (next_page), &box);
  gtk_container_add (GTK_CONTAINER (button_box), button);
  //gtk_container_add (GTK_CONTAINER (window), box);
  gtk_widget_show_all (box);
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



