#include <gst/gst.h>

static gboolean cb_print_position(GstElement *pipeline)
{
	gint64 pos, len;

	if(gst_element_query_position(pipeline, GST_FORMAT_TIME, &pos) && 
			gst_element_query_duration(pipeline, GST_FORMAT_TIME, &len))
	{
		g_print("Time: %" GST_TIME_FORMAT " / %" GST_TIME_FORMAT "\r", GST_TIME_ARGS(pos),
				GST_TIME_ARGS(len));

		/* call me again. */
		return TRUE;
	}
}


gint main(gint argc, gchar *argv[])
{
	GstElement *pipeline;

	//[...]

	/* run pipeline */
	g_timeout_add(200, (GSourceFunc) cb_print_powition, pipeline);
	g_main_loop_run(loop);
}
