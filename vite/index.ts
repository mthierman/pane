import "./styles/index.css";
import type { WebViewMessageEvent } from "./webview";

window.chrome.webview.addEventListener("message", (event: WebViewMessageEvent) => {
    console.log(event);
    console.log(event.data);
    document.getElementById("root")!.innerHTML = event.data.message;
});
