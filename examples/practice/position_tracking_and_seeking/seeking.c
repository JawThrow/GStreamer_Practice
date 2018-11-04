static void seek_to_time(GstElement *pipeline, gint64 time_nanoseconds)
{
	if(!gst_element_seek(pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
						 GST_SEEK_TYPE_SET, time_nanoseconds,
						 GST_SEEK_TYPE_NONE, GST_CLOCK_TIME_NONE))
	{
		g_print("Seek failed!\n");
	}
}
