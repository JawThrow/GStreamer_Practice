static void read_video_props(GstCaps* caps)
{
	gint width, height;
	const GstStructure *str;
	
	g_return_if_fail(gst_caps_is_fixed(caps));

	str = gst_caps_get_structure(caps, 0);
	if(!gst_structure_get_int(str, "width", &width)||
	   !gst_structure_get_int(str, "height", &height))
	{
		g_print("No, width/height available\n");
		return;
	}
	g_print("The video size of this set of capabiliteis in %dx%d\n",
	width, height);
}
