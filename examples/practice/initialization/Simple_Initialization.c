#include <stdio.h>
#include <gst/gst.h>

int main(int argc, char *argv[])
{
    const gchar *nano_str;
    guint major, minor, micro, nano;

    gst_init(&argc, &argv);

    //gst_version(&major, &minor, &micro, &nano);

    if(GST_VERSION_NANO == 1)
    {
        nano_str = "(CVS)";
    }
    else if(GST_VERSION_NANO == 2)
    {
        nano_str = "(Prerelease)";        
    }
    else 
    {
        nano_str = "";
    }

    printf("This program is linked against Gstreamer %d, %d, %d %s\n", GST_VERSION_MAJOR, GST_VERSION_MINOR,
																	   GST_VERSION_MICRO, nano_str);

    return 0;
}

