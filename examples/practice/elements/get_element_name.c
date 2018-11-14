#include <gst/gst.h>

int main(int argc, char* argv[])
{
    GstElement *element;
    gchar* name;

    /* init Gstreamer */
    gst_init(&argc, &argv);

    /* create element */
    element = gst_element_factory_make("fakesrc", "source");
	gst_object_set_name(GST_OBJECT(element), "SRC");
    /* get name */
	//g_object_get(G_OBJECT(element), "name", &name, NULL);
	name = gst_object_get_name(GST_OBJECT(element)); // don't be confused in G_OBJECT() and GST_OBJECT()
    g_print("the name of the element is '%s'.\n", name);
    g_free(name);

    gst_object_unref(GST_OBJECT(element));

    return 0;
}
