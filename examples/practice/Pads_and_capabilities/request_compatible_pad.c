static void link_to_multiplexer(GstPad *tolink_pad, GstElement *mux)
{
	GstPad* pad;
	gchar *srcname, *sinkname;
	
	srcname = gst_pad_get_name(tolink_pad);
	pad = gst_element_get_compatible_pad(mux, tolink_pad);
	gst_pad_link(tolink_pad, pad);
	sinkname = gst_pad_get_name(pad);
	gst_object_unref(GST_OBJECT(pad));

	g_print("A new pad %s was created and linked to %s\n", sinkname, srcname);
	g_free(sinkname);
	g_free(srcname);
}
