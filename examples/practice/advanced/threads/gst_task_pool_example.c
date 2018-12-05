#include <pthread.h>
#include <gst/gst.h>

typedef struct
{
	pthread_t thread;
}TestRTId;

G_DEFINE_TYPE(TestRTPool, test_rt_pool, GST_TYPE_TASK_POOL);

static void default_prepare(GstTaskPool *pool, GError ** error)
{

}

static void default_cleanup(GstTaskPool * pool)
{

}

static gpointer default_push(GstTaskPool * pool, GstTaskPoolFunction func, gpointer data, GError ** error)
{
	TestRTId *tid;
	gint res;
	pthread_attr_t attr;
	struct sched_param param;

	tid = g_slice_new0(TestRTId);

	pthread_attr_init(&attr);
	if((res = pthread_attr_setschedpolicy(&attr, SCHED_RR)) != 0)
		g_warning("setschedparam: failure: %p", g_strerror(res));

	param.sched_priority = 50;
	if((res = pthread_attr_setschedparam(&attr, &param)) != 0)
		g_warning("setschedparam: failure: %p", g_strerror(res));

	if((res = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED)) != 0)
		g_warning("setinheritsched: failure: %p", g_strerror(res));

	res = pthread_create(&tid->thread, &attr, (void *(*) (void *)) func, data);

	if(res != 0)
	{
		g_set_error(error, G_THREAD_ERROR, G_THREAD_ERROR_AGAIN, "Error creating thread : %s", g_strerror(res));
		g_slice_free(TestRTId, tid);
		tid = NULL;
	}

	return tid;
}

static void default_join(GstTaskPool * pool, gpointer id)
{
	TestRTId *tid = (TestRTId *) id;

	pthread_join(tid->thread, NULL);
	g_slice_free(TestRTId, tid);
}

static void test_rt_pool_class_init(TestRTPoolClass *klass)
{
	GstTastPoolClass *gsttaskpool_class;

	gsttaskpool_class = (GstTaskPoolClass *) klass;

	gsttaskpool_class->prepare = default_prepare;
	gsttaskpool_class->cleanup = default_cleanup;
	gsttaskpool_class->push = default_push;
	gsttaskpool_class->join = default_join;
}

static void test_rt_pool_init(TestRTPool * pool)
{
}

GstTaskPool * test_rt_pool_new(void)
{
	GstTaskPool *pool;

	pool = g_object_new(TEST_TYPE_RT_POOL, NULL);

	return pool;
}

static GMainLoop* loop;

static void on_stream_status(GstBus *bus, GstMessage *message, gpointer user_data)
{
	GstStreamStatusType type;
	GstElement *owner;
	const GValue *val;
	GstTask *task = NULL;

	gst_message_parse_stream_status(message, &type, &owner);

	val = gst_message_get_stream_status_object(message);

	/* see if we know how to deal with this object */
	if(G_VALUE_TYPE(val) == GST_TYPE_TASK)
	{
		task = g_value_get_object(val);
	}

	switch(type)
	{
		case GST_STREAM_STATUS_TYPE_CREATE:
			if(task)
			{
				GstTaskPool *pool;
				pool = test_rt_pool_new();
				gst_task_set_pool(task, pool);
			}
			break;
		default:
			break;
	}
}

static void on_error(GstBus *bus, GstMessage *message, gpointer user_data)
{
	g_main_loop_quit(loop);
}

int main(int argc, char *argv[])
{
	GstElement *bin, *fakesrc, *fakesink;
	GstBus *bus;
	GstStateChangeReturn ret;

	gst_init(&argc, &argv);

	bin = gst_pipeline_new("pipeline");
	g_assert(bin);

	fakesrc = gst_element_factory_make("fakesrc", "fakesrc");
	g_assert(fakesrc);
	g_object_set(fakesrc, "num-buffers", 50, NULL);

	fakesink = gst_element_factory_make("fakesink", "fakesink");
	g_assert(fakesink);

	gst_bin_add_many(GST_BIN(bin), fakesrc, fakesink, NULL);
	gst_element_link(fakesrc, fakesink);

	loop = g_main_loop_new(NULL, FALSE);

	bus = gst_pipeline_get_bus(GST_PIPELINE(bin));
	gst_bus_enable_sync_message_emission(bus);
	gst_bus_add_signal_watch(bus);

	g_signal_connect(bus, "sync-message::stream-status", (GCallback) on_stream_status, NULL);
	g_signal_connect(bus, "message::error", (GCallback) on_error, NULL);
	g_signal_connect(bus, "message::eos", (GCallback) on_eos, NULL);

	ret = gst_element_set_status(bin, GST_STATE_PLAYING);
	if(ret != GST_STATE_CHANGE_SUCCESS)
	{
		g_message("failed to change state");
		return -1;
	}

	g_main_loop_run(loop);

	gst_element_set_state(bin, GST_STATE_NULL);
	gst_object_unref(bus);
	g_main_loop_unref(loop);

	return 0;
}
