#include <gtk/gtk.h>
#include <glib.h>

GTimer *time_row;
unsigned long msecs;

static GtkWidget *create_row (const gchar *label);

static void row_hidden (GObject *revealer, GParamSpec *pspec, gpointer data)
{
  GtkWidget *row, *list;
  list = gtk_widget_get_parent (row);
  row = gtk_widget_get_parent (GTK_WIDGET (revealer));
  gtk_container_remove (GTK_CONTAINER (list), row);
}

static void row_displayed (GObject *revealer, GParamSpec *pspec, gpointer data)
{
  GtkWidget *row, *newrow;
  row = gtk_widget_get_parent (GTK_WIDGET (revealer));
  newrow = gtk_bin_get_child (GTK_BIN (revealer));
  g_object_ref (newrow);
  gtk_container_remove (GTK_CONTAINER (revealer), newrow);
  gtk_widget_destroy (GTK_WIDGET (revealer));
  gtk_container_add (GTK_CONTAINER (row), newrow);
  g_object_unref (newrow);
}

static GtkWidget * add_row (GtkWidget *button, GtkWidget *newrow)
{  
  GtkWidget *revealer, *row, *list;
  gint i;
  row = gtk_widget_get_parent (newrow);
  i = gtk_list_box_row_get_index (GTK_LIST_BOX_ROW (row));
  list = gtk_widget_get_parent (row);
  row = create_row ("New row");
  revealer = gtk_revealer_new ();
  gtk_container_add (GTK_CONTAINER (revealer), row);
  gtk_widget_show_all (revealer);
  g_signal_connect (revealer, "notify::child-revealed",G_CALLBACK (row_displayed), NULL);
  gtk_list_box_insert (GTK_LIST_BOX (list), revealer, i++);
  gtk_revealer_set_reveal_child (GTK_REVEALER (revealer), TRUE);
  
}

static void remove_row (GtkButton *button, GtkWidget *newrow)
{
  GtkWidget *row, *revealer;
  row = gtk_widget_get_parent (newrow);
  revealer = gtk_revealer_new ();
  gtk_revealer_set_reveal_child(GTK_REVEALER (revealer), TRUE);
  gtk_widget_show (revealer);
  G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  gtk_widget_reparent (newrow, revealer);
  G_GNUC_END_IGNORE_DEPRECATIONS
  gtk_container_add (GTK_CONTAINER(row), revealer);
  g_signal_connect (revealer, "notify::child-revealed",G_CALLBACK (row_hidden), NULL);
  gtk_revealer_set_reveal_child(GTK_REVEALER (revealer), FALSE);
}

static GtkWidget * create_row (const gchar *text)
{
  GtkWidget *row, *label, *button;
  row = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  label = gtk_label_new (text);
  gtk_container_add (GTK_CONTAINER (row), label);
  button = gtk_button_new_with_label ("Remove");
  gtk_widget_set_hexpand (button, TRUE);
  gtk_widget_set_valign (button, GTK_ALIGN_CENTER);
  gtk_widget_set_halign (button, GTK_ALIGN_END);
  gtk_container_add (GTK_CONTAINER (row), button);
  g_signal_connect (button, "clicked", G_CALLBACK (remove_row), row);
  button = gtk_button_new_with_label ("Add");
  gtk_widget_set_valign (button, GTK_ALIGN_CENTER);
  gtk_container_add (GTK_CONTAINER (row), button);
  g_signal_connect (button, "clicked", G_CALLBACK (add_row), row);
  return row;
}

static void add_separator (GtkListBoxRow *row, GtkListBoxRow *before, gpointer data)
{
  if (!before)
    return;
  gtk_list_box_row_set_header (row, gtk_separator_new (GTK_ORIENTATION_HORIZONTAL));
}



int main (int argc, char **argv)
{
  time_row = g_timer_new ();
  GtkApplication *app;
  GtkWidget *list, *row, *s, *window;
  gchar *text;
  int status;
  gtk_init (NULL, NULL);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size (GTK_WINDOW (window), 300, 300);

  list = gtk_list_box_new ();
  gtk_list_box_set_selection_mode (GTK_LIST_BOX (list), GTK_SELECTION_NONE);
  gtk_list_box_set_header_func (GTK_LIST_BOX (list), add_separator, NULL, NULL);
  s = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_add (GTK_CONTAINER (window), s);
  gtk_container_add (GTK_CONTAINER (s), list);

  gint i;
  g_timer_reset (time_row);
  g_timer_start (time_row);
  for (i=1;i<=2000;i++)
    {
      text = g_strdup_printf ("Row %d",i);
      row = create_row (text);
      gtk_list_box_insert (GTK_LIST_BOX (list),row,-1);
    }
  g_timer_stop (time_row);
  printf ("Time to add row: %lf\n",g_timer_elapsed (time_row, &msecs));
  gtk_widget_show_all (window);
  gtk_main ();

  return 0;
}
