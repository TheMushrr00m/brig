#include <node.h>
#include <QtGui>
#include <QObject>
#include <QTextCodec>
#include <uv.h>
#include "qapplication.h"
#include "eventloop.h"

namespace Brig {

	using namespace v8;
	using namespace node;

	Persistent<Function> QApplicationWrap::constructor;

	QApplicationWrap::QApplicationWrap() : ObjectWrap()
	{
		app_argc = 0;
		app_argv = NULL;
		dispatcher = new BrigEventDispatcher;
		QGuiApplication::setEventDispatcher(dispatcher);

		app = new QGuiApplication(app_argc, app_argv);
		QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

//		QCoreApplication *app1 = QCoreApplication::instance();
//		QCoreApplication::removePostedEvents(app1, QEvent::Quit);

		// Initializing event loop
		//eventloop = new EventLoop(app);
		//eventloop->Main();
	}

	QApplicationWrap::~QApplicationWrap()
	{
printf("RELEASE QApplication\n");
		delete app;
		delete dispatcher;
	}

	void QApplicationWrap::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<String> name = String::NewSymbol("QApplication");

		/* Constructor template */
		Persistent<FunctionTemplate> tpl = Persistent<FunctionTemplate>::New(FunctionTemplate::New(QApplicationWrap::New));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);  
		tpl->SetClassName(name);

		/* Prototype */
		NODE_SET_PROTOTYPE_METHOD(tpl, "exec", QApplicationWrap::Exec);
		NODE_SET_PROTOTYPE_METHOD(tpl, "test", QApplicationWrap::Test);

		constructor = Persistent<Function>::New(tpl->GetFunction());

		target->Set(name, constructor);
	}

	Handle<Value> QApplicationWrap::New(const Arguments& args)
	{
		HandleScope scope;

		QApplicationWrap *app_wrap = new QApplicationWrap();
		app_wrap->Wrap(args.This());

		return args.This();
	}

	Handle<Value> QApplicationWrap::Exec(const Arguments& args)
	{
		HandleScope scope;

		QApplicationWrap *app_wrap = ObjectWrap::Unwrap<QApplicationWrap>(args.This());
		app_wrap->quickview->show();
//		app_wrap->GetEventLoop()->Main();
//		app_wrap->app->processEvents();
//		app_wrap->app->exec();

//		while(1) {
//			app_wrap->app->processEvents(QEventLoop::WaitForMoreEvents);
//		}

printf("EXEC\n");

		return scope.Close(Undefined());
	}

	Handle<Value> QApplicationWrap::Test(const Arguments& args)
	{
		HandleScope scope;

		QApplicationWrap *app_wrap = ObjectWrap::Unwrap<QApplicationWrap>(args.This());
		app_wrap->dispatcher->wakeUp();
#if 0
printf("4\n");
//		QQuickView view;
//		view.setSource(QUrl::fromLocalFile("application.qml"));
//		view.show();
		app_wrap->quickview = new QQuickView;
		app_wrap->quickview->setSource(QUrl::fromLocalFile("application.qml"));
printf("5\n");
#if 0
//		app_wrap->app->processEvents(QEventLoop::WaitForMoreEvents);

//		app_wrap->app->exec();
#endif
#endif
		return scope.Close(Undefined());
	}
}
