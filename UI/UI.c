#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <gtk/gtkx.h>
#include <math.h>
#include <ctype.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
//#include "../ValueGrabber/fetcher.c"

GtkWidget *window;//we set global variables

GtkWidget *quit_button;

GtkWidget *btc_graph_button;
GtkWidget *eth_graph_button;
GtkWidget *doge_graph_button;

GtkWidget *buy_button;
GtkWidget *sell_button;
GtkWidget *value_entry;

GtkWidget *btc_possess;
GtkWidget *eth_possess;
GtkWidget *doge_possess;

GtkWidget *value_label;

GtkBuilder *builder;


int main()
{
  gtk_init(NULL,NULL); //we initialize the interface

  builder = gtk_builder_new_from_file("UI.glade");

  window = GTK_WIDGET(gtk_builder_get_object(builder,"window")); //we pick up all the widget boxes


 g_signal_connect(window,"destroy", G_CALLBACK(gtk_main_quit), NULL);

 gtk_builder_connect_signals(builder,NULL);


 //TODO
 quit_button = GTK_WIDGET(gtk_builder_get_object(builder,"quit_button"));
 btc_graph_button = GTK_WIDGET(gtk_builder_get_object(builder,"btc_graph_button"));
 eth_graph_button = GTK_WIDGET(gtk_builder_get_object(builder,"eth_graph_button"));
 doge_graph_button = GTK_WIDGET(gtk_builder_get_object(builder,"doge_graph_button"));
 buy_button = GTK_WIDGET(gtk_builder_get_object(builder,"buy_button"));
 sell_button = GTK_WIDGET(gtk_builder_get_object(builder,"sell_button"));
 
 value_entry = GTK_WIDGET(gtk_builder_get_object(builder,"value_entry"));

 btc_possess = GTK_WIDGET(gtk_builder_get_object(builder,"btc_possess"));

 eth_possess= GTK_WIDGET(gtk_builder_get_object(builder,"eth_possess"));

 doge_possess= GTK_WIDGET(gtk_builder_get_object(builder,"doge_possess"));

 value_label= GTK_WIDGET(gtk_builder_get_object(builder,"value_label"));
 // permet de directement montrer la valeur du BTC
 gtk_label_set_text(GTK_LABEL(value_label), (gchar*)"1 BTC = xxx$");
 
 gtk_widget_show(window);

 gtk_main();


 return EXIT_SUCCESS;
}

void on_quit_button_clicked()
{
  gtk_main_quit();
}


void on_btc_graph_button_toggled()
{
  gtk_label_set_text(GTK_LABEL(value_label), (gchar*)"1 BTC = xxx$");
}

void on_eth_graph_button_toggled()
{
  gtk_label_set_text(GTK_LABEL(value_label), (gchar*)"1 ETH = xxx$");
}  

void on_doge_graph_button_toggled()
{
  gtk_label_set_text(GTK_LABEL(value_label), (gchar*)"1 DOGE = xxx$");
} 

void on_buy_button_clicked();

void on_sell_button_clicked();

void on_value_entry();

void on_btc_possess();

void on_eth_possess();

void on_doge_possess();

void on_value_label();




