#include <gst/gst.h>

int main(int argc, char* argv[])
{

	//[..]
	switch(GST_MESSAGE_TYPE(message))
	{
		case GST_MESSAGE_BUFFERING:
		{
			gint percent;

			/* no state management needed for live pipelines */
			if(is_live)
				break;

			gst_message_parse_buffering(message, &percent);

			if(persent == 100)
			{
				/* 100% message means buffering is done  */
				buffering = FALSE;

				/* if the desired state is playing, go back */
				if(target_state == GST_STATE_PLAYING)
				{
					gst_element_set_state(pipeline, GST_STATE_PLAYING);
				}
			}
			
			else
			{
				/* buffering is busy */
				if(!buffering && target_state == GST_STATE_PLAYING)
				{
					/* we were not buffering but playing, pause the pipeline */
					gst_element_set_state(pipeline, GST_STATE_PAUSED);
				}
				buffering = TRUE;
			}
				break;
		}
	}
}
