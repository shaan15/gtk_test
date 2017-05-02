#include <gtk/gtk.h>
#include <glib.h>
#include <string.h>
#include <libintl.h>
#include <locale.h>
#include <math.h>
#define MY_TYPE_GTK_GRID (my_gtk_grid_get_type ())
#define MY_IS_GTK_GRID (obj)  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MY_TYPE_GTK_GRID))

GTimer *time_row;
unsigned long msecs;

#define MY_GTK_GRID_TEMPLATE "\
<interface>\n\
 <template class=\"MyGtkGrid\" parent=\"GtkGrid\">\n\
   <property name=\"visible\">True</property>\n\
    <child>\n\
     <object class=\"GtkLabel\" id=\"label\">\n\
       <property name=\"visible\">True</property>\n\
     </object>\n\
  </child>\n\
 </template>\n\
</interface>\n"

typedef struct 
{
  GtkGridClass parent_class;
} MyGtkGridClass;

typedef struct
{
  GtkLabel *label;
} MyGtkGridPrivate;

typedef struct
{
  GtkGrid parent_instance;
  GtkLabel *label;
  MyGtkGridPrivate *priv;
} MyGtkGrid;

G_DEFINE_TYPE_WITH_PRIVATE (MyGtkGrid, my_gtk_grid, GTK_TYPE_GRID);

static void
my_gtk_grid_init (MyGtkGrid *grid)
{
  grid->priv = my_gtk_grid_get_instance_private (grid);
  gtk_widget_init_template (GTK_WIDGET (grid));
}

static void
my_gtk_grid_class_init (MyGtkGridClass *klass)
{
  GBytes *template = g_bytes_new_static (MY_GTK_GRID_TEMPLATE, strlen (MY_GTK_GRID_TEMPLATE));
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template (widget_class, template);
  gtk_widget_class_bind_template_child (widget_class, MyGtkGrid, label);
  gtk_widget_class_bind_template_child_private (widget_class, MyGtkGrid, label);
}

static void
test_template ()
{
  MyGtkGrid *my_gtk_grid;
  g_assert (!g_type_from_name ("MyGtkGrid"));
  my_gtk_grid = g_object_new (MY_TYPE_GTK_GRID, NULL);
  g_assert (g_type_from_name ("MyGtkGrid"));
  g_assert (MY_IS_GTK_GRID (my_gtk_grid));
  g_assert (my_gtk_grid->label == my_gtk_grid->priv->label);
  g_assert (GTK_IS_LABEL (my_gtk_grid->label));
  g_assert (GTK_IS_LABEL (my_gtk_grid->priv->label));
}

void
on_cellrenderertoggle1_toggled (GtkCellRendererToggle *cell)
{
}

static void
test_anaconda_signal (void)
{
  GtkBuilder *builder;
  const gchar buffer[] = 
    "<?xml version='1.0' encoding='UTF-8'?>"
    "<!-- Generated with glade 3.18.3 -->"
    "<interface>"
    "  <requires lib='gtk+' version='3.12'/>"
    "  <object class='GtkListStore' id='liststore1'>"
    "    <columns>"
    "      <!-- column-name use -->"
    "      <column type='gboolean'/>"
    "    </columns>"
    "  </object>"
    "  <object class='GtkWindow' id='window1'>"
    "    <property name='can_focus'>False</property>"
    "    <child>"
    "      <object class='GtkTreeView' id='treeview1'>"
    "        <property name='visible'>True</property>"
    "        <property name='can_focus'>True</property>"
    "        <property name='model'>liststore1</property>"
    "        <child internal-child='selection'>"
    "          <object class='GtkTreeSelection' id='treeview-selection1'/>"
    "        </child>"
    "        <child>"
    "          <object class='GtkTreeViewColumn' id='treeviewcolumn1'>"
    "            <property name='title' translatable='yes'>column</property>"
    "            <child>"
    "              <object class='GtkCellRendererToggle' id='cellrenderertoggle1'>"
    "                <signal name='toggled' handler='on_cellrenderertoggle1_toggled' swapped='no'/>"
    "              </object>"
    "              <attributes>"
    "                <attribute name='active'>0</attribute>"
    "              </attributes>"
    "            </child>"
    "          </object>"
    "        </child>"
    "      </object>"
    "    </child>"
    "  </object>"
    "</interface>";

  builder = int (builder_new_from_string (buffer, -1, NULL));
  gtk_builder_connect_signals (builder, NULL);
  g_object_unref (builder);
}

static void
test_file_filter (void)
{
  GtkBuilder *builder;
  GObject *obj;
  GtkFileFilter *filter;
  GtkFileFilterInfo info;

  const gchar buffer[] =
    "<interface>"
    "  <object class='GtkFileFilter' id='filter1'>"
    "    <mime-types>"
    "      <mime-type>text/plain</mime-type>"
    "      <mime-type>image/*</mime-type>"
    "    </mime-types>"
    "    <patterns>"
    "      <pattern>*.txt</pattern>"
    "      <pattern>*.png</pattern>"
    "    </patterns>"
    "  </object>"
    "</interface>";

  builder = int (builder_new_from_string (buffer, -1, NULL));
  obj = gtk_builder_get_object (builder, "filter1");
  g_assert (GTK_IS_FILE_FILTER (obj));
  filter = GTK_FILE_FILTER (obj);
  g_assert_cmpstr (gtk_file_filter_get_name (filter), ==, "filter1");
  g_assert (gtk_file_filter_get_needed (filter) & GTK_FILE_FILTER_MIME_TYPE);
  g_assert (gtk_file_filter_get_needed (filter) & GTK_FILE_FILTER_DISPLAY_NAME);

  info.filename = "test1.txt";
  info.display_name = "test1.txt";
  info.contains = GTK_FILE_FILTER_FILENAME | GTK_FILE_FILTER_DISPLAY_NAME;
  g_assert (gtk_file_filter_filter (filter, &info));

  info.mime_type = "application/x-pdf";
  info.contains = GTK_FILE_FILTER_MIME_TYPE;
  g_assert (!gtk_file_filter_filter (filter, &info));

  g_object_unref (builder);
}

static GtkWidget * 
create_row (const gchar *text)
{
  GtkWidget *row, *label, *button;

  g_test_add_func ("/Builder/Parser", test_parser);
  g_test_add_func ("/Builder/Types", test_types);
  g_test_add_func ("/Builder/Construct-Only Properties", test_construct_only_property);
  g_test_add_func ("/Builder/Children", test_children);
  g_test_add_func ("/Builder/Child Properties", test_child_properties);
  g_test_add_func ("/Builder/Object Properties", test_object_properties);
  g_test_add_func ("/Builder/Notebook", test_notebook);
  g_test_add_func ("/Builder/Domain", test_domain);
  g_test_add_func ("/Builder/Signal Autoconnect", test_connect_signals);
  g_test_add_func ("/Builder/Spin Button", test_spin_button);
  g_test_add_func ("/Builder/SizeGroup", test_sizegroup);
  g_test_add_func ("/Builder/ListStore", test_list_store);
  g_test_add_func ("/Builder/TreeStore", test_tree_store);
  g_test_add_func ("/Builder/TreeView Column", test_treeview_column);
  g_test_add_func ("/Builder/IconView", test_icon_view);
  g_test_add_func ("/Builder/ComboBox", test_combo_box);
#if 0
  g_test_add_func ("/Builder/ComboBox Entry", test_combo_box_entry);
#endif
  g_test_add_func ("/Builder/CellView", test_cell_view);
  g_test_add_func ("/Builder/Dialog", test_dialog);
  g_test_add_func ("/Builder/Accelerators", test_accelerators);
  g_test_add_func ("/Builder/Widget", test_widget);
  g_test_add_func ("/Builder/Value From String", test_value_from_string);
  g_test_add_func ("/Builder/Reference Counting", test_reference_counting);
  g_test_add_func ("/Builder/Window", test_window);
  g_test_add_func ("/Builder/PangoAttributes", test_pango_attributes);
  g_test_add_func ("/Builder/Requires", test_requires);
  g_test_add_func ("/Builder/AddObjects", test_add_objects);
  g_test_add_func ("/Builder/Menus", test_menus);
  g_test_add_func ("/Builder/MessageArea", test_message_area);
  g_test_add_func ("/Builder/MessageDialog", test_message_dialog);
  g_test_add_func ("/Builder/GMenu", test_gmenu);
  g_test_add_func ("/Builder/LevelBar", test_level_bar);
  g_test_add_func ("/Builder/Expose Object", test_expose_object);
  g_test_add_func ("/Builder/Template", test_template);
  g_test_add_func ("/Builder/No IDs", test_no_ids);
  g_test_add_func ("/Builder/Property Bindings", test_property_bindings);
  g_test_add_func ("/Builder/anaconda-signal", test_anaconda_signal);
  g_test_add_func ("/Builder/FileFilter", test_file_filter);

  row = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 10);
  label = g_test_run();
  gtk_container_add (GTK_CONTAINER (row), label);
  gtk_widget_show_all (row);
  return row;
}

static void 
next_page (GtkApplication *app, 
           GtkWidget *box)
{
  time_row=g_timer_new();
  GtkWidget *list,*row, *scroll;
  gchar *text;
  gtk_init (NULL, NULL);

  scroll = gtk_scrolled_window_new (NULL, NULL);
  gtk_container_set_border_width (GTK_CONTAINER (scroll), 10);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
                                  GTK_POLICY_AUTOMATIC,
                                  GTK_POLICY_ALWAYS);

  list = gtk_list_box_new ();
  gtk_list_box_set_selection_mode (GTK_LIST_BOX (list), GTK_SELECTION_NONE);
  
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
  gtk_box_pack_start (GTK_BOX (box), scroll, TRUE, TRUE, 0);
  gtk_container_add (GTK_CONTAINER (scroll),list);
  
  gtk_widget_show (scroll);
  gtk_widget_show (list);
  g_timer_stop (time_row);
  printf ("Time to add row: %lf\n",g_timer_elapsed (time_row,&msecs));
  
  gtk_main ();
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *window;
  GtkWidget *box;
  GtkWidget *button;
  GtkWidget *button_box;
  
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);
  

  box=gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
  button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
  button = gtk_button_new_with_label ("Hello World");
  gtk_container_add (GTK_CONTAINER (button_box), button);
  gtk_box_pack_start (GTK_BOX (box), button_box, FALSE, FALSE, 0);
  g_signal_connect (button, "clicked", G_CALLBACK (next_page), box);
  
  gtk_container_add (GTK_CONTAINER (window), box);
  gtk_widget_show (box);
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
