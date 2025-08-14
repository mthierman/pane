import { Button } from "./button";
import "./index.css";
import type { webview_message } from "./webview_message";

Button.define();

const button = document.getElementById("button") as Button;

window.chrome.webview.addEventListener<webview_message>("message", (event) => {
    const data = event.data;

    switch (data.type) {
        case "init":
            {
                console.log(data);
            }
            break;
        case "test":
            {
                console.log(data);
            }
            break;
    }
});
