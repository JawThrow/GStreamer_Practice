#include <gst/gst.h>

static GMainLoop *loop;

static gboolean my_bus_callback(GstBus *bus, GstMessage *msg, gpointer data)
{

	g_print("Got %s message\n", GST_MESSAGE_TYPE_NAME(msg));

	switch(GST_MESSAGE_TYPE(msg))
	{
		case GST_MESSAGE_ERROR:
		{	
			GError *err;
			gchar *debug_info;

			gst_message_parse_error(msg, &err, &debug_info);
			g_print("Error: %s\n", err->message);
			g_error_free(err);
			g_free(debug_info);

			g_main_loop_quit(loop);
			break;	
		}
		case GST_MESSAGE_EOS:
		{
			g_main_loop_quit(loop);
			break;
		}
		default:
		break;
	}

	// if return FALSE, this function does not handle message anymore.
	return TRUE;
}

gint main(gint argc, gchar *argv[])
{
	GstElement* pipeline;
	GstBus* bus;
	guint bus_watch_id;

	gst_init(&argc, &argv);

	pipeline = gst_pipeline_new("my-pipeline");


	bus = gst_pipeline_get_bus(GST_PIPELINE(pipeline));
	bus_watch_id = gst_bus_add_watch(bus, my_bus_callback, NULL);
	gst_object_unref(bus);

	


	/* create a mainloop that runs/iterates the default GLib main context
	   (context: NULL), in other words: makes the context check if anything
	   it watches for has happened. When a message has been posted on the
	   bus, the default main context will automatically call our
	   my_bus_callback() function to notify us of that message.
	   The main loop will be run until someone calls g_main_loop_quit() */
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	gst_element_set_state (pipeline, GST_STATE_NULL);
	gst_object_unref(pipeline);
	g_source_remove(bus_watch_id);
	g_main_loop_unref(loop);
	return 0;
}
