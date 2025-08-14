import { Button } from "./button";
import "./index.css";
import type { webview_message } from "./webview_message";

Button.define();

const button = document.getElementById("button") as Button;

window.chrome.webview.addEventListener<webview_message>("message", (event) => {
    switch (event.data.type) {
        case "init":
            {
                console.log(event.data);
            }
            break;
        case "test":
            {
                console.log(event.data);
            }
            break;
    }
});
