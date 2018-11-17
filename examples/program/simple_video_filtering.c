#include <gst/gst.h>

int main(int argc, char* argv[])
{
	GstElement *inputElement;
	GstElement *filter;
	GstElement *outputElement;
	GstElement *player;

	GstBus *bus;
	GstMessage *msg;
	GstStateChangeReturn ret;

	gst_init(&argc, &argv);

	player = gst_pipeline_new("Video-Player");
	inputElement = gst_element_factory_make("filesrc", "input");
	//filter = gst_element_factory_make("identity", "filter");
	outputElement = gst_element_factory_make("autovideosink", "output");

	//if(!player || !inputElement || !filter || !outputElement)
	if(!player || !inputElement || !outputElement)
	{
		g_printerr("Not all ements could be created.\n");
		return -1;
	}

	// video file name is required to playback in command-line
	g_object_set(G_OBJECT(inputElement), "location", argv[1], NULL);

//	gst_bin_add_many(GST_BIN(player), inputElement, filter, outputElement, NULL);
	gst_bin_add_many(GST_BIN(player), inputElement, outputElement, NULL);


	//if(!gst_element_link_many(inputElement, filter, outputElement, NULL))
	if(!gst_element_link_many(inputElement, outputElement, NULL))
	{
		g_warning("linking elements fail!\n");
	}


	ret = gst_element_set_state(GST_ELEMENT(player), GST_STATE_PLAYING);
	if(ret == GST_STATE_CHANGE_FAILURE)
	{
		g_printerr("Unable to set bin to the playing state.\n");
		return -1;
	}

	bus = gst_element_get_bus(GST_ELEMENT(player));
	msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE, GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

	if(msg != NULL)
	{
		GError *err;
		gchar *debug_info;
		switch(GST_MESSAGE_TYPE(msg))
		{
		case GST_MESSAGE_ERROR:
			gst_message_parse_error(msg, &err, &debug_info);
			g_printerr("Error received from element %s : %s\n", GST_OBJECT_NAME(msg->src), err->message);
			g_printerr("Debugging information: %s\n", debug_info ? debug_info : "none");
			g_clear_error(&err);
			g_free(debug_info);
			break;
		case GST_MESSAGE_EOS:
			g_print("End of Stream reached.\n");
			break;
		default:
			g_printerr("Unexpected message received.\n");
			break;
		}

		gst_message_unref(msg);

	}
	

	gst_object_unref(bus);
	gst_element_set_state(player, GST_STATE_NULL);
	gst_object_unref(player);
	return 0;
}

