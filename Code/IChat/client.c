/*********************************
 * Client.c                    *
 * Function:                   *
 * create the chat window        *
 *********************************/

#include "client.h"
#include <gtk/gtk.h>

int issucceed=-1;
GtkTextBuffer *show_buffer,*input_buffer;
void get_ip(GtkWidget *,gpointer);
void quit_win(GtkWidget *,gpointer);

int main(int argc,char **argv)
{
GtkWidget *window;
GtkWidget *show_text,*input_text,*ip_text;
GtkWidget *ip_label,*space_label;
GtkWidget *link_button,*send_button,*quit_button,*cls_button;
GtkWidget *hbox,*vbox;
GtkWidget *scrolled1,*scrolled2;

gtk_init(&argc,&argv);
window=gtk_window_new(GTK_WINDOW_TOPLEVEL);

gtk_window_set_title(GTK_WINDOW(window),"Client");
gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
gtk_window_set_default_size(GTK_WINDOW(window),430,320);

/* "quit" button */
//g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(quit_win),NULL);
g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(quit_win),NULL);

ip_label=gtk_label_new("IP:");
space_label=gtk_label_new("");

/* set button */
link_button=gtk_button_new_with_label("Connect");
send_button=gtk_button_new_with_label("Send");
quit_button=gtk_button_new_with_label("Close");
cls_button=gtk_button_new_with_label("Clear");
/* set textbox */
ip_text=gtk_entry_new();
show_text=gtk_text_view_new();
input_text=gtk_text_view_new();

/* set length of IP box */
gtk_entry_set_max_length(GTK_ENTRY(ip_text),15);

/* get the buffer of textbox */
show_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(show_text));
input_buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(input_text));

/* set textbox to diseditable */
gtk_text_view_set_editable(GTK_TEXT_VIEW(show_text),FALSE);

/* scroll window */
scrolled1=gtk_scrolled_window_new(NULL,NULL);
scrolled2=gtk_scrolled_window_new(NULL,NULL);

/* create a textbox */
gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled1),show_text);
gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled2),input_text);

/* setting of window */
gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled1),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled2),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);

hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,2);
vbox=gtk_box_new(GTK_ORIENTATION_VERTICAL,2);

/* click quit to call quit_win*/
g_signal_connect(G_OBJECT(quit_button),"clicked",G_CALLBACK(quit_win),NULL);

/* click "connect" to call get_ip */
g_signal_connect(G_OBJECT(link_button),"clicked",G_CALLBACK(get_ip),ip_text);
/* click to clear input screen*/
g_signal_connect(G_OBJECT(cls_button),"clicked",G_CALLBACK(clean_send_text),NULL);
/* create window */
gtk_box_pack_start(GTK_BOX(hbox),ip_label,FALSE,FALSE,2);
gtk_box_pack_start(GTK_BOX(hbox),ip_text,FALSE,FALSE,2);
gtk_box_pack_start(GTK_BOX(hbox),link_button,FALSE,FALSE,2);
gtk_box_pack_start(GTK_BOX(hbox),space_label,TRUE,TRUE,2);
gtk_box_pack_start(GTK_BOX(hbox),send_button,FALSE,FALSE,2);
gtk_box_pack_start(GTK_BOX(hbox),cls_button,FALSE,FALSE,2);
gtk_box_pack_start(GTK_BOX(hbox),quit_button,FALSE,FALSE,2);
gtk_box_pack_start(GTK_BOX(vbox),scrolled1,TRUE,TRUE,2);
gtk_box_pack_start(GTK_BOX(vbox),scrolled2,FALSE,FALSE,2);
gtk_box_pack_start(GTK_BOX(vbox),hbox,FALSE,FALSE,2);

gtk_container_add(GTK_CONTAINER(window),vbox);

/* click send button ,then call send_text*/
g_signal_connect(G_OBJECT(send_button),"clicked",G_CALLBACK(send_text),NULL);

gtk_widget_show_all(window);
gtk_main();
return 0;
}

/* show the input text */
void show_local_text(const gchar* text)
{
GtkTextIter start,end;
gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(show_buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
gtk_text_buffer_insert(GTK_TEXT_BUFFER(show_buffer),&end,"Me:\n",4);/*插入文本到缓冲区*/
gtk_text_buffer_insert(GTK_TEXT_BUFFER(show_buffer),&end,text,strlen(text));/*插入文本到缓冲区*/
gtk_text_buffer_insert(GTK_TEXT_BUFFER(show_buffer),&end,"\n",1);/*插入文本到缓冲区*/
}

/* clean the input text */
void clean_send_text()
{
GtkTextIter start,end;
gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(input_buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
gtk_text_buffer_delete(GTK_TEXT_BUFFER(input_buffer),&start,&end);/*插入到缓冲区*/
}

/* get the input text,and send it */
void send_text()
{
GtkTextIter start,end;
gchar *text;

if(issucceed==-1){ /* Haven't create a socket */
show_err("Not connected...\n");
}
else
{ /* Socket creating has succeed ,so send message */
text=(gchar *)malloc(MAXSIZE);
if(text==NULL)
{
printf("Malloc error!\n");
exit(1);
}
/* get text */
gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(input_buffer),&start,&end);
text=gtk_text_buffer_get_text(GTK_TEXT_BUFFER(input_buffer),&start,&end,FALSE);

/* If there is no input,do nothing but return */
if(strcmp(text,"")!=0)
{
send_func(text);
clean_send_text();
show_local_text(text);
}
else
show_err("The message can not be empty...\n");
free(text);
}
}

/* show errors such as "no input","haven't create sockt" etc. */
void show_err(char *err)
{
GtkTextIter start,end;
gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(show_buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
gtk_text_buffer_insert(GTK_TEXT_BUFFER(show_buffer),&end,err,strlen(err));
}

/* get IP address and create socket */
void get_ip(GtkWidget *button,gpointer ip_text)
{
gchar *serv_ip;
int res;
serv_ip=(gchar *)gtk_entry_get_text(GTK_ENTRY((GtkWidget *)ip_text));
res=build_socket(serv_ip);

if(res==1)
show_err("IP Address is  Invalid...\n");
else if(res==-1)
show_err("Connect Failure... \n");/*插入文本到缓冲区*/
else{
show_err("Connect Successful... \n");
issucceed=0;
}
}

/* show the received message */
void show_remote_text(char rcvd_mess[])
{
GtkTextIter start,end;
gtk_text_buffer_get_bounds(GTK_TEXT_BUFFER(show_buffer),&start,&end);/*获得缓冲区开始和结束位置的Iter*/
gtk_text_buffer_insert(GTK_TEXT_BUFFER(show_buffer),&end,"Server:\n",8);/*插入文本到缓冲区*/
gtk_text_buffer_insert(GTK_TEXT_BUFFER(show_buffer),&end,rcvd_mess,strlen(rcvd_mess));/*插入文本到缓冲区*/
gtk_text_buffer_insert(GTK_TEXT_BUFFER(show_buffer),&end,"\n",1);/*插入换行到缓冲区*/
}

/* quit */
void quit_win(GtkWidget *window,gpointer data)
{
gtk_main_quit();
}
