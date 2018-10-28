#include <gst/gst.h>

int main(int argc, char* argv[])
{
    GstElement *element;
    gchar* name;

    /* init Gstreamer */
    gst_init(&argc, &argv);

    /* create element */
    element = gst_element_factory_make("fakesrc", "source");

    /* get name */
    g_object_get(G_OBJECT(element), "name", &name, NULL);
    g_print("the name of the element is '%s'.\n", name);
    g_free(name);

    gst_object_unref(GST_OBJECT(element));

    return 0;
}