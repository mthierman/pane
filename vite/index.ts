import "./styles/index.css";
import type { WebViewMessageEvent } from "./webview";

window.chrome.webview.addEventListener("message", (arg) => {
    const webview_message_event = arg as WebViewMessageEvent;
    console.log(webview_message_event.data);
});
