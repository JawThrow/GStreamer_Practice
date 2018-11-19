#include <gst/gst.h>

static void cb_new_pad(GstElement *element, GstPad *pad, gpointer data)
{
	gchar *name;
	name = gst_pad_get_name(pad);
	g_print(name);
	g_free(name);

	/* here, you would setup a new pad link for the newly created pad */
	// bla bla...
}

int main(int argc, char *argv[])
{
	GstElement *pipeline, *source, *demux;
	GMainLoop *loop;

	/* init */
	gst_init(&argc, &argv);

	/* create elements */
	pipeline = gst_pipeline_new("pipe1");
	source = gst_element_factory_make("filesrc", "source");
	g_object_set(source, "location", argv[1], NULL);
	demux = gst_element_factory_make("oggdemux", "demuxer");

	/* here, it need to be check that elements are created properly  */

	/* put together a pipeline */
	gst_bin_add_many(GST_BIN(pipeline), source, demux, NULL);
	gst_element_link_pads(source, "src",  demux, "sink"); // link_elements? link_pads?

	/* listen for newly created pads */
	g_signal_connect(demux, "pad-added", G_CALLBACK(cb_new_pad), NULL);

	/* start the pipeline */
	gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
	loop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(loop);

	// bla bla...
}
