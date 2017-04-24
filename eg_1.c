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
  gtk_widget_show_all(row);
  return row;
}

static void 
next_page (GtkApplication *app, 
           GtkBox *box)
{
  time_row=g_timer_new();
  GtkWidget *list,*row;
  gchar *text;
  gtk_init (NULL, NULL);

  list = gtk_list_box_new ();
  gtk_list_box_set_selection_mode (GTK_LIST_BOX (list), GTK_SELECTION_NONE);
  gtk_box_pack_start (box, list, TRUE, TRUE, 300);

  gint i;
  g_timer_reset (time_row);
  g_timer_start (time_row);
  for (i=1;i<=2000;i++)
  {
    text=g_strdup_printf("Row %d",i);
    row=create_row (text);
    gtk_list_box_insert (GTK_LIST_BOX (list),row,-1);
    gtk_widget_show (row);
  }
  gtk_widget_show (list);
  gtk_container_add (GTK_CONTAINER (box),list);
  g_timer_stop (time_row);
  printf ("Time to add row: %lf\n",g_timer_elapsed (time_row,&msecs));
  //gtk_widget_show (box);

  gtk_main ();
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  //static GtkWidget *window;
  GtkWidget *box;
  GtkWidget *button;
  GtkWidget *button_box;
  GtkWidget *scroll;
  
  //window = gtk_dialog_new ();

  scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (scroll), 10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);

  box=gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 300);
  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  button = gtk_button_new_with_label ("Hello World");
  gtk_container_add (GTK_CONTAINER (button_box), button);
  gtk_box_pack_start (GTK_BOX (box), button_box, TRUE, TRUE, 300);
  gtk_container_add (GTK_CONTAINER (scroll), box);
  //gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scroll), box);
  g_signal_connect (button, "clicked", G_CALLBACK (next_page), box);
  
  //gtk_container_add (GTK_CONTAINER (window), scroll);
   //gtk_box_pack_start (GTK_DIALOG(window), scroll, TRUE, TRUE, 0);
  gtk_widget_show (scroll);
  gtk_widget_show (box);
  //gtk_widget_show (window);
  
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
