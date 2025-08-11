import "./styles/index.css";
import type { WebViewMessageEvent } from "./webview";

window.chrome.webview.addEventListener("message", (event: WebViewMessageEvent) => {
    console.log(event);
});
