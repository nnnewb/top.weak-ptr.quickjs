#include <optional>
#include <cqcppsdk/cqcppsdk.h>

#include "quickjspp.hpp"

#include <Windows.h>

using namespace cq;
using namespace std;
using Message = cq::message::Message;
using MessageSegment = cq::message::MessageSegment;

static qjs::Runtime rt;
static std::unique_ptr<qjs::Context> ctx = nullptr;

void register_js_function() {
    auto& module = ctx->addModule("cqp");
    module.function<&logging::info>("println");
}

CQ_INIT {
    on_enable([] {
        ctx.reset(new qjs::Context(rt));
        register_js_function();

        try {
            ctx->evalFile((utils::string_to_coolq(get_app_directory()) + "index.js").c_str(), JS_EVAL_TYPE_MODULE);
        } catch (const qjs::exception&) {
            auto exc = ctx->getException();
            logging::error(APP_ID, (std::string)exc);
            if ((bool)exc["stack"]) logging::error(APP_ID, (std::string)exc["stack"]);
        } catch (const runtime_error& e) {
            logging::error(APP_ID, e.what());
        }

        // debug log
        auto pid = GetCurrentProcessId();
        logging::debug(APP_ID, string("QuickJS Interpreter initialized. Process ID is ") + to_string(pid));
    });

    on_private_message([](const PrivateMessageEvent& e) {
        try {
            ctx->eval("dispatch({eventType: 'privateMsg'})");
        } catch (const qjs::exception&) {
            auto exc = ctx->getException();
            logging::error(APP_ID, (std::string)exc);
            if ((bool)exc["stack"]) logging::error(APP_ID, (std::string)exc["stack"]);
        }
    });
}
